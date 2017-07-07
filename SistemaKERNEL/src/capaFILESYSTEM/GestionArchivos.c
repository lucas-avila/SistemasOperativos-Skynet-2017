#include<stdlib.h>
#include<stdio.h>
#include <string.h>
#include<stdbool.h>
#include <commons/collections/list.h>
#include <commons/string.h>
#include "../header/AppConfig.h"
#include "../interfaz/InterfazFS.h"
#include "../capaFILESYSTEM/TablaProcesoArchivo.h"
#include "../capaFILESYSTEM/TablaGlobalArchivo.h"
#include "../administrarProcesos/Proceso.h"

#include "../administrarPCB/EstadisticaProceso.h"
char* CU_ABRIR_ARCHIVO(char* PID, char* pathArchivo, bool flagCreate, bool flagRead, bool flagWrite) {
	Proceso* proc = buscar_proceso_by_PID(atoi(PID));
	if (proc == NULL) {
		return "ERROR_PROCESO_NO_EXISTE";
	}

	//1. Valido si el archivo existe
	char * respuesta_validar = validar_archivo(pathArchivo);
	int existeArchivoFS = strcmp(respuesta_validar, "OK") == 0;
	if (existeArchivoFS && flagCreate) {
		//2. A El archivo existe, y desea crearlo - error, para que crear si ya existe.
		return "ERROR - ARCHIVO EXISTE";
	} else if (!existeArchivoFS && !flagCreate) {
		//2. B El archivo no existe, y desea operar con el, sino lo crea no va a poder hacer nada.
		return "ERROR_ARCHIVO_NO_EXISTE";

	} else if (!existeArchivoFS && flagCreate) {

		//Informacion Estadistica
		incrementar_SYSCALL(PID, 1);

		crear_archivo(pathArchivo);
		return grabar_apertura_en_tablas_archivos(pathArchivo, proc, flagCreate, flagRead, flagWrite);
	} else if (existeArchivoFS && !flagCreate && (flagRead || flagWrite)) {
		return grabar_apertura_en_tablas_archivos(pathArchivo, proc, flagCreate, flagRead, flagWrite);
	} else {
		return "ERROR - FALTA MODO APERTURA";
	}

	return "OK";
}

char* CU_MOVER_CURSOR_ARCHIVO(char* PID, int FD, int cursor_bloque) {
	TablaProcesoArchivo* registro = buscar_registro_TablaProcesoArchivo(PID, FD);
	if (registro == NULL) {
		return "ERROR_ARCHIVO_NO_ABIERTO";
	}
	registro->cursor_bloque = cursor_bloque;

	//Informacion Estadistica
	incrementar_SYSCALL(PID, 1);
	return "OK";
}

char* CU_LEER_ARCHIVO(char* PID, int FD, int tamanio) {
	TablaProcesoArchivo* registro = buscar_registro_TablaProcesoArchivo(PID, FD);
	if (registro == NULL) {
		return "ERROR_ARCHIVO_NO_ABIERTO";
	}
	if (verificarFlag(registro->flags, 'L')) {
		//Informacion Estadistica
		incrementar_SYSCALL(PID, 1);
		return obtenerDatos(getNombreArchivo(registro->GlobalFD), registro->cursor_bloque, tamanio);
	} else {
		return "ERROR_FALTA_MODO_LECTURA";
	}
}

char* CU_ESCRIBIR_ARCHIVO(char* PID, int FD, int tamanio, char* contenido) {
	TablaProcesoArchivo* registro = buscar_registro_TablaProcesoArchivo(PID, FD);
	if (registro == NULL) {
		return "ERROR_ARCHIVO_NO_ABIERTO";
	}
	if (verificarFlag(registro->flags, 'E')) {
		//Informacion Estadistica
		incrementar_SYSCALL(PID, 1);
		return guardarDatos(getNombreArchivo(registro->GlobalFD), registro->cursor_bloque, tamanio, contenido);
	} else {
		return "ERROR_FALTA_MODO_ESCRITURA";
	}
}

char* CU_CERRAR_ARCHIVO(char* PID, int FD) {
	TablaProcesoArchivo* registro = buscar_registro_TablaProcesoArchivo(PID, FD);
	if (registro == NULL) {
		return "ERROR_ARCHIVO_NO_ABIERTO";
	}

	int IndiceGlobal = registro->GlobalFD;

	eliminar_registro_Tabla_Proceso_Archivo(((Proceso*) buscar_proceso_by_PID(atoi(PID)))->tablaProcesoArchivo, registro);
	TablaGlobalArchivo* registroGlobal = list_get(TABLA_GLOBAL_ARCHIVO, IndiceGlobal);
	registroGlobal->open -= 1;
	if (registroGlobal->open == 0) {
		//Informacion Estadistica
		incrementar_SYSCALL(PID, 1);
		eliminar_Tabla_Global_Archivo(registroGlobal);
	}
	return "OK";
}

char* CU_BORRAR_ARCHIVO(char* PID, char* rutaArchivo, int FD) {

	//1. Valido si el archivo existe
	bool existeArchivoFS = (strcmp(validar_archivo(rutaArchivo), "OK") == 0);
	if (!existeArchivoFS) {
		return "ERROR_ARCHIVO_NO_EXISTE";
	}
	TablaProcesoArchivo* registro = buscar_registro_TablaProcesoArchivo(PID, FD);
	if (registro == NULL) {
		return "ERROR_ARCHIVO_NO_ABIERTO";
	}

	int IndiceGlobal = registro->GlobalFD;
	bool existeArchivoTablaGlobal = (buscar_TablaGlobalArchivo_por_FILE(rutaArchivo) != NULL);
	if (existeArchivoTablaGlobal) {
		if (contar_Cantidad_FD_ABIERTO(IndiceGlobal) > 1) {
			return "ERROR_ARCHIVO_ABIERTO";
		}
	}

	existeArchivoFS = (strcmp(validar_archivo(rutaArchivo), "OK") == 0);
	if (!existeArchivoFS) {
		return "ERROR_ARCHIVO_NO_EXISTE";
	}
	eliminar_registro_Tabla_Proceso_Archivo(((Proceso*) buscar_proceso_by_PID(atoi(PID)))->tablaProcesoArchivo, registro);
	TablaGlobalArchivo* registroGlobal = list_get(TABLA_GLOBAL_ARCHIVO, IndiceGlobal);
	registroGlobal->open -= 1;
	if (registroGlobal->open == 0) {
		//Informacion Estadistica
		incrementar_SYSCALL(PID, 1);
		eliminar_Tabla_Global_Archivo(registroGlobal);
	}
	borrar(rutaArchivo);
	//Informacion Estadistica
	incrementar_SYSCALL(PID, 1);
	return "OK";
}
