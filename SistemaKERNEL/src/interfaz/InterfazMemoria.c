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

char* inicializar_Programa_memoria(char* PID, char* cantidad_paginas) {
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
char* finalizar_Programa_memoria(char* PID) {
	enviar_dato_serializado("FINALIZAR_PROGRAMA", servidor_Memoria);
	enviar_dato_serializado(PID, servidor_Memoria);
	return recibir_dato_serializado(servidor_Memoria);
}

int is_valid_line(char* line){
	if(is_white_line(line) || first_char(line) == '#')
		return 0;
	return 1;
}

int enviar_programa_memoria(char * codigo, int pid) {
	char line[256];

	char* numeroPagina;
	int tamanioPagina = 100;
	int contadorTamanioPagina = 0;

	char* processID = malloc(5);
	strcpy(processID, string_itoa(pid));

	numeroPagina = asignar_Paginas_Programa(processID, "1");
	if (strcmp(numeroPagina, "FALTA ESPACIO") == 0) {
		finalizar_Programa_memoria(processID);
		printf("ERROR no hay espacio suficiente");
		return -1;
	}
	char* contenidoPagina = malloc(tamanioPagina + 1);
	strcpy(contenidoPagina, "");
	while (get_line(codigo, line, 1)) {
		if (is_valid_line(line)) {
			if ( (contadorTamanioPagina + strlen(line)) <= tamanioPagina) {
				strcat(contenidoPagina, line);
				contadorTamanioPagina += strlen(line);
			} else {
				almacenar_Bytes_de_Pagina(processID, numeroPagina, "0", string_itoa(strlen(contenidoPagina)), contenidoPagina);
				numeroPagina = asignar_Paginas_Programa(processID, "1");
				if (strcmp(numeroPagina, "FALTA ESPACIO") == 0) {
					//TODO: handlear error cuando no hay mas memoria para reservar paginas
					finalizar_Programa_memoria(processID);
					printf("ERROR no hay espacio suficiente");
					return -2;
				}
				strcpy(contenidoPagina, line);
				contadorTamanioPagina = strlen(line);
			}
		}
	}
	char * resul = almacenar_Bytes_de_Pagina(processID, numeroPagina, "0", string_itoa(strlen(contenidoPagina)), contenidoPagina);
	printf("resultado es %s\n", resul);
	printf("contenidoPagina : %s\n", contenidoPagina);
	//free(contenidoPagina);
	return 1;
}

