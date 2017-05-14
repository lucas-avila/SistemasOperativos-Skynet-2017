#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <commons/collections/list.h>
#include <commons/string.h>

#include "header/PCB.h"
#include "general/Socket.h"
#define MIN_PIDS 1000

LISTA_SERIALIZADA * serializar_con_header(t_list * lista, char * tipo_lista);

int pids_reg = MIN_PIDS;


PCB* crear_pcb() {
	PCB * pcb = malloc(sizeof(PCB));

	strcpy(pcb->PID, string_itoa(pids_reg));
	pids_reg++;
	pcb->cantidad_paginas_codigo = 27;

	int status = enviar_pcb(pcb, 0);
	return pcb;
}

int enviar_pcb(PCB * pcb, int s_destino) {
	//Se envia el PCB descomponiendo el struct en string y enviandolo por paquetes
	enviar_dato_serializado("RECIBIR_PCB", s_destino);
	char * respuesta = recibir_dato_serializado(s_destino);

	if (strcmp(respuesta, "ENVIAR_PCB") != 0)
		return -1;

	int offset = 0;

	LISTA_SERIALIZADA * buffer_lista_codigo = serializar_con_header(pcb->codigo, "LISTA_CODIGO");
	LISTA_SERIALIZADA * buffer_lista_pila = serializar_con_header(pcb->pila, "LISTA_PILA");
	char * paquete = malloc(sizeof(uint32_t) * 4 + sizeof(int32_t) * 5 + buffer_lista_codigo->size + buffer_lista_pila->size + sizeof(IndiceEtiqueta));
	uint32_t pid = atoi(pcb->PID);
	memcpy(paquete, &pid, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(paquete, &pcb->program_counter, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(paquete, &pcb->cantidad_paginas_codigo, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(paquete, buffer_lista_codigo->buffer, buffer_lista_codigo->size);
	offset += buffer_lista_codigo->size;
	memcpy(paquete, &pcb->cantidad_codigo, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(paquete, buffer_lista_pila->buffer, buffer_lista_pila->size);
	offset += buffer_lista_pila->size;
	memcpy(paquete, pcb->etiqueta, sizeof(IndiceEtiqueta));
	offset += sizeof(IndiceEtiqueta);
	memcpy(paquete, &pcb->cantidad_etiqueta, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(paquete, &pcb->exit_code, sizeof(int32_t));
	offset += sizeof(int32_t);
	memcpy(paquete, &pcb->pagina_inicial_stack, sizeof(int32_t));
	offset += sizeof(int32_t);
	memcpy(paquete, &pcb->RR, sizeof(int32_t));
	offset += sizeof(int32_t);
	memcpy(paquete, &pcb->cantidad_rafagas, sizeof(int32_t));
	offset += sizeof(int32_t);
	memcpy(paquete, &pcb->cantidad_rafagas_ejecutadas, sizeof(int32_t));
	offset += sizeof(int32_t);

	//enviar_pcb_serializado(buffer, size);
}

PCB * recibir_pcb(int s_origen) {
	PCB * pcb;
	enviar_dato_serializado("ENVIAR_PCB", s_origen);

	//LISTA_SERIALIZADA * buffer = recibir_pcb_serializado(s_origen);
	LISTA_SERIALIZADA * buffer;
	char * paquete = buffer->buffer;


	return pcb;
}

LISTA_SERIALIZADA * serializar_con_header(t_list * lista, char * tipo_lista){
	/* FUNCION SERIALIZADORA PARA ESTRUCTURAS CON TAMAÑO VARIABLE
	 * POR EJEMPLO : LISTAS
	 * LA SERIALZIACION SE LLEVA A CABO DE LA SIGUIENTE MANERA
	 * SE AGREGA UN 'HEADER' ANTES DE 'STRINGIFICAR' CADA LISTA
	 * ESE HEADER CONTIENE LA CANTIDAD DE ELEMENTOS QUE TIENE ESA LISTA
	 * LISTA SIN SERIALIZAR = [ELEMENTO1, ELEMENTO2, ELEMENTO3]
	 * LISTA SERIALIZADA = 3|ELEMENTO1ELEMENTO2ELEMENTO3
	 * 						^ asi le digo al receptor que tiene para moverse por 3 elementos
	 * SI LOS ELEMENTOS TIENEN LISTAS ADENTRO, TAMBIEN SE SERIALIZAN DE ESTA MANERA
	 */
	char * buffer;
	int size = 0;
	int i = 0;
	uint32_t count_lista = list_size(lista);
	if(strcmp(tipo_lista, "LISTA_CODIGO") == 0){
		int offset = sizeof(uint32_t);
		size = sizeof(char) * sizeof(IndiceCodigo) * count_lista + offset;
		buffer = malloc(size);
		//inicializo el resultado con su header
		memcpy(buffer, &count_lista, sizeof(uint32_t));
		IndiceCodigo * elemento = malloc(sizeof(IndiceCodigo));
		for(i; i < count_lista; i++){
			elemento = list_get(lista, i);
			memcpy(buffer + offset, &elemento->program_counter, sizeof(elemento->program_counter));
			offset += sizeof(elemento->program_counter);
			memcpy(buffer + offset, &elemento->byte_inicial_codigo, sizeof(elemento->byte_inicial_codigo));
			offset += sizeof(elemento->byte_inicial_codigo);
			memcpy(buffer + offset, &elemento->byte_final_codigo, sizeof(elemento->byte_final_codigo));
			offset += sizeof(elemento->byte_final_codigo);
			memcpy(buffer + offset, &elemento->pagina, sizeof(elemento->pagina));
			offset += sizeof(elemento->pagina);
		}
	}else if(strcmp(tipo_lista, "LISTA_STACK") == 0){
		 /* LO QUE LLEGA
			uint32_t posicion;
			t_list* argumentos;
			t_list* variables;
			uint32_t retPos;
			ReturnVariable* retVar;
		*/
		/*	A LO QUE TRANSFORMO
		 * uint32_t posicion;
			char * argumentos; con header
			char * variables; con header
			uint32_t retPos;
			ReturnVariable retVar; (no puntero)
		 */

		IndiceStack * elemento = malloc(sizeof(IndiceStack));
		int offset = sizeof(uint32_t);
		int byte_size = 0;
		while(i < count_lista){
			elemento = list_get(lista, i);
			int size_arg = list_size(elemento->argumentos);
			int size_var = list_size(elemento->variables);
			//el buffer va a tener estructuras de tipo int + 3|Arg1Arg2Arg3 + 4|Var1Var2Var3Var4 + int + ReturnVariable
			byte_size += sizeof(uint32_t) * 2 + sizeof(ReturnVariable) + size_arg * sizeof(Argumento) + sizeof(uint32_t) + size_var * sizeof(Variable) + sizeof(uint32_t);
			i++;
		}
		i=0;

		size = byte_size + offset;
		buffer = malloc(size);

		//inicializo el resultado con su header
		memcpy(buffer, &count_lista, sizeof(uint32_t));

		//comienzo serializacion elemento por elemento
		for(i; i < count_lista; i++){
			elemento = list_get(lista, i);

			//cargo posicion
			memcpy(buffer + offset, &elemento->posicion, sizeof(elemento->posicion));
			offset += sizeof(elemento->posicion);

			//cargo lista de argumentos
			uint32_t size_arg = list_size(elemento->argumentos);
			memcpy(buffer + offset, &size_arg, sizeof(uint32_t));
			offset += sizeof(uint32_t);
			int x=0;
			while(x < size_arg){
				memcpy(buffer + offset, list_get(elemento->argumentos, x), sizeof(Argumento));
				offset += sizeof(Argumento);
				x++;
			}

			//cargo lista de variables
			int size_var = list_size(elemento->variables);
			memcpy(buffer + offset, &size_var, sizeof(uint32_t));
			offset += sizeof(uint32_t);
			x=0;
			while(x < size_var){
				memcpy(buffer + offset, list_get(elemento->variables, x), sizeof(Variable));
				offset += sizeof(Variable);
				x++;
			}

			//cargo retPos
			memcpy(buffer + offset, &elemento->retPos, sizeof(elemento->retPos));
			offset += sizeof(elemento->retPos);

			//cargo retVar
			memcpy(buffer + offset, elemento->retVar, sizeof(ReturnVariable));
			offset += sizeof(ReturnVariable);
		}

	}
	LISTA_SERIALIZADA * resultado = malloc(sizeof(LISTA_SERIALIZADA));
	resultado->buffer = buffer;
	resultado->size = size;
	return resultado;
}

t_list * deserializar_con_header(char * cadena, char * tipo_lista){
	/* FUNCION DESERIALIZADORA PARA ESTRUCTURAS CON TAMAÑO VARIABLE
	 * LA CANTIDAD DE ELEMENTOS DE LA LISTA RECIBIDA VIENE EN LOS PRIMEROS
	 * CARACTERES HASTA LA APARICION DEL DELIMITADOR DEL HEADER '|'
	 * 		24|ELEMENTO1ELEMENTO2ELEMENTO3...ELEMENTO24
	 * */

	t_list * resultado = list_create();
	// Obtenemos la cantidad de elementos de la lista recibida
	int count_lista = 0;
	memcpy(&count_lista, cadena, sizeof(uint32_t));
	if(strcmp(tipo_lista, "LISTA_CODIGO") == 0){
		int i = sizeof(uint32_t);

		for(i; i < count_lista * sizeof(IndiceCodigo); i+=sizeof(IndiceCodigo)){
			IndiceCodigo * elemento = malloc(sizeof(IndiceCodigo));
			memcpy(elemento, &cadena[i], sizeof(IndiceCodigo));
			list_add(resultado, elemento);
		}
	}else if(strcmp(tipo_lista, "LISTA_STACK") == 0){
		/*	LO QUE LLEGA
		 * uint32_t posicion;
			char * argumentos; con header
			char * variables; con header
			uint32_t retPos;
			ReturnVariable retVar; (no puntero)
		 */
		/*
		 * A LO QUE TRANSFORMO
			uint32_t posicion;
			t_list* argumentos;
			t_list* variables;
			uint32_t retPos;
			ReturnVariable* retVar;
		*/
		int i = sizeof(uint32_t);
		int length_argumentos = 0;
		int length_variables = 0;

		int x = 0;
		int j = 0;
		while(x < count_lista){
			IndiceStack * elemento = malloc(sizeof(IndiceStack));
			t_list * argumentos = list_create();
			t_list * variables = list_create();

			memcpy(&elemento->posicion, &cadena[i], sizeof(uint32_t));
			i += sizeof(uint32_t);

			memcpy(&length_argumentos, &cadena[i], sizeof(uint32_t));
			i+= sizeof(uint32_t);
			for(i, j=0; j < length_argumentos * sizeof(Argumento);j+=sizeof(Argumento), i+=sizeof(Argumento)){
				Argumento * arg = malloc(sizeof(Argumento));
				memcpy(arg, &cadena[i], sizeof(Argumento));
				list_add(argumentos, arg);
			}
			elemento->argumentos = argumentos;

			memcpy(&length_variables, &cadena[i], sizeof(uint32_t));
			i+= sizeof(uint32_t);
			for(i, j=0; j < length_variables * sizeof(Variable); j+=sizeof(Variable), i+=sizeof(Variable)){
				Variable * var = malloc(sizeof(Variable));
				memcpy(var, &cadena[i], sizeof(Variable));
				list_add(variables, var);
			}
			elemento->variables = variables;

			memcpy(&elemento->retPos, &cadena[i], sizeof(uint32_t));
			i += sizeof(uint32_t);
			ReturnVariable * retVar = malloc(sizeof(ReturnVariable));
			memcpy(retVar, &cadena[i], sizeof(ReturnVariable));
			i += sizeof(ReturnVariable);
			elemento->retVar = retVar;

			list_add(resultado, elemento);

			x++;
		}


	}
	return resultado;
}





