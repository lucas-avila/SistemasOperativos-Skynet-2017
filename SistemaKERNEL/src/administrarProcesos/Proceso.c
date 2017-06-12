#include "Proceso.h"

#include <commons/string.h>
#include <semaphore.h>
#include <stdlib.h>
#include "../general/Semaforo.h"


void inicializar_lista_procesos() {
	procesos = list_create();
}

void eliminar_todos_registros(){
	list_clean(procesos);
}

Proceso* new_Proceso(PCB* pcb) {
	Proceso * proceso = malloc(sizeof(Proceso));
	proceso->PID = pcb->PID;
	proceso->activo = 1;
	proceso->pcb = pcb;
	proceso->tablaProcesoArchivo = list_create();
	proceso->cpu = NULL;
	proceso->cola = string_new();
	return proceso;
}

void agregar_proceso(Proceso* proceso) {
	sem_wait(&mutex_lista_PROCESOS);
	list_add(procesos, proceso);
	sem_post(&mutex_lista_PROCESOS);
}

Proceso* buscar_proceso_by_PID(uint32_t PID) {
	int i = 0;
	sem_wait(&mutex_lista_PROCESOS);
	int tamanio = list_size(procesos);
	Proceso* proceso;
	for (i = 0; i < tamanio; i++) {
		proceso = list_get(procesos, i);
		if (proceso->PID == PID) {
			sem_post(&mutex_lista_PROCESOS);
			return proceso;
		}
	}
	sem_post(&mutex_lista_PROCESOS);
	return NULL;
}

Proceso* proceso(PCB * pcb){
	return buscar_proceso_by_PID(pcb->PID);
}
