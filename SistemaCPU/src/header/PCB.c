#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <commons/collections/list.h>
#include <commons/string.h>

#include "PCB.h"
#include "../general/Socket.h"
#define MIN_PIDS 1000

LISTA_SERIALIZADA * serializar_con_header(t_list * lista, char * tipo_lista);
LISTA_DESERIALIZADA * deserializar_con_header(char * cadena, char * tipo_lista);

//modificaciones a las funciones de enviar y recibir por sockets
void enviar_estructura_serializada(char* mensaje, int size, int conexion) {
	char * tamanio_dato = malloc(sizeof(uint32_t));
	memcpy(tamanio_dato, &size, sizeof(uint32_t));
	send(conexion, tamanio_dato, sizeof(uint32_t), 0);
	send(conexion, mensaje, size, 0);
}

LISTA_SERIALIZADA * recibir_estructura_serializada(int socket_conexion) {
	char * tamanio_dato = malloc(sizeof(uint32_t));
	int bytes_recibidos = recv(socket_conexion, tamanio_dato, sizeof(uint32_t), 0);

	int size = 0;
	memcpy(&size, tamanio_dato, sizeof(uint32_t));

	char * buffer = malloc(size);
	recv(socket_conexion, buffer, size, 0);

	LISTA_SERIALIZADA * l = malloc(sizeof(LISTA_SERIALIZADA));
	l->buffer = buffer;
	l->size = size;
	return l;
}

int enviar_pcb(PCB * pcb, int s_destino) {
	//Se envia el PCB descomponiendo el struct en string y enviandolo por paquetes
	enviar_dato_serializado("RECIBIR_PCB", s_destino);
	/*char * respuesta = recibir_dato_serializado(s_destino);

	if (strcmp(respuesta, "ENVIAR_PCB") != 0)
		return -1;*/

	int offset = 0;

	LISTA_SERIALIZADA * buffer_lista_codigo = serializar_con_header(pcb->codigo, "LISTA_CODIGO");
	LISTA_SERIALIZADA * buffer_lista_pila = serializar_con_header(pcb->pila, "LISTA_PILA");
	char * buffer_indice_etiqueta = string_new();
	int size_indice_etiqueta = serializar_indice_etiqueta(pcb->etiqueta, &buffer_indice_etiqueta);

	int size = sizeof(uint32_t) * 4 + sizeof(int32_t) * 5 + buffer_lista_codigo->size + buffer_lista_pila->size + size_indice_etiqueta;
	char * paquete = malloc(size);

	memcpy(paquete + offset, &pcb->PID, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(paquete + offset, &pcb->program_counter, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(paquete + offset, &pcb->cantidad_paginas_codigo, sizeof(uint32_t));
	offset += sizeof(uint32_t);

	memcpy(paquete + offset, buffer_lista_codigo->buffer, buffer_lista_codigo->size);
	offset += buffer_lista_codigo->size;

	memcpy(paquete + offset, &pcb->cantidad_codigo, sizeof(uint32_t));
	offset += sizeof(uint32_t);

	memcpy(paquete + offset, buffer_lista_pila->buffer, buffer_lista_pila->size);
	offset += buffer_lista_pila->size;

	memcpy(paquete + offset, buffer_indice_etiqueta, size_indice_etiqueta);
	offset += size_indice_etiqueta;

	memcpy(paquete + offset, &pcb->cantidad_etiqueta, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(paquete + offset, &pcb->exit_code, sizeof(int32_t));
	offset += sizeof(int32_t);
	memcpy(paquete + offset, &pcb->pagina_inicial_stack, sizeof(int32_t));
	offset += sizeof(int32_t);
	memcpy(paquete + offset, &pcb->RR, sizeof(int32_t));
	offset += sizeof(int32_t);
	memcpy(paquete + offset, &pcb->cantidad_rafagas, sizeof(int32_t));
	offset += sizeof(int32_t);
	memcpy(paquete + offset, &pcb->cantidad_rafagas_ejecutadas, sizeof(int32_t));
	offset += sizeof(int32_t);


	enviar_estructura_serializada(paquete, size, s_destino);
}

PCB * recibir_pcb(int s_origen) {
	PCB * pcb = malloc(sizeof(PCB));

	//enviar_dato_serializado("ENVIAR_PCB", s_origen);
	LISTA_SERIALIZADA * buffer = recibir_estructura_serializada(s_origen);

	char * paquete = buffer->buffer;

	memcpy(&pcb->PID, paquete, sizeof(uint32_t));
	int offset = sizeof(uint32_t);
	memcpy(&pcb->program_counter, paquete + offset, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(&pcb->cantidad_paginas_codigo, paquete + offset, sizeof(uint32_t));
	offset += sizeof(uint32_t);

	LISTA_DESERIALIZADA * lista_codigo = deserializar_con_header(paquete + offset, "LISTA_CODIGO");
	pcb->codigo = lista_codigo->lista;
	offset += lista_codigo->size;

	memcpy(&pcb->cantidad_codigo, paquete + offset, sizeof(uint32_t));
	offset += sizeof(uint32_t);

	LISTA_DESERIALIZADA * lista_pila = deserializar_con_header(paquete + offset, "LISTA_STACK");
	pcb->pila = lista_pila->lista;
	offset += lista_pila->size;

	pcb->etiqueta = malloc(sizeof(IndiceEtiqueta));
	uint32_t size_identificador_funcion = 0;
	uint32_t size_nombre_etiqueta = 0;
	memcpy(&size_identificador_funcion, paquete + offset, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	pcb->etiqueta->identificador_funcion = malloc(sizeof(char) * size_identificador_funcion);
	memcpy(pcb->etiqueta->identificador_funcion, paquete + offset, size_identificador_funcion);
	pcb->etiqueta->identificador_funcion[size_identificador_funcion] = '\0';
	offset += size_identificador_funcion;
	memcpy(&size_nombre_etiqueta, paquete + offset, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	pcb->etiqueta->nombre_etiqueta = malloc(sizeof(char) * size_nombre_etiqueta);
	memcpy(pcb->etiqueta->nombre_etiqueta, paquete + offset, size_nombre_etiqueta);
	pcb->etiqueta->nombre_etiqueta[size_nombre_etiqueta] = '\0';
	offset += size_nombre_etiqueta;
	memcpy(&pcb->etiqueta->valor_program_counter, paquete + offset, sizeof(uint32_t));
	offset += sizeof(uint32_t);

	memcpy(&pcb->cantidad_etiqueta, paquete + offset, sizeof(uint32_t));
	offset += sizeof(uint32_t);

	memcpy(&pcb->exit_code, paquete + offset, sizeof(int32_t));
	offset += sizeof(int32_t);
	memcpy(&pcb->pagina_inicial_stack, paquete + offset, sizeof(int32_t));
	offset += sizeof(int32_t);
	memcpy(&pcb->RR, paquete + offset, sizeof(int32_t));
	offset += sizeof(int32_t);
	memcpy(&pcb->cantidad_rafagas, paquete + offset, sizeof(int32_t));
	offset += sizeof(int32_t);
	memcpy(&pcb->cantidad_rafagas_ejecutadas, paquete + offset, sizeof(int32_t));
	offset += sizeof(int32_t);

	return pcb;
}

int serializar_indice_etiqueta(IndiceEtiqueta * in, char ** buffer){
	uint32_t size_identificador_funcion = strlen(in->identificador_funcion);
	uint32_t size_nombre_etiqueta = strlen(in->nombre_etiqueta);
	int size = sizeof(uint32_t) * 3 + size_identificador_funcion + size_nombre_etiqueta;
	*buffer = malloc(size);

	int offset = 0;
	memcpy(*buffer, &size_identificador_funcion, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(*buffer + offset, in->identificador_funcion, size_identificador_funcion);
	offset += size_identificador_funcion;
	memcpy(*buffer + offset, &size_nombre_etiqueta, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(*buffer + offset, in->nombre_etiqueta, size_nombre_etiqueta);
	offset += size_nombre_etiqueta;
	memcpy(*buffer + offset, &in->valor_program_counter, sizeof(uint32_t));
	return size;
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
	}else if(strcmp(tipo_lista, "LISTA_PILA") == 0){
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

LISTA_DESERIALIZADA * deserializar_con_header(char * cadena, char * tipo_lista){
	/* FUNCION DESERIALIZADORA PARA ESTRUCTURAS CON TAMAÑO VARIABLE
	 * LA CANTIDAD DE ELEMENTOS DE LA LISTA RECIBIDA VIENE EN LOS PRIMEROS
	 * CARACTERES HASTA LA APARICION DEL DELIMITADOR DEL HEADER '|'
	 * 		24|ELEMENTO1ELEMENTO2ELEMENTO3...ELEMENTO24
	 * */

	t_list * resultado = list_create();
	int size = 0;
	// Obtenemos la cantidad de elementos de la lista recibida
	int count_lista = 0;
	memcpy(&count_lista, cadena, sizeof(uint32_t));
	if(strcmp(tipo_lista, "LISTA_CODIGO") == 0){
		int i = sizeof(uint32_t);

		size = i + sizeof(IndiceCodigo) * count_lista;

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
		size = i;

	}

	LISTA_DESERIALIZADA * l = malloc(sizeof(LISTA_DESERIALIZADA));
	l->lista = resultado;
	l->size = size;
	return l;
}





