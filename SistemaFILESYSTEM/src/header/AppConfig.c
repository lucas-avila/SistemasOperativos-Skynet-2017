/*
 * AppConfig.c
 *
 *  Created on: 9/4/2017
 *      Author: utnso
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "AppConfig.h"
#include <commons/config.h>
#include <commons/string.h>


void inicializar_configuracion(char * PATH_ARCHIVO_CONFIGURACION) {

	t_config * config = config_create(PATH_ARCHIVO_CONFIGURACION);
	if (config == NULL) {
		perror("Archivo Inexistente");
		exit(-2);
	} else {
		if (config_has_property(config, "PUERTO") == true) {
					configuraciones.PUERTO = config_get_int_value(config, "PUERTO");
		}

		if (config_has_property(config, "PUNTO_MONTAJE") == true) {
			configuraciones.PUNTO_MONTAJE = config_get_string_value(config, "PUNTO_MONTAJE");
		}

		if (config_has_property(config, "CANTIDAD_MAXIMA_CONCURRENCIA") == true) {
					configuraciones.CANTIDAD_MAXIMA_CONCURRENCIA = config_get_int_value(config, "CANTIDAD_MAXIMA_CONCURRENCIA");
				}

	}
}


