#include "InterfazKernel.h"

#include <commons/collections/list.h>
#include <commons/string.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../general/Semaforo.h"
//#include "../general/Socket.h"
#include "../../../Sharedlib/Sharedlib/Socket.h"
#include "../header/AppConfig.h"
#include "../../../Sharedlib/Sharedlib/Socket.h"
#include "../primitivas/EstructurasDeDatosPrimitivas.h"
#include "../primitivas/FuncionesAuxiliares.h"
#include "../procesador/Ejecucion.h"

/* se encarga de recibir y llenar toda la estructura struct PCB */
PCB* recibir_PCB_de_kernel() {
	int kernel = servidor_kernel;
	PCB * pcb = recibir_pcb(kernel);
	printf("PID : %d\n", pcb->PID);
	/*
	 printf("El fucking PCB es :\n");
	 printf("program counter : %d\n", pcb->program_counter);
	 printf("cantidad paginas codigo : %d\n", pcb->cantidad_paginas_codigo);

	 printf("Lista de indices de codigo:\n\n");
	 IndiceCodigo* c; //= malloc(sizeof(IndiceCodigo));
	 int i = 0;
	 for (i; i < list_size(pcb->codigo); i++) {
	 c = list_get(pcb->codigo, i);
	 printf("Indice %d\n", i);
	 printf("--program counter:%d\n--byte inicial:%d\n--byte final:%d\n--pagina:%d\n", c->program_counter, c->byte_inicial_codigo, c->byte_final_codigo, c->pagina);
	 }

	 printf("cantidad codigo : %d\n", pcb->cantidad_codigo);

	 printf("Lista de indices de la pila:\n\n");
	 IndiceStack * e;// = malloc(sizeof(IndiceStack));
	 for (i = 0; i < list_size(pcb->pila); i++) {
	 e = list_get(pcb->pila, i);
	 printf("Elemento %d del stack:\n posicion: %d\n", i, e->posicion);
	 int x = 0;
	 for (x; x < list_size(e->argumentos); x++) {
	 Argumento * a = list_get(e->argumentos, x);
	 printf("argumentos:\n--id:%d\n--pagina:%d\n--byte_inicial:%d\n--tamanio:%d\n", a->id, a->pagina, a->byte_inicial, a->tamanio);
	 }
	 for (x = 0; x < list_size(e->variables); x++) {
	 Variable * v = list_get(e->variables, x);
	 printf("variables:\n--id:%d\n--pagina:%d\n--byte_inicial:%d\n--tamanio:%d\n--dinamica:%d\n", v->id, v->pagina, v->byte_inicial, v->tamanio, v->dinamica);
	 }
	 printf("retPos: %d\nretVar:\n--byte_inicial:%d\n--pagina:%d\n--tamanio:%d\n", e->retPos, e->retVar->byte_inicial, e->retVar->pagina, e->retVar->tamanio);
	 }

	 printf("Indice etiqueta:\n");
	 printf("--no voy a hacer una funcion que devuelva todas las etiquetas dame el nombre de una etiqueta y te tiro el PC correspondiente, asi funcionan las cosas mostro\n");
	 printf("Exit code: %d\n", pcb->exit_code);
	 printf("Pagina inicial de la pila: %d\n", pcb->pagina_inicial_stack);

	 printf("Cosas de planificacion\n");
	 printf("RR (0->fifo, 1->RR): %d\n", pcb->RR);
	 printf("Cantidad de rafagas para RR: %d\n", pcb->cantidad_rafagas);
	 printf("Quantum sleep %d\n", pcb->quantum_sleep);
	 printf("Cantidad de rafagas ejecutadas: %d\n", pcb->cantidad_rafagas_ejecutadas);
	 */
	return pcb;
}

/* Se encarga de devolver al kernel el PCB con sus modificaciones por la ejecucion*/
void enviar_PCB_a_kernel(PCB* pcb, char * modo) {
	enviar_dato_serializado(modo, servidor_kernel);
	enviar_pcb(pcb, servidor_kernel);
}

int enviar_SYSCALL_wait_semaforo_a_kernel(char* nombre, PCB * pcb) {
	//le sacamos los espacios
	char * nombre_semaforo = string_new();
	string_append(&nombre_semaforo, nombre);
	string_trim_left(&nombre_semaforo);
	string_trim_right(&nombre_semaforo);

	enviar_PCB_a_kernel(pcb, "WAIT_SEM");
	enviar_dato_serializado(nombre_semaforo, servidor_kernel);
	printf("WAIT %s\n", nombre_semaforo);
	char * respuesta = recibir_dato_serializado(servidor_kernel);
	printf("Continuando la cpu, respuesta recibida : %s\n", respuesta);
	if (strcmp(respuesta, "BLOQUEADO") == 0) {
		//TODO: el semaforo quedo bloqueando el proceso, se libera esta cpu
		printf("el semaforo quedo bloqueando el proceso, se libera esta cpu\n");
		return 1;
	} else if (strcmp(respuesta, "NO_BLOQUEADO") == 0) {
		//TODO: el semaforo no bloqueó el proceso, el proceso continua su ejecucion normal
		printf("el semaforo no bloqueo el proceso");
		return 0;
	} else if (strcmp(respuesta, "SEMAFORO_NO_EXISTE") == 0)
		lanzar_excepcion(respuesta);

	return 0;
}

int enviar_SYSCALL_signal_semaforo_a_kernel(char* nombre) {
	//le sacamos los espacios
	char * nombre_semaforo = string_new();
	string_append(&nombre_semaforo, nombre);
	string_trim_left(&nombre_semaforo);
	string_trim_right(&nombre_semaforo);

	enviar_dato_serializado("SIGNAL_SEM", servidor_kernel);
	enviar_dato_serializado(nombre_semaforo, servidor_kernel);
	printf("SIGNAL %s\n", nombre_semaforo);
	//TODO: que pasa si el nombre del semaforo no existe?
	char * respuesta = recibir_dato_serializado(servidor_kernel);
	if (strcmp(respuesta, "SEMAFORO_NO_EXISTE") == 0)
		lanzar_excepcion(respuesta);

	return -1;
}

char* enviar_SYSCALL_solicitar_memoria_dinamica_a_kernel(int PID, int espacio, int* pagina, int* byteIncial) {
	enviar_dato_serializado("GESTION_MEMORIA", servidor_kernel);
	enviar_dato_serializado("MALLOC_MEMORIA", servidor_kernel);
	enviar_dato_serializado(string_itoa(PID), servidor_kernel);
	enviar_dato_serializado(string_itoa(espacio), servidor_kernel);

	char* resultado = recibir_dato_serializado(servidor_kernel);
	if (strcmp(resultado, "OK") == 0) {
		//char* PIDR = recibir_dato_serializado(servidor_kernel);
		char* pagM = recibir_dato_serializado(servidor_kernel);
		char* byteIncialM = recibir_dato_serializado(servidor_kernel);
		*pagina = atoi(pagM);
		*byteIncial = atoi(byteIncialM);
		//char* pagina = recibir_dato_serializado(servidor_kernel);
		//	char* byteInicial = recibir_dato_serializado(servidor_kernel);

		//	t_puntero memoria_serializada = serializarMemoriaDinamica(pagina, byteInicial);
		return "OK";
		//return "0001";
	} else {

		printf("\n Error en MALLOC: %s", resultado);
		lanzar_excepcion(resultado);
		return "ERROR";
	}
}

char* enviar_SYSCALL_liberar_memoria_dinamica_a_kernel(DireccionMemoriaDinamica* varDinamica) {
	enviar_dato_serializado("GESTION_MEMORIA", servidor_kernel);
	enviar_dato_serializado("FREE", servidor_kernel);
	enviar_dato_serializado(string_itoa(varDinamica->pid), servidor_kernel);
	enviar_dato_serializado(string_itoa(varDinamica->pagina), servidor_kernel);
	enviar_dato_serializado(string_itoa(varDinamica->byteInicial), servidor_kernel);
	return recibir_dato_serializado(servidor_kernel);
}

int asignar_valor_a_variable_compartida_en_kernel(char* nombre_varComp, int valor_varComp) {
	enviar_dato_serializado("ASIGNAR_VAR_COMP", servidor_kernel);
	enviar_dato_serializado(nombre_varComp, servidor_kernel);
	enviar_dato_serializado(string_itoa(valor_varComp), servidor_kernel);

	char* resultado = recibir_dato_serializado(servidor_kernel);
	if (strcmp(resultado, "OK") == 0) {

		printf("OK");
		return 0;
	} else {
		lanzar_excepcion(resultado);
		printf("\n Error en ASIGNAR VARIABLE COMPARTIDA: %s", resultado);
		return -1;

	}
}

int obtener_valor_de_variable_compartida_en_kernel(char* nombre_varComp, int* valorVariable) {
	enviar_dato_serializado("BUSCAR_VAL_VAR_COMP", servidor_kernel);
	enviar_dato_serializado(nombre_varComp, servidor_kernel);

	char* resultado = recibir_dato_serializado(servidor_kernel);
	if (strcmp(resultado, "OK") == 0) {
		char* valor_var_comp = recibir_dato_serializado(servidor_kernel);
		(*valorVariable) = (atoi(valor_var_comp));
		printf("La variable solicitada tiene el valor: %d", (*valorVariable));
		return 0;
	} else {
		lanzar_excepcion(resultado);
		printf("\n Error en BUSCAR VARIABLE COMPARTIDA: %s", resultado);
		return -1;

	}
}

char* abrir_archivo(char* PID, char* pathArchivo, bool flagCreate, bool flagRead, bool flagWrite) {

	char* respuesta;

	enviar_dato_serializado("ABRIR_ARCHIVO", servidor_kernel);
	enviar_dato_serializado(PID, servidor_kernel);
	enviar_dato_serializado(pathArchivo, servidor_kernel);
	enviar_dato_serializado(string_itoa(flagCreate), servidor_kernel);
	enviar_dato_serializado(string_itoa(flagRead), servidor_kernel);
	enviar_dato_serializado(string_itoa(flagWrite), servidor_kernel);

	respuesta = recibir_dato_serializado(servidor_kernel);
// SI ALGO SALE MAL, DEVUELVE UN MSG DE ERROR--SI SALE BIEN DEVUELVE EL 'FD'
	printf("EL FD ES: %s\n", respuesta);
	return respuesta;

}

char* mover_cursor_archivo(char* PID, int FD, int cursor_bloque) {

	enviar_dato_serializado("MOVER_CURSOR_ARCHIVO", servidor_kernel);
	enviar_dato_serializado(PID, servidor_kernel);
	enviar_dato_serializado(string_itoa(FD), servidor_kernel);
	enviar_dato_serializado(string_itoa(cursor_bloque), servidor_kernel);

	char* respuesta = recibir_dato_serializado(servidor_kernel);

	return respuesta;
}

char* leer_archivo(char* PID, int FD, int tamanio) {

	enviar_dato_serializado("LEER_ARCHIVO", servidor_kernel);
	enviar_dato_serializado(PID, servidor_kernel);
	enviar_dato_serializado(string_itoa(FD), servidor_kernel);
	enviar_dato_serializado(string_itoa(tamanio), servidor_kernel);

	char* respuesta = recibir_dato_serializado(servidor_kernel);

	return respuesta;
}

char* escribir_archivo(char* PID, int FD, int tamanio, char* contenido) {
	char* respuesta ;
	if (FD != 0) {
		enviar_dato_serializado("ESCRIBIR_ARCHIVO", servidor_kernel);
		enviar_dato_serializado(PID, servidor_kernel);
		enviar_dato_serializado(string_itoa(FD), servidor_kernel);
		enviar_dato_serializado(string_itoa(tamanio), servidor_kernel);
		enviar_dato_serializado(contenido, servidor_kernel);
		 respuesta = recibir_dato_serializado(servidor_kernel);
	} else {
		CU_Escribir_Pantalla_AnSISOP(contenido, PID);
		 respuesta = "OK";
	}


	return respuesta;
}

char* cerrar_archivo(char* PID, int FD) {

	enviar_dato_serializado("CERRAR_ARCHIVO", servidor_kernel);
	enviar_dato_serializado(PID, servidor_kernel);
	enviar_dato_serializado(string_itoa(FD), servidor_kernel);

	char* respuesta = recibir_dato_serializado(servidor_kernel);

	return respuesta;
}

char* borrar_archivo(char* PID, char* rutaArchivo) {

	enviar_dato_serializado("BORRAR_ARCHIVO", servidor_kernel);
	enviar_dato_serializado(PID, servidor_kernel);
	enviar_dato_serializado(rutaArchivo, servidor_kernel);
	char* respuesta = recibir_dato_serializado(servidor_kernel);

	return respuesta;

}
void CU_Escribir_Pantalla_AnSISOP(char* mensaje, char* PID) {
	enviar_dato_serializado("IMPRIMIR_POR_PANTALLA", servidor_kernel);

	enviar_dato_serializado(PID, servidor_kernel);
	enviar_dato_serializado(mensaje, servidor_kernel);
}
