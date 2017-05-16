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
#include "PCB.h"
#include "../administrarProcesos/Proceso.h"


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

void mostrar_menu_listado_procesos(){
	printf("\n******* Listado de procesos: ******");
	printf("\n 1 - Todos.");
	printf("\n 2 - En algún estado (cola).");
	printf("\n 3 - Volver.");
	printf("\n Opcion: ");
}

void listar_procesos(){
	int opcion = 0;
	do {
		mostrar_menu_listado_procesos();
		opcion = validarNumeroInput(1, 3);
		system("clear");
		switch(opcion) {

		case 1:
			mostrar_info_procesos(procesos);
			break;
		case 2:
			listar_procesos_por_cola();
			break;
		}
	} while (opcion != 3);
	system("clear");
	atender_solicitudes_de_usuario();
}

void listar_procesos_por_cola(){
	mostrar_menu_colas();
	int opcion = 0;
	scanf("%d", &opcion);
	if(opcion != 6) {
		int buscar_procesos_por_cola(Proceso * proceso){
			return proceso->cola == opcion;
		}
		t_list * procesos_filtrados = list_filter(procesos, &buscar_procesos_por_cola);
		mostrar_info_procesos(procesos_filtrados);
	}
}

void mostrar_info_procesos(t_list * procesos_lista){
	int size = list_size(procesos_lista);
	int i = 0;
	Proceso * proceso;
	while(i < size){
		proceso = list_get(procesos_lista, i);
		printf("----------------------------\n");
		printf("Proceso ID: %d\n", proceso->PID);
		printf("Su consola es: %d\n", proceso->consola);
		proceso->cola = 1;
		mostrar_cola(proceso->cola);
		i++;
	}
	printf("---> Cantidad de procesos totales encontrados: %d\n", i);
	printf("-----Fin de busqueda-----\n");
}

void mostrar_menu_colas(){
	printf("\n******* Elija una cola de procesos: ******");
	printf("\n 1 - New.");
	printf("\n 2 - Ready.");
	printf("\n 3 - Running.");
	printf("\n 4 - Finished.");
	printf("\n 5 - Waiting.");
	printf("\n 6 - Volver.");
	printf("\n Opcion: ");
}

void mostrar_cola(COLA cola){

	switch(cola){
	case NEW:
		printf("Su cola actual es: NEW\n");
		break;
	case READY:
		printf("Su cola actual es: READY\n");
		break;
	case EJECUTANDO:
		printf("Su cola actual es: RUNNING\n");
		break;
	case EXIT:
		printf("Su cola actual es: EXIT\n");
		break;
	case BLOQUEADO:
		printf("Su cola actual es: WAITING\n");
		break;
	}
}

void atender_solicitudes_de_usuario() {
	int opcion = 0;
	do {
		//system("clear");
		mostrar_menu_usuario();
		opcion = validarNumeroInput(1, 8);
		system("clear");
		switch (opcion) {

		case 1:
			listar_procesos();
			break;
		case 2:
			break;

		case 3:

			break;
		case 4:

			break;
		case 5: {
			int pid;
			Proceso * proceso_a_eliminar;
			printf("\nPor favor ingrese el PID del proceso que desea MATAR: ");
			scanf("%d", &pid);
			proceso_a_eliminar = actualizar_exit_code(-7, pid);
			finalizar_proceso(proceso_a_eliminar);
		}
			break;
		case 6:
			break;
		case 7:
			menu_principal_testing();
			break;

		}
	} while (opcion != 8);
}
