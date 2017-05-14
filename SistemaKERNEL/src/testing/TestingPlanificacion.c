#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include "../planificacion/Planificacion.h"
#include "commons/collections/queue.h"

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
		mostrar_datos_cola(COLA_EJECUTANDO);
		printf("\n\tCOLA EXIT: ");
		mostrar_datos_cola(COLA_EXIT);
		printf("\n\n Presione 0 para terminar");
		sleep(1);

	}
}
