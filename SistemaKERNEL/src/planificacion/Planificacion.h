/*
 * Planificacion.h
 *
 *  Created on: 22/4/2017
 *      Author: utnso
 */

#ifndef HEADER_PLANIFICACION_H_
#define HEADER_PLANIFICACION_H_

#include <commons/collections/dictionary.h>
#include <commons/collections/queue.h>
#include <Sharedlib/PCB.h>

#include "../administrarProcesos/Proceso.h"
#include "../header/Estructuras.h"


t_dictionary * BUFFER_CODIGO;
t_dictionary * COLAS;
static const char NEW[] = "NEW";
static const char READY[] = "READY";
static const char WAITING[] = "WAITING";
static const char EXEC[] = "EXEC";
static const char EXIT[] = "EXIT";
//t_queue* COLA_SUSPENDIDO;

void EJECUTAR_ALGORITMO_PLANIFICACION();

void inicializar_colas_semaforos();

void inicializar_colas_5_estados();

t_queue* cola(char * nombre);

void proceso_a_NEW(Proceso * p);

int mover_PCB_de_cola(PCB* pcb, char * origen, char * destino);

CPUInfo* obtener_CPU_Disponible();

int cantidad_en_WAITING();

int cantidad_procesos_en_memoria();

void planificador_largo_plazo();

PCB* obtener_proceso_de_cola_READY();

void enviar_PCB_Serializado_a_CPU(CPUInfo* cpu, PCB* pcb);

void marcar_CPU_Ocupada(CPUInfo* cpu);

void marcar_CPU_Disponible(CPUInfo* cpu);

void recepcion_PCB_en_COLA_EXIT();

int recepcion_SIGNAL_semaforo_ansisop(char * nombre_sem);

void recibir_PCB_de_CPU(int clienteCPU, char * modo);

void wait_cola(char * cola);

void signal_cola(char * cola);

void limpiar_procesos_CPU(int clienteCPU, char * modo);

#endif /* HEADER_PLANIFICACION_H_ */
