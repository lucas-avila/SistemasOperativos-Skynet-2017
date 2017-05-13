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

t_list * deserializar_con_header(char * cadena, char * tipo_lista){
	/* FUNCION DESERIALIZADORA PARA ESTRUCTURAS CON TAMAÑO VARIABLE
	 * LA CANTIDAD DE ELEMENTOS DE LA LISTA RECIBIDA VIENE EN LOS PRIMEROS
	 * CARACTERES HASTA LA APARICION DEL DELIMITADOR DEL HEADER '|'
	 * 		24|ELEMENTO1ELEMENTO2ELEMENTO3...ELEMENTO24
	 * */

	t_list * resultado = list_create();
	// Obtenemos la cantidad de elementos de la lista recibida
	int count_lista = atoi(substr_hasta(cadena, '|'));
	if(strcmp(tipo_lista, "LISTA_CODIGO") == 0){
		//length del header para saltearlo
		int i = strlen(string_itoa(count_lista)) + 1;

		for(i; i < count_lista * sizeof(IndiceCodigo); i+=sizeof(IndiceCodigo)){
			IndiceCodigo * elemento = malloc(sizeof(IndiceCodigo));
			memcpy(elemento, &cadena[i], sizeof(IndiceCodigo));
			list_add(resultado, elemento);
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


