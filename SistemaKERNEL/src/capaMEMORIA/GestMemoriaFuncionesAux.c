#include<stdbool.h>
#include<stdint.h>
#include <commons/collections/list.h>
#include<string.h>
#include<stdlib.h>
#include "GestMemoriaFuncionesAux.h"

#include "../interfaz/InterfazMemoria.h"
#include "commons/string.h"
#include "../administrarPCB/EstadisticaProceso.h"
#include "../general/Semaforo.h"
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
	sem_wait(&mutex_tabla_DINAMICA);
	list_add(TABLA_PROCESS_MEMORY, registro);
	sem_post(&mutex_tabla_DINAMICA);
	//Guardar informacion estadistica
	incrementar_en_uno_paginas_HEAP(registro->PID);
}

void modificar_registro_tabla_memoria(TABLA_MEMORIA_PROCESO* registro) {
	int indice = buscar_indice_elemento_tabla_memoria(registro->PID, registro->nroPagina);
	if (indice == -1) {
		return;
	}
	sem_wait(&mutex_tabla_DINAMICA);
	list_replace(TABLA_PROCESS_MEMORY, indice, registro);
	sem_post(&mutex_tabla_DINAMICA);
}

void eliminar_registro_tabla_memoria(TABLA_MEMORIA_PROCESO* registro) {
	int indice = buscar_indice_elemento_tabla_memoria(registro->PID, registro->nroPagina);
	if (indice == -1) {
		return;
	}
	sem_wait(&mutex_tabla_DINAMICA);
	list_remove(TABLA_PROCESS_MEMORY, indice);
	sem_post(&mutex_tabla_DINAMICA);
}

void eliminar_registro_tabla_memoria_por_PID(char* PID) {
	int tamanioLista = list_size(TABLA_PROCESS_MEMORY);
	int i = 0;
	for (i = 0; i < tamanioLista; i++) {
		TABLA_MEMORIA_PROCESO* element = list_get(TABLA_PROCESS_MEMORY, i);
		if (element != NULL && strcmp(element->PID, PID) == 0) {
			sem_wait(&mutex_tabla_DINAMICA);
			list_remove(TABLA_PROCESS_MEMORY, i);
			sem_post(&mutex_tabla_DINAMICA);
		}
	}

}

TABLA_MEMORIA_PROCESO* buscar_ultima_pagina_asignada_a_proceso(char* PID) {
	sem_wait(&mutex_tabla_DINAMICA);
	int tamanioLista = list_size(TABLA_PROCESS_MEMORY);
	int i = 0;
	TABLA_MEMORIA_PROCESO* element;
	for (i = (tamanioLista - 1); i >= 0; i--) {
		element = list_get(TABLA_PROCESS_MEMORY, i);
		if (element != NULL && strcmp(element->PID, PID) == 0) {
			sem_post(&mutex_tabla_DINAMICA);
			return element;
		}
	}
	sem_post(&mutex_tabla_DINAMICA);
	return NULL;
}

TABLA_MEMORIA_PROCESO* solicitar_nueva_pagina_memoria(char* PID) {
	char* nroPagina = asignar_Paginas_Programa(PID, "1");
	if(strcmp(nroPagina,"FALTA ESPACIO") == 0){
		return NULL;
	}

	//char* metadata = solicitar_bytes_memoria(PID,nroPagina,"0","5");

	char* metadata = malloc(5 + 1);
	char numeroBytesOcupados[4 + 1];
	strcpy(metadata, "1");

	char * aux = string_itoa(tamanio_pagina_memoria - 5);
	strcpy(numeroBytesOcupados, string_repeat('0', 4 - strlen(aux)));
	strcat(numeroBytesOcupados, string_itoa(tamanio_pagina_memoria - 5));
	strcat(metadata, numeroBytesOcupados);

	almacenar_Bytes_de_Pagina(PID, nroPagina, "0", "5", metadata);

	return crear_item_Tabla_memoria_proceso(PID, atoi(nroPagina), tamanio_pagina_memoria - 20);
	//free(nroPagina);
	//free(aux);
}

/**
 * 1 - Entra en pagina
 * 2 - No hay espacio en la pagina
 * 3 - Es demaciado grande el tamanio, no se puede reservar en este tipo de memoria
 */
int verificar_si_malloc_entra_en_pagina(TABLA_MEMORIA_PROCESO* registro, unsigned espacioSolicitado) {
	if (espacioSolicitado > tamanio_pagina_memoria - 10) {
		return 3;
	} else if (espacioSolicitado > (registro->espacioDisponible - 5)) {
		return 2;
	} else {
		return 1;
	}

}

int reservar_espacio_memoria_en_pagina(TABLA_MEMORIA_PROCESO* registro, unsigned espacioSolicitado) {
	//1. Solicitar a la memoria la pagina que resta disponible

	//10 para el metadata inicial.

	Metadata_Memoria* metadataMemoria = analizar_espacio_pagina(registro, espacioSolicitado);
	if (metadataMemoria == NULL) {
		return -1;
	}

	int tamanioSolicitar = espacioSolicitado + 5 + 5;
	if (metadataMemoria->reservarBloqueFinal == 0) {
		tamanioSolicitar -= 5;
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
	//strcat(paginaMemoria,"\0");
	registro->espacioDisponible -= espacioSolicitado;
	if (metadataMemoria->reservarBloqueFinal == 1) {

		strcat(paginaMemoria, "1");


		strcpy(numeroBytesOcupados, string_repeat('0', (4 - strlen(string_itoa(metadataMemoria->espacioDisponible)))));
		strcat(numeroBytesOcupados, string_itoa(metadataMemoria->espacioDisponible));

		strcat(paginaMemoria, numeroBytesOcupados);
	}
	char * nroPagina = string_itoa(registro->nroPagina);
	char * byteInicial = string_itoa(metadataMemoria->byteInicial);
	char * tamanioSolicitud = string_itoa(tamanioSolicitar);
	almacenar_Bytes_de_Pagina(registro->PID, nroPagina, byteInicial, tamanioSolicitud, paginaMemoria);

//	modificar_registro_tabla_memoria(registro);

	free(nroPagina);
	free(byteInicial);
	free(tamanioSolicitud);
	free(paginaMemoria);
	free(tamanioMalloc_String);

	return metadataMemoria->byteInicial;
}

TABLA_MEMORIA_PROCESO* buscar_pagina_por_PID_NroPagina(char* PID, unsigned pagina) {
	sem_wait(&mutex_tabla_DINAMICA);
	int tamanioLista = list_size(TABLA_PROCESS_MEMORY);
	int i = 0;
	TABLA_MEMORIA_PROCESO* element;
	for (i = 0; i < tamanioLista; i++) {
		element = list_get(TABLA_PROCESS_MEMORY, i);
		if ((element != NULL) && (strcmp(element->PID, PID) == 0) && (element->nroPagina == pagina)) {
			sem_post(&mutex_tabla_DINAMICA);
			return element;
		}
	}
	sem_post(&mutex_tabla_DINAMICA);
	return NULL;
}

int liberar_pagina_encontrada(TABLA_MEMORIA_PROCESO* pagina_Buscada, unsigned byteInicial) {
	if(pagina_Buscada==NULL){
		return 2;
	}
	char * nroPagina = string_itoa(pagina_Buscada->nroPagina);
	char * byteIni = string_itoa(byteInicial);

	char * paginaBuscada = solicitar_bytes_memoria(pagina_Buscada->PID, nroPagina, byteIni, "1");
	if (strcmp(paginaBuscada, "PAGINA_NO_EXISTE") == 0) {
		free(nroPagina);
		free(byteIni);
		free(paginaBuscada);
		return 2;
	} else if (strcmp(paginaBuscada, "1") == 0) {
		free(nroPagina);
		free(byteIni);
		free(paginaBuscada);
		return 3;
	} else {
		almacenar_Bytes_de_Pagina(pagina_Buscada->PID, nroPagina, byteIni, "1", "1");
		char * aux = string_itoa(byteInicial + 1);
		int tamanioLiberado = atoi(solicitar_bytes_memoria(pagina_Buscada->PID, nroPagina, aux, "4"));
		pagina_Buscada->espacioDisponible += tamanioLiberado;
		modificar_registro_tabla_memoria(pagina_Buscada);
		//Lleno informacion estadistica
		incrementar_FREE(pagina_Buscada->PID, tamanioLiberado);
		free(aux);
		free(nroPagina);
		free(byteIni);
		free(paginaBuscada);
		return 1;
	}
	return -1;
}

void aplicar_algoritmo_Desfragmentacion_Interna(TABLA_MEMORIA_PROCESO* pagina_Buscada) {

	char * nroPagina = string_itoa(pagina_Buscada->nroPagina);
	char * tamanio_pag_mem = string_itoa(tamanio_pagina_memoria);
	char * paginaMemoria = solicitar_bytes_memoria(pagina_Buscada->PID, nroPagina, "0", tamanio_pag_mem);

	int contadorIndice = 0;
	int tamanioBloque = 0;

	int indicePrimerLibre = -1;
	int indiceSegundoLibre = -1;

	int tamanioPrimerLibre = 0;
	int tamanioSegundoLibre = 0;

	int espacioDisponible = 0;

	while (contadorIndice < tamanio_pagina_memoria) {
		tamanioBloque = atoi(string_substring(paginaMemoria, (contadorIndice + 1), 4));
		if (paginaMemoria[contadorIndice] == '1') {
			espacioDisponible += tamanioBloque;
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
		contadorIndice = contadorIndice + tamanioBloque + 4 + 1;
	}
	pagina_Buscada->espacioDisponible = espacioDisponible;
	if (indiceSegundoLibre == -1) {
		return;
	}
	//Metadata Primer Bloque
	//Metadata Segundo Bloque
	//Tamanio Primer Bloque
	//Tamanio Segundo Bloque

	pagina_Buscada->espacioDisponible += 5;

	int espacioContenidoNuevo = 5 + tamanioPrimerLibre + tamanioSegundoLibre;
	char* paginaNueva = malloc(5 + espacioContenidoNuevo + 1);

	char numeroBytesOcupados[4 + 1];
	strcpy(paginaNueva, "1");

	char* contenido_String = string_itoa((int) espacioContenidoNuevo);

	strcpy(numeroBytesOcupados, string_repeat('0', (4 - strlen(contenido_String))));
	strcat(numeroBytesOcupados, contenido_String);
	strcat(paginaNueva, numeroBytesOcupados);

	strcat(paginaNueva, string_repeat('-', espacioContenidoNuevo));
	char * indice_Primer_Libre = string_itoa(indicePrimerLibre);
	char * aux = string_itoa(5 + espacioContenidoNuevo);
	almacenar_Bytes_de_Pagina(pagina_Buscada->PID, nroPagina, indice_Primer_Libre, aux, paginaNueva);

	liberar_pagina(pagina_Buscada);
	free(nroPagina);
	free(tamanio_pag_mem);
	free(indice_Primer_Libre);
	free(aux);
	free(contenido_String);
}

void liberar_pagina(TABLA_MEMORIA_PROCESO* pagina_Buscada) {
	if (pagina_Buscada->espacioDisponible == (tamanio_pagina_memoria - 5)) {
		char * nroPagina = string_itoa(pagina_Buscada->nroPagina);
		liberar_pagina_proceso(pagina_Buscada->PID, nroPagina);
		eliminar_registro_tabla_memoria(pagina_Buscada);
		free(nroPagina);
	}
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

	char * nroPagina = string_itoa(registro->nroPagina);
	char * aux = string_itoa(tamanio_pagina_memoria - 10);
	char * paginaMemoria = solicitar_bytes_memoria(registro->PID, nroPagina, "0", aux);
	int contadorIndice = 0;
	int tamanioBloque = 0;
	while (contadorIndice < tamanio_pagina_memoria) {
		tamanioBloque = atoi(string_substring(paginaMemoria, (contadorIndice + 1), 4));
		if (paginaMemoria[contadorIndice] == '1') {
			if (espacioRequerido < (tamanioBloque - 5)) {
				Metadata_Memoria* metadata = malloc(sizeof(Metadata_Memoria));
				metadata->byteInicial = contadorIndice;
				metadata->espacioDisponible = tamanioBloque;
				metadata->reservarBloqueFinal = 1;
				free(nroPagina);
				free(aux);
				//return contadorIndice;
				return metadata;
			} else if (espacioRequerido == tamanioBloque) {

				Metadata_Memoria* metadata = malloc(sizeof(Metadata_Memoria));
				metadata->byteInicial = contadorIndice;
				metadata->espacioDisponible = tamanioBloque;
				metadata->reservarBloqueFinal = 0;
				//return contadorIndice;
				free(nroPagina);
				free(aux);
				return metadata;
			}
		}
		contadorIndice = contadorIndice + tamanioBloque + 4 + 1;
	}
	free(nroPagina);
	free(aux);

	return NULL;
}

