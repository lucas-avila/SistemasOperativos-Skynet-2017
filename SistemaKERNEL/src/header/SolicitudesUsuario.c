/*
 * SolicitudesUsuario.c
 *
 *  Created on: 1/4/2017
 *      Author: utnso
 */

#include <commons/collections/list.h>
#include <commons/log.h>
#include <commons/string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../administrarPCB/EstadisticaProceso.h"
#include "../administrarProcesos/Proceso.h"
#include "../general/funcionesUtiles.h"
#include "../general/Socket.h"
#include "../planificacion/Planificacion.h"
#include "../testing/TestingMenu.h"
#include "AppConfig.h"

char * info_log;

void mostrar_menu_usuario() {
	printf("\n******* MENU KERNEL ******");
	printf("\n 1 - Obtener listado de procesos del Sistema.");
	printf("\n 2 - Obtener Informacion de un proceso.");
	printf("\n 3 - Obtener la tabla global de archivos.");
	printf("\n 4 - Modificar grado de multiprogramacion.");
	printf("\n 5 - Finalizar proceso.");
	printf("\n 6 - Detener la planificacion.");
	printf("\n 7 - MENU TESTING.");
	printf("\n 8 - Salir.");
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
			mostrar_procesos(procesos);
			break;
		case 2:
			listar_procesos_por_cola();
			break;
		}
	} while (opcion != 3);
	system("clear");
	atender_solicitudes_de_usuario();
}

char* devolver_cola(int opcion){

	switch(opcion){

	case 1:
		return NEW;
		break;
	case 2:
		return READY;
		break;
	case 3:
		return EXEC;
		break;
	case 4:
		return EXIT;
		break;
	case 5:
		return EXIT;
	}

}

void listar_procesos_por_cola(){
	int opcion = 0;
	do {
		mostrar_menu_colas();
		opcion = validarNumeroInput(1, 6);
		system("clear");
		char * cola_elegida = devolver_cola(opcion);
		if(opcion != 6) {
			int buscar_procesos_por_cola(Proceso * proceso){
			return strcmp(proceso->cola, cola_elegida) == 0;
			}
			t_list * procesos_filtrados = list_filter(procesos, &buscar_procesos_por_cola);
			mostrar_procesos(procesos_filtrados);
		}
	} while(opcion!= 6);
}

void mostrar_procesos(t_list * procesos_lista){
	int size = list_size(procesos_lista);
	int i = 0;
	Proceso * proceso;
	while(i < size){
		proceso = list_get(procesos_lista, i);
		printf("⚛⚛⚛⚛⚛⚛⚛⚛⚛⚛⚛⚛⚛⚛⚛⚛⚛⚛⚛⚛\n");
		printf("Proceso ID: %d\n", proceso->PID);
		printf("Su socket es: %d\n", proceso->socket);
		printf("Su cola actual es: %s\n", proceso->cola);
		i++;
	}
	printf("---> Cantidad de procesos totales encontrados: %d\n", i);
	printf("⚛⚛⚛⚛⚛ Fin de busqueda ⚛⚛⚛⚛⚛\n");
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

void mostrar_menu_informacion_proceso(){
	printf("\n******* Elija la informacion deseada: ******");
	printf("\n 1 - Cantidad de rafagas ejecutadas.");
	printf("\n 2 - Cantidad de operaciones privilegiadas ejecutadas.");
	printf("\n 3 - Tabla de archivos abiertos por el proceso.");
	printf("\n 4 - Cantidad de paginas de Heap utilizadas.");
	printf("\n 5 - Cantidad de syscalls ejecutadas.");
	printf("\n 6 - Volver.");
	printf("\n Opcion: ");
}

void generar_log(){
	string_append(&info_log, "------Fin del LOG------\n\0");
	printf("\n%s", info_log);
    t_log* logger = log_create(configuraciones.PATH_ARCHIVO_LOG, "KERNEL",false, LOG_LEVEL_INFO);
    log_info(logger, "\nInfo logs del KERNEL: %s", info_log);
    log_destroy(logger);
}

void obtener_informacion_proceso(){
	int opcion = 0;
	int pid;
	EstadisticaProceso * estadistica_proceso;
	printf("\nPor favor ingrese el PID del proceso del cual desea informacion: ");
	scanf("%d", &pid);
	estadistica_proceso = buscar_registro_por_PID(pid);
	info_log = string_new();
		do {
			mostrar_menu_informacion_proceso();
			opcion = validarNumeroInput(1, 6);
			system("clear");
			switch(opcion) {

			case 1:
				string_append(&info_log, "Cantidad de rafagas ejecutadas: ");
				string_append(&info_log, string_itoa(estadistica_proceso->cantidad_Rafagas_Ejecutadas));
				string_append(&info_log, "\n");
				break;
			case 2:
				string_append(&info_log, "Cantidad operaciones privilegiadas ejecutadas: ");
				string_append(&info_log, string_itoa(estadistica_proceso->cantidad_Operaciones_Privilegadas_Ejecutadas));
				string_append(&info_log, "\n");
				break;
			case 3:
				//mostrar_tabla_archivos(proceso); TODO
				break;
			case 4:
				//mostrar_paginas_heap(proceso); TODO
				break;
			case 5:
				string_append(&info_log, "Cantidad syscalls ejecutadas: ");
				string_append(&info_log, string_itoa(estadistica_proceso->cantidad_SysCall_Ejecutadas));
				string_append(&info_log, "\n");
				break;
			}
		} while (opcion != 6);
		generar_log();
		system("clear");
		atender_solicitudes_de_usuario();
}

void modificar_grado_multiprogramacion(){
	int grado_multiprog;

	system("clear");
	do {
		printf("Ingrese el nuevo grado de multiprogramacion: ");
		scanf("%d", &grado_multiprog);
		if( grado_multiprog < 0) printf("Por favor vuelva a intentarlo con un numero mayor/igual a 0.\n");
	} while (grado_multiprog < 0);

	configuraciones.GRADO_MULTIPROG = grado_multiprog;
	printf("--- Modificacion exitosa ---\n");
}

void detener_planificacion(){
	configuraciones.planificacion_activa = 0;
	printf("--- Se detuvo la planificacion ---\n");
}

void verificar_estado(uint32_t pid){

	Proceso * proceso_a_eliminar = buscar_proceso_by_PID(pid);
	while(strcmp(proceso_a_eliminar->cola, "EXEC") == 0);

	if(strcmp(proceso_a_eliminar->cola, "EXIT") == 0) {
		printf("El proceso ya ha finalizado.\n");
	} else {
		actualizar_exit_code(proceso_a_eliminar, -7);
		finalizar_proceso(proceso_a_eliminar);
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
			obtener_informacion_proceso();
			break;
		case 3:
			//mostrar_tabla_global_archivos(); TODO
			enviar_dato_serializado("MENSAJE 1", 8);
			enviar_dato_serializado("MENSAJE 2", 9);
			break;
		case 4:
			modificar_grado_multiprogramacion();
			break;
		case 5: {
			int pid;
			Proceso * proceso_a_eliminar;
			printf("\nPor favor ingrese el PID del proceso que desea MATAR: ");
			scanf("%d", &pid);
			verificar_estado(pid);
		}
			break;
		case 6:
			detener_planificacion();
			break;
		case 7:
			menu_principal_testing();
			break;
		}
	} while (opcion != 8);
}

