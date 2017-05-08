/*
 * GestionArchivos.h
 *
 *  Created on: 7/5/2017
 *      Author: utnso
 */

#ifndef CAPAFILESYSTEM_GESTIONARCHIVOS_H_
#define CAPAFILESYSTEM_GESTIONARCHIVOS_H_
#include<stdbool.h>
#include "TablaGlobalArchivo.h"
#include "TablaProcesoArchivo.h"

char* CU_ABRIR_ARCHIVO(char* PID,char* pathArchivo, bool flagCreate,bool flagRead,bool flagWrite);
char* CU_MOVER_CURSOR_ARCHIVO(char* PID,char* pathArchivo,int cursor_bloque);
char* CU_LEER_ARCHIVO(char* PID,char* pathArchivo, int byteInicial,int tamanio);
char* CU_ESCRIBIR_ARCHIVO(char* PID,char* pathArchivo, int byteInicial,int tamanio,char* contenido);
char* CU_CERRAR_ARCHIVO(char* PID,char* pathArchivo);
char* CU_BORRAR_ARCHIVO(char* PID,char* pathArchivo);
#endif /* CAPAFILESYSTEM_GESTIONARCHIVOS_H_ */
