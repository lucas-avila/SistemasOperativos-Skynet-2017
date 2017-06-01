/*
 * AppConfig.h
 *
 *  Created on: 18/5/2017
 *      Author: utnso
 */

#ifndef HEADER_APPCONFIG_H_
#define HEADER_APPCONFIG_H_

#include <commons/collections/dictionary.h>

typedef struct {
	int PUERTO_PROG;
	int PUERTO_CPU;
	char *IP_MEMORIA;
	int PUERTO_MEMORIA;
	char *IP_FS;
	int PUERTO_FS;
	int QUANTUM;
	int QUANTUM_SLEEP;
	char *ALGORITMO;
	int GRADO_MULTIPROG;
	char** SEM_IDS;
	char** SEM_INIT;
	int cantidad_sem;
	char** VAR_COMP;

	int cantidad_var_comp;
	int STACK_SIZE;

	int CANTIDAD_MAXIMA_CONCURRENCIA;
	int planificacion_activa;
} AppConfig;

//Lo declaro como variable global
AppConfig configuraciones;

int VECTOR_VAR_COM[];
t_dictionary * dict_semaforos_ansisop;

void inicializar_configuracion(char *PATH_ARCHIVO_CONFIGURACION);
void inicializar_vec_variables_compartidas();

#endif /* HEADER_APPCONFIG_H_ */
