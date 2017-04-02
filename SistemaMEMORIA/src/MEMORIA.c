/*
 ============================================================================
 Name        : MEMORIA.c
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

#include "header/AppConfig.h"
#include "header/Socket.h"

#include "header/SolicitudesUsuario.h"
#include "header/funcionesUtiles.h"

void reservar_memoria_principal();
void crear_e_inicializar_tabla_paginas_invertidas();
void atender_solicitudes_de_usuario();


void CU_Recibir_Conexion_KERNEL(int cliente);
void CU_Recibir_Conexion_CPU(int cliente);
void CU_Recibir_conexiones(int servidor);

void escuchar_peticiones_cliente(int servidor);

int main(int argc, char *argv[]) {

	//CUANDO SE INVOCA ENVIAR POR PARAMETRO EL PATH DEL ARCHIVO


	puts(argv[1]); /** El primer argumento path de archivo **/
	inicializar_configuracion(argv[1]);
	reservar_memoria_principal();
	crear_e_inicializar_tabla_paginas_invertidas();
	int servidor = iniciar_servidor(configuraciones.PUERTO, configuraciones.CANTIDAD_MAXIMA_CONCURRENCIA);
	escuchar_peticiones_cliente(servidor); // asincronico - multihilo
	atender_solicitudes_de_usuario();
	destruir_conexion_servidor(servidor);
	free(MEMORIA_PRINCIPAL); //Ver si esta bien que este asi.
	return EXIT_SUCCESS;
}

void reservar_memoria_principal() {
	MEMORIA_PRINCIPAL = malloc(configuraciones.MARCOS*configuraciones.MARCO_SIZE);
	//strcpy(MEMORIA_PRINCIPAL[10],"2");

	//int i=0;
	//for( i;i<configuraciones.MARCOS*configuraciones.MARCO_SIZE;i++){

	////	printf(" %s ",MEMORIA_PRINCIPAL[i]);
	//}

}
void crear_e_inicializar_tabla_paginas_invertidas() {

}



void mostrar_menu_usuario() {
	printf("******* MENU PRINCIPAL MEMORIA ******");
	printf("\n 1 - Modifcar Retardo");
	printf("\n 2 - Generar Reporte");
	printf("\n 3 - Limpiar CACHE");
	printf("\n 4 - Mostrar Tamaño");
	printf("\n 5 - Salir");
	printf("\n Opcion: ");
}

void atender_solicitudes_de_usuario() {
	int opcion = 0;
	do {
		mostrar_menu_usuario();
		opcion = validarNumeroInput(1, 5);
		switch (opcion) {


		case 1:
			CU_Modificar_Retardo();
			break;
		case 2:
			CU_Generar_Reporte();
			break;
		case 3:
			CU_Limpiar_CACHE();
			break;
		case 4:
			CU_Mostrar_Tamanio_MEMORIA_PID();
			break;
		}
	} while (opcion != 5);
}


void CU_Recibir_Conexion_KERNEL(int cliente) {

}
void CU_Recibir_Conexion_CPU(int cliente) {

}

void escuchar_peticiones_cliente(int servidor) {
	pthread_t mihilo1;
	pthread_create(&mihilo1, NULL, &CU_Recibir_conexiones, servidor);
	pthread_detach(&mihilo1);
}

void CU_Recibir_conexiones(int servidor) {
	do {
		int cliente = aceptar_conexion_cliente(servidor);
		char* codigo_IDENTIFICACION = recibir_dato_serializado(cliente);

		pthread_t mihilo1;
		if (strcmp(codigo_IDENTIFICACION, "KERNEL")==0) {
			pthread_create(&mihilo1, NULL, &CU_Recibir_Conexion_KERNEL, cliente);
			pthread_detach(&mihilo1);
		} else if (strcmp(codigo_IDENTIFICACION, "CPU")==0) {
			pthread_create(&mihilo1, NULL, &CU_Recibir_Conexion_CPU, cliente);
			pthread_detach(&mihilo1);
		} else {
			destruir_conexion_cliente(cliente);
		}

	} while (1);
}
