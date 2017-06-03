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
sem_t * mutex_pids;

//PLANIFICACION
sem_t * mutex_cola_READY;
sem_t * mutex_lista_PROCESOS;

void inicializar_semaforo(sem_t* variable_semaforo);
void destruir_semaforo(sem_t* variable_semaforo);
void activar_semaforo(sem_t* variable_semaforo);
void desactivar_semaforo(sem_t* variable_semaforo);


#endif /* GENERAL_SEMAFORO_H_ */
