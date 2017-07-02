/*
 * Socket.c
 *
 *  Created on: 24/3/2017
 *      Author: utnso
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include <netinet/in.h>
#include <signal.h>

#include "Socket.h"

//FUNCIONES DEL LADO DEL SERVER
int crear_servidor(int puerto, int max_conexiones) {
	/* recibe puerto y maximo de conexiones acumulables,
	 crea un socket servidor y lo hace empezar a escuchar
	 devuelve ese socket servidor */

	struct sockaddr_in direccion_servidor;
	direccion_servidor.sin_family = AF_INET;
	direccion_servidor.sin_addr.s_addr = INADDR_ANY;
	direccion_servidor.sin_port = htons(puerto);

	int servidor = socket(AF_INET, SOCK_STREAM, 0);

//	Codigo que setea flag para que se pueda reusar la dir del socket sin esperar 2 min
	int activado = 1;
	setsockopt(servidor, SOL_SOCKET, SO_REUSEADDR, &activado, sizeof(activado));
	if (bind(servidor, (void *) &direccion_servidor, sizeof direccion_servidor) != 0) {
		//perror("---Falló el binding entre el socket del servidor y la direccion");
		return -1;
	}

	/* 					<-num maximo de conexiones acumulables*/
	listen(servidor, max_conexiones);

	return servidor;
}

int aceptar_conexion_cliente(int socket_servidor) {
	struct sockaddr_in direccion_cliente;
	unsigned int tamanio_direccion = sizeof(struct sockaddr_in);
	int cliente = accept(socket_servidor, (void *) &direccion_cliente, &tamanio_direccion);
	return cliente;
}



int enviar_dato(char * dato, uint32_t tamanio, int conexion){
	//el dato es en BYTES, NO SON CHAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAARS (o sea un char ES un byte)
	//.............cualquier duda consultar el kernigan & ritchie
	if(conexion == -1)
				return -1;

	//msg_nosignal para que no tire SIGPIPE y handlear nosotros las desconexiones
	int bytes_enviados = send(conexion, &tamanio, sizeof(uint32_t), MSG_NOSIGNAL);

	bytes_enviados = send(conexion, dato, tamanio, MSG_NOSIGNAL);

	int nuevos = 0;
	while(bytes_enviados < tamanio){
		nuevos = send(conexion, dato + bytes_enviados, tamanio - bytes_enviados, MSG_NOSIGNAL);
		if(nuevos == -1) return -1;
		if(nuevos == 0) return 0;
		bytes_enviados += nuevos;
	}
	return bytes_enviados;
}

DATO_SERIALIZADO * recibir_dato(int conexion){
	uint32_t tamanio_dato = 0;
	int bytes_recibidos = recv(conexion, &tamanio_dato, sizeof(uint32_t), MSG_NOSIGNAL);

	char * buffer = calloc(tamanio_dato, sizeof(char));
	bytes_recibidos = recv(conexion, buffer, tamanio_dato, MSG_NOSIGNAL);

	int nuevos = 0;
	while(bytes_recibidos < tamanio_dato){
		nuevos = recv(conexion, buffer + bytes_recibidos, tamanio_dato - bytes_recibidos, MSG_NOSIGNAL);
		if(nuevos == -1) break;
		if(nuevos == 0) break;
		bytes_recibidos += nuevos;
	}

	DATO_SERIALIZADO * l = malloc(sizeof(DATO_SERIALIZADO));
	l->buffer = buffer;
	l->size = tamanio_dato;
	return l;
}

char* recibir_dato_serializado(int conexion) {
	DATO_SERIALIZADO * dato = recibir_dato(conexion);
	return dato->buffer;
}

int enviar_dato_serializado(char* mensaje, int conexion) {
	uint32_t tamanio = (strlen(mensaje) == 0)?0:strlen(mensaje) + 1;
	return enviar_dato(mensaje, tamanio, conexion);
}

void atender_clientes(int servidor, void (*f)(int)) {
	pthread_t mihilo1;
	pthread_create(&mihilo1, NULL, f, servidor);
	pthread_detach(&mihilo1);
}

//FUNCIONES DEL LADO DEL CLIENTE
int conectar_servidor(char * ip_addr, int puerto) {
	struct sockaddr_in direccion_servidor;
	direccion_servidor.sin_family = AF_INET;
	direccion_servidor.sin_addr.s_addr = inet_addr(ip_addr);
	direccion_servidor.sin_port = htons(puerto);

	int cliente = socket(AF_INET, SOCK_STREAM, 0);

	if (connect(cliente, (void *) &direccion_servidor, sizeof direccion_servidor) != 0) {
		//perror("No se pudo conectar al servidor");
		return -1;
	}

	return cliente;
}

