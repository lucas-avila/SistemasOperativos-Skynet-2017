/*
 * PCB.h
 *
 *  Created on: 30/4/2017
 *      Author: utnso
 */

#ifndef HEADER_PCB_H_
#define HEADER_PCB_H_

#include "commons/collections/list.h"
/*
 * PCB.h
 *
 *  Created on: 16/4/2017
 *      Author: utnso
 */

typedef struct {
	unsigned int program_counter;
	unsigned int byte_inicial_codigo;
	unsigned int byte_final_codigo;
	unsigned int pagina;
//Offset byte inicial
//longitud diferencia byte_final_codigo - byte_inicial_codigo
} IndiceCodigo;

typedef struct {
	char id;
	unsigned int pagina;
	unsigned int byte_inicial;
	unsigned int tamanio;
} Argumento;

typedef struct {
	char id;
	unsigned int pagina;
	unsigned int byte_inicial;
	unsigned int tamanio;
} Variable;

typedef struct {
	unsigned int pagina;
	unsigned int byte_inicial;
	unsigned int tamanio;
} ReturnVariable;

typedef struct {
	unsigned int posicion;
	t_list* argumentos;
	t_list* variables;
	unsigned int retPos;
	ReturnVariable* retVar;
} IndiceStack;

typedef struct {
	char* identificador_funcion; //funcion donde se encuentra la etiqueta
	char* nombre_etiqueta; //nombre de la etiqueta
	unsigned int valor_program_counter; //valor que debe tomar el program_counter al pasar por ahi
} IndiceEtiqueta;

typedef struct {
	char PID[5];
	unsigned int program_counter;
	unsigned int cantidad_paginas_codigo;
	IndiceCodigo* codigo;
	unsigned int cantidad_codigo;

	t_list* pila;

	IndiceEtiqueta* etiqueta;
	unsigned int cantidad_etiqueta;
	int exit_code;  //Modificado porque puede ser valor negativo

	int pagina_inicial_stack;
} PCB;

#endif /* HEADER_PCB_H_ */
