/*
 * Socket.c
 *
 *  Created on: 24/3/2017
 *      Author: utnso
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>

int Abre_Socket_Inet(int puerto, int cantidad_maxima_concurrencia) {

	struct sockaddr_in direccionServidor;
	direccionServidor.sin_family = AF_INET;
	direccionServidor.sin_addr.s_addr = INADDR_ANY;
	direccionServidor.sin_port = htons(puerto);

	int servidor = socket(AF_INET, SOCK_STREAM, 0);

	int activado = 1;
	setsockopt(servidor, SOL_SOCKET, SO_REUSEADDR, &activado, sizeof(activado));

	if (bind(servidor, (void*) &direccionServidor, sizeof(direccionServidor)) != 0) {
		perror("Falló el bind");
		return 1;
	}

	printf("Estoy escuchando\n");
	listen(servidor, cantidad_maxima_concurrencia);

	return servidor;
}

int iniciar_servidor(int puerto, int cantidad_maxima_concurrencia) {
	int Socket_Servidor = Abre_Socket_Inet(puerto, cantidad_maxima_concurrencia);
	if (Socket_Servidor == -1) {
		printf("No se puede abrir socket servidor\n");
		exit(-1);
	} else {
		printf("\nServidor Iniciado");
		printf("\n-----------------\n");
	}
	return Socket_Servidor;
}

int iniciar_conexion_servidor(char* ipServidor, int puerto) {
	struct sockaddr_in direccionServidor;
	direccionServidor.sin_family = AF_INET;

	direccionServidor.sin_addr.s_addr = INADDR_ANY;
	direccionServidor.sin_port = htons(puerto);

	int cliente = socket(AF_INET, SOCK_STREAM, 0);
	if (connect(cliente, (void*) &direccionServidor, sizeof(direccionServidor)) != 0) {
		perror("No se pudo conectar");
		return 1;
	}

	return cliente;
}

void destruir_conexion_servidor(int conexionServidor) {
	close(conexionServidor);
}
int destruir_conexion_cliente(int conexion) {
	close(conexion);
	return 0;
}

char* recibir_dato_serializado(int conexion) {
	char* tamanioDato = malloc(4);
	int byteRecibidos = 0;
	do {
		byteRecibidos = recv(conexion, tamanioDato, 4, 0);
	} while (byteRecibidos == 0);
	tamanioDato[byteRecibidos] = '\0';

	char* dato = malloc(atoi(tamanioDato));
	recv(conexion, dato, atoi(tamanioDato), 0);
	dato[atoi(tamanioDato)] = '\0';

	printf("\n Recibi: %s, conexion : %d", dato, conexion);

	free(tamanioDato);
	return dato;
}

void enviar_dato_serializado(char* mensajeEnviar, int conexion) {
	char *tamanio = malloc(4);
	sprintf(tamanio, "%d", strlen(mensajeEnviar));
	send(conexion, tamanio, 4, 0);
	send(conexion, mensajeEnviar, strlen(mensajeEnviar), 0);
	printf("\n Envie: %s, conexion : %d", mensajeEnviar, conexion);
}

int aceptar_conexion_cliente(int conexion) {

	int Cliente;

	Cliente = Acepta_Conexion_Cliente(conexion);
	if (Cliente == -1) {
		printf("No se puede abrir socket de cliente\n");
		exit(-1);
	} else {
		printf("\n-->Nuevo Cliente conectado\n");
	}
	return Cliente;

}

int Acepta_Conexion_Cliente(int Descriptor) {
	socklen_t Longitud_Cliente;
	struct sockaddr Cliente;
	int Hijo;

	Longitud_Cliente = sizeof(Cliente);
	Hijo = accept(Descriptor, &Cliente, &Longitud_Cliente);
	if (Hijo == -1)
		return -1;

	return Hijo;
}
