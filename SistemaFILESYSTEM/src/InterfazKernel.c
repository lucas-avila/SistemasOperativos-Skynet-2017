/*
 * InterfazKernel.c
 *
 *  Created on: 1/4/2017
 *      Author: utnso
 */

#include "header/InterfazKernel.h"

#include <commons/string.h>
#include <stdio.h>
#include <stdlib.h>
#include <Sharedlib/Socket.h>
#include <unistd.h>

#include "header/Archivo.h"
#include "header/FileManager.h"

void validar_archivo(char* path) {
	if(access(path, F_OK) == -1){
		//enviar_dato_serializado("ARCHIVO_NO_EXISTE", clienteKernel);
	}
	else {
		//enviar_dato_serializado("ARCHIVO_EXISTE", clienteKernel);
	}
}

void crear_archivo(char* path) {
	char * path_abs = generar_path_absoluto(PATH_ARCHIVOS, path);
	FILE * f;
	if((f = fopen(path_abs, "wb")) != NULL){
		Archivo * archivo = new_Archivo();
		archivo->bloques[0] = obtener_BLOQUE_libre();
		if(archivo->bloques[0] == -1){
			enviar_dato_serializado("SIN_ESPACIO", servidor);
			return;
		}

		char * serializado = serializar_archivo(archivo);

		fwrite(serializado, sizeof(char), sizeof(archivo->tamanio) + obtener_cantidad_bloques(archivo) * sizeof(int), f);
		//guardar en el archivo
		free(archivo);
		fclose(f);
	}
	free(path_abs);
}

void obtener_datos(char * path, int offset, int size){
	/* Ante un pedido de datos el File System devolverá del path enviado por parámetro,
	 * la cantidad de bytes definidos por el Size a partir del offset solicitado.
	 * Requiere que el archivo se encuentre abierto en modo lectura (“r”).
	 */
	char * path_abs = generar_path_absoluto(PATH_ARCHIVOS, path);
	char * texto_leido = malloc(size);
	if(access(path_abs, F_OK) != -1){
		FILE * f;
		if((f = fopen(path_abs, "rb")) != NULL){
			char *buffer;
			int filelen;

			fseek(f, 0, SEEK_END);
			filelen = ftell(f);
			rewind(f);

			buffer = malloc((filelen + 1) * sizeof(char));
			fread(buffer, filelen, 1, f);

			//buffer = read_Archivo(f);

			Archivo * archivo = deserializar_archivo(buffer);

			int cantidad_bloques = obtener_cantidad_bloques(archivo);
			int offset_en_bloques = offset / metadata->tamanio_bloques;
			int offset_final = offset - offset_en_bloques * metadata->tamanio_bloques;
			FILE * f_bloque;
			char * block_path;
			int bloques_leidos = 0;
			int ultimos_bytes_leidos = 0;

			int i,x=0;
			for(i = offset_en_bloques; i < cantidad_bloques + offset_en_bloques; i++){
				ultimos_bytes_leidos = x;
				char * block = string_new();
				string_append(&block, string_itoa(i));
				string_append(&block, ".bin");

				block_path = generar_path_absoluto(PATH_BLOQUES, block);

				char c;
				if((f_bloque = fopen(block_path, "r")) != NULL){

					//Si es el primer bloque leido posicionamos el cursor en el offset (si es que tiene)
					if(i == offset_en_bloques && offset_final > 0)
						fseek(f_bloque, offset_final, SEEK_SET);

					while ((c = getc(f_bloque)) != EOF && x < size && (x + offset_final - ultimos_bytes_leidos) < metadata->tamanio_bloques){
						texto_leido[x] = c;
						x++;
					}
					bloques_leidos++;
					offset_final = 0;
					fclose(f_bloque);
				}

				free(block);
			}
			texto_leido[x] = '\0';
			free(archivo);
			free(buffer);
			fclose(f);
		}
	}

	printf("El texto leido es %s\n", texto_leido);
	free(texto_leido);
}

void guardar_datos(char * path, int offset, int size, char * buffer) {
	/* Ante un pedido de escritura el File System almacenará en el path enviado por parámetro,
	 * los bytes del buffer, definidos por el valor del Size y a partir del offset solicitado.
	 * Requiere que el archivo se encuentre abierto en modo escritura ("w").
	 */

	char * path_abs = generar_path_absoluto(PATH_ARCHIVOS, path);
	//char *
	if(access(path_abs, F_OK) != -1) {
		FILE * file;
		if((file = fopen(path_abs, "wb")) != NULL) {
			char * buffer_auxiliar;

			buffer_auxiliar = read_Archivo(file);

			Archivo * archivo = deserializar_archivo(buffer_auxiliar);
			int cantidad_bloques = obtener_cantidad_bloques(archivo);

			int k = 1;
			if(size > (cantidad_bloques * metadata->tamanio_bloques - offset)) {
				archivo->tamanio = offset + size;
				int bloque_libre;
				while(size > ((cantidad_bloques + k) * metadata->tamanio_bloques - offset)) {
					bloque_libre = obtener_BLOQUE_libre();
					if(bloque_libre != -1) {
						archivo->bloques = (int *)malloc(sizeof(archivo->tamanio) + k * sizeof(bloque_libre) + cantidad_bloques * sizeof(int));
						archivo->bloques[cantidad_bloques + k - 1] = bloque_libre;
						k++;
					}
					else {
						perror("No hay espacio suficiente para escribir el archivo.");
						exit(-1);
					}
				}
			}
			cantidad_bloques = obtener_cantidad_bloques(archivo);

			int offset_en_bloques = offset / metadata->tamanio_bloques;
			int offset_final = offset % metadata->tamanio_bloques;
			FILE * file_bloque;
			char * path_bloque;
			int contador_bloques = 0;
			int contador_bytes = 0; //Se usará para verificar que no terminamos de escribir la información pedida.
			int i, j; // "i" para movernos entre bloques, "j" para movernos entre letras.

			for(i = offset_en_bloques; i < cantidad_bloques; i++) {
				char letra;
				char * path_bloque_parcial = string_new();
				string_append(&path_bloque_parcial, string_itoa(i));
				string_append(&path_bloque_parcial, ".bin");

				path_bloque = generar_path_absoluto(PATH_BLOQUES, path_bloque_parcial);

				file_bloque = fopen(path_bloque, "w");

				if(i == offset_en_bloques && offset_final > 0) {
					fseek(file_bloque, offset_final, SEEK_SET);
					j = offset_final;
				}
				else j = 0;

				while(contador_bytes < size && j < metadata->tamanio_bloques) {
					letra = buffer[contador_bytes];
					putc(letra, file_bloque);

					contador_bytes++;
					j++;
				}

			}

			char * serializado = serializar_archivo(archivo);
			fwrite(serializado, sizeof(char), sizeof(archivo->tamanio) + obtener_cantidad_bloques(archivo) * sizeof(int), file);
			free(archivo);
			free(buffer_auxiliar);
			fclose(file);
		}
	}
	else {
		perror("No se pudo acceder al archivo solicitado");
		exit(-1);
	}
}

void borrar(char * path) {
	/*Borrará el archivo en el path indicado, eliminando
	 * su archivo de metadata y marcando los
	 * bloques como libres dentro del bitmap.
	 */
	int cant_bloques_archivo;
	char * buffer;
	char * path_abs = generar_path_absoluto(PATH_ARCHIVOS, path);
	FILE * file;
	Archivo * arch;

	if(access(path_abs, F_OK) == -1){
		perror("No se pudo acceder al archivo");
		exit(-1);
	}

	if((file = fopen(path_abs, "rb")) == NULL)
		exit(-1);

	buffer = read_Archivo(file);
	arch = deserializar_archivo(buffer);
	cant_bloques_archivo = obtener_cantidad_bloques(arch);
	release_blocks(arch->bloques, cant_bloques_archivo);
	remove(path_abs);
	free(buffer);

}



/*
Parámetros: [Path]
Cuando el Proceso Kernel reciba la operación de abrir un archivo deberá validar que el
archivo exista.
Crear Archivo
Parámetros: [Path]
Cuando el Proceso Kernel reciba la operación de abrir un archivo deberá, en caso que el
archivo no exista y este sea abierto en modo de creación (“c”), llamar a esta operación que
creará el archivo dentro del path solicitado. Por default todo archivo creado se le debe
asignar al menos 1 bloque de datos.
Borrar
Parámetros: [Path]
Borrará el archivo en el path indicado, eliminando su archivo de metadata y marcando los
bloques como libres dentro del bitmap.
Obtener Datos
Parámetros: [Path, Offset, Size]
Ante un pedido de datos el File System devolverá del path enviado por parámetro, la
cantidad de bytes definidos por el Size a partir del offset solicitado. Requiere que el archivo
se encuentre abierto en modo lectura (“r”).
Guardar Datos
Parámetros: [Path, Offset, Size, Buffer]
Ante un pedido de escritura el File System almacenará en el path enviado por parámetro, los
bytes del buffer, definidos por el valor del Size y a partir del offset solicitado. Requiere que el
archivo se encuentre abierto en modo escritura (“w”).
 */
