#include <commons/collections/list.h>
#include <commons/collections/queue.h>
#include <commons/string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../administrarPCB/EstadisticaProceso.h"
#include "../administrarPCB/PCBData.h"
#include "../administrarProcesos/Proceso.h"
#include "../general/funcionesUtiles.h"
#include "../header/PCB.h"
#include "../interfaz/InterfazMemoria.h"
#include "../planificacion/Planificacion.h"

PCB * crear_PCB_TEST_2();

void mostrar_menu_planificacion() {
	int opcion = 0;
	do {

		printf("\n1 - Ver Colas");
		printf("\n2 - Crear PCB y testear planificacion");
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
		mostrar_datos_cola(cola(NEW));
		printf("\n\tCOLA READY: ");
		mostrar_datos_cola(cola(READY));
		printf("\n\tCOLA EJECUTANDO: ");
		mostrar_datos_cola(cola(EXEC));
		printf("\n\tCOLA EXIT: ");
		mostrar_datos_cola(cola(EXIT));
		printf("\n\n Presione 0 para terminar");
		sleep(1);

	}
}
void crear_PCB_y_ENVIAR() {
	printf("Test con efecto de lado, todos los procesos creados aca siguen estando luego en el sistema\n");
	PCB * pcb_nuevo = crear_PCB_TEST_2();

	Proceso * proceso_nuevo = new_Proceso(pcb_nuevo);
	crear_Proceso_en_tabla(proceso_nuevo->PID);
	agregar_proceso(proceso_nuevo);

	proceso_a_NEW(proceso_nuevo);

}

IndiceCodigo* crearIndiceCodigo(int programCounter, int byteInicial, int tamanio, int pagina) {
	IndiceCodigo* indice1 = malloc(sizeof(IndiceCodigo));
	indice1->byte_inicial_codigo = byteInicial;
	indice1->byte_final_codigo = byteInicial + tamanio;
	indice1->pagina = pagina;
	indice1->program_counter = programCounter;
	return indice1;
}

int numProceso = 1888;

void crear_PCB_TEST();
PCB * crear_PCB_TEST_2() {
	PCB * pcbDePrueba = crear_pcb();

	IndiceStack* filaInicial = malloc(sizeof(IndiceStack));
	filaInicial->argumentos = list_create();
	filaInicial->variables = list_create();
	filaInicial->posicion = 0;
	list_add(pcbDePrueba->pila, filaInicial);
	pcbDePrueba->pagina_inicial_stack = 0;

	pcbDePrueba->RR = 0;

	/****** HACEMOS DE CUENTA QUE CARGAMOS NOSOTROS EL CODIGO FUENTE*******/
	int paginaSentencia = 0;
	char sentencias[][40] = { "variables a, b, x,d,e,f,g,h,i,j,k,l", "a = 3", "b = 5", "a = b + 12", "alocar x 50 ", "liberar x", "alocar d 50 ", "liberar d", "alocar e 50 ", "liberar e", "alocar f 50 ", "liberar f", "alocar g 50 ", "liberar g", "alocar h 50 ", "liberar h", "alocar i 50 ", "liberar i", "alocar j 50 ", "liberar j", "alocar k 50 ", "liberar k", "alocar l 50 ", "liberar l" };/**
	 * variables a, b
	 a = 3
	 b = 5
	 a = b + 12
	 */
	int cantidadSentencias = 11;
	int i = 0;
	int indiceInicial = 0;
	pcbDePrueba->codigo = list_create();
	for (i = 0; i < cantidadSentencias; i++) {
		IndiceCodigo* indiceNuevo = crearIndiceCodigo(i, indiceInicial, strlen(sentencias[i]), paginaSentencia);
		indiceInicial = indiceInicial + strlen(sentencias[i]);
		list_add(pcbDePrueba->codigo, indiceNuevo);


	}

	pcbDePrueba->etiquetas = string_new();
	string_append(&pcbDePrueba->etiquetas, "holachaunorevimos");
	pcbDePrueba->etiquetas_size = strlen(pcbDePrueba->etiquetas);

	return pcbDePrueba;

}
