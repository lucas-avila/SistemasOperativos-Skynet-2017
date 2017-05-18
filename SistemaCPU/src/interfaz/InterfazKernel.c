#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include "../general/Socket.h"
#include "../header/PCB.h"
#include "../header/AppConfig.h"
#include "../primitivas/PrimitivasFunciones.h"
#include "../primitivas/FuncionesAuxiliares.h"

#include "InterfazKernel.h"
#include "commons/string.h"
/* se encarga de recibir y llenar toda la estructura struct PCB */
PCB* recibir_PCB_de_kernel() {
	int kernel =servidor_kernel;
	PCB * pcb = recibir_pcb(kernel);

	printf("El fucking PCB es :\n");
	printf("PID : %d\n", pcb->PID);
	printf("program counter : %d\n", pcb->program_counter);
	printf("cantidad paginas codigo : %d\n", pcb->cantidad_paginas_codigo);

	printf("Lista de indices de codigo:\n\n");
	IndiceCodigo * c = malloc(sizeof(IndiceCodigo));
	int i = 0;
	for (i; i < list_size(pcb->codigo); i++) {
		c = list_get(pcb->codigo, i);
		printf("Indice %d\n", i);
		printf("--program counter:%d\n--byte inicial:%d\n--byte final:%d\n--pagina:%d\n", c->program_counter, c->byte_inicial_codigo, c->byte_final_codigo, c->pagina);
	}

	printf("cantidad codigo : %d\n", pcb->cantidad_codigo);

	printf("Lista de indices de la pila:\n\n");
	IndiceStack * e = malloc(sizeof(IndiceStack));
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
	printf("--id funcion: %s\n--nombre etiqueta: %s\n--valor program counter: %d\n", pcb->etiqueta->identificador_funcion, pcb->etiqueta->nombre_etiqueta, pcb->etiqueta->valor_program_counter);

	printf("Cantidad etiqueta: %d\n", pcb->cantidad_etiqueta);
	printf("Exit code: %d\n", pcb->exit_code);
	printf("Pagina inicial de la pila: %d\n", pcb->pagina_inicial_stack);

	printf("Cosas de planificacion\n");
	printf("RR (0->fifo, 1->RR): %d\n", pcb->RR);
	printf("Cantidad de rafagas para RR: %d\n", pcb->cantidad_rafagas);
	printf("Cantidad de rafagas ejecutadas: %d\n", pcb->cantidad_rafagas_ejecutadas);
	return pcb;
}

/* Se encarga de devolver al kernel el PCB con sus modificaciones por la ejecucion*/
void enviar_PCB_a_kernel(PCB* pcb) {

}

char* enviar_SYSCALL_solicitar_memoria_dinamica_a_kernel(int PID, int espacio) {
	enviar_dato_serializado("GESTION_MEMORIA", servidor_kernel);
	enviar_dato_serializado("MALLOC_MEMORIA", servidor_kernel);
	enviar_dato_serializado(string_itoa(PID), servidor_kernel);
	enviar_dato_serializado(string_itoa(espacio), servidor_kernel);

	char* resultado = recibir_dato_serializado(servidor_kernel);
	if (strcmp(resultado, "OK") == 0) {
		char* PIDR = recibir_dato_serializado(servidor_kernel);
		char* pagina = recibir_dato_serializado(servidor_kernel);
		char* byteInicial = recibir_dato_serializado(servidor_kernel);

		t_puntero memoria_serializada = serializarMemoriaDinamica(pagina, byteInicial);
		return string_itoa(memoria_serializada);
		//return "0001";
	} else {

		printf("\n Error en MALLOC: %s", resultado);
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

void enviar_SYSCALL_operacion_crear_archivo_a_kernel(int PID, PCB* pcb) {
}
/*
void enviar_SYSCALL_operacion_leer_archivo_a_kernel(int PID, PCB pcb) {
}*/

void enviar_SYSCALL_operacion_guardar_datos_archivo_a_kernel(int PID, PCB* pcb) {
}

void enviar_SYSCALL_operacion_borrar_archivo_a_kernel(int PID, PCB* pcb) {
}

int asignar_valor_a_variable_compartida_en_kernel(char* nombre_varComp, int valor_varComp) {
	enviar_dato_serializado("ASIGNAR_VAR_COMP", servidor_kernel);
	enviar_dato_serializado(nombre_varComp, servidor_kernel);
	enviar_dato_serializado(string_itoa(valor_varComp), servidor_kernel);

	char* resultado = recibir_dato_serializado(servidor_kernel);
	if (strcmp(resultado, "OK") == 0) {

		printf(resultado);
		return 0;
	}
	else {

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
		valorVariable = (atoi(valor_var_comp));
		return 0;
	}
	else {

		printf("\n Error en BUSCAR VARIABLE COMPARTIDA: %s", resultado);
		return -1;

	}
}


char* abrir_archivo(char* PID, char* pathArchivo, bool flagCreate, bool flagRead, bool flagWrite){

char* respuesta;

enviar_dato_serializado("ABRIR_ARCHIVO", servidor_kernel);
enviar_dato_serializado(PID, servidor_kernel);
enviar_dato_serializado(pathArchivo,servidor_kernel);
enviar_dato_serializado(string_itoa(flagCreate),servidor_kernel);
enviar_dato_serializado(string_itoa(flagRead),servidor_kernel);
enviar_dato_serializado(string_itoa(flagWrite),servidor_kernel);

respuesta = recibir_dato_serializado(servidor_kernel);
// SI ALGO SALE MAL, DEVUELVE UN MSG DE ERROR--SI SALE BIEN DEVUELVE EL 'FD'
return respuesta;

}

char* mover_cursor_archivo(char* PID, int FD, int cursor_bloque){

enviar_dato_serializado("MOVER_CURSOR_ARCHIVO", servidor_kernel);
enviar_dato_serializado(PID, servidor_kernel);
enviar_dato_serializado(string_itoa(FD),servidor_kernel);
enviar_dato_serializado(string_itoa(cursor_bloque),servidor_kernel);

char* respuesta = recibir_dato_serializado(servidor_kernel);

return respuesta;
}


char* leer_archivo(char* PID, int FD, int tamanio){

enviar_dato_serializado("LEER_ARCHIVO", servidor_kernel);
enviar_dato_serializado(PID, servidor_kernel);
enviar_dato_serializado(string_itoa(FD),servidor_kernel);
enviar_dato_serializado(string_itoa(tamanio),servidor_kernel);

char* respuesta = recibir_dato_serializado(servidor_kernel);

return respuesta;
}


char* cerrar_archivo(char* PID, int FD){

enviar_dato_serializado("CERRAR_ARCHIVO", servidor_kernel);
enviar_dato_serializado(PID, servidor_kernel);
enviar_dato_serializado(string_itoa(FD),servidor_kernel);

char* respuesta = recibir_dato_serializado(servidor_kernel);

return respuesta;
}

char* borrar_archivo(char* PID, char* rutaArchivo){

enviar_dato_serializado("BORRAR_ARCHIVO", servidor_kernel);
enviar_dato_serializado(PID, servidor_kernel);
enviar_dato_serializado(rutaArchivo,servidor_kernel);
char* respuesta = recibir_dato_serializado(servidor_kernel);

return respuesta;

}
