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
#include "../administrarProcesos/Proceso.h"

t_queue* COLA_NEW;
t_queue* COLA_WAITING;
t_queue* COLA_READY;
t_queue* COLA_EXEC;
t_queue* COLA_EXIT;
//t_queue* COLA_SUSPENDIDO;

t_list lista_CPU;

void EJECUTAR_ALGORITMO_PLANIFICACION();

void inicializar_lista_CPU();

void inicializar_colas_5_estados();

void proceso_a_NEW(Proceso * p);

void mover_PCB_de_cola(PCB* pcb, COLA origen, COLA destino);

CPUInfo* obtener_CPU_Disponible();

PCB* obtener_proceso_de_cola_READY();

void enviar_PCB_Serializado_a_CPU(CPUInfo* cpu, PCB* pcb);

void marcar_CPU_Ocupada(CPUInfo* cpu);

void recepcion_PCB_en_COLA_EXIT();

void planificador_mediano_plazo();

void recibir_PCB_de_CPU(int clienteCPU, char * modo);

#endif /* HEADER_PLANIFICACION_H_ */
