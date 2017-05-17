/*
 * Proceso.h
 *
 *  Created on: 13/5/2017
 *      Author: utnso
 */

#ifndef ADMINISTRARPROCESOS_PROCESO_H_
#define ADMINISTRARPROCESOS_PROCESO_H_

#include "../header/PCB.h"
#include "../capaFILESYSTEM/TablaProcesoArchivo.h"

#include "commons/collections/list.h"
#include "../planificacion/Planificacion.h"

typedef struct {
	uint32_t PID;
	PCB* pcb;
	t_list* tablaProcesoArchivo;
	int activo;

	COLA cola;
	uint32_t consola;
	uint32_t operaciones_privilegiadas_ejecutadas;
	uint32_t syscalls_ejecutadas;
} Proceso;

t_list* procesos;

void inicializar_lista_procesos();
void eliminar_todos_registros();
Proceso* new_Proceso(PCB* pcb);

void agregar_proceso(Proceso* proceso);

Proceso* buscar_proceso_by_PID(uint32_t PID);

#endif /* ADMINISTRARPROCESOS_PROCESO_H_ */
