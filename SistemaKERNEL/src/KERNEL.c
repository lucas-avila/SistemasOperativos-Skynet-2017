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

#include "general/Socket.h"
#include "header/SolicitudesUsuario.h"

#include "header/PCB.h"

#include "general/funcionesUtiles.h"
#include "header/AppConfig.h"
#include "interfaz/InterfazMemoria.h"
#include "testing/TestingInterfazMemoria.h"

void atender_solicitudes_de_usuario();

void escuchar_Conexiones_CPU(int servidorCPU);
void escuchar_Conexiones_Consola(int servidorConsola);

int main(int argc, char *argv[]) {
	inicializar_configuracion(argv[1]);

	int servidor_Consola = crear_servidor(configuraciones.PUERTO_PROG, configuraciones.CANTIDAD_MAXIMA_CONCURRENCIA);
	atender_clientes(servidor_Consola, &escuchar_Conexiones_Consola); // asincronico - multihilo

	int servidor_CPU = crear_servidor(configuraciones.PUERTO_CPU, configuraciones.CANTIDAD_MAXIMA_CONCURRENCIA);
	atender_clientes(servidor_CPU, &escuchar_Conexiones_CPU); // asincronico - multihilo

	iniciar_conexion_servidor_memoria();

	atender_solicitudes_de_usuario();

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
	printf("\n 7 - Salir");
	printf("\n Opcion: ");
}

void atender_solicitudes_de_usuario() {
	int opcion = 0;
	do {
		mostrar_menu_usuario();
		opcion = validarNumeroInput(1, 10);
		switch (opcion) {

		case 1: {
			//testeando crear PCB
			PCB * pcb_nuevo = crear_pcb();
			printf("PCB creado, PID es : %d\n", pcb_nuevo->pid);
			break;
		}
		case 2: {
			//testeando leer archivo de programa
			enviar_programa_memoria("../resource/programa_prueba.txt");

			break;
		}
		case 3:

			break;
		case 4:

			break;
		case 5:

			break;
		case 6:

			break;
		case 8:
			//provisorio para el checkpoint
			CU_iniciar_programa(conectar_servidor(configuraciones.IP_FS, configuraciones.PUERTO_FS));
			break;
		case 10:
			testear_intefaz();
			break;

		}
	} while (opcion != 7);
}

