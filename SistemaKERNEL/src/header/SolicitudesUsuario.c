/*
 * SolicitudesUsuario.c
 *
 *  Created on: 1/4/2017
 *      Author: utnso
 */

#include<stdio.h>
#include<stdlib.h>
#include "../general/funcionesUtiles.h"
#include "../header/AppConfig.h"
#include "../testing/TestingMenu.h"
void mostrar_menu_usuario() {
	printf("\n******* MENU KERNEL ******");
	printf("\n 1 - Obtener listado de procesos del Sistema.");
	printf("\n 2 - Obtener Informacion de un proceso");
	printf("\n 3 - Obtener la tabla global de archivos");
	printf("\n 4 - Modificar grado de multiprogramacion");
	printf("\n 5 - Finalizar proceso");
	printf("\n 6 - Detener la planificacion");
	printf("\n 7 - MENU TESTING");
	printf("\n 8 - Salir");
	printf("\n Opcion: ");
}

void atender_solicitudes_de_usuario() {
	int opcion = 0;
	do {
		system("clear");
		mostrar_menu_usuario();
		opcion = validarNumeroInput(1, 8);
		system("clear");
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
			menu_principal_testing();
			break;

		}
	} while (opcion != 8);
}
