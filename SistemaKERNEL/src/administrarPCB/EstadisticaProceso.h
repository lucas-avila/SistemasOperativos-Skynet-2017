/*
 * EstadisticaProceso.h
 *
 *  Created on: 7/5/2017
 *      Author: utnso
 */

#ifndef ADMINISTRARPCB_ESTADISTICAPROCESO_H_
#define ADMINISTRARPCB_ESTADISTICAPROCESO_H_
#include <stdint.h>

typedef struct {
	uint32_t PID;
	uint32_t cantidad_Rafagas_Ejecutadas;
	uint32_t cantidad_Paginas_HEAP_Utilizadas;
	uint32_t cantidad_Operaciones_Alocar;
	uint32_t tamanio_Total_Alocar;
	uint32_t cantidad_Operaciones_Liberar;
	uint32_t tamanio_Total_Liberar;
	uint32_t cantidad_SysCall_Ejecutadas;
} EstadisticaProceso;

int buscar_indice_tabla_estadistica_proceso(uint32_t PID);

void incrementar_rafagas_ejecutadas(uint32_t, uint32_t rafagasEjecutadas);
void incrementar_en_uno_paginas_HEAP(uint32_t PID);
void incrementar_MALLOC(uint32_t PID, uint32_t tamanioMalloc);
void incrementar_FREE(uint32_t PID, uint32_t tamanioFREE);
void incrementar_SYSCALL(uint32_t PID, uint32_t cantidadSysCall);
void inicializar_tabla_proceso_estadistica();
void crear_Proceso_en_tabla(uint32_t PID);
void guardar_registro_proceso(EstadisticaProceso* estadisticaProceso);
void actualizar_registro_proceso(EstadisticaProceso* estadisticaProceso);

EstadisticaProceso* crear_Estadistica_Proceso(uint32_t PID , uint32_t cantidad_Rafagas_Ejecutadas, uint32_t cantidad_Paginas_HEAP_Utilizadas, uint32_t cantidad_Operaciones_Alocar, uint32_t tamanio_Total_Alocar, uint32_t cantidad_Operaciones_Liberar, uint32_t tamanio_Total_Liberar, uint32_t cantidad_SysCall_Ejecutadas);
EstadisticaProceso* buscar_registro_por_PID(uint32_t PID);





#endif /* ADMINISTRARPCB_ESTADISTICAPROCESO_H_ */
