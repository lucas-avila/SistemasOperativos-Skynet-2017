/*
 * AdministrarSemaforos.c
 *
 *  Created on: 27/5/2017
 *      Author: utnso
 */
#include <commons/collections/dictionary.h>
#include <stdlib.h>

#include "../header/AppConfig.h"

//Obtiene el valor actual de un determinado semaforo ansisop

int obtener_valor_semaforo(char * nombre_sem){
	if(dictionary_has_key(dict_semaforos_ansisop, nombre_sem))
		return *((int *) dictionary_get(dict_semaforos_ansisop, nombre_sem));
	return -1;
}

//Incrementa el semaforo ansisop y devuelve el valor, o -1 si no existe ese semaforo
int signal_semaforo_ansisop(char * nombre_sem){
	if(dictionary_has_key(dict_semaforos_ansisop, nombre_sem)){
		int * sem_val = malloc(sizeof(int));
		sem_val = dictionary_get(dict_semaforos_ansisop, nombre_sem);
		return *sem_val += 1;
	}
	return -1;
}

//Decrementa el semaforo ansisop y devuelve el valor, o -1 si no existe ese semaforo
//Si ya es cero, no lo decrementa
int wait_semaforo_ansisop(char * nombre_sem){
	if(dictionary_has_key(dict_semaforos_ansisop, nombre_sem)){
		int * sem_point = malloc(sizeof(int));
		sem_point = (int *) dictionary_get(dict_semaforos_ansisop, nombre_sem);
		int sem_val = *sem_point;
		return (sem_val > 0)? sem_val-- : sem_val;
	}
	return -1;
}

int semaforo_ansisop_index(char * nombre_sem){
	int i = 0;
	while(i < configuraciones.cantidad_sem){
		if(strcmp(configuraciones.SEM_IDS[i], nombre_sem) == 0)
			return i;
		i++;
	}
	return -1;
}
