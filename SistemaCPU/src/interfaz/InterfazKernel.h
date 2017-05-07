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
void recibir_PCB_de_kernel(int conexionKernel, PCB *pcb);

/* Se encarga de devolver al kernel el PCB con sus modificaciones por la ejecucion*/
void enviar_PCB_a_kernel(int conexionKernel, PCB pcb);

char* enviar_SYSCALL_solicitar_memoria_dinamica_a_kernel(char* PID, int espacio);

char* enviar_SYSCALL_liberar_memoria_dinamica_a_kernel(DireccionMemoriaDinamica* varDinamica);

void enviar_SYSCALL_operacion_crear_archivo_a_kernel(char* PID, Variable* variableLiberar);

void enviar_SYSCALL_operacion_leer_archivo_a_kernel(int conexionKernel, PCB pcb);

void enviar_SYSCALL_operacion_guardar_datos_archivo_a_kernel(int conexionKernel, PCB pcb);

void enviar_SYSCALL_operacion_borrar_archivo_a_kernel(int conexionKernel, PCB pcb);

DireccionVariable* obtener_valor_Variable_Compartida(int conexionKernel, char *nombreVariableCompartida);

#endif /* HEADER_INTERFAZKERNEL_H_ */
