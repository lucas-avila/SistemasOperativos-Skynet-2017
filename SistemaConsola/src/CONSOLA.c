/*
 ============================================================================
 Name        : CONSOLA.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "header/AppConfig.h"
#include "header/Socket.h"
#include "header/funcionesUtiles.h"

void atender_solicitudes_de_usuario();

int kernel;

int main(int argc, char *argv[]) {
	inicializar_configuracion(argv[1]);
	kernel = conectar_servidor(configuraciones.IP_KERNEL, configuraciones.PUERTO_KERNEL);

	atender_solicitudes_de_usuario();

	return EXIT_SUCCESS;
}

void mostrar_menu_usuario() {
	printf("\n******* Bienvenido a Esther ******");
	printf("\n 1 - Iniciar programa -> ruta al programa (Handshake Kernel)");
	printf("\n 2 - Finalizar programa -> PID");
	printf("\n 3 - Desconectar consola");
	printf("\n 4 - Limpiar mensajes");
	printf("\n 0 - Salir");
	printf("\n Opcion: ");
}

void atender_solicitudes_de_usuario() {
	int opcion = -1;
	do {
		mostrar_menu_usuario();
		opcion = validarNumeroInput(0, 4);
		switch (opcion) {

		case 1:
			CU_iniciar_programa(kernel);
			break;
		case 2:

			break;
		case 3:

			break;
		case 4:

			break;
		}
	} while (opcion != 0);
}

void CU_iniciar_programa(int kernel){
	enviar_dato_serializado("CONSOLA", kernel);

	char * respuesta = recibir_dato_serializado(kernel);
	if(strcmp(respuesta, "KERNEL") == 0){
		printf("Handshake exitoso\n");
		exit(0);
	}
}
