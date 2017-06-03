#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "EstadisticaProceso.h"
#include "commons/collections/list.h"
t_list* TABLA_PROCESO_ESTADISTICA;

void inicializar_tabla_proceso_estadistica() {
	TABLA_PROCESO_ESTADISTICA = list_create();
}

EstadisticaProceso* crear_Estadistica_Proceso(uint32_t PID, uint32_t cantidad_Rafagas_Ejecutadas, uint32_t cantidad_Operaciones_Privilegadas_Ejecutadas, uint32_t cantidad_Paginas_HEAP_Utilizadas, uint32_t cantidad_Operaciones_Alocar, uint32_t tamanio_Total_Alocar, uint32_t cantidad_Operaciones_Liberar, uint32_t tamanio_Total_Liberar, uint32_t cantidad_SysCall_Ejecutadas) {
	EstadisticaProceso* estadistica = malloc(sizeof(EstadisticaProceso));
	estadistica->PID = PID;
	estadistica->cantidad_Rafagas_Ejecutadas = cantidad_Rafagas_Ejecutadas;
	estadistica->cantidad_Operaciones_Privilegadas_Ejecutadas = cantidad_Operaciones_Privilegadas_Ejecutadas;
	estadistica->cantidad_Paginas_HEAP_Utilizadas = cantidad_Paginas_HEAP_Utilizadas;
	estadistica->cantidad_Operaciones_Alocar = cantidad_Operaciones_Alocar;
	estadistica->tamanio_Total_Alocar = tamanio_Total_Alocar;
	estadistica->cantidad_Operaciones_Liberar = cantidad_Operaciones_Liberar;
	estadistica->tamanio_Total_Liberar = tamanio_Total_Liberar;
	estadistica->cantidad_SysCall_Ejecutadas = cantidad_SysCall_Ejecutadas;
	return estadistica;
}

void crear_Proceso_en_tabla(uint32_t PID) {
	EstadisticaProceso* estadistica = crear_Estadistica_Proceso(PID, 0, 0, 0, 0, 0, 0, 0, 0);
	guardar_registro_proceso(estadistica);
}

void guardar_registro_proceso(EstadisticaProceso* estadisticaProceso) {
	list_add(TABLA_PROCESO_ESTADISTICA, estadisticaProceso);
}

EstadisticaProceso* buscar_registro_por_PID(uint32_t PID) {
	int tamanio = list_size(TABLA_PROCESO_ESTADISTICA);
	int i = 0;
	for (i = 0; i < tamanio; i++) {
		EstadisticaProceso* element = list_get(TABLA_PROCESO_ESTADISTICA, i);
		if (element->PID == PID) {
			return element;
		}
	}
	return NULL;
}

void actualizar_registro_proceso(EstadisticaProceso* estadisticaProceso) {
	list_replace(TABLA_PROCESO_ESTADISTICA, buscar_indice_tabla_estadistica_proceso(estadisticaProceso->PID), estadisticaProceso);
}

int buscar_indice_tabla_estadistica_proceso(uint32_t PID) {
	int tamanio = list_size(TABLA_PROCESO_ESTADISTICA);
	int i = 0;
	for (i = 0; i < tamanio; i++) {
		EstadisticaProceso* element = list_get(TABLA_PROCESO_ESTADISTICA, i);
		if (element->PID == PID) {
			return i;
		}
	}
	return -1;
}

void incrementar_rafagas_ejecutadas(uint32_t PID, uint32_t rafagasEjecutadas) {
	EstadisticaProceso* elemento = buscar_registro_por_PID(PID);
	if (elemento == NULL) {
		crear_Proceso_en_tabla(PID);
		elemento = buscar_registro_por_PID(PID);
	}
	elemento->cantidad_Rafagas_Ejecutadas += rafagasEjecutadas;
	actualizar_registro_proceso(elemento);
}
void incrementar_en_uno_paginas_HEAP(uint32_t PID) {
	EstadisticaProceso* elemento = buscar_registro_por_PID(PID);
	if (elemento == NULL) {
		crear_Proceso_en_tabla(PID);
		elemento = buscar_registro_por_PID(PID);
	}
	elemento->cantidad_Paginas_HEAP_Utilizadas += 1;
	actualizar_registro_proceso(elemento);

}
void incrementar_MALLOC(uint32_t PID, uint32_t tamanioMalloc) {
	EstadisticaProceso* elemento = buscar_registro_por_PID(PID);
	if (elemento == NULL) {
		crear_Proceso_en_tabla(PID);
		elemento = buscar_registro_por_PID(PID);
	}
	elemento->cantidad_SysCall_Ejecutadas += 1;
	elemento->cantidad_Operaciones_Alocar += 1;
	elemento->tamanio_Total_Alocar += tamanioMalloc;
	actualizar_registro_proceso(elemento);
}
void incrementar_FREE(uint32_t PID, uint32_t tamanioFREE) {
	EstadisticaProceso* elemento = buscar_registro_por_PID(PID);
	if (elemento == NULL) {
		crear_Proceso_en_tabla(PID);
		elemento = buscar_registro_por_PID(PID);
	}
	elemento->cantidad_SysCall_Ejecutadas += 1;
	elemento->cantidad_Operaciones_Liberar += 1;
	elemento->tamanio_Total_Liberar += tamanioFREE;
	actualizar_registro_proceso(elemento);
}
void incrementar_SYSCALL(uint32_t PID, uint32_t cantidadSysCall) {
	EstadisticaProceso* elemento = buscar_registro_por_PID(PID);
	if (elemento == NULL) {
		crear_Proceso_en_tabla(PID);
		elemento = buscar_registro_por_PID(PID);
	}
	elemento->cantidad_SysCall_Ejecutadas += cantidadSysCall;
	actualizar_registro_proceso(elemento);
}
