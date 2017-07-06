#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<commons/string.h>
#include<stdbool.h>
#include<commons/log.h>
#include "general/funcionesUtiles.h"
#include "header/MemoriaCache.h"
#include "header/AppConfig.h"

int cantidadEntradasCache = 0;

int cantidadMaximaEntradaPorProceso = 0;

int instanteUltimaReferencia = 0;

int secuenciador_referencia() {
	instanteUltimaReferencia++;
	return instanteUltimaReferencia;
}

void inicializar_memoria_cache(int cantidadEntradas, int tamanioPagina, int cantMaximaPorProceso) {
	memoriaCacheGlobal = malloc(sizeof(MEMORIA_CACHE) * cantidadEntradas);
	int i = 0;
	for (i = 0; i < cantidadEntradas; i++) {
		strcpy(memoriaCacheGlobal[i].PID, "");
		memoriaCacheGlobal[i].contenidoPagina = malloc(tamanioPagina);
		memoriaCacheGlobal[i].nroPagina = -1;

		memoriaCacheGlobal[i].vecesUsada = -1;
	}
	cantidadEntradasCache = cantidadEntradas;
	cantidadMaximaEntradaPorProceso = cantMaximaPorProceso;
}

void ingresar_valor_en_cache(char* PID, int nroPagina, char* punteroAPagina) {
	int i = 0;
	for (i = 0; i < cantidadEntradasCache; i++) {
		if ((strcmp(memoriaCacheGlobal[i].PID, PID) == 0) && memoriaCacheGlobal[i].nroPagina == nroPagina) {
			memoriaCacheGlobal[i].vecesUsada = secuenciador_referencia();
			//memoriaCacheGlobal[i].contenidoPagina = malloc(strlen(contenidoPagina));
			memcpy(memoriaCacheGlobal[i].contenidoPagina, punteroAPagina, configuraciones.MARCO_SIZE);
			return;
		}
	}

	MEMORIA_CACHE fila;
	strcpy(fila.PID, PID);
	fila.contenidoPagina = malloc(configuraciones.MARCO_SIZE);
	memcpy(fila.contenidoPagina, punteroAPagina, configuraciones.MARCO_SIZE);
	fila.nroPagina = nroPagina;

	fila.vecesUsada = secuenciador_referencia();

	int codigo_resultado = reemplazar_linea_aplicando_algoritmo(fila);
	if (codigo_resultado == 0) {

		for (i = 0; i < cantidadEntradasCache; i++) {
			if (strcmp(memoriaCacheGlobal[i].PID, "") == 0) {

				memoriaCacheGlobal[i] = fila;

				return;
			}
		}
	}
	/** SI llego a esta instancia es porque no hay lugar disponible entonces
	 * aplico el algoritmo LRU para insertar esta nueva fila a la tabla
	 * TODO: tener en cuenta cantidad maxima de entradas por proceso, que no se pase
	 * de ese numero
	 */

}

char* buscar_valor_en_cache(char* PID, int nroPagina) {

	int i = 0;
	char* valorBuscado;
	for (i = 0; i < cantidadEntradasCache; i++) {
		if ((strcmp(memoriaCacheGlobal[i].PID, PID) == 0) && (memoriaCacheGlobal[i].nroPagina == nroPagina)) {
			valorBuscado = string_new();
			string_append(&valorBuscado, memoriaCacheGlobal[i].contenidoPagina);
			memoriaCacheGlobal[i].vecesUsada = secuenciador_referencia();
			return valorBuscado;
		}
	}
	return "No existe en Cache";
}

int obtener_cantidad_registros_de_proceso(char* PID) {
	int cantidadFilas = 0;
	int i = 0;
	for (i = 0; i < cantidadEntradasCache; i++) {
		if (strcmp(memoriaCacheGlobal[i].PID, PID) == 0) {
			cantidadFilas++;
		}
	}
	return cantidadFilas;

}

void eliminar_filas_de_procesos_en_cache(char* PID) {
	int i = 0;
	for (i = 0; i < cantidadEntradasCache; i++) {
		if (strcmp(memoriaCacheGlobal[i].PID, PID) == 0) {
			strcpy(memoriaCacheGlobal[i].PID, "");
			strcpy(memoriaCacheGlobal[i].contenidoPagina, "");
			memoriaCacheGlobal[i].nroPagina = -1;

			memoriaCacheGlobal[i].vecesUsada = -1;

		}
	}
}

int obtener_indice_tabla_menos_usado(char* PID) {
	int indice = -1;
	int i = 0;
	int minCantUso = 0;

	int controlPrimeraVez = 0;
	for (i = 0; i < cantidadEntradasCache; i++) {

		if (strcmp(PID, "NO") == 0 || strcmp(PID, memoriaCacheGlobal[i].PID) == 0) {
			if (controlPrimeraVez == 0) {
				minCantUso = memoriaCacheGlobal[i].vecesUsada;
				indice = i;
				controlPrimeraVez = 1;
			} else if (minCantUso > memoriaCacheGlobal[i].vecesUsada && memoriaCacheGlobal[i].vecesUsada>=0) {
				minCantUso = memoriaCacheGlobal[i].vecesUsada;
				indice = i;
			}
		}
	}

	return indice;

}
/*
 * CACHE LLENA	PROCESO LLENO	RESULTADO
 NO	NO	INSERTO
 NO	SI	SACO MENOS USADO DE ESE PROCESO E INSERT
 SI	NO	SACO PAGINA MENOS USADA E INSERTO
 SI	SI	SACO MENOS USADO DE ESE PROCESO E INSERT
 */

bool cacheLlena() {
	int i = 0;
	for (i = 0; i < cantidadEntradasCache; i++) {
		if (strcmp(memoriaCacheGlobal[i].PID, "") == 0) {

			return 0;
		}
	}
	return 1;
}

bool procesoLleno(char* pid) {
	int cantidad = obtener_cantidad_registros_de_proceso(pid);
	return cantidad == configuraciones.CACHE_X_PROC;
}

int reemplazar_linea_aplicando_algoritmo(MEMORIA_CACHE fila) {
	int indice = 0;

	if (cacheLlena() == 1) {
		if (procesoLleno(fila.PID) == 1) {
			indice = obtener_indice_tabla_menos_usado(fila.PID);
			strcpy(memoriaCacheGlobal[indice].PID, "");
			return 0;
		} else {
			indice = obtener_indice_tabla_menos_usado(fila.PID);
			strcpy(memoriaCacheGlobal[indice].PID, "NO");
			return 0;
		}
	} else {
		if (procesoLleno(fila.PID) == 1) {
			indice = obtener_indice_tabla_menos_usado(fila.PID);
			strcpy(memoriaCacheGlobal[indice].PID, "");
			return 0;
		} else {
			return 0;
		}
	}

	return 0;
}

void mostrar_tabla_memoria_cache() {

	char* textoLoguear = string_new();
	string_append(&textoLoguear, "\nContenido de la Memoria CACHE: ");

	int i = 0;
	string_append(&textoLoguear, "\n---------------------------");
	string_append(&textoLoguear, "\n PID PAG CONTENIDO");
	string_append(&textoLoguear, "\n---------------------------");

	for (i = 0; i < cantidadEntradasCache; i++) {
		if (strcmp(memoriaCacheGlobal[i].PID, "") != 0) {

			string_append(&textoLoguear, string_from_format("\n %s %d  %s", memoriaCacheGlobal[i].PID, memoriaCacheGlobal[i].nroPagina, memoriaCacheGlobal[i].contenidoPagina));
			string_append(&textoLoguear, "\n---------------------------");
		}
	}
	logSO(textoLoguear);


}

void vaciar_tabla_memoria_cache(int tamanioPagina) {

	int i = 0;
	for (i = 0; i < cantidadEntradasCache; i++) {
		strcpy(memoriaCacheGlobal[i].PID, "");
		memoriaCacheGlobal[i].contenidoPagina = malloc(tamanioPagina);
		memoriaCacheGlobal[i].nroPagina = -1;

		memoriaCacheGlobal[i].vecesUsada = -1;
	}

}
