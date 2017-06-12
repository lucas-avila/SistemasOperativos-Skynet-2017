/*
 * InterfazKernel.c
 *
 *  Created on: 29/4/2017
 *      Author: utnso
 */

#include <stdio.h>
#include "Socket.h"
#include "funcionesUtiles.h"

void CU_handshake_programa(int kernel_programa){

	enviar_dato_serializado("INICIAR_PROGRAMA", kernel_programa);
}

char * enviar_programa_ANSISOP(char * path_archivo_fuente, int kernel_programa){

	char * literal;

	literal = obtener_codigo(path_archivo_fuente);
	enviar_dato_serializado(literal, kernel_programa);

	return recibir_dato_serializado(kernel_programa);
}

void solicitar_fin_programa(int pid, int socket){
	enviar_dato_serializado("FINALIZAR_PROGRAMA", socket);
	enviar_dato_serializado(string_itoa(pid), socket);
}

void solicitar_desconexion(int pid, int socket){
	enviar_dato_serializado("DESCONEXION_CONSOLA", socket);
	enviar_dato_serializado(string_itoa(pid), socket);
}
