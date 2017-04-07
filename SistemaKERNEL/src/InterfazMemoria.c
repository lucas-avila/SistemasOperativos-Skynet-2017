/*
 * InterfazMemoria.c
 *
 *  Created on: 1/4/2017
 *      Author: utnso
 */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "header/AppConfig.h"
#include "header/Socket.h"
void solicitar_bytes_memoria() {
	int serverMemoria = crear_servidor(configuraciones.IP_MEMORIA, configuraciones.PUERTO_MEMORIA);
	/**
	 * Enviar datos para solicitar memoria
	 */
	enviar_dato_serializado("KERNEL", serverMemoria); //identificacion
	enviar_dato_serializado("SOLICITAR_BYTE_MEMORIA", serverMemoria); //operacion

	enviar_dato_serializado("1", serverMemoria); //pagina
	enviar_dato_serializado("0", serverMemoria); //byte inicial de pagina
	enviar_dato_serializado("10", serverMemoria); //longitud de bytes a pedir

	//recibir datos
	char* dato = recibir_dato_serializado(serverMemoria);
	printf("DATO RECIBIDO DE MEMORIA : %s", dato);
	close(serverMemoria);
}
