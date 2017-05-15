/*
 * InterfazKernel.h
 *
 *  Created on: 1/4/2017
 *      Author: utnso
 */

#ifndef HEADER_INTERFAZKERNEL_H_
#define HEADER_INTERFAZKERNEL_H_

#include "../header/PCB.h"
#include "../primitivas/PrimitivasFunciones.h"

/* se encarga de recibir y llenar toda la estructura struct PCB */
PCB* recibir_PCB_de_kernel();

/* Se encarga de devolver al kernel el PCB con sus modificaciones por la ejecucion*/
void enviar_PCB_a_kernel(PCB* pcb);

char* enviar_SYSCALL_solicitar_memoria_dinamica_a_kernel(int PID, int espacio);

char* enviar_SYSCALL_liberar_memoria_dinamica_a_kernel(DireccionMemoriaDinamica* varDinamica);

void enviar_SYSCALL_operacion_crear_archivo_a_kernel(int PID, PCB* variableLiberar);

void enviar_SYSCALL_operacion_leer_archivo_a_kernel(int PID, PCB* pcb);

void enviar_SYSCALL_operacion_guardar_datos_archivo_a_kernel( int PID, PCB* pcb);

void enviar_SYSCALL_operacion_borrar_archivo_a_kernel(int  PID, PCB* pcb);

DireccionVariable* obtener_valor_Variable_Compartida(int  PID, char *nombreVariableCompartida);

#endif /* HEADER_INTERFAZKERNEL_H_ */
