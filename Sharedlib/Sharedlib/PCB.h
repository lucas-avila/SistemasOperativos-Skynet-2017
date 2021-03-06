#ifndef HEADER_PCB_H_
#define HEADER_PCB_H_

#include <commons/collections/list.h>
#include <stdint.h>
#include <sys/types.h>
#include "Socket.h"

#define valorEXIT -1

typedef struct {
	uint32_t program_counter;
	uint32_t byte_inicial_codigo;
	uint32_t byte_final_codigo;
	uint32_t pagina;
//Offset byte inicial
//longitud diferencia byte_final_codigo - byte_inicial_codigo
}__attribute__((packed))
IndiceCodigo;

typedef struct {
	uint32_t id;
	uint32_t pagina;
	uint32_t byte_inicial;
	uint32_t tamanio;
}__attribute__((packed))
Argumento;

typedef struct {
	uint32_t id;
	uint32_t pagina;
	uint32_t byte_inicial;
	uint32_t tamanio;
	uint32_t dinamica;

}__attribute__((packed))
Variable;

typedef struct {
	uint32_t pagina;
	uint32_t byte_inicial;
	uint32_t tamanio;
}__attribute__((packed))
ReturnVariable;

typedef struct {
	uint32_t posicion;
	t_list* argumentos;
	t_list* variables;
	uint32_t retPos;
	ReturnVariable* retVar;
}__attribute__((packed))
IndiceStack;

typedef struct {
	uint32_t PID;
	uint32_t program_counter;
	uint32_t cantidad_paginas_codigo;
	t_list* codigo; //tiene elementos de tipo IndiceCodigo
	uint32_t cantidad_codigo;

	t_list* pila; //tiene elementos de tipo IndiceStack

	uint32_t etiquetas_size; //tamaño total en bytes de la lista
	char * etiquetas; //lista de etiquetas serializadas por el parser

	int32_t exit_code;  //Modificado porque puede ser valor negativo

	int32_t pagina_inicial_stack;
	int32_t posicion_pagina_stack;

	//Agregados PARA EJECUCION
	int32_t RR; //0 - FIFO , 1  -RR
	int32_t cantidad_rafagas; //RR se le da pelota, si no , nada
	int32_t quantum_sleep;
	int32_t cantidad_rafagas_ejecutadas;

}__attribute__((packed))
PCB;

/* estructuras auxiliares */

typedef struct {
	t_list * lista;
	int size;
} LISTA_DESERIALIZADA;

DATO_SERIALIZADO * serializar_con_header(t_list * lista, char * tipo_lista);
LISTA_DESERIALIZADA * deserializar_con_header(char * cadena, char * tipo_lista);


#endif /* HEADER_PCB_H_ */
