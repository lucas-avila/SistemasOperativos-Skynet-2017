#include "Planificacion.h"

#include <commons/collections/list.h>
#include <semaphore.h>
#include <string.h>

#include "../general/Semaforo.h"
#include "../header/AppConfig.h"
#include "PlanificacionFIFO.h"
#include "PlanificacionRR.h"

void EJECUTAR_ALGORITMO_PLANIFICACION() {
	if (strcmp(configuraciones.ALGORITMO, "FIFO") == 0) {
		dispatcher_FIFO();
	} else if (strcmp(configuraciones.ALGORITMO, "RR") == 0) {
		dispatcher_RR();
	}
}

void inicializar_colas_5_estados() {
	COLA_NEW = queue_create();
	COLA_READY = queue_create();
	COLA_WAITING = queue_create();
	COLA_EXEC = queue_create();
	COLA_EXIT = queue_create();
}

t_queue* obtener_cola(COLA nombreCola) {
	switch (nombreCola) {
	case 1:
		return COLA_NEW;
	case 2:
		return COLA_READY;
	case 3:
		return COLA_EXEC;
	case 4:
		return COLA_EXIT;
	case 5:
		return COLA_WAITING;
	}
	return NULL;
}

void proceso_a_NEW(Proceso * p){
	p->cola = NEW;
	queue_push(obtener_cola(NEW), p->pcb);
}

void mover_PCB_de_cola(PCB* pcb, COLA origen, COLA destino) {
	queue_pop(obtener_cola(origen));
	queue_push(obtener_cola(destino), pcb);
	if(destino == EXIT){
		pcb->exit_code = 0;
		finalizar_proceso(pcb);
	}

	Proceso * proceso = buscar_proceso_by_PID(pcb->PID);
	proceso->cola = destino;
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

void planificador_mediano_plazo() {
	while (configuraciones.planificacion_activa == 1) {

		if (configuraciones.GRADO_MULTIPROG > (queue_size(COLA_READY) + queue_size(COLA_EXEC) + queue_size(COLA_WAITING))) {
			if (!queue_is_empty(COLA_NEW)) {
				//mover_PCB_de_cola(queue_peek(COLA_NEW), NEW, READY);
			}
		}
	}
}

PCB* obtener_proceso_de_cola_READY() {
	while (configuraciones.planificacion_activa == 1) {
		//SE CONTROLA LA MULTIPROGRAMACION DE ESTA MANERA
		if (!queue_is_empty(COLA_READY)) {
			sem_wait(&mutex_cola_READY);
			PCB* pcb = queue_peek(COLA_READY);
			sem_post(&mutex_cola_READY);
			return pcb;
		}
	}
	return NULL;
}

void enviar_PCB_Serializado_a_CPU(CPUInfo* cpu, PCB* pcb) {
	enviar_pcb(pcb, cpu->numeroConexion);
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
	}else {
		recepcion_PCB_en_COLA_EXIT_RR();
	}
}

void recibir_PCB_de_CPU(int clienteCPU, char * modo) {
	PCB* pcb = recibir_pcb(clienteCPU);

	if(strcmp(modo, "TERMINADO") == 0){
		mover_PCB_de_cola(pcb, EXEC, EXIT);
	} else if(strcmp(modo, "QUANTUM") == 0){
		mover_PCB_de_cola(pcb, EXEC, READY);
	} else if(strcmp(modo, "WAITING_ARCHIVO") == 0){
		mover_PCB_de_cola(pcb, EXEC, WAITING);
	} else if(strcmp(modo, "WAIT_SEM") == 0){
		char * nombre_sem = recibir_dato_serializado(clienteCPU);

	}

}

