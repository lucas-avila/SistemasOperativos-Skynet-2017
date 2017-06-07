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
void enviar_PCB_a_kernel(PCB* pcb, char * modo);

char* enviar_SYSCALL_solicitar_memoria_dinamica_a_kernel(int PID, int espacio);

char* enviar_SYSCALL_liberar_memoria_dinamica_a_kernel(DireccionMemoriaDinamica* varDinamica);

void enviar_SYSCALL_operacion_crear_archivo_a_kernel(int PID, PCB* variableLiberar);

void enviar_SYSCALL_operacion_leer_archivo_a_kernel(int PID, PCB* pcb);

void enviar_SYSCALL_operacion_guardar_datos_archivo_a_kernel( int PID, PCB* pcb);

void enviar_SYSCALL_operacion_borrar_archivo_a_kernel(int  PID, PCB* pcb);

int asignar_valor_a_variable_compartida_en_kernel(char* nombre_varComp, int valor_varComp);

int obtener_valor_de_variable_compartida_en_kernel(char* nombre_varComp, t_valor_variable* valorVariable);


//DESDE ACÁ FALATA IMPLEMENTAR ESTAS FUNCIONES, TAMBIEN INVOCARLAS EN LAS PRIMITIVAS.
char* abrir_archivo(char* PID, char* pathArchivo, bool flagCreate, bool flagRead, bool flagWrite);

char* mover_cursor_archivo(char* PID, int FD, int cursor_bloque);

char* leer_archivo(char* PID, int FD, int tamanio);

char* escribir_archivo(char* PID, int FD, int tamanio, char* contenido);

char* cerrar_archivo(char* PID, int FD);

char* borrar_archivo(char* PID, char* rutaArchivo);

void CU_Escribir_Pantalla_AnSISOP(char* mensaje,char* PID);

#endif /* HEADER_INTERFAZKERNEL_H_ */
