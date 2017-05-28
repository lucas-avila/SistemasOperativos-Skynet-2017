/*
 * EstadisticaProceso.h
 *
 *  Created on: 7/5/2017
 *      Author: utnso
 */

#ifndef ADMINISTRARPCB_ESTADISTICAPROCESO_H_
#define ADMINISTRARPCB_ESTADISTICAPROCESO_H_

typedef struct {
	char* PID;
	unsigned cantidad_Rafagas_Ejecutadas;
	unsigned cantidad_Operaciones_Privilegadas_Ejecutadas;
	int cantidad_Paginas_HEAP_Utilizadas;
	unsigned cantidad_Operaciones_Alocar;
	int tamanio_Total_Alocar;
	unsigned cantidad_Operaciones_Liberar;
	int tamanio_Total_Liberar;
	int cantidad_SysCall_Ejecutadas;
} EstadisticaProceso;

void incrementar_rafagas_ejecutadas(char *PID,unsigned rafagasEjecutadas);
void incrementar_en_uno_paginas_HEAP(char *PID);
void incrementar_MALLOC(char *PID,int tamanioMalloc);
void incrementar_FREE(char *PID,int tamanioFREE);
void incrementar_SYSCALL(char *PID, int cantidadSysCall);

void inicializar_tabla_proceso_estadistica();
EstadisticaProceso* crear_Estadistica_Proceso(char*PID ,unsigned cantidad_Rafagas_Ejecutadas, unsigned cantidad_Operaciones_Privilegadas_Ejecutadas, int cantidad_Paginas_HEAP_Utilizadas, unsigned cantidad_Operaciones_Alocar, int tamanio_Total_Alocar, unsigned cantidad_Operaciones_Liberar, int tamanio_Total_Liberar, int cantidad_SysCall_Ejecutadas);

void crear_Proceso_en_tabla(char *PID);
void guardar_registro_proceso(EstadisticaProceso* estadisticaProceso);

EstadisticaProceso* buscar_registro_por_PID(char* PID);

void actualizar_registro_proceso(EstadisticaProceso* estadisticaProceso);

int buscar_indice_tabla_estadistica_proceso(char* PID);

#endif /* ADMINISTRARPCB_ESTADISTICAPROCESO_H_ */
