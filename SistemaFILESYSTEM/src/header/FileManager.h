/*
 * FileManager.h
 *
 *  Created on: 14/6/2017
 *      Author: utnso
 */

#ifndef HEADER_FILEMANAGER_H_
#define HEADER_FILEMANAGER_H_

#include <commons/bitarray.h>
#include <commons/config.h>
#include <stdint.h>
#include <stdio.h>

#include "Archivo.h"

int clienteKernel;

typedef struct {
	uint32_t tamanio_bloques;
	uint32_t cantidad_bloques;
	char * magic_number;
} Metadata;

Metadata * metadata;
t_bitarray * bitmap;

static const char PATH_ARCHIVOS[] = "/Archivos/";
static const char PATH_METADATA[] = "/Metadata/";
static const char PATH_BLOQUES[] = "/Bloques/";
static const char ARCHIVO_BITMAP[] = "Bitmap.bin";
static const char ARCHIVO_METADATA[] = "Metadata.bin";
static const char MAGIC_NUMBER[] = "SADICA";

void restaurar_metadata();
void guardar_bitmap();
void restaurar_bitmap();
void wipe_data(int block_size, int block_quantity);
void release_blocks(int * blocks, int cant_blocks_archivo);
void actualizar_bitmap(int * blocks, int cant_blocks_archivo);

char * serializar_archivo(Archivo * archivo);
char * generar_path_absoluto(char * intermedio, char * path);
char * generar_path_absoluto(char * intermedio, char * path);
char * read_Archivo(FILE * file);

int obtener_BLOQUE_libre();
int obtener_cantidad_bloques(Archivo * archivo);
int * obtener_bloques_de_config(char ** bloques, int cant_bloques);

Archivo * deserializar_archivo(char * serializado);
Archivo * restaurar_archivo(char * path);

#endif /* HEADER_FILEMANAGER_H_ */
