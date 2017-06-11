/*
 * AppConfig.h
 *
 *  Created on: 1/4/2017
 *      Author: utnso
 */

#ifndef HEADER_APPCONFIG_H_
#define HEADER_APPCONFIG_H_

#include <semaphore.h>

typedef struct {
     char * IP_KERNEL;
     char * PUERTO_KERNEL;
     char * PATH_ARCHIVO_LOG;
} AppConfig;

//Lo declaro como variable global
AppConfig configuraciones;

void inicializar_configuracion(char *PATH_ARCHIVO_CONFIGURACION);

#endif /* HEADER_APPCONFIG_H_ */
