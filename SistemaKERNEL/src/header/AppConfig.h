/*
 * AppConfig.h
 *
 *  Created on: 1/4/2017
 *      Author: utnso
 */

#ifndef HEADER_APPCONFIG_H_
#define HEADER_APPCONFIG_H_

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
     char** SHERED_VARS;
     int STACK_SIZE;


     int CANTIDAD_MAXIMA_CONCURRENCIA;
} AppConfig;

//Lo declaro como variable global
AppConfig configuraciones;

void inicializar_configuracion(char *PATH_ARCHIVO_CONFIGURACION);

#endif /* HEADER_APPCONFIG_H_ */
