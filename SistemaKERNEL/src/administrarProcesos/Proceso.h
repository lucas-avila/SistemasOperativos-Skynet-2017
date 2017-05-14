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
typedef struct {
	char* PID;
	PCB* pcb;
	t_list* tablaProcesoArchivo;
	int activo;
} Proceso;

t_list* procesos;

void inicializar_lista_procesos();
void eliminar_todos_registros();
Proceso* new_Proceso(PCB* pcb);

void agregar_proceso(Proceso* proceso);

Proceso* buscar_proceso_by_PID(char* PID);

#endif /* ADMINISTRARPROCESOS_PROCESO_H_ */
