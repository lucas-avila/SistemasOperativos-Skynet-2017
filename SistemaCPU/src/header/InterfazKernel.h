/*
 * InterfazKernel.h
 *
 *  Created on: 1/4/2017
 *      Author: utnso
 */

#ifndef HEADER_INTERFAZKERNEL_H_
#define HEADER_INTERFAZKERNEL_H_


typedef struct{
   int pid;
   int cantidad_paginas;
} PCB;

/* se encarga de recibir y llenar toda la estructura struct PCB */
PCB * recibir_pcb(int s_origen);

/* Se encarga de devolver al kernel el PCB con sus modificaciones por la ejecucion*/
void enviar_PCB_a_kernel(int conexionKernel,PCB pcb);

void enviar_SYSCALL_solicitar_memoria_dinamica_a_kernel(int conexionKernel,PCB pcb);

void enviar_SYSCALL_liberar_memoria_dinamica_a_kernel(int conexionKernel,PCB pcb);

void enviar_SYSCALL_operacion_crear_archivo_a_kernel(int conexionKernel,PCB pcb);

void enviar_SYSCALL_operacion_leer_archivo_a_kernel(int conexionKernel,PCB pcb);

void enviar_SYSCALL_operacion_guardar_datos_archivo_a_kernel(int conexionKernel,PCB pcb);

void enviar_SYSCALL_operacion_borrar_archivo_a_kernel(int conexionKernel,PCB pcb);



#endif /* HEADER_INTERFAZKERNEL_H_ */
