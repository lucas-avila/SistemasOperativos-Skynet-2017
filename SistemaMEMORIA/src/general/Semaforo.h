/*
 * Semaforo.h
 *
 *  Created on: 12/4/2017
 *      Author: utnso
 */

#ifndef GENERAL_SEMAFORO_H_
#define GENERAL_SEMAFORO_H_

#include <semaphore.h>

sem_t mutex_SOLICITAR_BYTES;
sem_t mutex_ALMACENAR_BYTES;
sem_t mutex_LIBERAR_PAGINA;
sem_t mutex_ASIGNAR_PAGINAS;
sem_t mutex_INICIAR_PROGRAMA;
sem_t mutex_FINALIZAR_PROGRAMA;


void inicializar_semaforo(sem_t* variable_semaforo);
void destruir_semaforo(sem_t* variable_semaforo);
void activar_semaforo(sem_t* variable_semaforo);
void desactivar_semaforo(sem_t* variable_semaforo);


#endif /* GENERAL_SEMAFORO_H_ */
