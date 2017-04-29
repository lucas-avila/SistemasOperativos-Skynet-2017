/*
 * InterfazKernel.c
 *
 *  Created on: 29/4/2017
 *      Author: utnso
 */

#include <stdio.h>
#include "Socket.h"
#include "funcionesUtiles.h"

void CU_handshake(int kernel){

	enviar_dato_serializado("CONSOLA", kernel);
	printf("--Handshake exitoso--\n");

}

char * enviar_programa_ANSISOP(char * path_archivo_fuente, int kernel){

	char * literal;
	literal = obtener_codigo(path_archivo_fuente);

	enviar_dato_serializado("INICIAR_PROGRAMA",kernel);
	enviar_dato_serializado(literal, kernel);

	return recibir_dato_serializado(kernel);
}
