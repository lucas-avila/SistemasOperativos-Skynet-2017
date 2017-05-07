/*
 * AppConfig.h
 *
 *  Created on: 1/4/2017
 *      Author: utnso
 */
#include <stdbool.h>
#ifndef HEADER_APPCONFIG_H_
#define HEADER_APPCONFIG_H_

typedef struct {

	int PUERTO_KERNEL;
	char* IP_KERNEL;
	int PUERTO_MEMORIA;
	char* IP_MEMORIA;
	int CANTIDAD_MAXIMA_CONCURRENCIA;

} AppConfig;

int servidor_Memoria ;
int servidor_kernel;


int tamanio_pagina_memoria ;

//Lo declaro como variable global
AppConfig configuraciones;

bool controlEjecucionPrograma;

void inicializar_configuracion(char *PATH_ARCHIVO_CONFIGURACION);

#endif /* HEADER_APPCONFIG_H_ */
