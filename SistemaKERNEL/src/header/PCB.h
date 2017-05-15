#ifndef HEADER_PCB_H_
#define HEADER_PCB_H_

#include <commons/collections/list.h>
#include <stdint.h>
#include <sys/types.h>

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
	char* identificador_funcion; //funcion donde se encuentra la etiqueta
	char* nombre_etiqueta; //nombre de la etiqueta
	uint32_t valor_program_counter; //valor que debe tomar el program_counter al pasar por ahi
}__attribute__((packed))
IndiceEtiqueta;

typedef struct {
	uint32_t PID;
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
	int32_t quantum_sleep;
	int32_t cantidad_rafagas_ejecutadas;

	uint32_t consola; // Para saber a qué consola pertenece el proceso y facilitar su finalización.
}__attribute__((packed))
PCB;

/**typedef struct{
 int pid;
 unsigned int cantidad_paginas;
 t_metadata_program * info_codigo;
 } PCB; **/

int enviar_pcb(PCB * pcb, int s_destino);
PCB * recibir_pcb(int s_origen);
PCB * crear_pcb();

/* estructuras auxiliares */

typedef struct {
	char * buffer;
	int size;
} LISTA_SERIALIZADA;

typedef struct {
	t_list * lista;
	int size;
} LISTA_DESERIALIZADA;

#endif /* HEADER_PCB_H_ */
