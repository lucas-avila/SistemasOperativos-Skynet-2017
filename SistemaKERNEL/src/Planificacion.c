#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "header/Planificacion.h"
#include "header/AppConfig.h"
#include "header/PCB.h"
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
	queue_push(obtener_cola(destino),pcb);
}

CPU* obtener_CPU_Disponible() {
	while (configuraciones.planificacion_activa == 1) {
		//Funcion que aplica a cada elemento de la lista
		// si la cpu esta disponible o no
	}
	return NULL;
}

PCB* obtener_proceso_de_cola_NEW() {
	while (configuraciones.planificacion_activa == 1) {
		if (!queue_is_empty(COLA_NEW)) {
			//TODO: Semaforo
			PCB* pcb = queue_pop(COLA_NEW);
			//TODO: Fin Semaforo
			return pcb;
		}
	}
	return NULL;
}

void enviar_PCB_Serializado_a_CPU(CPU* cpu, PCB* pcb) {

}

void marcar_CPU_Ocupada(CPU* cpu) {

}

void* ejecutar_algoritmo_planificacion_FIFO() {
	PCB* pcb;
	CPU* cpu;
	while (configuraciones.planificacion_activa == 1) {
		pcb = obtener_proceso_de_cola_NEW();
		mover_PCB_de_cola(pcb, NEW, READY);
		cpu = obtener_CPU_Disponible();
		mover_PCB_de_cola(pcb, READY, EJECUTANDO);
		enviar_PCB_Serializado_a_CPU(cpu, pcb);
		marcar_CPU_Ocupada(cpu);
	}
}
