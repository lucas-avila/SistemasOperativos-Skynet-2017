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

int validarArchivo(char * path){

	return access(path, F_OK);
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

char * diferencia_entre_tiempos(char * tiempo_A, char * tiempo_B) {
	char * diferencia_temporal = string_new();

	char horaA[2] = {tiempo_A[0], tiempo_A[1]};
	char minutoA[2] = {tiempo_A[3], tiempo_A[4]};
	char segundoA[2] = {tiempo_A[6], tiempo_A[7]};
	char milisegundoA[2] = {tiempo_A[9], tiempo_A[10], tiempo_A[11]};

	char horaB[2] = {tiempo_B[0], tiempo_B[1]};
	char minutoB[2] = {tiempo_B[3], tiempo_B[4]};
	char segundoB[2] = {tiempo_B[6], tiempo_B[7]};
	char milisegundoB[2] = {tiempo_B[3], tiempo_B[4], tiempo_B[11]};

	int a = 0;
	int b = 0;
	int c = 0;

	int diferenciaMilisegundo = atoi(milisegundoB) - atoi(milisegundoA);
	if(diferenciaMilisegundo < 0) {
		diferenciaMilisegundo = diferenciaMilisegundo + 1000;
		c = -1;
	}

	int diferenciaSegundo = atoi(segundoB) - atoi(segundoA) + c;
	if(diferenciaSegundo < 0) {
		diferenciaSegundo = diferenciaSegundo + 60;
		b = -1;
	}

	int diferenciaMinuto = atoi(minutoB) - atoi(minutoA) + b;
	if(diferenciaMinuto < 0) {
		diferenciaMinuto = diferenciaMinuto + 60;
		a = -1;
	}

	int diferenciaHora = atoi(horaB) - atoi(horaA) + a;
	if(diferenciaHora < 0) {
		diferenciaHora = diferenciaHora + 24;
	}

	string_append(&diferencia_temporal, string_itoa(diferenciaHora));
	string_append(&diferencia_temporal, ":");
	string_append(&diferencia_temporal, string_itoa(diferenciaMinuto));
	string_append(&diferencia_temporal, ":");
	string_append(&diferencia_temporal, string_itoa(diferenciaSegundo));
	string_append(&diferencia_temporal, ":");
	string_append(&diferencia_temporal, string_itoa(diferenciaMilisegundo));
	string_append(&diferencia_temporal, "\0");

	return diferencia_temporal;
}
