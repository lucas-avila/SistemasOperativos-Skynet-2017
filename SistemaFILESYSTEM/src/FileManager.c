/*
 * FileManager.c
 *
 *  Created on: 14/6/2017
 *      Author: utnso
 */

#include "header/FileManager.h"

#include <commons/string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

void inicializar_estructuras_administrativas(){
	//hardcodeado para probar
	metadata = malloc(sizeof(Metadata));
	metadata->cantidad_bloques = 10;
	metadata->magic_number = string_new();
	string_append(&metadata->magic_number, "SADICA");
	metadata->tamanio_bloques = 64;

	/*  char data[] = { 0b10000000, 0, 0b00000001 };
            bitarray = bitarray_create_with_mode(data, sizeof(data), LSB_FIRST); */
	char data[] = { 0b11110000, 0, 0 };
	bitmap = bitarray_create_with_mode(data, 3, MSB_FIRST);
}

int obtener_BLOQUE_libre(){
	int size = bitarray_get_max_bit(bitmap);
	int i=0;
	for(i; i < size; i++){
		if(!bitarray_test_bit(bitmap, i)){
			bitarray_set_bit(bitmap, i);
			return i;
		}
	}
	return -1;
}

int obtener_cantidad_bloques(Archivo * archivo){
	if(archivo->tamanio == 0){
		return 1;
	}else{
		return (archivo->tamanio / metadata->tamanio_bloques) + 1;
	}
}

char * serializar_archivo(Archivo * archivo){
	int cant_bloques = obtener_cantidad_bloques(archivo);
	char * serializado = malloc(sizeof(archivo->tamanio) + cant_bloques * sizeof(int));

	memcpy(serializado, &archivo->tamanio, sizeof(archivo->tamanio));
	int i = 0;
	for(i; i < cant_bloques; i++){
		memcpy(serializado + (i+1) * sizeof(int), &archivo->bloques[i], sizeof(int));
	}

	return serializado;
}

Archivo * deserializar_archivo(char * serializado){
	Archivo * archivo = malloc(sizeof(Archivo));
	memcpy(&archivo->tamanio, serializado, sizeof(int));

	int cant_bloques = obtener_cantidad_bloques(archivo);

	archivo->bloques = (int *) malloc(cant_bloques * sizeof(int));
	int i = 0;
	for(i; i < cant_bloques; i++){
		memcpy(&archivo->bloques[i], serializado + (i+1) * sizeof(int), sizeof(int));
	}

	return archivo;
}





















