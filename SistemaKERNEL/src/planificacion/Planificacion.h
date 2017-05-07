/*
 * Planificacion.h
 *
 *  Created on: 22/4/2017
 *      Author: utnso
 */

#ifndef HEADER_PLANIFICACION_H_
#define HEADER_PLANIFICACION_H_

#include "commons/collections/queue.h"
#include "../header/PCB.h"
#include "../header/Estructuras.h"

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



t_list lista_CPU;



void* EJECUTAR_ALGORITMO_PLANIFICACION();

void inicializar_lista_CPU();


void inicializar_colas_5_estados();

void mover_PCB_de_cola(PCB* pcb,COLA origen, COLA destino);

CPUInfo* obtener_CPU_Disponible();

PCB*  obtener_proceso_de_cola_NEW();



void enviar_PCB_Serializado_a_CPU(CPUInfo* cpu, PCB* pcb);

void marcar_CPU_Ocupada(CPUInfo* cpu);

void* ejecutar_algoritmo_planificacion_FIFO();

#endif /* HEADER_PLANIFICACION_H_ */
