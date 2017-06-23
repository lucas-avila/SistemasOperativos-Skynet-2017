#include "PCBData.h"

#include <commons/collections/list.h>
#include <commons/string.h>
#include <parser/metadata_program.h>
#include <semaphore.h>
#include <stdlib.h>

#include "../general/Semaforo.h"
#include "../../../Sharedlib/Sharedlib/PCB.h"
#include "../interfaz/InterfazMemoria.h"

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
	IndiceStack * elemento_pila_inicial_vacio = malloc(sizeof(IndiceStack));
	elemento_pila_inicial_vacio->posicion = 0;
	elemento_pila_inicial_vacio->argumentos = list_create();
	elemento_pila_inicial_vacio->variables = list_create();
	elemento_pila_inicial_vacio->retVar = malloc(sizeof(ReturnVariable));
	elemento_pila_inicial_vacio->retVar->byte_inicial= 9999;
	elemento_pila_inicial_vacio->retVar->pagina = 9999;
	elemento_pila_inicial_vacio->retVar->tamanio = 9999;

	list_add(pcb->pila, elemento_pila_inicial_vacio);

	pcb->cantidad_rafagas_ejecutadas = 0;

	return pcb;
}

void procesar_programa(char * programa, PCB * pcb){
	t_metadata_program * meta = metadata_desde_literal(programa);

	pcb->etiquetas = meta->etiquetas;
	pcb->etiquetas_size = meta->etiquetas_size;

	pcb->program_counter = meta->instruccion_inicio;

	int cantidad_paginas = enviar_programa_memoria(meta, pcb, programa);
	if(cantidad_paginas >= 0)
		pcb->cantidad_paginas_codigo = cantidad_paginas;

}

/* //ejemplo de busqueda de etiqueta espeficifica en la lista serializada
	char etiqueta[] = "Proximo";
	t_puntero_instruccion instruccion_obtenida = metadata_buscar_etiqueta(etiqueta, meta->etiquetas, meta->etiquetas_size);
	printf("Instruccion correspondiente a la etiqueta imprimir es %s\n", string_substring(programa, meta->instrucciones_serializado[instruccion_obtenida].start, meta->instrucciones_serializado[instruccion_obtenida].offset));
 *
 */


/*
	typedef struct {
		t_puntero_instruccion	start;
		t_size		offset;
	} t_intructions;

	typedef struct {
		t_puntero_instruccion	instruccion_inicio;	//El numero de la primera instruccion (Begin)
		t_size			instrucciones_size;				// Cantidad de instrucciones
		t_intructions*	instrucciones_serializado; 		// Instrucciones del programa

		t_size			etiquetas_size;					// Tamaño del mapa serializado de etiquetas
		char*			etiquetas;							// La serializacion de las etiquetas

		int				cantidad_de_funciones;
		int				cantidad_de_etiquetas;
	} t_metadata_program;
 */


