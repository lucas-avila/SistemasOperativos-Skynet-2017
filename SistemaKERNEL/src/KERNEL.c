/*
 ============================================================================
 Name        : KERNEL.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
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
#include <semaphore.h>
#include <signal.h>

#include <commons/string.h>

#include "header/AppConfig.h"
#include "header/Socket.h"
#include "header/SolicitudesUsuario.h"
#include "header/funcionesUtiles.h"

#include "header/InterfazMemoria.h"

void atender_solicitudes_de_usuario();

void escuchar_Conexiones_CPU(int servidorCPU);
void escuchar_Conexiones_Consola(int servidorConsola);

void CU_Recibir_Conexiones_Consola(int clienteConsola);
void CU_Recibir_Conexiones_CPU(int clienteCPU);
void CU_iniciar_programa(int filesystem);


int main(int argc, char *argv[]) {

	//CUANDO SE INVOCA ENVIAR POR PARAMETRO EL PATH DEL ARCHIVO

	inicializar_configuracion(argv[1]);

	//INICIAR SERVIDOR PARA ESCUCHAR CONSOLA:
	int servidor_Consola = crear_servidor(configuraciones.PUERTO_PROG, configuraciones.CANTIDAD_MAXIMA_CONCURRENCIA);
	atender_clientes(servidor_Consola, &escuchar_Conexiones_Consola); // asincronico - multihilo

	//INICIAR SERVIDOR PARA ESCUCHAR CPU:
	int servidor_CPU = crear_servidor(configuraciones.PUERTO_CPU, configuraciones.CANTIDAD_MAXIMA_CONCURRENCIA);
	atender_clientes(servidor_CPU, &escuchar_Conexiones_CPU); // asincronico - multihilo

	//CLIENTE PARA EL FILESYSTEM:
	//provisoriamente va en la opcion de usuario 8 para el handshake
	//int filesystem = conectar_servidor(configuraciones.IP_FS, configuraciones.PUERTO_FS);
	//CU_iniciar_programa(filesystem);

	atender_solicitudes_de_usuario();

	//destruir_conexion_servidor(servidor_Consola);
	//destruir_conexion_servidor(servidor_CPU);
	return EXIT_SUCCESS;
}

void mostrar_menu_usuario() {
	printf("\n******* MENU KERNEL ******");
	printf("\n 1 - Obtener listado de procesos del Sistema.");
	printf("\n 2 - Obtener Informacion de un proceso");
	printf("\n 3 - Obtener la tabla global de archivos");
	printf("\n 4 - Modificar grado de multiprogramacion");
	printf("\n 5 - Finalizar proceso");
	printf("\n 6 - Detener la planificacion");
	printf("\n 7 - CONECTARSE CON LA MEMORIA - PRIMER CHEKPOINT");
	printf("\n 8 - CONECTARSE CON FILE SYSTEM - PRIMER CHEKPOINT");
	printf("\n 9 - Salir");
	printf("\n Opcion: ");
}

void atender_solicitudes_de_usuario() {
	int opcion = 0;
	do {
		mostrar_menu_usuario();
		opcion = validarNumeroInput(1, 9);
		switch (opcion) {

		case 1:

			break;
		case 2:

			break;
		case 3:

			break;
		case 4:

			break;
		case 5:

			break;
		case 6:

			break;
		case 7:
            solicitar_bytes_memoria();
			break;
		case 8:
			//provisorio para el checkpoint
			CU_iniciar_programa(conectar_servidor(configuraciones.IP_FS, configuraciones.PUERTO_FS));
			break;
		}
	} while (opcion != 9);
}

void escuchar_Conexiones_Consola(int servidorConsola) {
	do {
		int cliente = aceptar_conexion_cliente(servidorConsola);
		char* codigo_IDENTIFICACION = recibir_dato_serializado(cliente);
		pthread_t mihilo1;
		if (strcmp(codigo_IDENTIFICACION, "CONSOLA") == 0) {
			pthread_create(&mihilo1, NULL, &CU_Recibir_Conexiones_Consola, cliente);
			pthread_detach(&mihilo1);
		} else {
			close(cliente);
		}
	} while (1);
}

void escuchar_Conexiones_CPU(int servidorCPU) {
	do {
		int cliente = aceptar_conexion_cliente(servidorCPU);
		char* codigo_IDENTIFICACION = recibir_dato_serializado(cliente);
		pthread_t mihilo1;
		if (strcmp(codigo_IDENTIFICACION, "CPU") == 0) {
			pthread_create(&mihilo1, NULL, &CU_Recibir_Conexiones_CPU, cliente);
			pthread_detach(&mihilo1);
		} else {
			close(cliente);
		}
	} while (1);
}

void CU_Recibir_Conexiones_Consola(int clienteConsola) {
	printf("Se conecto CONSOLA\n");
	enviar_dato_serializado("KERNEL", clienteConsola);

}
void CU_Recibir_Conexiones_CPU(int clienteCPU) {
	printf("Se conecto CPU\n");

	enviar_dato_serializado("RECIBIR_PCB", clienteCPU);
	enviar_dato_serializado("PEDIR_MEMORIA", clienteCPU);
	enviar_dato_serializado("SIGUSR1", clienteCPU);


	close(clienteCPU);
}

void CU_iniciar_programa(int filesystem){
	printf("Se conecto FILE SYSTEM\n");
	enviar_dato_serializado("KERNEL", filesystem);

	char * respuesta = recibir_dato_serializado(filesystem);

	if(strcmp(respuesta, "FILESYSTEM") == 0){
		printf("Handshake exitoso\n");

	}
	close(filesystem);
}
