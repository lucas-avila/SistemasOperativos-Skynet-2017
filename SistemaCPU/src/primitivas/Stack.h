/*
 * Stack.h
 *
 *  Created on: 30/4/2017
 *      Author: utnso
 */

#ifndef PRIMITIVAS_STACK_H_
#define PRIMITIVAS_STACK_H_

#include "../../../Sharedlib/Sharedlib/PCB.h"
#include "commons/collections/list.h"
#include "PrimitivasFunciones.h"
void insertar_nueva_fila_Indice_Stack(PCB* pcb);

void eliminar_fila_Indice_Stack(PCB* pcb, IndiceStack* pila);

IndiceStack* obtener_Ultima_fila_Indice_Stack(PCB* pcb);

IndiceStack* crear_argumento_en_Indice_Stack(IndiceStack* pila, Argumento* argumento);

IndiceStack* crear_variable_en_Indice_Stack(IndiceStack* pila, Variable* variable);

IndiceStack* crear_variable_retorno_en_Indice_Stack(IndiceStack* pila, ReturnVariable* variable_retorno);

Variable* crear_variable(char id, unsigned int pagina, unsigned int byte_inicial, unsigned int tamanio,int dinamica) ;

ReturnVariable* crear_return_variable(unsigned int pagina, unsigned int byte_inicial, unsigned int tamanio);

Argumento* crear_argumento(char id, unsigned int pagina, unsigned int byte_inicial, unsigned int tamanio);

Variable* obtener_Ultima_variable_declarada(t_list* pila);

PunteroVariable* buscar_posicion_variable_por_nombre(IndiceStack* pila, t_nombre_variable variable);

Variable* buscar_variable_por_stack_y_fila(PCB* pcb, unsigned int stack, unsigned int filaTabla);
Argumento* buscar_argumento_por_stack_y_fila(PCB* pcb, unsigned int stack, unsigned int filaTabla);

void asingar_espacio_memoria_variable(PCB* pcb, Variable* var, int filaStack, int filaTabla);

Variable* buscar_variable_stack_por_nombre(PCB* pcb, char nombre);
Argumento* buscar_argumento_stack_por_nombre(PCB* pcb, char nombre);


#endif /* PRIMITIVAS_STACK_H_ */
