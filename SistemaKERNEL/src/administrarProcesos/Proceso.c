#include<stdlib.h>
#include<stdio.h>
#include <string.h>

#include "../header/PCB.h"
#include "../capaFILESYSTEM/TablaProcesoArchivo.h"
#include "commons/collections/list.h"
#include "Proceso.h"

void inicializar_lista_procesos() {
	procesos = list_create();
}

Proceso* new_Proceso(PCB* pcb) {
	Proceso * proceso = malloc(sizeof(Proceso));
	proceso->PID = pcb->PID;
	proceso->activo = 1;
	proceso->pcb = pcb;
	proceso->tablaProcesoArchivo = list_create();
	return proceso;
}

void agregar_proceso(Proceso* proceso) {
	list_add(procesos, proceso);
}

void eliminar_todos_registros() {
	list_clean(procesos);
}

void eliminar_proceso(Proceso* proceso) {
	list_remove(procesos, proceso);
}

Proceso* buscar_proceso_by_PID(uint32_t PID) {
	int tamanio = list_size(procesos);
	int i = 0;
	Proceso* proceso;
	for (i = 0; i < tamanio; i++) {
		proceso = list_get(procesos, i);
		if (proceso->PID == PID) {
			return proceso;
		}
	}
	return NULL;
}
