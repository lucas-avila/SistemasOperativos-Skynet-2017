#include "PCBData.h"

#include <commons/collections/list.h>
#include <commons/string.h>
#include <parser/metadata_program.h>
#include <semaphore.h>
#include <stdlib.h>
#include <Sharedlib/PCB.h>

#include "../general/Semaforo.h"
#include "../interfaz/InterfazMemoria.h"
#include "../planificacion/Planificacion.h"
#include "../header/AppConfig.h"
#define MIN_PIDS 1000
int pids_reg = MIN_PIDS;

PCB * crear_pcb() {
	PCB * pcb = malloc(sizeof(PCB));

	sem_wait(&mutex_pids);
	pcb->PID = pids_reg;
	pids_reg++;
	sem_post(&mutex_pids);
	pcb->etiquetas = string_new();
	pcb->etiquetas_size = 0;
	pcb->codigo = list_create();
	pcb->pila = list_create();
	pcb->pagina_inicial_stack = 0;
	pcb->posicion_pagina_stack = 0;
	IndiceStack * elemento_pila_inicial_vacio = malloc(sizeof(IndiceStack));
	elemento_pila_inicial_vacio->posicion = 0;
	elemento_pila_inicial_vacio->argumentos = list_create();
	elemento_pila_inicial_vacio->variables = list_create();
	elemento_pila_inicial_vacio->retVar = malloc(sizeof(ReturnVariable));
	elemento_pila_inicial_vacio->retVar->byte_inicial = 9999;
	elemento_pila_inicial_vacio->retVar->pagina = 9999;
	elemento_pila_inicial_vacio->retVar->tamanio = 9999;

	list_add(pcb->pila, elemento_pila_inicial_vacio);

	pcb->cantidad_rafagas_ejecutadas = 0;

	pcb->exit_code = 0;

	return pcb;
}

void procesar_programa(char * programa, PCB * pcb) {
	t_metadata_program * meta = metadata_desde_literal(programa);

	pcb->etiquetas = meta->etiquetas;
	pcb->etiquetas_size = meta->etiquetas_size;

	pcb->program_counter = meta->instruccion_inicio;

	int resultado = enviar_programa_memoria(meta, pcb, programa);
	/* Si el Resultado es >= 0 son las páginas asignadas.
	 * Si el Resultado es < 0 es un exit_code.
	 */
	pcb->cantidad_paginas_codigo = configuraciones.STACK_SIZE;
	if (resultado <= 0) {
		pcb->exit_code = resultado;
		mover_PCB_de_cola(pcb, NEW, EXIT);
	}
}
