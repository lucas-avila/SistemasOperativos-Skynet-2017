/*
 * InterfazFS.h
 *
 *  Created on: 1/4/2017
 *      Author: utnso
 */

#ifndef HEADER_INTERFAZFS_H_
#define HEADER_INTERFAZFS_H_

int servidor_filesystem;

void inicializar();
int iniciar_conexion_servidor_FS();

char* validar_archivo(char *path);
char* crear_archivo(char *path);
char* borrar(char* path);
char* obtenerDatos(char* path,int bloqueInicial,int size);
char* guardarDatos(char* path,int bloqueInicial,int size,char* contenido);

#endif /* HEADER_INTERFAZFS_H_ */
