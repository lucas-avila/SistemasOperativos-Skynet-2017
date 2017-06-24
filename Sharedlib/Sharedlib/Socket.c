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

int recibir_dato_generico(int socket_conexion, char * buffer, int tam_bytes) {
//	recibe el socket del cliente, un array de chars y el tamanio de bytes a leer(
	int bytes_recibidos = recv(socket_conexion, buffer, tam_bytes, 0);
	if (bytes_recibidos < 0) {
		//perror("---Se perdio la conexion \n");
		return -1;
	} else if (bytes_recibidos == 0) {
		//printf("---Conexion terminada \n");
		return 0;
	}
	buffer[bytes_recibidos] = '\0';
	return bytes_recibidos;
}

char* recibir_dato_serializado(int socket_conexion) {
	char tamanio_dato[4];
	int bytes_recibidos = recv(socket_conexion, &tamanio_dato, 4, 0);
	tamanio_dato[bytes_recibidos] = '\0';
	char dato[40];
	int tamanioFinal = atoi(tamanio_dato);
	if (atoi(tamanio_dato) > 40) {
		char* datoNuevo;
		datoNuevo = malloc(tamanioFinal);
		recv(socket_conexion, datoNuevo, tamanioFinal, 0);
		datoNuevo[tamanioFinal] = '\0';
		char* resultado;
		resultado = string_substring(datoNuevo, 0, tamanioFinal);
		//printf("\n Recibi: %s, conexion : %d \n", resultado, socket_conexion);
		free(datoNuevo);
		return resultado;
	} else {

		//dato =  malloc(atoi(tamanio_dato));
		recv(socket_conexion, &dato, atoi(tamanio_dato), 0);
		dato[atoi(tamanio_dato)] = '\0';

		//printf("\n Recibi: %s, conexion : %d \n", dato, socket_conexion);
		return string_substring(&dato, 0, atoi(tamanio_dato));
	}
}

void enviar_dato_serializado(char* mensaje, int conexion) {
	char tamanio_dato[4];
	int tamanio = 0;
	if (mensaje[1] == '\0') {
		tamanio = 1;
	} else {
		tamanio = string_length(mensaje);
	}
	//int tamanio= string_length(mensaje);
	sprintf(tamanio_dato, "%d", tamanio);
	send(conexion, tamanio_dato, 4, 0);
	send(conexion, mensaje, tamanio, 0);
	//printf("\n Envie: %s, conexion : %d", mensaje, conexion);
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

