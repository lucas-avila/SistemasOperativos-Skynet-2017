/*
 * FileManager.c
 *
 *  Created on: 14/6/2017
 *      Author: utnso
 */

#include "header/FileManager.h"

#include <commons/string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "header/AppConfig.h"

void inicializar_estructuras_administrativas(){

	restaurar_metadata();
	restaurar_bitmap();
	/*char data[] = { 0b11110000, 0, 0 };
	bitmap = bitarray_create_with_mode(data, 3, MSB_FIRST); --> ESTO NO VA...*/
}

int obtener_BLOQUE_libre(){
	int size = bitarray_get_max_bit(bitmap);
	int i=0;
	for(i; i < size; i++){
		if(!bitarray_test_bit(bitmap, i)){
			bitarray_set_bit(bitmap, i);
			guardar_bitmap();
			return i;
			//empiezan desde 1.bin
		}
	}
	return -1;
}

int obtener_cantidad_bloques(Archivo * archivo){
	if(archivo->tamanio == 0){
		return 1;
	}else{
		return (archivo->tamanio / metadata->tamanio_bloques);
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

void restaurar_metadata(){
	metadata = malloc(sizeof(Metadata));
	metadata->magic_number = malloc(50);

	FILE * f_metadata;
	char * path_metadata = generar_path_absoluto(PATH_METADATA, ARCHIVO_METADATA);
	if((f_metadata = fopen(path_metadata, "r")) != NULL){
		fscanf(f_metadata, "TAMANIO_BLOQUES=%d\nCANTIDAD_BLOQUES=%d\nMAGIC_NUMBER=%s", &metadata->tamanio_bloques, &metadata->cantidad_bloques, metadata->magic_number);
		fclose(f_metadata);
	}
	free(path_metadata);

}

void guardar_bitmap(){

	char * path = generar_path_absoluto(PATH_METADATA, ARCHIVO_BITMAP);
	FILE * f_bitmap;
	if((f_bitmap = fopen(path, "wb")) != NULL) {
		fwrite(bitmap->bitarray, sizeof(char), bitmap->size, f_bitmap);
		fclose(f_bitmap);
	}
	free(path);
}

void restaurar_bitmap(){
	char * path = generar_path_absoluto(PATH_METADATA, ARCHIVO_BITMAP);
	char * buffer;
	int filelen = 0;
	FILE * f_bitmap;

	if((f_bitmap = fopen(path, "rb")) != NULL) {
		fseek(f_bitmap, 0, SEEK_END);
		filelen = ftell(f_bitmap);
		rewind(f_bitmap);
		buffer = malloc(filelen*sizeof(char));
		fread(buffer, filelen, 1, f_bitmap);
		fclose(f_bitmap);
	}

	bitmap = bitarray_create_with_mode(buffer, filelen, MSB_FIRST);
	free(path);
}

char * generar_path_absoluto(char * intermedio, char * path){
	char * path_abs = string_new();

	string_append(&path_abs, configuraciones.PUNTO_MONTAJE);
	string_append(&path_abs, intermedio);
	string_append(&path_abs, path);

	return path_abs;
}

void wipe_data(int block_size, int block_quantity){

	if(block_quantity % 8 != 0){
		perror("La cantidad de bloques debe ser multiplo de 8.\n");
		exit(-1);
	}

	int i = 0;
	char * block;
	char * path;
	char * bitarray;
	FILE * f_block;
	while(i < block_quantity){

		block = string_new();
		char * num = string_itoa(i);
		string_append(&block, num);
		string_append(&block, ".bin");
		path = generar_path_absoluto(PATH_BLOQUES, block);
		f_block = fopen(path, "w");
		fclose(f_block);
		free(num);
		free(block);
		free(path);
		i++;
	}

	int size = block_quantity / 8;
	bitarray = malloc(size);
	for(i = 0; i < size; i++){
		memcpy(bitarray+i, "\0",1);
	}

	free(bitmap);
	bitmap = bitarray_create_with_mode(bitarray, size, MSB_FIRST);
	guardar_bitmap();

	FILE * f_metadata;
	char * path_metadata = generar_path_absoluto(PATH_METADATA, ARCHIVO_METADATA);

	//La abrimos para volverla a crear
	f_metadata = fopen(path_metadata, "w");
	fclose(f_metadata);

	if((f_metadata = fopen(path_metadata, "a")) != NULL){
		fprintf(f_metadata, "TAMANIO_BLOQUES=%d\nCANTIDAD_BLOQUES=%d\nMAGIC_NUMBER=%s", block_size, block_quantity, MAGIC_NUMBER);
		fclose(f_metadata);
	}
	free(path_metadata);
}











