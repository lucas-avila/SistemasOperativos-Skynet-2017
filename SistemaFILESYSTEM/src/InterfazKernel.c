/*
 * InterfazKernel.c
 *
 *  Created on: 1/4/2017
 *      Author: utnso
 */

#include "header/InterfazKernel.h"

#include <commons/collections/dictionary.h>
#include <commons/config.h>
#include <commons/string.h>
#include <stdio.h>
#include <stdlib.h>
#include <Sharedlib/Socket.h>
#include <unistd.h>
#include <sys/stat.h>

#include "header/Archivo.h"
#include "header/FileManager.h"

void validar_archivo(char * path) {
	/*Cuando el Proceso Kernel reciba la operación de abrir
	 * un archivo deberá validar que el archivo exista.*/
	char * path_abs = generar_path_absoluto(PATH_ARCHIVOS, path);
	if(access(path_abs, F_OK) == -1)
		enviar_dato_serializado("ARCHIVO_NO_EXISTE", clienteKernel);
	else
		enviar_dato_serializado("OK", clienteKernel);
	free(path_abs);
}

void crear_archivo(char * path) {
	/*Cuando el Proceso Kernel reciba la operación de abrir un archivo deberá, en caso que el
	 * archivo no exista y este sea abierto en modo de creación (“c”), llamar a esta operación que
	 * creará el archivo dentro del path solicitado. Por default todo archivo creado se le debe
	 * asignar al menos 1 bloque de datos.*/

	char * path_abs = generar_path_absoluto(PATH_ARCHIVOS, "");
	char ** directorios = string_split(path, "/");
	int i = 0;
	while(directorios[i+1] != NULL){
		string_append(&path_abs, directorios[i]);
		string_append(&path_abs, "/");
		printf("\n %s \n", path_abs);
		struct stat st = {0};
		if (stat(path_abs, &st) == -1) {
		    mkdir(path_abs, 0700);
		}
		i++;
	}

	string_append(&path_abs, directorios[i]);

	FILE * f;
	//Validaciones...
	if(access(path_abs, F_OK) != -1)
		return;
	if((f = fopen(path_abs, "w")) == NULL){
		perror("No se pudo crear el archivo.");
		exit(-1);
	}

	fclose(f);
	t_config * file = config_create(path_abs);

	if(file == NULL){
		perror("No se pudo configurar el archivo.");
		exit(-1);
	}

	// Asignación de bloque + print en el archivo:

	Archivo * archivo = new_Archivo();
	archivo->bloques[0] = obtener_BLOQUE_libre();
	if(archivo->bloques[0] == -1){
		enviar_dato_serializado("ERROR - SIN_ESPACIO EN FS", clienteKernel);
		return;
	}

	char * bloques_string = convertir_bloques_a_array_chars(archivo->bloques, obtener_cantidad_bloques(archivo));
	config_set_value(file, "TAMANIO", string_itoa(archivo->tamanio));
	config_set_value(file, "BLOQUES", bloques_string);
	config_save(file);

	enviar_dato_serializado("OK", clienteKernel);

	free(directorios);
	free(bloques_string);
	free(archivo);
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
		Archivo * archivo = restaurar_archivo(path_abs);

		int cantidad_bloques = obtener_cantidad_bloques(archivo);
		int offset_en_bloques = offset / metadata->tamanio_bloques;
		int offset_final = offset - offset_en_bloques * metadata->tamanio_bloques;
		FILE * f_bloque;
		char * block_path;
		int bloques_leidos = 0;
		int ultimos_bytes_leidos = 0;
		char * block;

		int i,x=0;
		for(i = offset_en_bloques; i < cantidad_bloques + offset_en_bloques; i++){
			ultimos_bytes_leidos = x;
			block = string_new();
			string_append(&block, string_itoa(archivo->bloques[i]));
			string_append(&block, ".bin");

			block_path = generar_path_absoluto(PATH_BLOQUES, block);

			char c;
			if((f_bloque = fopen(block_path, "r")) != NULL){

				//Si es el primer bloque leido posicionamos el cursor en el offset (si es que tiene)
				if(i == offset_en_bloques && offset_final > 0)
					fseek(f_bloque, offset_final, SEEK_SET);

				while ((c = getc(f_bloque)) != EOF && x < size && x < archivo->tamanio && (x + offset_final - ultimos_bytes_leidos) < metadata->tamanio_bloques){
					texto_leido[x] = c;
					x++;
				}
				bloques_leidos++;
				offset_final = 0;
				fclose(f_bloque);
			}

			free(block);
			free(block_path);
		}
		texto_leido[x] = '\0';
		free(archivo);
	}

	if(strlen(texto_leido)<1){
		int indiceEspacio= 0;
	//for( indiceEspacio= 0; indiceEspacio<size;indiceEspacio++){
		   texto_leido[indiceEspacio]= ' ';
	//	}
		//texto_leido[size]= '\0';
		   texto_leido[1]= '\0';

	}
	//printf("El texto leido es : %s\n", texto_leido);

	enviar_dato_serializado(texto_leido, clienteKernel);
	free(texto_leido);
	free(path_abs);
}

void guardar_datos(char * path, int offset, int size, char * buffer) {
	/* Ante un pedido de escritura el File System almacenará en el path enviado por parámetro,
	 * los bytes del buffer, definidos por el valor del Size y a partir del offset solicitado.
	 * Requiere que el archivo se encuentre abierto en modo escritura ("w").
	 */

	char * path_abs = generar_path_absoluto(PATH_ARCHIVOS, path);

	if(access(path_abs, F_OK) != -1) {

			Archivo * archivo = restaurar_archivo(path_abs);
			int cantidad_bloques = obtener_cantidad_bloques(archivo);

			archivo->tamanio += (size + offset > archivo->tamanio)? (size + offset - archivo->tamanio) : 0;

			if(size + offset > cantidad_bloques * metadata->tamanio_bloques) {
				int bloque_libre;
				while(size + offset > cantidad_bloques * metadata->tamanio_bloques) {
					bloque_libre = obtener_BLOQUE_libre();
					if(bloque_libre != -1) {
						cantidad_bloques++;
						archivo->bloques = (int*)realloc(archivo->bloques, sizeof(int) * cantidad_bloques);
						archivo->bloques[cantidad_bloques - 1] = bloque_libre;
					}
					else {
						enviar_dato_serializado("ERROR - SIN_ESPACIO EN FS", clienteKernel);
						return;
					}
				}
			}

			int offset_en_bloques = offset / metadata->tamanio_bloques;
			int offset_final = offset % metadata->tamanio_bloques;
			FILE * file_bloque;
			int i, j=0, bytes_a_escribir=0;
			char * block_path;


			for(i = offset_en_bloques; i < cantidad_bloques; i++) {
				block_path = string_new();
				string_append(&block_path, string_itoa(archivo->bloques[i]));
				string_append(&block_path, ".bin");

				block_path = generar_path_absoluto(PATH_BLOQUES, block_path);
				file_bloque = fopen(block_path, "r+");

				bytes_a_escribir = (size - j > metadata->tamanio_bloques)? metadata->tamanio_bloques : size - j;
				if(i == offset_en_bloques && offset_final > 0){
					fseek(file_bloque, sizeof(char) * offset_final, SEEK_SET);
					if(bytes_a_escribir + offset_final > metadata->tamanio_bloques)
						bytes_a_escribir = metadata->tamanio_bloques - offset_final;
					offset_final = 0;
				}

				if(bytes_a_escribir == 0) i = cantidad_bloques;
				fwrite(buffer + j, sizeof(char), bytes_a_escribir, file_bloque);
				j+=bytes_a_escribir;

				fclose(file_bloque);
				free(block_path);
			}

			t_config * file = config_create(path_abs);
			char * bloques_string = convertir_bloques_a_array_chars(archivo->bloques, obtener_cantidad_bloques(archivo));
			config_set_value(file, "TAMANIO", string_itoa(archivo->tamanio));
			config_set_value(file, "BLOQUES", bloques_string);
			config_save(file);

			free(archivo);
			free(bloques_string);
			free(file);
		}

	enviar_dato_serializado("OK", clienteKernel);
	free(path_abs);
}

void borrar(char * path) {
	/*Borrará el archivo en el path indicado, eliminando
	 * su archivo de metadata y marcando los
	 * bloques como libres dentro del bitmap.
	 */
	int cant_bloques_archivo;
	char * path_abs = generar_path_absoluto(PATH_ARCHIVOS, path);
	Archivo * arch;
	t_config * config;

	if(access(path_abs, F_OK) == -1){
		perror("No se pudo acceder al archivo.\n");
		exit(-1);
	}

	arch = restaurar_archivo(path_abs);
	cant_bloques_archivo = obtener_cantidad_bloques(arch);
	release_blocks(arch->bloques, cant_bloques_archivo);
	enviar_dato_serializado("OK", clienteKernel);
	remove(path_abs);
	free(arch);
}
