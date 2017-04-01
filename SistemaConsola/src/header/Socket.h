/*
 * Socket.h
 *
 *  Created on: 24/3/2017
 *      Author: utnso
 */

#ifndef HEADER_SOCKET_H_
#define HEADER_SOCKET_H_

int iniciar_conexion_servidor(char* ipServidor, int puerto);
void destruir_conexion_servidor(int conexionServidor);
int iniciar_servidor(int puerto, int cantidad_maxima_concurrencia);
char* recibir_dato_serializado(int conexion);
void enviar_dato_serializado(char* mensajeEnviar, int conexion);
void escuchar_peticiones_cliente(int servidor);
int aceptar_conexion_cliente(int conexion);
int destruir_conexion_cliente(int conexion);
int Abre_Socket_Inet(int puerto, int cantidad_maxima_concurrencia);
int Acepta_Conexion_Cliente(int Descriptor);
int Acepta_Conexion_Cliente(int Descriptor);
#endif /* HEADER_SOCKET_H_ */
