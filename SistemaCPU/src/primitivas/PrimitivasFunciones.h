/*
 * PrimitivasFunciones.h
 *
 *  Created on: 30/4/2017
 *      Author: utnso
 */

#ifndef PRIMITIVAS_PRIMITIVASFUNCIONES_H_
#define PRIMITIVAS_PRIMITIVASFUNCIONES_H_

#include "../../../Sharedlib/Sharedlib/PCB.h"
#include "EstructurasDeDatosPrimitivas.h"
#include "parser/parser.h"
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

void WAIT(t_nombre_semaforo identificador_semaforo);

void SIGNAL(t_nombre_semaforo identificador_semaforo);

void LIBERAR(t_puntero puntero);

void IR_A_LABEL(t_nombre_etiqueta nombreEtiqueta);

void LLAMAR_SIN_RETORNO(t_nombre_etiqueta nombreEtiqueta);

void LLAMAR_CON_RETORNO(t_nombre_etiqueta nombre_etiqueta, t_puntero direccionRetorno);

void RETORNAR(t_valor_variable variableRetorno);

void FINALIZAR();

t_valor_variable OBTENER_VALOR_COMPARTIDA(t_nombre_compartida variable);

t_valor_variable ASIGNAR_VALOR_COMPARTIDA(t_nombre_compartida variable, t_valor_variable valor);
t_descriptor_archivo ABRIR_ARCHIVO_PRIM(t_direccion_archivo direccion, t_banderas flags);
void BORRAR_ARCHIVO_PRIM(t_descriptor_archivo descriptor_archivo);
void CERRAR_ARCHIVO_PRIM(t_descriptor_archivo descriptor_archivo);
void ESCRIBIR_ARCHIVO_PRIM(t_descriptor_archivo descriptor_archivo, void* informacion, t_valor_variable tamanio);
void LEER_ARCHIVO_PRIM(t_descriptor_archivo descriptor_archivo, t_puntero informacion, t_valor_variable tamanio);
void MOVER_CURSOR_PRIM(t_descriptor_archivo descriptor_archivo, t_valor_variable posicion);
char * intToChar4(int num);
#endif /* PRIMITIVAS_PRIMITIVASFUNCIONES_H_ */
