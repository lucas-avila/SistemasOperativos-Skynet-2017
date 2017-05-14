/*
 * InterfazFS.h
 *
 *  Created on: 1/4/2017
 *      Author: utnso
 */

#ifndef HEADER_INTERFAZFS_H_
#define HEADER_INTERFAZFS_H_

void inicializar();

char* validar_archivo(char *path);
char* crear_archivo(char *path);
char* borrar(char* path);
char* obtenerDatos(char* path,int bloqueInicial,int size);
char* guardarDatos(char* path,int bloqueInicial,int size,char* contenido);

#endif /* HEADER_INTERFAZFS_H_ */
