/*
 * Socket.h
 *
 *  Created on: 24/3/2017
 *      Author: utnso
 */

#ifndef GENERAL_SOCKET_H_
#define GENERAL_SOCKET_H_

#include <stdint.h>

int crear_servidor(int puerto, int max_conexiones);
int aceptar_conexion_cliente(int socket_servidor);
int recibir_dato_generico(int socket_conexion, char * buffer, int tam_bytes);
int enviar_dato(char * dato, uint32_t tamanio, int conexion);
char * recibir_dato_serializado(int socket_conexion);
int enviar_dato_serializado(char * mensaje, int conexion);
void atender_clientes(int servidor, void (*f)(int));
int conectar_servidor(char * ip_addr, int puerto);

typedef struct {
	char * buffer;
	int size;
} DATO_SERIALIZADO;

#endif /* GENERAL_SOCKET_H_ */
