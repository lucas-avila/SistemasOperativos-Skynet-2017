/*
 * AppConfig.c
 *
 *  Created on: 9/4/2017
 *      Author: utnso
 */
#include<stdlib.h>
#include<stdio.h>

#include<stdbool.h>

#include "AppConfig.h"
#include <commons/config.h>
#include <commons/string.h>

#include <string.h>

void inicializar_configuracion(char *PATH_ARCHIVO_CONFIGURACION) {

	t_config *config = config_create(PATH_ARCHIVO_CONFIGURACION);
	if (config == NULL) {
		perror("archivo inexistente");
		exit(-1);
	} else {
		if (config_has_property(config, "PUERTO_PROG") == true) {
			configuraciones.PUERTO_PROG = config_get_int_value(config, "PUERTO_PROG");
		}

		if (config_has_property(config, "PUERTO_CPU") == true) {
			configuraciones.PUERTO_CPU = config_get_int_value(config, "PUERTO_CPU");
		}

		if (config_has_property(config, "PUERTO_MEMORIA") == true) {
			configuraciones.PUERTO_MEMORIA = config_get_int_value(config, "PUERTO_MEMORIA");
		}

		if (config_has_property(config, "PUERTO_FS") == true) {
			configuraciones.PUERTO_FS = config_get_int_value(config, "PUERTO_FS");
		}

		if (config_has_property(config, "QUANTUM") == true) {
			configuraciones.QUANTUM = config_get_int_value(config, "QUANTUM");
		}

		if (config_has_property(config, "QUANTUM_SLEEP") == true) {
			configuraciones.QUANTUM_SLEEP = config_get_int_value(config, "QUANTUM_SLEEP");
		}

		if (config_has_property(config, "GRADO_MULTIPROG") == true) {
			configuraciones.GRADO_MULTIPROG = config_get_int_value(config, "GRADO_MULTIPROG");
		}
		if (config_has_property(config, "STACK_SIZE") == true) {
			configuraciones.STACK_SIZE = config_get_int_value(config, "STACK_SIZE");
		}

		if (config_has_property(config, "IP_MEMORIA") == true) {
			configuraciones.IP_MEMORIA = config_get_string_value(config, "IP_MEMORIA");
		}

		if (config_has_property(config, "IP_FS") == true) {
			configuraciones.IP_FS = config_get_string_value(config, "IP_FS");
		}

		if (config_has_property(config, "ALGORITMO") == true) {
			configuraciones.ALGORITMO = config_get_string_value(config, "ALGORITMO");
		}

		if (config_has_property(config, "SEM_IDS") == true) {
			configuraciones.SEM_IDS = config_get_array_value(config, "SEM_IDS");
		}
		if (config_has_property(config, "SHARED_VARS") == true) {
			configuraciones.VAR_COMP = config_get_array_value(config, "SHARED_VARS");
		}

		if (config_has_property(config, "SEM_INIT") == true) {
			configuraciones.SEM_INIT = config_get_array_value(config, "SEM_INIT");
		}

		if (config_has_property(config, "CANT_SEM") == true) {

			configuraciones.cantidad_sem = config_get_int_value(config, "CANT_SEM");
		}

		if (config_has_property(config, "CANTIDAD_MAXIMA_CONCURRENCIA") == true) {
			configuraciones.CANTIDAD_MAXIMA_CONCURRENCIA = config_get_int_value(config, "CANTIDAD_MAXIMA_CONCURRENCIA");
		}

		if (config_has_property(config, "CANT_VAR_COM") == true) {

			configuraciones.cantidad_var_comp = config_get_int_value(config, "CANT_VAR_COM");
		}

		configuraciones.planificacion_activa = 1;
	}


	inicializar_vec_variables_compartidas();

}

/*void inicializar_vec_variables_compartidas() {
	VECTOR_VAR_COM = malloc(configuraciones.cantidad_var_comp);
<<<<<<< HEAD
	inicializar_vec_variables_compartidas(VECTOR_VAR_COM);
}*/

void inicializar_vec_variables_compartidas(int** VECTOR_VAR_COMP) {
	VECTOR_VAR_COMP = malloc(configuraciones.cantidad_var_comp);


	int i = 0;
	while ( i < configuraciones.cantidad_var_comp) {
		VECTOR_VAR_COM[i] = 0;
		i++;
	}
}

void inicializar_dict_semaforos_ansisop(){
	dict_semaforos_ansisop = dictionary_create();
	int i = 0;
	while(i < configuraciones.cantidad_sem){
		dictionary_put(dict_semaforos_ansisop, configuraciones.SEM_IDS[i], atoi(configuraciones.SEM_INIT[i]));
		i++;
	}
}

