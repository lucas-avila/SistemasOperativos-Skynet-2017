#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include "../planificacion/Planificacion.h"
#include "commons/collections/queue.h"
#include "../interfaz/InterfazMemoria.h"
PCB* procesoPrueba;

void mostrar_menu_planificacion() {
	int opcion = 0;
	do {

		printf("\n1 - Ver Colas");
		printf("\n2 - Enviar PCB a CPU Serializado");
		printf("\n3 - Salir");
		printf("\nOpcion: ");

		opcion = validarNumeroInput(1, 3);
		switch (opcion) {

		case 1:
			mostrar_estado_colas();
			break;
		case 2:
			crear_PCB_y_ENVIAR();
			break;
		}
	} while (opcion != 3);
}

void mostrar_datos_cola(t_queue* cola) {

	t_list* elements = cola->elements;
	int tamanio = list_size(elements);
	int i = 0;
	for (i = 0; i < tamanio; i++) {
		PCB* pcb = list_get(elements, i);
		printf(" %s ", pcb->PID);

	}
}

void mostrar_estado_colas() {
	int tiempo = 0;
	while (getchar() != '0') {
		system("clear");
		tiempo++;
		if (tiempo >= 99999) {
			tiempo = 0;
		}
		printf("\n\n Tiempo: %d", tiempo);
		printf("\n\tCOLA NEW: ");
		mostrar_datos_cola(COLA_NEW);
		printf("\n\tCOLA READY: ");
		mostrar_datos_cola(COLA_READY);
		printf("\n\tCOLA EJECUTANDO: ");
		mostrar_datos_cola(COLA_EXEC);
		printf("\n\tCOLA EXIT: ");
		mostrar_datos_cola(COLA_EXIT);
		printf("\n\n Presione 0 para terminar");
		sleep(1);

	}
}
void crear_PCB_y_ENVIAR() {
	crear_PCB_TEST();
	enviar_pcb(procesoPrueba, ((CPUInfo*) list_get(lista_CPUs, 0))->numeroConexion);
	//enviar_pcb(procesoPrueba, 0);
	free(procesoPrueba);
}

IndiceCodigo* crearIndiceCodigo(int programCounter, int byteInicial, int tamanio, int pagina) {
	IndiceCodigo* indice1 = malloc(sizeof(IndiceCodigo));
	indice1->byte_inicial_codigo = byteInicial;
	indice1->byte_final_codigo = byteInicial + tamanio;
	indice1->pagina = pagina;
	indice1->program_counter = programCounter;
	return indice1;
}

int numProceso = 1241;
void crear_PCB_TEST() {
	procesoPrueba = malloc(sizeof(PCB));
	procesoPrueba->PID = numProceso++;
	procesoPrueba->program_counter = 0;
	procesoPrueba->cantidad_paginas_codigo = 1;
	procesoPrueba->cantidad_rafagas_ejecutadas=0;
	procesoPrueba->pila = list_create();

	IndiceStack* filaInicial = malloc(sizeof(IndiceStack));
	filaInicial->argumentos = list_create();
	filaInicial->variables = list_create();
	filaInicial->posicion = 0;
	list_add(procesoPrueba->pila, filaInicial);
	procesoPrueba->pagina_inicial_stack = atoi(asignar_Paginas_Programa(string_itoa(procesoPrueba->PID), "1"));

	procesoPrueba->RR = 0;

	/****** HACEMOS DE CUENTA QUE CARGAMOS NOSOTROS EL CODIGO FUENTE*******/
	int paginaSentencia = atoi(asignar_Paginas_Programa(string_itoa(procesoPrueba->PID), "1"));
	char sentencias[][40] = { "variables a, b, x,d,e,f,g,h,i,j,k,l", "a = 3", "b = 5", "a = b + 12", "alocar x 50 ", "liberar x", "alocar d 50 ", "liberar d", "alocar e 50 ", "liberar e", "alocar f 50 ", "liberar f", "alocar g 50 ", "liberar g", "alocar h 50 ", "liberar h", "alocar i 50 ", "liberar i", "alocar j 50 ", "liberar j", "alocar k 50 ", "liberar k", "alocar l 50 ", "liberar l" };/**
	 * variables a, b
	 a = 3
	 b = 5
	 a = b + 12
	 */
	int cantidadSentencias = 11;
	int i = 0;
	int indiceInicial = 0;
	procesoPrueba->codigo = list_create();
	for (i = 0; i < cantidadSentencias; i++) {
		IndiceCodigo* indiceNuevo = crearIndiceCodigo(i, indiceInicial, strlen(sentencias[i]), paginaSentencia);
		indiceInicial = indiceInicial + strlen(sentencias[i]);
		list_add(procesoPrueba->codigo, indiceNuevo);

		almacenar_Bytes_de_Pagina(string_itoa(procesoPrueba->PID), string_itoa(indiceNuevo->pagina), string_itoa(indiceNuevo->byte_inicial_codigo), string_itoa(indiceNuevo->byte_final_codigo - indiceNuevo->byte_inicial_codigo), sentencias[i]);
	}

	procesoPrueba->etiquetas = string_new();
	string_append(&procesoPrueba->etiquetas, "holachaunorevimos");
	procesoPrueba->etiquetas_size = strlen(procesoPrueba->etiquetas);

}
