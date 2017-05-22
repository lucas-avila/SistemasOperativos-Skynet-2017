/*
 * Estructuras.c
 *
 *  Created on: 20/5/2017
 *      Author: utnso
 */
#include "Estructuras.h"

void inicializar_lista_Info_procesos() {
	Info_procesos = list_create();
}

Info_ejecucion* new_Info_ejecucion(int pid, int socket) {
	Info_ejecucion * info_ejecucion = malloc(sizeof(Info_ejecucion));
	info_ejecucion->pid = pid;
	info_ejecucion->fecha_inicio = time(NULL);
	info_ejecucion->cant_impresiones = 0;
	info_ejecucion->socket = socket;

	return info_ejecucion;
}

void agregar_proceso(Info_ejecucion* info_ejecucion) {
	list_add(Info_procesos, info_ejecucion);
}

void eliminar_todas_infos() {
	list_clean(Info_procesos);
}

void eliminar_info_proceso(Info_ejecucion* info_ejecucion) {
	list_remove(Info_procesos, info_ejecucion);
}

int buscar_indice_Info_proceso(uint32_t PID) {
	int tamanio = list_size(Info_procesos);
	int i = 0;
	Info_ejecucion* info_ejecucion;
	for (i = 0; i < tamanio; i++) {
		info_ejecucion = list_get(Info_procesos, i);
		if (info_ejecucion->pid == PID) {
			return i;
		}
	}
	return -1;
}

Info_ejecucion* buscar_info_por_PID(uint32_t PID) {
	int tamanio = list_size(Info_procesos);
	int i = 0;
	Info_ejecucion* info_ejecucion;
	for (i = 0; i < tamanio; i++) {
		info_ejecucion = list_get(Info_procesos, i);
		if (info_ejecucion->pid == PID) {
			return info_ejecucion;
		}
	}
	return NULL;
}
