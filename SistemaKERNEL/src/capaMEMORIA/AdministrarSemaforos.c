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

int es_semaforo(char* nombre_sem) {
	if(dictionary_has_key(dict_semaforos_ansisop, nombre_sem)) {
		return 1;
	}
	return 0;
}

int obtener_valor_semaforo(char * nombre_sem){
	if(dictionary_has_key(dict_semaforos_ansisop, nombre_sem)){
		int * sem_pointer = dictionary_get(dict_semaforos_ansisop, nombre_sem);
		return *sem_pointer;
	}
	return -2;
}

//Incrementa el semaforo ansisop y devuelve el valor, o -2 si no existe ese semaforo
int signal_semaforo_ansisop(char * nombre_sem){
	if(dictionary_has_key(dict_semaforos_ansisop, nombre_sem)){
		int * sem_pointer = dictionary_get(dict_semaforos_ansisop, nombre_sem);
		(*sem_pointer)++;
		return *sem_pointer;
	}
	return -2;
}

//Decrementa el semaforo ansisop y devuelve el valor, o -2 si no existe ese semaforo
//Si ya es cero, lo deja en -1
int wait_semaforo_ansisop(char * nombre_sem){
	if(dictionary_has_key(dict_semaforos_ansisop, nombre_sem)){
		int * sem_pointer = dictionary_get(dict_semaforos_ansisop, nombre_sem);
		if(*sem_pointer == 0)
			return -1;
		else{
			(*sem_pointer)--;
			return *sem_pointer;
		}
	}
	return -2; //no existe
}

int semaforo_ansisop_index(char * nombre_sem){
	int i = 0;
	while(i < configuraciones.cantidad_sem){
		if(strcmp(configuraciones.SEM_IDS[i], nombre_sem) == 0)
			return i;
		i++;
	}
	return -2;
}
