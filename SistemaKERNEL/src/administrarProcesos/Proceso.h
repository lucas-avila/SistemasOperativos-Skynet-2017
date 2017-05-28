/*
 * Proceso.h
 *
 *  Created on: 13/5/2017
 *      Author: utnso
 */

#ifndef ADMINISTRARPROCESOS_PROCESO_H_
#define ADMINISTRARPROCESOS_PROCESO_H_

#include <commons/collections/list.h>
#include <stdint.h>

#include "../header/Estructuras.h"
#include "../header/PCB.h"

typedef struct {
	uint32_t PID;
	PCB* pcb;
	t_list* tablaProcesoArchivo;
	int activo;

	char * cola;
	uint32_t socket;
	CPUInfo * cpu;

} Proceso;

t_list* procesos;

void inicializar_lista_procesos();
void eliminar_todos_registros();
Proceso* new_Proceso(PCB* pcb);

void agregar_proceso(Proceso* proceso);

Proceso* buscar_proceso_by_PID(uint32_t PID);

Proceso* proceso(PCB * pcb);

#endif /* ADMINISTRARPROCESOS_PROCESO_H_ */
