#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <commons/collections/list.h>
#include <commons/string.h>

#include "header/PCB.h"
#include "general/Socket.h"
#define MIN_PIDS 1000

char * serializar_con_header(t_list * lista, char * tipo_lista);

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

	//pid
	char * pid = pcb->PID;
	char * program_counter = string_itoa(pcb->program_counter);
	char * cantidad_paginas_codigo = string_itoa(pcb->cantidad_paginas_codigo);
	char * codigo = serializar_con_header(pcb->codigo, "LISTA_CODIGO");

	enviar_dato_serializado(cantidad_paginas_codigo, s_destino);
	printf("Enviando cantidad pagians %s\n", cantidad_paginas_codigo);

}

char * serializar_con_header(t_list * lista, char * tipo_lista){
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
	int i = 0;
	uint32_t count_lista = list_size(lista);
	if(strcmp(tipo_lista, "LISTA_CODIGO") == 0){
		char * count_lista_s = string_itoa(count_lista);
		int offset = strlen(count_lista_s) + 1;
		buffer = malloc(sizeof(char) * sizeof(IndiceCodigo) * count_lista + offset);
		//inicializo el resultado con su header
		strcpy(buffer, count_lista_s);
		string_append(&buffer, "|");
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
		char * count_lista_s = string_itoa(count_lista);
		int offset = strlen(count_lista_s) + 1;
		int byte_size = 0;
		while(i < count_lista){
			elemento = list_get(lista, i);
			int size_arg = list_size(elemento->argumentos);
			int size_var = list_size(elemento->variables);
			//el buffer va a tener estructuras de tipo int + 3|Arg1Arg2Arg3 + 4|Var1Var2Var3Var4 + int + ReturnVariable
			byte_size += sizeof(uint32_t) * 2 + sizeof(ReturnVariable) + size_arg * sizeof(Argumento) + (strlen(size_arg)+1) + size_var * sizeof(Variable) + (strlen(size_arg)+1);
			i++;
		}
		i=0;

		buffer = malloc(byte_size + offset);
		//inicializo el resultado con su header
		strcpy(buffer, count_lista_s);
		string_append(&buffer, "|");

		//comienzo serializacion elemento por elemento
		for(i; i < count_lista; i++){
			elemento = list_get(lista, i);

			//cargo posicion
			memcpy(buffer + offset, &elemento->posicion, sizeof(elemento->posicion));
			offset += sizeof(elemento->posicion);

			//cargo lista de argumentos
			int size_arg = list_size(elemento->argumentos);
			char * size_arg_s = string_itoa(size_arg);
			string_append(&size_arg_s, "|");
			char * aux = buffer + offset;
			string_append(&aux, size_arg_s);
			int x=0;
			offset += strlen(size_arg_s);
			while(x < size_arg){
				memcpy(buffer + offset, list_get(elemento->argumentos, x), sizeof(Argumento));
				offset += sizeof(Argumento);
				x++;
			}

			//cargo lista de variables (reuso size_arg)
			size_arg = list_size(elemento->variables);
			size_arg_s = string_itoa(size_arg);
			string_append(&size_arg_s, "|");
			aux = buffer + offset;
			string_append(&aux, size_arg_s);
			x=0;
			offset += strlen(size_arg_s);
			while(x < size_arg){
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
	return buffer;
}

char * substr_hasta(char * cadena, char delimitador){
	char * count_lista_s = string_new();
	int i = 0;
	while(cadena[i] != delimitador){
		count_lista_s[i] = cadena[i];
		i++;
	}
	count_lista_s[i] = '\0';
	return count_lista_s;
}

int obtener_length_lista(char * cadena){
	return atoi(substr_hasta(cadena, '|'));
}

t_list * deserializar_con_header(char * cadena, char * tipo_lista){
	/* FUNCION DESERIALIZADORA PARA ESTRUCTURAS CON TAMAÑO VARIABLE
	 * LA CANTIDAD DE ELEMENTOS DE LA LISTA RECIBIDA VIENE EN LOS PRIMEROS
	 * CARACTERES HASTA LA APARICION DEL DELIMITADOR DEL HEADER '|'
	 * 		24|ELEMENTO1ELEMENTO2ELEMENTO3...ELEMENTO24
	 * */

	t_list * resultado = list_create();
	// Obtenemos la cantidad de elementos de la lista recibida
	int count_lista = obtener_length_lista(cadena);
	if(strcmp(tipo_lista, "LISTA_CODIGO") == 0){
		//length del header para saltearlo
		int i = strlen(string_itoa(count_lista)) + 1;

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
		int i = strlen(string_itoa(count_lista)) + 1;
		int length_argumentos = 0;
		int length_variables = 0;

		int x = 0;
		while(x < count_lista){
			IndiceStack * elemento = malloc(sizeof(IndiceStack));
			memcpy(elemento->posicion, &cadena[i], sizeof(uint32_t));
			i += sizeof(uint32_t);
			length_argumentos = obtener_length_lista(&cadena[i]);
			t_list * argumentos = list_create();
			i+= strlen(string_itoa(length_argumentos)) + 1;
			for(i; i < length_argumentos * sizeof(Argumento); i+=sizeof(Argumento)){
				Argumento * arg = malloc(sizeof(Argumento));
				memcpy(arg, &cadena[i], sizeof(Argumento));
				list_add(argumentos, arg);
			}
			elemento->argumentos = argumentos;
			//fijarse aca i
			length_variables = obtener_length_lista(&cadena[i]);
			t_list * variables = list_create();
			i+= strlen(string_itoa(length_variables)) + 1;
			for(i; i < length_variables * sizeof(Variable); i+=sizeof(Variable)){
				Variable * var = malloc(sizeof(Variable));
				memcpy(var, &cadena[i], sizeof(Variable));
				list_add(variables, var);
			}
			//fijarse aca i
			memcpy(elemento->retPos, &cadena[i], sizeof(uint32_t));
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


PCB * recibir_pcb(int s_origen) {
	PCB * pcb;
	enviar_dato_serializado("ENVIAR_PCB", s_origen);

	//pid

	strcpy(pcb->PID, recibir_dato_serializado(s_origen));
	//cantidad paginas codigo
	pcb->cantidad_paginas_codigo = atoi(recibir_dato_serializado(s_origen));

	return pcb;
}


