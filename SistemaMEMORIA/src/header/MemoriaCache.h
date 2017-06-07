#ifndef HEADER_MEMORIACACHE_H_
#define HEADER_MEMORIACACHE_H_

#define TAMANIO 5

typedef struct {
	char PID[TAMANIO];
	int nroPagina;
	char* contenidoPagina;

	//para implementar algoritmo LRU
	int vecesUsada;
} MEMORIA_CACHE;

//LRU:
//Este algoritmo reemplaza las líneas que hace más
//tiempo que no se utilizan por las líneas nuevas que necesita el procesador.

MEMORIA_CACHE *memoriaCacheGlobal;

void inicializar_memoria_cache(int cantidadEntradas,int tamanioPagina,int cantMaximaPorProceso);

void ingresar_valor_en_cache(char* PID,int nroPagina,char* contenidoPagina);

char* buscar_valor_en_cache(char* PID,int nroPagina);

int obtener_cantidad_registros_de_proceso(char* PID);

void eliminar_filas_de_procesos_en_cache(char* PID);

int reemplazar_linea_aplicando_algoritmo(MEMORIA_CACHE fila);

void mostrar_tabla_memoria_cache();

void vaciar_tabla_memoria_cache(int tamanioPagina);

#endif /* HEADER_MEMORIACACHE_H_ */
