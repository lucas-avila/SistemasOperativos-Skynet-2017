/*
 * Ejecucion.h
 *
 *  Created on: 7/5/2017
 *      Author: utnso
 */

#ifndef PROCESADOR_EJECUCION_H_
#define PROCESADOR_EJECUCION_H_
#include "../../../Sharedlib/Sharedlib/PCB.h"
#include "parser/parser.h"
AnSISOP_funciones *funciones;
AnSISOP_kernel *kernel ;

PCB* pcbEjecutar;

void setPCBEjecucion(PCB* pcb);

void ejecutar_Programa();

void inicializar_contexto_ejecucion();

void ejecutar_programa_por_FIFO() ;

void ejecutar_programa_por_RR() ;

char* solicitar_sentencia_ejecutar();

void marcarFinDePrograma();

void marcarBloqueado();

void lanzar_excepcion(char * mensaje);

bool esFinPrograma;
bool programaBloqueado;
bool hubo_excepcion;

#endif /* PROCESADOR_EJECUCION_H_ */
