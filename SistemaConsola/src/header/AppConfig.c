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

#include <string.h>
#include "Semaforo.h"

void inicializar_configuracion(char *PATH_ARCHIVO_CONFIGURACION) {

	t_config *config = config_create(PATH_ARCHIVO_CONFIGURACION);
	if (config == NULL) {
		perror("archivo inexistente");
		exit(-1);
	} else {
		if (config_has_property(config, "PUERTO_KERNEL") == true) {
			configuraciones.PUERTO_KERNEL = config_get_int_value(config, "PUERTO_KERNEL");
		}

		if (config_has_property(config, "IP_KERNEL") == true) {
			configuraciones.IP_KERNEL = config_get_string_value(config, "IP_KERNEL");
		}
		configuraciones.PATH_ARCHIVO_LOG = malloc(strlen(config_get_string_value(config,"PATH_ARCHIVO_LOG")) + 1);
		configuraciones.PATH_ARCHIVO_LOG = config_get_string_value(config,"PATH_ARCHIVO_LOG");
	}
}


