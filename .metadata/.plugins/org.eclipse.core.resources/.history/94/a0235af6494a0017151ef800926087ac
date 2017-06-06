/*
 * PrimitivasFunciones.h
 *
 *  Created on: 30/4/2017
 *      Author: utnso
 */

#ifndef PRIMITIVAS_PRIMITIVASFUNCIONES_H_
#define PRIMITIVAS_PRIMITIVASFUNCIONES_H_

#include "../header/PCB.h"
#include "EstructurasDeDatosPrimitivas.h"

void setearPCB(PCB* pcbEnviado);
/**
 - Reservar espacio para Int (4bytes) en el stack del pcb.
 - Guardar la info de la variable y la posición de memoria en la estructura del stack
 - Devolver a la memoria para que impacte el cambio
 */
t_puntero DEFINIR_VARIABLE(t_nombre_variable variable);
/**
 - Buscar en la estructura del stack la dirección de la variable
 - devolver dirección.
 -  '$0' significa el valor de la variable en la primera posición del stack.
 */
t_puntero OBTENER_DIRECCION_DE_VARIABLE(t_nombre_variable variable);
/**
 - Buscar variable en el stack
 - Copiar valor en la estructura
 - Devolver a la memoria para que impacte el cambio.
 */
void ASIGNAR_VARIABLE(t_puntero direccion_variable, t_valor_variable valor);

char* OBTENER_VALOR_VARIABLE(PCB* pcb, char* variable);
t_valor_variable DEREFERENCIAR(t_puntero puntero);

t_puntero ALOCAR(t_valor_variable espacio);

void LIBERAR(t_puntero puntero);

void IR_A_LABEL (t_nombre_etiqueta nombreEtiqueta);

void LLAMAR_SIN_RETORNO (t_nombre_etiqueta nombreEtiqueta );

void LLAMAR_CON_RETORNO(t_nombre_etiqueta nombre_etiqueta, t_puntero direccionRetorno );

void RETORNAR(t_valor_variable variableRetorno);

void FINALIZAR();

t_valor_variable OBTENER_VALOR_COMPARTIDA(t_nombre_compartida variable);

t_valor_variable ASIGNAR_VALOR_COMPARTIDA(t_nombre_compartida variable, t_valor_variable valor);
#endif /* PRIMITIVAS_PRIMITIVASFUNCIONES_H_ */
