#ifndef HEADER_APPCONFIG_H_
#define HEADER_APPCONFIG_H_

#include <semaphore.h>

typedef struct {
	int PUERTO;
	int MARCOS;
	int MARCO_SIZE;
	int ENTRADAS_CACHE;
	int CACHE_X_PROC;
	int RETARDO_MEMORIA;

	//Agregado

	int CANTIDAD_MAXIMA_CONCURRENCIA;
	char* PATH_ARCHIVO_LOG;
//	int FRAME_INICIAL;



} AppConfig;

/*
 * VARIABLES GLOBALES PROPIAS DEL SISTEMA MEMORIA
 */
AppConfig configuraciones;

char * MEMORIA_PRINCIPAL;

void inicializar_configuracion(char *PATH_ARCHIVO_CONFIGURACION);



/**
 * Semaforos de la aplicacion
 */

sem_t semaforo_Tabla_MEMORY;
sem_t semaforo_Tabla_CACHE;

sem_t semaforo_Proceso_Asignar_Pagina;
sem_t semaforo_Proceso_Finalizar_Programa;


int conexion_MEMORIA;
int conexion_FS;





#endif
