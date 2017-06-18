/*
 * InterfazKernel.c
 *
 *  Created on: 1/4/2017
 *      Author: utnso
 */

#include "header/InterfazKernel.h"

#include <stdio.h>
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
	if(access(path_abs, F_OK) == -1){
		//archivo no existe
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
			fclose(f);
		}
	}
	free(path_abs);
}

void borrar(char* path) {
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
