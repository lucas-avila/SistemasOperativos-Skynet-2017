/*
 * FileManager.h
 *
 *  Created on: 14/6/2017
 *      Author: utnso
 */

#ifndef HEADER_FILEMANAGER_H_
#define HEADER_FILEMANAGER_H_

#include <commons/bitarray.h>
#include <stdint.h>

#include "Archivo.h"

typedef struct {
	uint32_t tamanio_bloques;
	uint32_t cantidad_bloques;
	char * magic_number;
} Metadata;

Metadata * metadata;
t_bitarray * bitmap;

int obtener_BLOQUE_libre();
int obtener_cantidad_bloques(Archivo * archivo);
char * serializar_archivo(Archivo * archivo);
Archivo * deserializar_archivo(char * serializado);



#endif /* HEADER_FILEMANAGER_H_ */
