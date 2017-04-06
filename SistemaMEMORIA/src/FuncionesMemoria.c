/*
 * FuncionesMemoria.c
 *
 *  Created on: 6/4/2017
 *      Author: utnso
 */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "header/FuncionesMemoria.h"
#include "header/AppConfig.h"
#include<commons/collections/list.h>
#include<commons/string.h>
int getFrame(char*PID, int pagina) {
	int i = 0;
	for (i = 0; i < list_size(tablaPaginasInvertidas); i++) {
		Tabla_Pagina_Invertida *registro = list_get(tablaPaginasInvertidas, i);
		if( (strcmp(registro->PID, PID) == 0) && (strcmp(registro->pagina, pagina) == 0) )  {
			return atoi(registro->frame) ;
		}
	}
	return "0";
}

char* obtenerNumeroPaginaNew(char* PID) {
	int i = 0;
	for (i = 0; i < list_size(tablaPaginasInvertidas); i++) {
		Tabla_Pagina_Invertida *registro = list_get(tablaPaginasInvertidas, i);
		if (strcmp(registro->PID, PID) == 0) {
			return registro->pagina + 1;
		}
	}
	return "0";
}

char* inicializar_programa(char* PID, int cantidad_paginas_requeridas) {
	int i = 0;
	for (i = 0; i < list_size(tablaPaginasInvertidas); i++) {
		Tabla_Pagina_Invertida *registro = list_get(tablaPaginasInvertidas, i);
		if (strcmp(registro->PID, VACIO) == 0) {
			strcpy(registro->PID, PID);
			list_replace(tablaPaginasInvertidas, i, registro);
			actualizar_informacion_memoria_principal(registro, i);
		}
	}
	return "OK";
}

char* solicitar_bytes_de_una_pagina(char* PID, int pagina, int byteInicial, int longitud) {
	return string_substring(MEMORIA_PRINCIPAL, indiceInicialPaginas + configuraciones.MARCO_SIZE * getFrame(PID,pagina) + byteInicial, longitud);

}

void almacenar_bytes_de_una_pagina(char* PID, int pagina, int byteInicial, int longitud, char*contenido) {

	char* primeraParte = string_substring(MEMORIA_PRINCIPAL, 0, indiceInicialPaginas + getFrame(PID,pagina) * configuraciones.MARCO_SIZE + byteInicial);
	printf("\n%d", strlen(primeraParte));

	char* textoMedio = contenido;
	char* segundaParte = string_substring_from(MEMORIA_PRINCIPAL, getFrame(PID,pagina) * configuraciones.MARCO_SIZE + byteInicial + longitud);
	printf("\n%d", strlen(segundaParte));
	string_capitalized(MEMORIA_PRINCIPAL);

	MEMORIA_PRINCIPAL = string_new();

	string_append(&MEMORIA_PRINCIPAL, primeraParte);
	string_append(&MEMORIA_PRINCIPAL, textoMedio);
	string_append(&MEMORIA_PRINCIPAL, segundaParte);

	string_capitalized(primeraParte);
	string_capitalized(segundaParte);
	printf("\n%s", MEMORIA_PRINCIPAL);

}

char* asignar_paginas_a_proceso(char *PID, int cantidad_paginas_requeridas) {
	int cantidad_paginas_pedidas = 0;

	while (cantidad_paginas_pedidas < cantidad_paginas_requeridas) {
		Tabla_Pagina_Invertida *registro = buscar_pagina_disponible();
		if (registro == NULL) {
			return "FALTA ESPACIO";
		}
		strcpy(registro->PID, PID);
		strcpy(registro->pagina, obtenerNumeroPaginaNew(PID));


		actualizar_tabla_pagina(registro);
		cantidad_paginas_pedidas++;
	}
	return "OK";

}

void actualizar_tabla_pagina( Tabla_Pagina_Invertida *registro ){

}

Tabla_Pagina_Invertida* buscar_pagina_disponible() {
	int i = 0;
	for (i = 0; i < list_size(tablaPaginasInvertidas); i++) {
		Tabla_Pagina_Invertida *registro = list_get(tablaPaginasInvertidas, i);
		if (strcmp(registro->PID, VACIO) == 0) {
			return registro;
		}
	}
	return NULL;
}

void finalizar_programa(char *PID) {
	int i = 0;
	for (i = 0; i < list_size(tablaPaginasInvertidas); i++) {
		Tabla_Pagina_Invertida *registro = list_get(tablaPaginasInvertidas, i);
		if (strcmp(registro->PID, PID) == 0) {
			strcpy(registro->PID, VACIO);
			strcpy(registro->pagina, VACIO);
			list_replace(tablaPaginasInvertidas, i, registro);
			actualizar_informacion_memoria_principal(registro, i);
		}
	}
}

void actualizar_informacion_memoria_principal(Tabla_Pagina_Invertida registro, int indiceRegistro) {
	int bloqueAdmin = TAMANIO * 3; //tamaño que ocupara en bytes de memoria 3 atributos por el tamaño que ocupa cada uno

	char* primeraParte = string_substring(MEMORIA_PRINCIPAL, 0, indiceRegistro * bloqueAdmin);
	printf("\n%d", strlen(primeraParte));

	char* textoMedio = string_new();
	string_append(textoMedio, registro.frame);
	string_append(textoMedio, registro.pagina);
	string_append(textoMedio, registro.PID);

	char* segundaParte = string_substring_from(MEMORIA_PRINCIPAL, indiceRegistro * bloqueAdmin + bloqueAdmin);
	printf("\n%d", strlen(segundaParte));
	string_capitalized(MEMORIA_PRINCIPAL);

	MEMORIA_PRINCIPAL = string_new();

	string_append(&MEMORIA_PRINCIPAL, primeraParte);
	string_append(&MEMORIA_PRINCIPAL, textoMedio);
	string_append(&MEMORIA_PRINCIPAL, segundaParte);

	string_capitalized(primeraParte);
	string_capitalized(segundaParte);
	printf("\n%s", MEMORIA_PRINCIPAL);
}

