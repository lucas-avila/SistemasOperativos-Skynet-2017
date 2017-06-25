#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#include "Stack.h"
#include "EstructurasDeDatosPrimitivas.h"

#include "commons/string.h"
const int TAMANIO_VAR = 4;
int TAMANIO_MAXIMO = 2;

t_puntero serializarPuntero(int pagina, int byte_inicial, int tamanioPagina) {
	return pagina * tamanioPagina + byte_inicial;
}

void deserializar_puntero(t_puntero direccion_variable, int* pagina, int* offset, int tamanioPagina) {
	if (direccion_variable <= tamanioPagina) {
		*pagina = 0;
		*offset=direccion_variable;
	} else {

		int tam = (int) (direccion_variable / tamanioPagina);
		float result = ((float) direccion_variable / (float) tamanioPagina);
		*pagina = 0;
		if (tam != result) {
			*pagina = tam + 1;
		} else {
			*pagina = tam;
		}

		*offset = *pagina * tamanioPagina  - direccion_variable;
		if (*offset < 0) {
			*offset = (int) *offset * (-1);
		}
	}
}


t_puntero serializarMemoriaDinamica(char* pagina, char* byteInicial) {

	char puntero[2 + 2 + 3 + 1];
	//PROCESO + PAGINA + BYTEINICIAL
	strcpy(puntero, "");

	char* completaVacio;
	char contenido_Formateado[TAMANIO_MAXIMO + 1];

	strcat(puntero, "88");

	completaVacio = string_repeat('0', (int) (2 - strlen(pagina)));
	strcpy(contenido_Formateado, completaVacio);
	strcat(contenido_Formateado, pagina);

	strcat(puntero, contenido_Formateado);

	completaVacio = string_repeat('0', (int) (3 - strlen(byteInicial)));

	strcpy(contenido_Formateado, completaVacio);
	strcat(contenido_Formateado, byteInicial);

	strcat(puntero, contenido_Formateado);

	t_puntero memoria_serializada = atoi(puntero);

	return memoria_serializada;
}

DireccionMemoriaDinamica* deserializarMemoriaDinamica(char* proceso, t_puntero memoria_serializada) {
	char* datos_memoria = string_itoa(memoria_serializada);

	DireccionMemoriaDinamica* punteroMemoria = malloc(sizeof(DireccionMemoriaDinamica));
	//char* proceso = string_substring(datos_memoria, 0, 4);
	char* pag = string_substring(datos_memoria, 2, 2);
	char* byte = string_substring(datos_memoria, 4, 3);

	punteroMemoria->pid = atoi(proceso);
	punteroMemoria->pagina = atoi(pag);
	punteroMemoria->byteInicial = atoi(byte);
	//No estoy seguro si dejando el tamanio en blanco trae problemas
	return punteroMemoria;
}
