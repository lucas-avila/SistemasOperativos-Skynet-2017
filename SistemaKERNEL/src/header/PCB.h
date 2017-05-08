#ifndef HEADER_PCB_H_
#define HEADER_PCB_H_

//#include "parser/metadata_program.h"

#include <commons/collections/list.h>
#include <stdint.h>
#include <sys/types.h>

/*
 * PCB.h
 *
 *  Created on: 16/4/2017
 *      Author: utnso
 */


/*
 *
 *
 * Un identificador único (PID), el cual deberá informar a la consola.

● Program Counter (PC)

● Referencia a la tabla de Archivos del Proceso

● Posición del Stack (SP)

● El Exit Code (EC) del proceso

Páginas de código Cantidad de páginas utilizadas por el código del Programa AnSISOP, empezando por la página cero.

Índice de código Estructura auxiliar que contiene el offset del inicio y del fin de cada sentencia del Programa.

Índice de etiquetas Estructura auxiliar utilizada para conocer las líneas de código correspondientes al inicio de los procedimientos y a las etiquetas.

Índice del Stack Estructura auxiliar encargada de ordenar los valores almacenados en el Stack.
 */
typedef struct {
	uint32_t program_counter;
	uint32_t byte_inicial_codigo;
	uint32_t byte_final_codigo;
	uint32_t pagina;
//Offset byte inicial
//longitud diferencia byte_final_codigo - byte_inicial_codigo
} __attribute__((packed))
IndiceCodigo;

typedef struct {
	char id;
	uint32_t pagina;
	uint32_t byte_inicial;
	uint32_t tamanio;
} __attribute__((packed))
Argumento;

typedef struct {
	char id;
	uint32_t pagina;
	uint32_t byte_inicial;
	uint32_t tamanio;
} __attribute__((packed))
Variable;

typedef struct {
	uint32_t pagina;
	uint32_t byte_inicial;
	uint32_t tamanio;
} __attribute__((packed))
ReturnVariable;

typedef struct {
	uint32_t posicion;
	t_list* argumentos;
	t_list* variables;
	uint32_t retPos;
	ReturnVariable* retVar;
} __attribute__((packed))
IndiceStack;

typedef struct {
	char* identificador_funcion; //funcion donde se encuentra la etiqueta
	char* nombre_etiqueta; //nombre de la etiqueta
	uint32_t valor_program_counter; //valor que debe tomar el program_counter al pasar por ahi
} __attribute__((packed))
IndiceEtiqueta;

typedef struct {
	char PID[5];
	uint32_t program_counter;
	uint32_t cantidad_paginas_codigo;
	t_list* codigo; //tiene elementos de tipo IndiceCodigo
	uint32_t cantidad_codigo;

	t_list* pila; //tiene elementos de tipo IndiceStack

	IndiceEtiqueta* etiqueta;
	uint32_t cantidad_etiqueta;
	int32_t exit_code;  //Modificado porque puede ser valor negativo

	int32_t pagina_inicial_stack;


	//Agregados PARA EJECUCION
	int32_t RR; //0 - FIFO , 1  -RR
	int32_t cantidad_rafagas; //RR se le da pelota, si no , nada

	int32_t cantidad_rafagas_ejecutadas;
} __attribute__((packed))
PCB;


int enviar_pcb(PCB * pcb, int s_destino);
PCB * recibir_pcb(int s_origen);
PCB * crear_pcb();


#endif /* HEADER_PCB_H_ */
