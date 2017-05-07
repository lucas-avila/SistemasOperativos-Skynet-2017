/*
 * InterfazKernel.c
 *
 *  Created on: 29/4/2017
 *      Author: utnso
 */

#include <stdio.h>
#include "Socket.h"
#include "funcionesUtiles.h"

void CU_handshake_consola(int kernel){

	enviar_dato_serializado("CONSOLA", kernel);
}

void CU_handshake_programa(int kernel){

	enviar_dato_serializado("INICIAR_PROGRAMA", kernel);
}

char * enviar_programa_ANSISOP(char * path_archivo_fuente, int kernel){

	char * literal;

	literal = obtener_codigo(path_archivo_fuente);
	enviar_dato_serializado(literal, kernel);

	return recibir_dato_serializado(kernel);
}
