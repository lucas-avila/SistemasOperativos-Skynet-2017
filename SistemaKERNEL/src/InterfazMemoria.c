/*
 * InterfazMemoria.c
 *
 *  Created on: 1/4/2017
 *      Author: utnso
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/string.h>
#include "header/AppConfig.h"
#include "header/Socket.h"
#include "header/funcionesUtiles.h"

int is_valid_line(char * line);

void solicitar_bytes_memoria() {
	int serverMemoria = conectar_servidor(configuraciones.IP_MEMORIA, configuraciones.PUERTO_MEMORIA);
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

void enviar_programa_memoria(char * path_programa){
	FILE * programa = fopen(path_programa, "r");
	char line[256];

	while (fgets(line, sizeof(line), programa)) {
		if(is_valid_line(line))
			printf("%s", line);
			//aca enviamos la linea a memoria con la funcion de johnny
	}

	/* TODO: chequear aca que realmente se haya llegado al EOF
	 * y que no haya terminado por un error */

	fclose(programa);
}

int is_valid_line(char * line){
	if(is_white_line(line) || first_char(line) == '#')
		return 0;
	return 1;
}
