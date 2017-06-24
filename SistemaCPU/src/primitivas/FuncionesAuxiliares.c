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
		pagina = 0;
		if (tam != result) {
			*pagina = tam + 1;
		} else {
			*pagina = tam;
		}

		*offset = pagina - direccion_variable;
		if (offset < 0) {
			*offset = (int) offset * (-1);
		}
	}
}

t_puntero generarPunteroStack(char* filaStack, char*tipoVariable, char* filaTabla) {

	char puntero[2 + (TAMANIO_MAXIMO * 3) + 1];
	//para que no corra los ceros
	strcpy(puntero, "99");

	char* completaVacio;

	char contenido_Formateado[TAMANIO_MAXIMO + 1];

	completaVacio = string_repeat('0', (int) (TAMANIO_MAXIMO - strlen(filaStack)));
	strcpy(contenido_Formateado, completaVacio);
	strcat(contenido_Formateado, filaStack);

	strcat(puntero, contenido_Formateado);

	completaVacio = string_repeat('0', (int) (TAMANIO_MAXIMO - strlen(tipoVariable)));
	strcpy(contenido_Formateado, completaVacio);
	strcat(contenido_Formateado, tipoVariable);

	strcat(puntero, contenido_Formateado);

	completaVacio = string_repeat('0', (int) (TAMANIO_MAXIMO - strlen(filaTabla)));
	strcpy(contenido_Formateado, completaVacio);
	strcat(contenido_Formateado, filaTabla);

	strcat(puntero, contenido_Formateado);

	t_puntero numeroPuntero = atoi(puntero);
	return numeroPuntero;

}
//99000100
PunteroVariable* deserializarPunteroStack(t_puntero punteroDireccion) {
	char* datoString = string_itoa(punteroDireccion);

	PunteroVariable* punteroVariable = malloc(sizeof(PunteroVariable));
	char* parte1 = string_substring(datoString, 2, 2);
	char* parte2 = string_substring(datoString, 4, 2);

	char* parte3 = string_substring(datoString, 6, 2);

	punteroVariable->filaStack = atoi(parte1);
	punteroVariable->esVariable = atoi(parte2);
	punteroVariable->filaTabla = atoi(parte3);
	return punteroVariable;
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
