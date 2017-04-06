/*
 * AppConfig.h
 *
 *  Created on: 1/4/2017
 *      Author: utnso
 */

#ifndef HEADER_APPCONFIG_H_
#define HEADER_APPCONFIG_H_

typedef struct {
     int PUERTO;
     int MARCOS;
     int MARCO_SIZE;
     int ENTRADAS_CACHE;
     int CACHE_X_PROC;
     int RETARDO_MEMORIA;

     //Agregado

     int CANTIDAD_MAXIMA_CONCURRENCIA;
} AppConfig;

//Lo declaro como variable global
AppConfig configuraciones;


char * MEMORIA_PRINCIPAL;
/****
 * VARIABLES GLOBALES PROPIAS DEL SISTEMA MEMORIA
 */




void inicializar_configuracion(char *PATH_ARCHIVO_CONFIGURACION);

#endif /* HEADER_APPCONFIG_H_ */
