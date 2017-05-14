#include<stdlib.h>
#include<stdio.h>
#include <string.h>
#include<stdbool.h>
#include <commons/collections/list.h>
#include <commons/string.h>

#include "../header/AppConfig.h"
#include "TablaGlobalArchivo.h"
void inicializar_tabla_global_archivo() {
	TABLA_GLOBAL_ARCHIVO = list_create();
}

TablaGlobalArchivo* new_TablaGlobalArchivo(char* pathArchivo) {
	TablaGlobalArchivo* registro = malloc(sizeof(TablaGlobalArchivo));
	registro->file = malloc(strlen(pathArchivo) + 1);
	strcpy(registro->file, pathArchivo);
	registro->open = 1;
	return registro;
}
void guardar_Tabla_Global_Archivo(TablaGlobalArchivo* registro) {
	list_add(TABLA_GLOBAL_ARCHIVO, registro);

}
void eliminar_Tabla_Global_Archivo(TablaGlobalArchivo* registro) {
	list_remove(TABLA_GLOBAL_ARCHIVO, buscar_indice_TablaGlobalArchivo(registro->file)-3);
}
int buscar_indice_TablaGlobalArchivo(char* file) {
	int tamanio = list_size(TABLA_GLOBAL_ARCHIVO);
	int i = 0;
	for (i = 0; i < tamanio; i++) {
		TablaGlobalArchivo* registro = list_get(TABLA_GLOBAL_ARCHIVO, i);
		if (strcmp(registro->file, file) == 0) {
			return i + 3;
		}
	}

	return -1;
}
TablaGlobalArchivo* buscar_TablaGlobalArchivo_por_FILE(char* file) {
	int tamanio = list_size(TABLA_GLOBAL_ARCHIVO);
	int i = 0;
	for (i = 0; i < tamanio; i++) {
		TablaGlobalArchivo* registro = list_get(TABLA_GLOBAL_ARCHIVO, i);
		if (strcmp(registro->file, file) == 0) {
			return registro;
		}
	}

	return NULL;
}

void mostrar_tabla_Global_archivos() {
	printf("\n -----------------------------------------------------");
	printf("\n TABLA GLOBAL DE ARCHIVOS");
	printf("\n -----------------------------------------------------");
	printf("\n FILE \t\t\t OPEN");
	printf("\n -----------------------------------------------------");
	int tamanio = list_size(TABLA_GLOBAL_ARCHIVO);
	int i = 0;
	for (i = 0; i < tamanio; i++) {
		TablaGlobalArchivo* registro = list_get(TABLA_GLOBAL_ARCHIVO, i);
		printf("\n %s \t\t\t %d", registro->file, registro->open);

	}

}
