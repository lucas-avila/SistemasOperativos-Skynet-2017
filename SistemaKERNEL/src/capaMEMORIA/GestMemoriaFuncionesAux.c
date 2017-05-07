#include<stdbool.h>
#include<stdint.h>
#include <commons/collections/list.h>
#include<string.h>
#include<stdlib.h>
#include "GestMemoriaFuncionesAux.h"

#include "../interfaz/InterfazMemoria.h"
#include "commons/string.h"
void inicializar_tabla_proceso_memoria() {
	TABLA_PROCESS_MEMORY = list_create();
}

TABLA_MEMORIA_PROCESO* crear_item_Tabla_memoria_proceso(char* PID, unsigned nroPagina, unsigned espacioDisponible) {
	TABLA_MEMORIA_PROCESO* item = malloc(sizeof(TABLA_MEMORIA_PROCESO));
	item->PID = malloc(strlen(PID) + 1);
	strcpy(item->PID, PID);
	item->nroPagina = nroPagina;
	item->espacioDisponible = espacioDisponible;
	return item;
}

void guardar_registro_tabla_memoria(TABLA_MEMORIA_PROCESO* registro) {
	list_add(TABLA_PROCESS_MEMORY, registro);
}

void modificar_registro_tabla_memoria(TABLA_MEMORIA_PROCESO* registro) {
	int indice = buscar_indice_elemento_tabla_memoria(registro->PID, registro->nroPagina);
	if (indice == -1) {
		return;
	}
	list_replace(TABLA_PROCESS_MEMORY, indice, registro);
}

void eliminar_registro_tabla_memoria(TABLA_MEMORIA_PROCESO* registro) {
	int indice = buscar_indice_elemento_tabla_memoria(registro->PID, registro->nroPagina);
	if (indice == -1) {
		return;
	}
	list_remove(TABLA_PROCESS_MEMORY, indice);
}

void eliminar_registro_tabla_memoria_por_PID(char* PID) {
	int tamanioLista = list_size(TABLA_PROCESS_MEMORY);
	int i = 0;
	for (i = 0; i < tamanioLista; i++) {
		TABLA_MEMORIA_PROCESO* element = list_get(TABLA_PROCESS_MEMORY, i);
		if (element != NULL && strcmp(element->PID, PID) == 0) {
			list_remove(TABLA_PROCESS_MEMORY, i);
		}
	}

}

TABLA_MEMORIA_PROCESO* buscar_ultima_pagina_asignada_a_proceso(char* PID) {
	int tamanioLista = list_size(TABLA_PROCESS_MEMORY);
	int i = 0;
	TABLA_MEMORIA_PROCESO* element;
	for (i = (tamanioLista - 1); i >= 0; i--) {
		element = list_get(TABLA_PROCESS_MEMORY, i);
		if (element != NULL && strcmp(element->PID, PID) == 0) {
			return element;
		}
	}
	return NULL;
}

TABLA_MEMORIA_PROCESO* solicitar_nueva_pagina_memoria(char* PID) {
	char* nroPagina = asignar_Paginas_Programa(PID, "1");
	//char* metadata = solicitar_bytes_memoria(PID,nroPagina,"0","5");

	char* metadata = malloc(5 + 1);
	char numeroBytesOcupados[4 + 1];
	strcpy(metadata, "1");

	strcpy(numeroBytesOcupados, string_repeat('0', 4 - strlen(string_itoa(tamanio_pagina_memoria - 5))));
	strcat(numeroBytesOcupados, string_itoa(tamanio_pagina_memoria - 5));
	strcat(metadata, numeroBytesOcupados);

	almacenar_Bytes_de_Pagina(PID, nroPagina, "0", "5", metadata);

	return crear_item_Tabla_memoria_proceso(PID, atoi(nroPagina), tamanio_pagina_memoria - 20);
}

/**
 * 1 - Entra en pagina
 * 2 - No hay espacio en la pagina
 * 3 - Es demaciado grande el tamanio, no se puede reservar en este tipo de memoria
 */
int verificar_si_malloc_entra_en_pagina(TABLA_MEMORIA_PROCESO* registro, unsigned espacioSolicitado) {
	if (espacioSolicitado > tamanio_pagina_memoria - 20) {
		return 3;
	} else if (espacioSolicitado > (registro->espacioDisponible - 5)) {
		return 2;
	} else {
		return 1;
	}

}

int reservar_espacio_memoria_en_pagina(TABLA_MEMORIA_PROCESO* registro, unsigned espacioSolicitado) {
	//1. Solicitar a la memoria la pagina que resta disponible

	int tamanioSolicitar = espacioSolicitado + 5 + 5;
	//10 para el metadata inicial.

	Metadata_Memoria* metadataMemoria = analizar_espacio_pagina(registro, espacioSolicitado);
	if (metadataMemoria == NULL) {
		return -1;
	}

	metadataMemoria->espacioDisponible -= (espacioSolicitado + 5);

	// - 10 para posicionarse desde donde empieza el metadata;

	char *paginaMemoria = malloc(tamanioSolicitar + 1);
	char numeroBytesOcupados[4 + 1];

	strcpy(paginaMemoria, "0");

	char* tamanioMalloc_String = string_itoa((int) espacioSolicitado);

	strcpy(numeroBytesOcupados, string_repeat('0', (4 - strlen(tamanioMalloc_String))));
	strcat(numeroBytesOcupados, tamanioMalloc_String);
	strcat(paginaMemoria, numeroBytesOcupados);

	strcat(paginaMemoria, string_repeat(' ', espacioSolicitado));

	strcat(paginaMemoria, "1");
	registro->espacioDisponible -= tamanioSolicitar;

	strcpy(numeroBytesOcupados, string_repeat('0', (4 - strlen(string_itoa(metadataMemoria->espacioDisponible)))));
	strcat(numeroBytesOcupados, string_itoa(metadataMemoria->espacioDisponible));

	strcat(paginaMemoria, numeroBytesOcupados);

	almacenar_Bytes_de_Pagina(registro->PID, string_itoa(registro->nroPagina), string_itoa(metadataMemoria->byteInicial), string_itoa(tamanioSolicitar), paginaMemoria);

	modificar_registro_tabla_memoria(registro);

	free(paginaMemoria);
	return metadataMemoria->byteInicial;

}

TABLA_MEMORIA_PROCESO* buscar_pagina_por_PID_NroPagina(char* PID, unsigned pagina) {
	int tamanioLista = list_size(TABLA_PROCESS_MEMORY);
	int i = 0;
	TABLA_MEMORIA_PROCESO* element;
	for (i = 0; i < tamanioLista; i++) {
		element = list_get(TABLA_PROCESS_MEMORY, i);
		if ((element != NULL) && (strcmp(element->PID, PID) == 0) && (element->nroPagina == pagina)) {
			return element;
		}
	}
	return NULL;
}

int liberar_pagina_encontrada(TABLA_MEMORIA_PROCESO* pagina_Buscada, unsigned byteInicial) {
	char* paginaBuscada = solicitar_bytes_memoria(pagina_Buscada->PID, string_itoa(pagina_Buscada->nroPagina), string_itoa(byteInicial), "1");
	if (strcmp(paginaBuscada, "PAGINA_NO_EXISTE") == 0) {
		return 2;
	} else if (strcmp(paginaBuscada, "1") == 0) {
		return 3;
	} else {
		almacenar_Bytes_de_Pagina(pagina_Buscada->PID, string_itoa(pagina_Buscada->nroPagina), string_itoa(byteInicial), "1", "1");
		pagina_Buscada->espacioDisponible += atoi(solicitar_bytes_memoria(pagina_Buscada->PID, string_itoa(pagina_Buscada->nroPagina), string_itoa(byteInicial + 1), "4"));
		modificar_registro_tabla_memoria(pagina_Buscada);

		//eliminar_registro_tabla_memoria(pagina_Buscada);
		return 1;
	}
	return -1;
}

void aplicar_algoritmo_Desfragmentacion_Interna(TABLA_MEMORIA_PROCESO* pagina_Buscada) {
	char* paginaMemoria = solicitar_bytes_memoria(pagina_Buscada->PID, string_itoa(pagina_Buscada->nroPagina), "0", string_itoa(tamanio_pagina_memoria));

	int contadorIndice = 0;
	int tamanioBloque = 0;

	int indicePrimerLibre = -1;
	int indiceSegundoLibre = -1;

	int tamanioPrimerLibre = 0;
	int tamanioSegundoLibre = 0;

	while (contadorIndice < tamanio_pagina_memoria) {
		tamanioBloque = atoi(string_substring(paginaMemoria, (contadorIndice + 1), 4));
		if (paginaMemoria[contadorIndice] == '1') {
			if (indicePrimerLibre == -1) {
				indicePrimerLibre = contadorIndice;
				tamanioPrimerLibre = tamanioBloque;
			} else {
				indiceSegundoLibre = contadorIndice;
				tamanioSegundoLibre = tamanioBloque;
				//Salgo del ciclo
				contadorIndice = tamanio_pagina_memoria;
			}
		} else {
			indicePrimerLibre = -1;
			indiceSegundoLibre = -1;
		}
		contadorIndice = contadorIndice + tamanioBloque + 4+1;
	}

	if (indiceSegundoLibre == -1) {
		return;
	}
	//Metadata Primer Bloque
	//Metadata Segundo Bloque
	//Tamanio Primer Bloque
	//Tamanio Segundo Bloque
	int espacioContenidoNuevo = 5 + tamanioPrimerLibre + tamanioSegundoLibre;
	char* paginaNueva = malloc(5 + espacioContenidoNuevo + 1);

	char numeroBytesOcupados[4 + 1];
	strcpy(paginaNueva, "1");

	char* contenido_String = string_itoa((int) espacioContenidoNuevo);

	strcpy(numeroBytesOcupados, string_repeat('0', (4 - strlen(contenido_String))));
	strcat(numeroBytesOcupados, contenido_String);
	strcat(paginaNueva, numeroBytesOcupados);

	strcat(paginaNueva, string_repeat('-', espacioContenidoNuevo));
	almacenar_Bytes_de_Pagina(pagina_Buscada->PID, string_itoa(pagina_Buscada->nroPagina), string_itoa(indicePrimerLibre), string_itoa(5 + espacioContenidoNuevo), paginaNueva);

}

int buscar_indice_elemento_tabla_memoria(char* PID, unsigned pagina) {
	int tamanioLista = list_size(TABLA_PROCESS_MEMORY);
	int i = 0;
	TABLA_MEMORIA_PROCESO* element;
	for (i = 0; i < tamanioLista; i++) {
		element = list_get(TABLA_PROCESS_MEMORY, i);
		if ((element != NULL) && (strcmp(element->PID, PID) == 0) && (element->nroPagina == pagina)) {
			return i;
		}
	}
	return -1;
}

Metadata_Memoria* analizar_espacio_pagina(TABLA_MEMORIA_PROCESO* registro, int espacioRequerido) {
	char* paginaMemoria = solicitar_bytes_memoria(registro->PID, string_itoa(registro->nroPagina), "0", string_itoa(tamanio_pagina_memoria - 10));
	int contadorIndice = 0;
	int tamanioBloque = 0;
	while (contadorIndice < tamanio_pagina_memoria) {
		tamanioBloque = atoi(string_substring(paginaMemoria, (contadorIndice + 1), 4));
		if (paginaMemoria[contadorIndice] == '1') {
			if (espacioRequerido <= (tamanioBloque - 5)) {
				Metadata_Memoria* metadata = malloc(sizeof(Metadata_Memoria));
				metadata->byteInicial = contadorIndice;
				metadata->espacioDisponible = tamanioBloque;
				//return contadorIndice;
				return metadata;
			}
		}
		contadorIndice = contadorIndice + tamanioBloque + 4 + 1;
	}
	return NULL;

}
