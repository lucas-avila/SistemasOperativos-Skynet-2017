/*
 * AppConfig.c
 *
 *  Created on: 1/4/2017
 *      Author: utnso
 */
#include<stdlib.h>
#include<stdio.h>

#include<stdbool.h>

#include "AppConfig.h"
#include <commons/config.h>
#include <commons/string.h>

void inicializar_configuracion(char *PATH_ARCHIVO_CONFIGURACION) {
	t_config *config = config_create(PATH_ARCHIVO_CONFIGURACION);
	if (config == NULL) {
		perror("archivo inexistente");
		exit(-1);
	} else {
		if (config_has_property(config, "PUERTO_KERNEL") == true) {
			configuraciones.PUERTO_KERNEL = config_get_int_value(config, "PUERTO_KERNEL");
		}

		if (config_has_property(config, "PUERTO_MEMORIA") == true) {
			configuraciones.PUERTO_MEMORIA = config_get_int_value(config, "PUERTO_MEMORIA");
		}

		if (config_has_property(config, "IP_KERNEL") == true) {
			configuraciones.IP_KERNEL = malloc(strlen(config_get_string_value(config, "IP_KERNEL")) + 1);
			strcpy(configuraciones.IP_KERNEL, config_get_string_value(config, "IP_KERNEL"));

		}

		if (config_has_property(config, "IP_MEMORIA") == true) {
			configuraciones.IP_MEMORIA = malloc(strlen(config_get_string_value(config, "IP_MEMORIA")) + 1);
			strcpy(configuraciones.IP_MEMORIA, config_get_string_value(config, "IP_MEMORIA"));

		}
		configuraciones.PATH_ARCHIVO_LOG = malloc(strlen(config_get_string_value(config, "PATH_ARCHIVO_LOGUEO")) + 1);
		strcpy(configuraciones.PATH_ARCHIVO_LOG, config_get_string_value(config, "PATH_ARCHIVO_LOGUEO"));


	}

	//Agregado
	configuraciones.CANTIDAD_MAXIMA_CONCURRENCIA = 10;

	config_destroy(config);
}
