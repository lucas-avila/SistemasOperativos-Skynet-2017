/*
 * InterfazKernel.h
 *
 *  Created on: 1/4/2017
 *      Author: utnso
 */

#ifndef HEADER_INTERFAZKERNEL_H_
#define HEADER_INTERFAZKERNEL_H_


typedef struct {
	//lista de sentencias
	//lista de pila
	//lista de etiquetas
} Programa_Enviar;



void enviar_programa_a_kernel(Programa_Enviar programa); //Envia el Request lleno
char *recibir_PID_programa_de_kernel(); //llena el Response




void enviar_solicitud_cancelar_proceso();


//Este directamente recibe string string string
void recibir_mensajes_consola();

#endif /* HEADER_INTERFAZKERNEL_H_ */
