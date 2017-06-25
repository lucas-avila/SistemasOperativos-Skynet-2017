/*
 * SolicitudesUsuario.c
 *
 *  Created on: 1/4/2017
 *      Author: utnso
 */

#include "SolicitudesUsuario.h"

#include <commons/collections/list.h>
#include <commons/log.h>
#include <commons/string.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../administrarPCB/EstadisticaProceso.h"
#include "../administrarProcesos/Proceso.h"
#include "../general/funcionesUtiles.h"
#include "../general/Semaforo.h"
#include "../../../Sharedlib/Sharedlib/Socket.h"
#include "../planificacion/Planificacion.h"
#include "../testing/TestingMenu.h"
#include "../capaFILESYSTEM/TablaGlobalArchivo.h"
#include "AppConfig.h"

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

void mostrar_menu_listado_procesos() {
	printf("\n******* Listado de procesos: ******");
	printf("\n 1 - Todos.");
	printf("\n 2 - En algún estado (cola).");
	printf("\n 3 - Volver.");
	printf("\n Opcion: ");
}

void listar_procesos() {
	int opcion = 0;
	do {
		mostrar_menu_listado_procesos();
		opcion = validarNumeroInput(1, 3);
		system("clear");
		switch (opcion) {

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

char* devolver_cola(int opcion) {

	switch (opcion) {

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

void listar_procesos_por_cola() {
	int opcion = 0;
	do {
		mostrar_menu_colas();
		opcion = validarNumeroInput(1, 6);
		system("clear");
		char * cola_elegida = devolver_cola(opcion);
		if (opcion != 6) {
			int buscar_procesos_por_cola(Proceso * proceso) {
				return strcmp(proceso->cola, cola_elegida) == 0;
			}
			sem_wait(&mutex_lista_PROCESOS);
			t_list * procesos_filtrados = list_filter(procesos, &buscar_procesos_por_cola);
			sem_post(&mutex_lista_PROCESOS);
			mostrar_procesos(procesos_filtrados);
		}
	} while (opcion != 6);
}

void mostrar_procesos(t_list * procesos_lista) {
	int size = list_size(procesos_lista);
	int i = 0;
	Proceso * proceso;
	while (i < size) {
		proceso = list_get(procesos_lista, i);
		string_append(&info_log, "\n---------------------\n");
		string_append(&info_log, "Proceso ID: ");
		string_append(&info_log, string_itoa(proceso->PID));
		i++;
	}
	string_append(&info_log, "\n---------------------");
	string_append(&info_log, "\n---> Cantidad de procesos totales encontrados: ");
	string_append(&info_log, string_itoa(i));
	generar_log();
}

void mostrar_menu_colas() {
	printf("\n******* Elija una cola de procesos: ******");
	printf("\n 1 - New.");
	printf("\n 2 - Ready.");
	printf("\n 3 - Running.");
	printf("\n 4 - Finished.");
	printf("\n 5 - Waiting.");
	printf("\n 6 - Volver.");
	printf("\n Opcion: ");
}

void mostrar_menu_informacion_proceso() {
	printf("\n******* Elija la informacion deseada: ******");
	printf("\n 1 - Cantidad de rafagas ejecutadas.");
	printf("\n 2 - Tabla de archivos abiertos por el proceso.");
	printf("\n 3 - Cantidad de paginas de Heap utilizadas.");
	printf("\n 4 - Cantidad de syscalls ejecutadas.");
	printf("\n 5 - Volver.");
	printf("\n Opcion: ");
}

void generar_log() {
	string_append(&info_log, "\n------Fin del LOG------\n\0");
	printf("\n%s", info_log);
	t_log* logger = log_create(configuraciones.PATH_ARCHIVO_LOG, "KERNEL", true, LOG_LEVEL_INFO);
	log_info(logger, "\n***LOGS del KERNEL***\n %s", info_log);
	log_destroy(logger);
	strcpy(info_log, "");
}

void mencionar_proceso(int pid) {
	string_append(&info_log, "\n-->Logs del Proceso ");
	string_append(&info_log, string_itoa(pid));
	string_append(&info_log, ":\n");
}

void obtener_informacion_proceso() {
	int opcion = 0;
	int pid;
	EstadisticaProceso * estadistica_proceso;
	printf("\nPor favor ingrese el PID del proceso del cual desea informacion: ");
	scanf("%d", &pid);
	Proceso* proceso ;
	proceso = buscar_proceso_by_PID(pid);
	if(proceso==NULL){
		printf("\n el Proceso ingresado no existe.");
		return;
	}
	estadistica_proceso = buscar_registro_por_PID(pid);
	mencionar_proceso(pid);

	strcpy(info_log, "");
	do {
		mostrar_menu_informacion_proceso();
		opcion = validarNumeroInput(1, 5);
		system("clear");
		switch (opcion) {

		case 1:
			string_append(&info_log, "Cantidad de rafagas ejecutadas: ");
			string_append(&info_log, string_itoa(estadistica_proceso->cantidad_Rafagas_Ejecutadas));
			string_append(&info_log, "\n");
			break;
		case 2:

			mostrar_tabla_proceso_archivos(proceso->tablaProcesoArchivo,info_log);
			break;
		case 3:
			string_append(&info_log, "Cantidad de rafagas ejecutadas: ");
			string_append(&info_log, string_itoa(estadistica_proceso->cantidad_Paginas_HEAP_Utilizadas));
			string_append(&info_log, "\n");

			break;
		case 4:
			string_append(&info_log, "Cantidad syscalls ejecutadas: ");
			string_append(&info_log, string_itoa(estadistica_proceso->cantidad_SYSCALL_Ejecutadas));
			string_append(&info_log, "\n");
			break;
		}
		//printf("\n Presione 0 para continuar");
	//	while (getchar() != '0')
		//	;
	} while (opcion != 5);
	generar_log();
	system("clear");
	printf("\n Se ha generado los logs en archivo.");
	printf("\n Presione 0 para continuar");
	while (getchar() != '0')
				;
	//atender_solicitudes_de_usuario();
}

void modificar_grado_multiprogramacion() {
	int grado_multiprog;

	system("clear");
	do {
		printf("Ingrese el nuevo grado de multiprogramacion: ");
		scanf("%d", &grado_multiprog);
		if (grado_multiprog < 0)
			printf("Por favor vuelva a intentarlo con un numero mayor/igual a 0.\n");
	} while (grado_multiprog < 0);

	configuraciones.GRADO_MULTIPROG = grado_multiprog;
	string_append(&info_log, "--> Cambio de grado de multiprogramacion a: ");
	string_append(&info_log, string_itoa(grado_multiprog));
	generar_log();
}

void detener_planificacion() {
	configuraciones.planificacion_activa = 0;
	string_append(&info_log, "--- Se detuvo la planificacion ---\n");
	generar_log();
}

void verificar_estado(uint32_t pid, int exit_code) {

	Proceso * proceso_a_eliminar = buscar_proceso_by_PID(pid);
	while (strcmp(proceso_a_eliminar->cola, "EXEC") == 0)
		;

	if (strcmp(proceso_a_eliminar->cola, "EXIT") == 0) {
		printf("El proceso ya ha finalizado.\n");
	} else {
		actualizar_exit_code(proceso_a_eliminar, exit_code);
		finalizar_proceso(proceso_a_eliminar);
	}
}

void mostrar_tabla_global_archivos() {
	string_append(&info_log, "\n---TABLA GLOBAL DE ARCHIVOS---\n");
	string_append(&info_log, "\n FILE \t\t\t OPEN\n");
	int size = list_size(TABLA_GLOBAL_ARCHIVO);
	int i = 0;
	while (i < size) {
		TablaGlobalArchivo * elemento = list_get(TABLA_GLOBAL_ARCHIVO, i);
		string_append(&info_log, elemento->file);
		string_append(&info_log, " \t\t\t ");
		string_append(&info_log, string_itoa(elemento->open));
		string_append(&info_log, "\n");
	}
	generar_log();
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
			mostrar_tabla_global_archivos();
			break;
		case 4:
			modificar_grado_multiprogramacion();
			break;
		case 5: {
			int pid;
			Proceso * proceso_a_eliminar;
			printf("\nPor favor ingrese el PID del proceso que desea MATAR: ");
			scanf("%d", &pid);
			verificar_estado(pid, -7);
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

