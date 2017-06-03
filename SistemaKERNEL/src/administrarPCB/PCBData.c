#include "PCBData.h"

#include <commons/collections/list.h>
#include <commons/string.h>
#include <parser/metadata_program.h>
#include <parser/parser.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../header/PCB.h"
#include "../interfaz/InterfazMemoria.h"


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

void procesar_programa(char * programa, PCB * pcb){
	t_metadata_program * meta = metadata_desde_literal(programa);

	pcb->etiquetas = meta->etiquetas;
	pcb->etiquetas_size = meta->etiquetas_size;

	int cantidad_paginas = enviar_programa_memoria(meta, pcb, programa);
	if(cantidad_paginas >= 0)
		pcb->cantidad_paginas_codigo = cantidad_paginas;

	printf("LLego el programa de consola, prueba (si enviaste el completo.ansisop te tira la primera etiqueta): %s\n", meta->etiquetas);
	//TODO enviar_programa_memoria deberia devolver algo que no sea 1 .-.
		/* La variable RESULTADO es para saber si se le pudo
		 * asignar memoria o no. En el caso de que SI el
		 * resultado va a ser mayor a 0 y se utilizará para
		 * actualizar el valor de cantidad_paginas del PCB.
		 * En el caso de que NO, entonces su valor sera menor
		 * a 0 y se empleara como EXIT_CODE.
		 */
}

/* //ejemplo de busqueda de etiqueta espeficifica en la lista serializada
	char etiqueta[] = "Proximo";
	t_puntero_instruccion instruccion_obtenida = metadata_buscar_etiqueta(etiqueta, meta->etiquetas, meta->etiquetas_size);
	printf("Instruccion correspondiente a la etiqueta imprimir es %s\n", string_substring(programa, meta->instrucciones_serializado[instruccion_obtenida].start, meta->instrucciones_serializado[instruccion_obtenida].offset));
 *
 */

