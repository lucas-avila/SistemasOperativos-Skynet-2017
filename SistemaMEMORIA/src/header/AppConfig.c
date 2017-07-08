#include<stdlib.h>
#include <string.h>
#include<stdio.h>
#include<stdbool.h>

#include <commons/config.h>
#include <commons/string.h>

#include "AppConfig.h"
#include "../general/funcionesUtiles.h"
#include "../general/Semaforo.h"

void inicializar_configuracion(char *PATH_ARCHIVO_CONFIGURACION) {
	t_config *config = config_create(PATH_ARCHIVO_CONFIGURACION);
	if (config == NULL) {
		perror("archivo inexistente");
		exit(-1);
	} else {
		if (config_has_property(config, "PUERTO") == true) {
			configuraciones.PUERTO = config_get_int_value(config, "PUERTO");
		}
		if (config_has_property(config, "MARCOS") == true) {
			configuraciones.MARCOS = config_get_int_value(config, "MARCOS");
		}
		if (config_has_property(config, "MARCO_SIZE") == true) {
			configuraciones.MARCO_SIZE = config_get_int_value(config, "MARCO_SIZE");
		}
		if (config_has_property(config, "ENTRADAS_CACHE") == true) {
			configuraciones.ENTRADAS_CACHE = config_get_int_value(config, "ENTRADAS_CACHE");
		}
		if (config_has_property(config, "CACHE_X_PROC") == true) {
			configuraciones.CACHE_X_PROC = config_get_int_value(config, "CACHE_X_PROC");
		}
		if (config_has_property(config, "RETARDO_MEMORIA") == true) {
			configuraciones.RETARDO_MEMORIA = config_get_int_value(config, "RETARDO_MEMORIA");
		}
		if (config_has_property(config, "CANTIDAD_MAXIMA_CONCURRENCIA") == true) {
			configuraciones.CANTIDAD_MAXIMA_CONCURRENCIA = config_get_int_value(config, "CANTIDAD_MAXIMA_CONCURRENCIA");
		}
		configuraciones.PATH_ARCHIVO_LOG = malloc(strlen(config_get_string_value(config, "PATH_ARCHIVO_LOGUEO")) + 1);
		strcpy(configuraciones.PATH_ARCHIVO_LOG, config_get_string_value(config, "PATH_ARCHIVO_LOGUEO"));

	}

	config_destroy(config);

	/**
	 * Inicializar semaforos
	 */
	inicializar_semaforo(&semaforo_Tabla_MEMORY);
	inicializar_semaforo(&semaforo_ARCHIVO);
	inicializar_semaforo(&semaforo_Tabla_CACHE);
	//inicializar_semaforo(&semaforo_Tabla_CACHE_INGRESO);
	inicializar_semaforo(&semaforo_Proceso_Asignar_Pagina);
	inicializar_semaforo(&semaforo_Proceso_Finalizar_Programa);


	inicializar_semaforo(&mutex_SOLICITAR_BYTES);
	inicializar_semaforo(&mutex_ALMACENAR_BYTES);
	inicializar_semaforo(&mutex_LIBERAR_PAGINA);
	inicializar_semaforo(&mutex_ASIGNAR_PAGINAS);
	inicializar_semaforo(&mutex_INICIAR_PROGRAMA);
	inicializar_semaforo(&mutex_FINALIZAR_PROGRAMA);

	char* mensajeLogueo = string_from_format("\n Parametros de Archivo de Configuracion \nPUERTO=%d	\nMARCOS=%d	\nMARCO_SIZE=%d	\nENTRADAS_CACHE=%d	\nCACHE_X_PROC=%d	\nRETARDO_MEMORIA=%d	\nCANTIDAD_MAXIMA_CONCURRENCIA=%d	\nPATH_ARCHIVO_LOGUEO=%s ", configuraciones.PUERTO, configuraciones.MARCOS, configuraciones.MARCO_SIZE, configuraciones.ENTRADAS_CACHE, configuraciones.CACHE_X_PROC, configuraciones.RETARDO_MEMORIA, configuraciones.CANTIDAD_MAXIMA_CONCURRENCIA, configuraciones.PATH_ARCHIVO_LOG);
	logSO(mensajeLogueo);
	if (mensajeLogueo != NULL) {
		free(mensajeLogueo);
	}
}
