/*
 * GestMemoriaFuncionesAux.h
 *
 *  Created on: 1/5/2017
 *      Author: utnso
 */

#ifndef CAPAMEMORIA_GESTMEMORIAFUNCIONESAUX_H_
#define CAPAMEMORIA_GESTMEMORIAFUNCIONESAUX_H_
#include<stdbool.h>
#include<stdint.h>
#include <commons/collections/list.h>
typedef struct {
	char* PID;
	unsigned nroPagina;
	unsigned espacioDisponible;
} TABLA_MEMORIA_PROCESO;

typedef struct {
	uint32_t size;
	bool isFree;
} HeapMetadata;

typedef struct {
	int espacioDisponible;
	int byteInicial;
} Metadata_Memoria;

t_list* TABLA_PROCESS_MEMORY;

TABLA_MEMORIA_PROCESO* crear_item_Tabla_memoria_proceso(char* PID, unsigned nroPagina, unsigned espacioDisponible);

void guardar_registro_tabla_memoria(TABLA_MEMORIA_PROCESO* registro);

void modificar_registro_tabla_memoria(TABLA_MEMORIA_PROCESO* registro);

void eliminar_registro_tabla_memoria(TABLA_MEMORIA_PROCESO* registro);

void eliminar_registro_tabla_memoria_por_PID(char* PID);

TABLA_MEMORIA_PROCESO* buscar_ultima_pagina_asignada_a_proceso(char* PID);

TABLA_MEMORIA_PROCESO* solicitar_nueva_pagina_memoria(char* PID);

/**
 * 1 - Entra en pagina
 * 2 - No hay espacio en la pagina
 * 3 - Es demaciado grande el tamanio, no se puede reservar en este tipo de memoria
 */
int verificar_si_malloc_entra_en_pagina(TABLA_MEMORIA_PROCESO* registro, unsigned espacioSolicitado);

int reservar_espacio_memoria_en_pagina(TABLA_MEMORIA_PROCESO* registro, unsigned espacioSolicitado);

TABLA_MEMORIA_PROCESO* buscar_pagina_por_PID_NroPagina(char* PID, unsigned pagina);
int liberar_pagina_encontrada(TABLA_MEMORIA_PROCESO* pagina_Buscada, unsigned byteInicial);
void aplicar_algoritmo_Desfragmentacion_Interna(TABLA_MEMORIA_PROCESO* pagina_Buscada);

int buscar_indice_elemento_tabla_memoria(char* PID, unsigned pagina);

void inicializar_tabla_proceso_memoria();

Metadata_Memoria* analizar_espacio_pagina(TABLA_MEMORIA_PROCESO* registro, int espacioRequerido);

#endif /* CAPAMEMORIA_GESTMEMORIAFUNCIONESAUX_H_ */
