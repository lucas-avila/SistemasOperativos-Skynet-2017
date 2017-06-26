/*
 * Estructuras.h
 *
 *  Created on: 27/4/2017
 *      Author: utnso
 */

#ifndef HEADER_ESTRUCTURAS_H_
#define HEADER_ESTRUCTURAS_H_

#include <commons/collections/list.h>
#include <pthread.h>

typedef struct {
	int consola_ID;
	t_list * programas;
} ConsolaInfo;

typedef struct {
	int numeroConexion;
	pthread_t hilo;
	int disponible; //0 no disponible, 1 disponible esto se modifica cada vez que se le manda
//a ejecutar algo
} CPUInfo;


t_list * lista_consolas;
t_list * lista_CPUs;

#endif /* HEADER_ESTRUCTURAS_H_ */
