/*
 * funcionesUtiles.c
 *
 *  Created on: 1/4/2017
 *      Author: utnso
 */

#include "header/funcionesUtiles.h"

#include <commons/log.h>
#include <commons/string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "header/AppConfig.h"
#include "header/Estructuras.h"

int validarNumeroInput(int rangoMinimo, int rangoMaximo) {
	int numero = 0;
	do {
		scanf("%d", &numero);
		if ((numero < rangoMinimo || numero > rangoMaximo)) {
			printf("\n Numero invalido, por favor ingrese otro: ");
		}
	} while (numero < rangoMinimo || numero > rangoMaximo);
	return numero;
}

void validarArchivo(char * path){

	if(access(path, F_OK) == -1) {
			perror("Archivo Inexistente.");
			exit(-2);
		}
}

char * obtener_codigo(char * path_archivo_fuente){
	FILE * archivo_fuente;
	char * literal;
	int size_buffer;

	archivo_fuente = fopen(path_archivo_fuente, "r");

	// Ahora necesitamos saber el tamaño del archivo.
	fseek(archivo_fuente, 0, SEEK_END);
	size_buffer = ftell(archivo_fuente);
	fseek(archivo_fuente, 0, SEEK_SET);

	//Para evitar segmentation fault allocamos memoria.
	literal = malloc(sizeof(char) * size_buffer + 1);
	fread(literal, sizeof(char), size_buffer, archivo_fuente);
	literal[size_buffer] = '\0';
	fclose(archivo_fuente);
	return literal;
}

int pertenece_a_la_consola(uint32_t pid) {
	if(buscar_indice_Info_proceso(pid) == -1)
		return 0;
	return 1;
}

void incializar_log(){
	info_log = string_new();
}

void generar_log(){
	string_append(&info_log, "\n------Fin del LOG------\n\0");
	printf("\n%s", info_log);
    t_log* logger = log_create(configuraciones.PATH_ARCHIVO_LOG, "CONSOLA",false, LOG_LEVEL_INFO);
    log_info(logger, "\n***LOGS de la CONSOLA***\n %s", info_log);
    log_destroy(logger);
    //strcpy(info_log, "");
}
