/*
 * InterfazKernel.c
 *
 *  Created on: 29/4/2017
 *      Author: utnso
 */

#include <stdio.h>
#include "Socket.h"
#include "funcionesUtiles.h"

int consola_ID;

void CU_handshake_consola(int kernel){

	enviar_dato_serializado("CONSOLA", kernel);
	consola_ID = atoi(recibir_dato_serializado(kernel));
	printf("%d\n", consola_ID);
}

void CU_handshake_programa(int kernel_programa){

	enviar_dato_serializado("INICIAR_PROGRAMA", kernel_programa);
	enviar_dato_serializado(string_itoa(consola_ID), kernel_programa);
}

char * enviar_programa_ANSISOP(char * path_archivo_fuente, int kernel_programa){

	char * literal;

	literal = obtener_codigo(path_archivo_fuente);
	enviar_dato_serializado(literal, kernel_programa);

	return recibir_dato_serializado(kernel_programa);
}
