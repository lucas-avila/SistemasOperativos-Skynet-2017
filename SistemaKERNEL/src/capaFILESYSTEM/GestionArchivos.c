#include<stdlib.h>
#include<stdio.h>
#include <string.h>
#include<stdbool.h>
#include <commons/collections/list.h>
#include <commons/string.h>

#include "../header/AppConfig.h"
#include "../interfaz/InterfazFS.h"

char* CU_ABRIR_ARCHIVO(char* PID, char* pathArchivo, bool flagCreate, bool flagRead, bool flagWrite) {
	//1. Valido si el archivo existe
	bool existeArchivoFS = validar_archivo(pathArchivo);
	if (existeArchivoFS && flagCreate) {
		//2. A El archivo existe, y desea crearlo - error, para que crear si ya existe.
		return "ERROR - ARCHIVO EXISTE";
	} else if (!existeArchivoFS && !flagCreate) {
		//2. B El archivo no existe, y desea operar con el, sino lo crea no va a poder hacer nada.
		return "ERROR - ARCHIVO NO EXISTE";
	} else if (!existeArchivoFS && flagCreate) {
		crear_archivo(pathArchivo);
		return grabar_apertura_en_tablas_archivos(pathArchivo, PID, flagCreate, flagRead, flagWrite);
	} else if (existeArchivoFS && !flagCreate && (flagRead || flagWrite)) {
		return grabar_apertura_en_tablas_archivos(pathArchivo, PID, flagCreate, flagRead, flagWrite);
	} else {
		return "ERROR - FALTA MODO APERTURA";
	}

	return "OK";
}



char* CU_MOVER_CURSOR_ARCHIVO(char* PID, char* pathArchivo, int cursor_bloque) {
	return "OK";
}
char* CU_LEER_ARCHIVO(char* PID, char* pathArchivo, int byteInicial, int tamanio) {
	return "OK";
}
char* CU_ESCRIBIR_ARCHIVO(char* PID, char* pathArchivo, int byteInicial, int tamanio, char* contenido) {
	return "OK";
}
char* CU_CERRAR_ARCHIVO(char* PID, char* pathArchivo) {
	return "OK";
}
char* CU_BORRAR_ARCHIVO(char* PID, char* pathArchivo) {
	return "OK";
}
