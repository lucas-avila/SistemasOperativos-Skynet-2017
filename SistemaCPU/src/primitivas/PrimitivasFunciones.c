#include <commons/collections/list.h>
#include <commons/string.h>
#include <ctype.h>
#include <parser/metadata_program.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../../Sharedlib/Sharedlib/PCB.h"
#include "../interfaz/InterfazKernel.h"
#include "../interfaz/InterfazMemoria.h"
#include "../procesador/Ejecucion.h"
#include "EstructurasDeDatosPrimitivas.h"
#include "FuncionesAuxiliares.h"
#include "Stack.h"
#include "PrimitivasFunciones.h"

PCB* pcb;
const int TAMANIO_VARIABLE = 4;

void setearPCB(PCB* pcbEnviado) {
	pcb = pcbEnviado;
}

void buscarPaginaDisponibleStack(PCB* pcbRecibido) {
	if (pcb->posicion_pagina_stack + sizeof(uint32_t) > tamanio_pagina_memoria) {
		pcb->posicion_pagina_stack = sizeof(uint32_t);
		pcb->pagina_inicial_stack++;

	} else {
		pcb->posicion_pagina_stack += sizeof(uint32_t);
	}

	//ATACAR EL PROBLEMA DEL STACK OVERFLOW

	if (pcb->pagina_inicial_stack >= pcb->cantidad_paginas_codigo) {
		lanzar_excepcion("STACKOVERFLOW");
	}
}

t_puntero DEFINIR_VARIABLE(t_nombre_variable variable) {
	IndiceStack* pila = obtener_Ultima_fila_Indice_Stack(pcb);

	int paginaDiponible = 0, offsetDisp = 0;
	buscarPaginaDisponibleStack(pcb);

	if (hubo_excepcion == true) {
		return 0;
	}

	if (isalpha(variable)) {

		Variable* var_new = crear_variable(variable, pcb->pagina_inicial_stack, pcb->posicion_pagina_stack - sizeof(uint32_t), sizeof(uint32_t), 0);
		crear_variable_en_Indice_Stack(pila, var_new);

		return serializarPuntero(var_new->pagina, var_new->byte_inicial, tamanio_pagina_memoria);
	} else {
		Argumento* arg_new = crear_argumento(variable, pcb->pagina_inicial_stack, pcb->posicion_pagina_stack - sizeof(uint32_t), sizeof(uint32_t));

		crear_argumento_en_Indice_Stack(pila, arg_new);

		return serializarPuntero(arg_new->pagina, arg_new->byte_inicial, tamanio_pagina_memoria);
	}

}

t_puntero OBTENER_DIRECCION_DE_VARIABLE(t_nombre_variable variable) {

	if (isalpha(variable)) {

		Variable* var = buscar_variable_stack_por_nombre(pcb, variable);
		return serializarPuntero(var->pagina, var->byte_inicial, tamanio_pagina_memoria);
	} else {
		Argumento* arg = buscar_argumento_stack_por_nombre(pcb, variable);
		return serializarPuntero(arg->pagina, arg->byte_inicial, tamanio_pagina_memoria);
	}

}

char* n_char;

char* intToChar4(int num) {

	return num;
}

int char4ToInt(char* chars) {
	int a;
	if (strcmp(chars, " ") != 0) {
		string_trim(&chars);

		/*int i = 0;
		 for (i; i < 4; i++) {
		 if (n_char[i] == '.') {
		 n_char[i] = '\0';
		 }

		 }*/
	}
	memcpy(&a, chars, sizeof(int));
	return a;
}

void ASIGNAR_VARIABLE(t_puntero direccion_variable, t_valor_variable valor) {
	int pagina = 0, offset = 0;
	deserializar_puntero(direccion_variable, &pagina, &offset, tamanio_pagina_memoria);
	//printf("\nASGINAR VARIABLE Pagina: %d, Bity Inicial %d",pagina,offset);
	almacenar_Bytes_de_Pagina(string_itoa(pcb->PID), string_itoa(pagina), string_itoa(offset), string_itoa(sizeof(uint32_t)), valor, "");

}

void WAIT(t_nombre_semaforo identificador_semaforo) {
	//Incremento el program counter para pasar a la siguiente linea del wait
	//Si se bloqueó, ya se mando al kernel con el program counter apuntando a la siguiente sentencia para que cuando vuelva
	//A una cpu no ejecute de nuevo el wait
	//Si resulta que no se bloqueó, retrocedemos el program counter y el ++ se hace en Ejecucion.c luego de analizarLinea
	pcb->program_counter++;

	int result = enviar_SYSCALL_wait_semaforo_a_kernel(identificador_semaforo, pcb);
	if (result == 1)
		marcarBloqueado();
	else if (result == -1)
		return;
	else
		pcb->program_counter--;

	//Si da error terminamos la cpu
	//TODO: Chequear si esta bien finalizar cuando ocurren esos errores
	//TODO: Chequear en el kernel tambien que esta cpu crasheó
}

void SIGNAL(t_nombre_semaforo identificador_semaforo) {
	printf("\nPRE-SIGNAL %s\n", identificador_semaforo);
	enviar_SYSCALL_signal_semaforo_a_kernel(identificador_semaforo);
}
/*
 t_valor_variable DEREFERENCIAR(t_puntero puntero) {
 int pagina = 0, offset = 0;
 deserializar_puntero(puntero, &pagina, &offset, tamanio_pagina_memoria);
 int* resultado = solicitar_bytes_memoria(string_itoa(pcb->PID), string_itoa(pagina), string_itoa(offset), string_itoa(sizeof(uint32_t)));
 printf("LLegaresultado %d\n", resultado);
 return *resultado; //Modificado

 }*/
t_valor_variable DEREFERENCIAR(t_puntero puntero) {
	int pagina = 0, offset = 0;
	deserializar_puntero(puntero, &pagina, &offset, tamanio_pagina_memoria);
	void* resultado = solicitar_bytes_memoria(string_itoa(pcb->PID), string_itoa(pagina), string_itoa(offset), string_itoa(sizeof(uint32_t)));

	if (strcmp(resultado, "PAGINA_NO_EXISTE") == 0) {
		lanzar_excepcion(resultado);
		return 0;
	}

	return *((int*)resultado); //Modificado

}
t_puntero ALOCAR(t_valor_variable espacio) {
	int pagina, offset;
	char* mensaje = enviar_SYSCALL_solicitar_memoria_dinamica_a_kernel(pcb->PID, espacio, &pagina, &offset);

	if (strcmp(mensaje, "OK") == 0) {
		return serializarPuntero(pagina, offset, tamanio_pagina_memoria);
	} else {
		lanzar_excepcion(mensaje);
	}
	//TODO: aca se tendria que bloquear el programa y lanzar excepcion

	return 0;

}

void LIBERAR(t_puntero memoria_serializada) {

	//int punteroLiberar = DEREFERENCIAR(memoria_serializada);
	int pagina = 0, offset = 0;
	int* ptPagina = &pagina;
	int* ptoffset = &offset;
	deserializar_puntero(memoria_serializada, &pagina, &offset, tamanio_pagina_memoria);
	char* valorPuntero = solicitar_bytes_memoria(string_itoa(pcb->PID), string_itoa(pagina), string_itoa(offset), "4");
	if(pagina < pcb->cantidad_paginas_codigo){
		deserializar_puntero(*((int*)valorPuntero), &pagina, &offset, tamanio_pagina_memoria);
	}



	// punteroLiberar = 1536;
	//deserializar_puntero(memoria_serializada, ptPagina, ptoffset, tamanio_pagina_memoria);
	DireccionMemoriaDinamica* varDinamica = malloc(sizeof(DireccionMemoriaDinamica));
	varDinamica->pid = pcb->PID;
	varDinamica->pagina = pagina;
	varDinamica->byteInicial = offset;

	char* resultado = enviar_SYSCALL_liberar_memoria_dinamica_a_kernel(varDinamica);
	if (strcmp("OK", resultado) != 0)
		lanzar_excepcion(resultado);
	//printf("\nResultado de LIBERAR el puntero %d: %s", memoria_serializada, resultado);

}

void IR_A_LABEL(t_nombre_etiqueta nombre_etiqueta) {
	if (nombre_etiqueta[strlen(nombre_etiqueta) - 1] == '\n') {
		nombre_etiqueta[strlen(nombre_etiqueta) - 1] = '\0'; //esto es porque las etiquetas vienen con el \n al final
	}

	pcb->program_counter = metadata_buscar_etiqueta(nombre_etiqueta, pcb->etiquetas, pcb->etiquetas_size);
	pcb->program_counter -= 1;
}

void RETORNAR(t_valor_variable variableRetorno) {

	IndiceStack* stackDeFuncion = list_get(pcb->pila, (list_size(pcb->pila) - 1));
	if (stackDeFuncion->retVar->tamanio != 0) {
		int punteroVariableRetorno = serializarPuntero(stackDeFuncion->retVar->pagina, stackDeFuncion->retVar->byte_inicial, tamanio_pagina_memoria);

		ASIGNAR_VARIABLE(punteroVariableRetorno, variableRetorno);
	}

}

void FINALIZAR() {
	//FALTA DETERMINAR SI DEBE CAMBIAR EL EXIT_CODE DEL PCB (SI TERMINO EL PROCESO)
	if (list_size(pcb->pila) != 1) {
		IndiceStack* stackDeFuncion = list_get(pcb->pila, (list_size(pcb->pila) - 1));
		pcb->program_counter = stackDeFuncion->retPos;
		list_remove(pcb->pila, (list_size(pcb->pila) - 1));

	} else {
		esFinPrograma = true;

		pcb->exit_code = 0;

	}
}

void LLAMAR_SIN_RETORNO(t_nombre_etiqueta nombre_etiqueta) {

	insertar_nueva_fila_Indice_Stack(pcb);

	IndiceStack * pila = obtener_Ultima_fila_Indice_Stack(pcb);
	pila->retPos = pcb->program_counter;

	// tiene que buscar la etiqueta en la lista de etiquetas y conseguir la direccion
	t_puntero_instruccion direccionEtiqueta = metadata_buscar_etiqueta(nombre_etiqueta, pcb->etiquetas, pcb->etiquetas_size);
	pcb->program_counter = direccionEtiqueta;

	pila->retVar = malloc(sizeof(ReturnVariable));

	pila->retVar->tamanio = 0;

}

void LLAMAR_CON_RETORNO(t_nombre_etiqueta nombre_etiqueta, t_puntero direccionRetorno) {

	insertar_nueva_fila_Indice_Stack(pcb);
	IndiceStack * pila = obtener_Ultima_fila_Indice_Stack(pcb);
	pila->retPos = pcb->program_counter;
	//pila->retVar
	//TODO:Aca hay que poner la fucking RETVAR
	// tiene que buscar la etiqueta en la lista de etiquetas y conseguir la direccion
	//char* instruccionBuscar = string_substring(nombre_etiqueta, 0, strlen(nombre_etiqueta) - 1);

	t_puntero_instruccion direccionEtiqueta = metadata_buscar_etiqueta(nombre_etiqueta, pcb->etiquetas, pcb->etiquetas_size);
	IndiceCodigo* indiceCodigo = list_get(pcbEjecutar->codigo, direccionEtiqueta);

	pcb->program_counter = indiceCodigo->program_counter - 1;

	int pagina = 0, offset = 0;
	deserializar_puntero(direccionRetorno, &pagina, &offset, tamanio_pagina_memoria);

	pila->retVar = malloc(sizeof(ReturnVariable));
	pila->retVar->byte_inicial = offset;
	pila->retVar->pagina = pagina;
	pila->retVar->tamanio = 4;

}

t_valor_variable OBTENER_VALOR_COMPARTIDA(t_nombre_compartida variable) {
	string_substring(variable, 0, strlen(variable) - 2);
	t_valor_variable valorVariable;
	if ((obtener_valor_de_variable_compartida_en_kernel(variable, &valorVariable)) == 0) {
		return valorVariable;
	} else {
		printf("Ocurrio un error al buscar la variable %s\n", variable);
		return -1;
	}
}

t_valor_variable ASIGNAR_VALOR_COMPARTIDA(t_nombre_compartida variable, t_valor_variable valor) {
	if ((asignar_valor_a_variable_compartida_en_kernel(variable, valor)) == 0) {
		printf("Asignacion exitosa!!\n");
		return valor;
	} else {
		printf("Ocurrio un error al asiignar el valor a la variable\n");
		return valor;
	}
}

t_descriptor_archivo ABRIR_ARCHIVO_PRIM(t_direccion_archivo direccion, t_banderas flags) {
	char* mensaje = abrir_archivo(string_itoa(pcb->PID), direccion, flags.creacion, flags.lectura, flags.escritura);

	int i = 0;
	bool controlNumerico = 1;
	for (i = 0; i < strlen(mensaje); i++) {
		if (!isdigit(mensaje[i])) {
			controlNumerico = 0;
			i = strlen(mensaje);
		}
	}

	if (controlNumerico == 0) {
		lanzar_excepcion(mensaje);
		return 0;
	} else {
		return atoi(mensaje);
	}
}

void BORRAR_ARCHIVO_PRIM(t_descriptor_archivo descriptor_archivo) {
	char* mensaje = borrar_archivo(string_itoa(pcb->PID), descriptor_archivo);
	if (strcmp("OK", mensaje) != 0)
		lanzar_excepcion(mensaje);

}

void CERRAR_ARCHIVO_PRIM(t_descriptor_archivo descriptor_archivo) {
	char* mensaje = cerrar_archivo(string_itoa(pcb->PID), descriptor_archivo);
	if (strcmp("OK", mensaje) != 0)
		lanzar_excepcion(mensaje);
}

void MOVER_CURSOR_PRIM(t_descriptor_archivo descriptor_archivo, t_valor_variable posicion) {
	char* mensaje = mover_cursor_archivo(string_itoa(pcb->PID), descriptor_archivo, posicion);
	if (strcmp("OK", mensaje) != 0)
		lanzar_excepcion(mensaje);
}

void ESCRIBIR_ARCHIVO_PRIM(t_descriptor_archivo descriptor_archivo, void* informacion, t_valor_variable tamanio) {

	if (descriptor_archivo < 3) {
		CU_Escribir_Pantalla_AnSISOP(informacion, string_itoa(pcb->PID));
	} else {

		char* mensaje = escribir_archivo(string_itoa(pcb->PID), descriptor_archivo, tamanio, informacion);
		if (strcmp("OK", mensaje) != 0)
			lanzar_excepcion(mensaje);
	}

}
/*
 void LEER_ARCHIVO_PRIM(t_descriptor_archivo descriptor_archivo, t_puntero informacion, t_valor_variable tamanio) {
 char* mensaje = leer_archivo(string_itoa(pcb->PID), descriptor_archivo, tamanio);
 //if (strcmp("OK", mensaje) != 0)
 //		lanzar_excepcion(mensaje);
 int pagina = 0, offset = 0;
 deserializar_puntero(informacion, &pagina, &offset, tamanio_pagina_memoria);
 offset+=5;
 almacenar_Bytes_de_Pagina(string_itoa(pcb->PID), string_itoa(pagina), string_itoa(offset), string_itoa(tamanio), -1,mensaje);


 }*/
void LEER_ARCHIVO_PRIM(t_descriptor_archivo descriptor_archivo, t_puntero informacion, t_valor_variable tamanio) {
	char* mensaje = leer_archivo(string_itoa(pcb->PID), descriptor_archivo, tamanio);
	//if (strcmp("OK", mensaje) != 0)
	//		lanzar_excepcion(mensaje);
	int pagina = 0, offset = 0;
	deserializar_puntero(informacion, &pagina, &offset, tamanio_pagina_memoria);
	char* valorPuntero = solicitar_bytes_memoria(string_itoa(pcb->PID), string_itoa(pagina), string_itoa(offset), "4");
	if(pagina < pcb->cantidad_paginas_codigo){
		deserializar_puntero(*((int*)valorPuntero), &pagina, &offset, tamanio_pagina_memoria);
	}
	//deserializar_puntero(*((int*)valorPuntero), &pagina, &offset, tamanio_pagina_memoria);
	offset += 5;
	almacenar_Bytes_de_Pagina(string_itoa(pcb->PID), string_itoa(pagina), string_itoa(offset), string_itoa(tamanio), -1, mensaje);

}
