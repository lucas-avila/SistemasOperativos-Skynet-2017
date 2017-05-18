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

void procesar_programa(char * programa){
	t_metadata_program * meta = metadata_desde_literal(programa);

	PCB * pcb = crear_pcb();

	//ejemplo de busqueda de etiqueta espeficifica en la lista serializada
	char etiqueta[] = "Proximo";
	t_puntero_instruccion instruccion_obtenida = metadata_buscar_etiqueta(etiqueta, meta->etiquetas, meta->etiquetas_size);
	printf("Instruccion correspondiente a la etiqueta imprimir es %s\n", string_substring(programa, meta->instrucciones_serializado[instruccion_obtenida].start, meta->instrucciones_serializado[instruccion_obtenida].offset));


	int paginaSentencia = atoi(asignar_Paginas_Programa(string_itoa(pcb->PID), "1"));

	int cantidadSentencias = meta->instrucciones_size;
	int i = 0;
	int indiceInicial = 0;
	for (i = 0; i < cantidadSentencias; i++) {
		char * instruccion = string_substring(programa, meta->instrucciones_serializado[i].start, meta->instrucciones_serializado[i].offset);
		instruccion[meta->instrucciones_serializado[i].offset] = '\0';

		IndiceCodigo* indiceNuevo = crear_IndiceCodigo(i, indiceInicial, strlen(instruccion), paginaSentencia);
		indiceInicial = indiceInicial + strlen(instruccion);
		list_add(pcb->codigo, indiceNuevo);

		almacenar_Bytes_de_Pagina(string_itoa(pcb->PID), string_itoa(indiceNuevo->pagina), string_itoa(indiceNuevo->byte_inicial_codigo), string_itoa(indiceNuevo->byte_final_codigo - indiceNuevo->byte_inicial_codigo), instruccion);
		free(instruccion);
	}


	int debugeame = 0;
}

IndiceCodigo* crear_IndiceCodigo(int programCounter, int byteInicial, int tamanio, int pagina) {
	IndiceCodigo* indice1 = malloc(sizeof(IndiceCodigo));
	indice1->byte_inicial_codigo = byteInicial;
	indice1->byte_final_codigo = byteInicial + tamanio;
	indice1->pagina = pagina;
	indice1->program_counter = programCounter;
	return indice1;
}

