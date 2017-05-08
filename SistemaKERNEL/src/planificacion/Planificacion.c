#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "Planificacion.h"
#include "PlanificacionFIFO.h"
#include "../header/AppConfig.h"
#include "../header/PCB.h"
#include "../header/Estructuras.h"

#include "../administrarPCB/EstadisticaProceso.h"

void EJECUTAR_ALGORITMO_PLANIFICACION() {
	if (strcmp(configuraciones.ALGORITMO, "FIFO") == 0) {
		ejecutar_algoritmo_planificacion_FIFO();

	} /*else   if(strcmp(configuraciones.ALGORITMO,"RR")==0){
	 ejecutar_algoritmo_planificacion_FIFO();

	 }*/
}

void inicializar_colas_5_estados() {
	COLA_NEW = queue_create();
	COLA_READY = queue_create();
	COLA_EJECUTANDO = queue_create();
	COLA_EXIT = queue_create();
}

t_queue* obtener_cola(COLA nombreCola) {
	switch (nombreCola) {
	case 1:
		return COLA_NEW;
	case 2:
		return COLA_READY;
	case 3:
		return COLA_EJECUTANDO;
	case 4:
		return COLA_EXIT;
	}
	return NULL;
}

void mover_PCB_de_cola(PCB* pcb, COLA origen, COLA destino) {
	queue_pop(obtener_cola(origen));
	queue_push(obtener_cola(destino), pcb);
}

CPUInfo* obtener_CPU_Disponible() {
	int i = 0;
	while (configuraciones.planificacion_activa == 1) {
		int tamanioLista = list_size(lista_CPUs);
		for (i = 0; i < tamanioLista; i++) {
			CPUInfo* cpu_element = list_get(lista_CPUs, i);
			if (cpu_element->disponible == 1) {
				return cpu_element;
			}
		}
	}
	return NULL;
}

PCB* obtener_proceso_de_cola_NEW() {
	while (configuraciones.planificacion_activa == 1) {
		//SE CONTROLA LA MULTIPROGRAMACION DE ESTA MANERA
		if (configuraciones.GRADO_MULTIPROG > queue_size(COLA_READY)) {
			if (!queue_is_empty(COLA_NEW)) {
				//TODO: Semaforo
				PCB* pcb = queue_pop(COLA_NEW);
				//TODO: Fin Semaforo
				return pcb;
			}
		}
	}
	return NULL;
}

void enviar_PCB_Serializado_a_CPU(CPUInfo* cpu, PCB* pcb) {
	//TODO: Invocar la funcion de POLO y DELL que envia todaaaaa la serializacion del PCB a la CPU.
}

//TODO: VOLVER A VERIFICAR SI ESTA FUNCION ANDA BIEN, Y SI NECESITA SEMAFOROS.
void marcar_CPU_Ocupada(CPUInfo* cpu) {
	int i = 0;
	int tamanioLista = list_size(lista_CPUs);
	for (i = 0; i < tamanioLista; i++) {
		CPUInfo* cpu_element = list_get(lista_CPUs, i);
		if (cpu_element->numeroConexion == cpu->numeroConexion) {
			cpu_element->disponible = 0;
			list_replace(lista_CPUs, i, cpu_element);
			return;
		}
	}

}
/**
 * Es cuando el proceso ya llega a la cola EXIT
 */
void recepcion_PCB_en_COLA_EXIT() {
	if (strcmp(configuraciones.ALGORITMO, "FIFO") == 0) {
		recepcion_PCB_en_COLA_EXIT_FIFO();

	}

}
