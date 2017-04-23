/*
 * Planificacion.h
 *
 *  Created on: 22/4/2017
 *      Author: utnso
 */

#ifndef HEADER_PLANIFICACION_H_
#define HEADER_PLANIFICACION_H_

#include "commons/collections/queue.h"
#include "PCB.h"


t_queue* COLA_NEW;
t_queue* COLA_READY;
t_queue* COLA_EJECUTANDO;
t_queue* COLA_EXIT;
//t_queue* COLA_SUSPENDIDO;
typedef enum {
	  NEW = 1,
	  READY = 2,
	  EJECUTANDO= 3,
	  EXIT= 4
} COLA;

typedef struct{

}CPU;

t_list lista_CPU;





void inicializar_lista_CPU();


void inicializar_colas_5_estados();

void mover_PCB_de_cola(PCB* pcb,COLA origen, COLA destino);

CPU* obtener_CPU_Disponible();

PCB*  obtener_proceso_de_cola_NEW();

void* ejecutar_algoritmo_planificacion_FIFO();

void enviar_PCB_Serializado_a_CPU(CPU* cpu, PCB* pcb);

void marcar_CPU_Ocupada(CPU* cpu);

#endif /* HEADER_PLANIFICACION_H_ */
