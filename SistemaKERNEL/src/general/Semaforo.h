/*
 * Semaforo.h
 *
 *  Created on: 12/4/2017
 *      Author: utnso
 */

#ifndef GENERAL_SEMAFORO_H_
#define GENERAL_SEMAFORO_H_

#include <semaphore.h>

//LISTA DE SEMAFOROS GLOBALES
sem_t mutex_pids;

sem_t mutex_lista_PROCESOS;
sem_t mutex_lista_CPUs;

//PLANIFICACION
sem_t mutex_cola_NEW;
sem_t mutex_cola_READY;
sem_t mutex_cola_EXEC;
sem_t mutex_cola_WAITING;
sem_t mutex_cola_EXIT;

//EVITAR ESPERA ACTIVA
sem_t grado_multiprogramacion;
sem_t cpu_disponible;
sem_t proceso_new;
sem_t proceso_ready;

//OTROS
sem_t mutex_memoria;
sem_t mutex_tabla_estadistica;
sem_t mutex_tabla_estadistica_busqueda;
sem_t buffer_codigo;
sem_t eliminacion;
sem_t escribir_log;

void inicializar_semaforo(sem_t* variable_semaforo, int valor);
void destruir_semaforo(sem_t* variable_semaforo);
void activar_semaforo(sem_t* variable_semaforo);
void desactivar_semaforo(sem_t* variable_semaforo);


#endif /* GENERAL_SEMAFORO_H_ */
