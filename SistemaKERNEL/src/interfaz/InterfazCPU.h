/*
 * InterfazCPU.h
 *
 *  Created on: 1/4/2017
 *      Author: utnso
 */

#ifndef HEADER_INTERFAZCPU_H_
#define HEADER_INTERFAZCPU_H_

#include <pthread.h>

void iniciar_conexion_servidor_cpu();
void escuchar_Conexiones_CPU(int servidorCPU);
void CU_Recibir_Conexiones_CPU(int clienteCPU) ;

int index_of_CPU(int numeroConexion);
void agregar_CPU_global(int numeroConexion, pthread_t hilo);
void retirar_CPU_global(int numeroConexion);

#endif /* HEADER_INTERFAZCPU_H_ */
