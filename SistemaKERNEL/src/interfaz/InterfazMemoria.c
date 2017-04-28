/*
 * InterfazMemoria.c
 *
 *  Created on: 1/4/2017
 *      Author: utnso
 */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include "../header/AppConfig.h"
#include "../general/Socket.h"
#include "InterfazMemoria.h"
#include "../general/funcionesUtiles.h"

int servidor_Memoria = 0;
int tamanio_pagina_memoria = 0;
void iniciar_conexion_servidor_memoria() {
	servidor_Memoria = conectar_servidor(configuraciones.IP_MEMORIA, configuraciones.PUERTO_MEMORIA);
	enviar_dato_serializado("KERNEL", servidor_Memoria);
	tamanio_pagina_memoria = atoi(recibir_dato_serializado(servidor_Memoria));
}

char* solicitar_bytes_memoria(char* PID, char* pagina, char* byteInicial, char* longitud) {
	enviar_dato_serializado("SOLICITAR_BYTE_MEMORIA", servidor_Memoria);
	enviar_dato_serializado(PID, servidor_Memoria);
	enviar_dato_serializado(pagina, servidor_Memoria);
	enviar_dato_serializado(byteInicial, servidor_Memoria);
	enviar_dato_serializado(longitud, servidor_Memoria);
	return recibir_dato_serializado(servidor_Memoria);
}

char* almacenar_Bytes_de_Pagina(char* PID, char* pagina, char* byteInicial, char* longitud, char* texto) {
	enviar_dato_serializado("ALMACENAR_BYTE_MEMORIA", servidor_Memoria);
	enviar_dato_serializado(PID, servidor_Memoria);
	enviar_dato_serializado(pagina, servidor_Memoria);
	enviar_dato_serializado(byteInicial, servidor_Memoria);
	enviar_dato_serializado(longitud, servidor_Memoria);
	enviar_dato_serializado(texto, servidor_Memoria);
	return recibir_dato_serializado(servidor_Memoria);
}

char* inicializar_Programa(char* PID, char* cantidad_paginas) {
	enviar_dato_serializado("INICIALIZAR_PROGRAMA", servidor_Memoria);
	enviar_dato_serializado(PID, servidor_Memoria);
	enviar_dato_serializado(cantidad_paginas, servidor_Memoria);
	return recibir_dato_serializado(servidor_Memoria);
}
char* asignar_Paginas_Programa(char* PID, char* cantidad_paginas) {
	enviar_dato_serializado("ASIGNAR_PAGINAS_PROCESO", servidor_Memoria);
	enviar_dato_serializado(PID, servidor_Memoria);
	enviar_dato_serializado(cantidad_paginas, servidor_Memoria);
	return recibir_dato_serializado(servidor_Memoria);
}
char* finalizar_Programa(char* PID) {
	enviar_dato_serializado("FINALIZAR_PROGRAMA", servidor_Memoria);
	enviar_dato_serializado(PID, servidor_Memoria);
	return recibir_dato_serializado(servidor_Memoria);
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

int is_valid_line(char* line){
	if(is_white_line(line) || first_char(line) == '#')
		return 0;
	return 1;
}
