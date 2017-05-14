/*
 * TablaGlobalArchivo.h
 *
 *  Created on: 7/5/2017
 *      Author: utnso
 */

#ifndef CAPAFILESYSTEM_TABLAGLOBALARCHIVO_H_
#define CAPAFILESYSTEM_TABLAGLOBALARCHIVO_H_
#include "commons/collections/list.h"
typedef struct {
	char* file;
	int open;
} TablaGlobalArchivo;

t_list* TABLA_GLOBAL_ARCHIVO;

void inicializar_tabla_global_archivo();

TablaGlobalArchivo* new_TablaGlobalArchivo(char* pathArchivo);
void guardar_Tabla_Global_Archivo(TablaGlobalArchivo* registro);
void eliminar_Tabla_Global_Archivo(TablaGlobalArchivo* registro);
int buscar_indice_TablaGlobalArchivo(char* file);
TablaGlobalArchivo* buscar_TablaGlobalArchivo_por_FILE(char* file);
void mostrar_tabla_Global_archivos() ;
#endif /* CAPAFILESYSTEM_TABLAGLOBALARCHIVO_H_ */
