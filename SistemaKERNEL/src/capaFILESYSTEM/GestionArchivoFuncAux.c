#include<stdlib.h>
#include<stdio.h>
#include <string.h>
#include<stdbool.h>
#include <commons/collections/list.h>
#include <commons/string.h>
#include "../capaFILESYSTEM/TablaGlobalArchivo.h"
#include "../header/AppConfig.h"
#include "GestionArchivoFuncAux.h"
#include "../administrarProcesos/Proceso.h"
char* grabar_apertura_en_tablas_archivos(char* pathArchivo, Proceso* proc, bool flagCreate, bool flagRead, bool flagWrite) {
	guardar_apertura_archivo_tabla_global_archivos(pathArchivo);
	char* modoApertura = obtener_modo_apertura(flagCreate, flagRead, flagWrite);
	int FD = guardar_archivo_tabla_local_archivo(proc, pathArchivo, modoApertura);
	return string_itoa(FD);
}

int guardar_archivo_tabla_local_archivo(Proceso* proc, char* pathArchivo, char* modoApertura) {

	t_list* tablaArchivoLocal = proc->tablaProcesoArchivo;

	int FD = buscar_indice_TablaGlobalArchivo(pathArchivo);

	int FDLocal = list_size(tablaArchivoLocal) + 3;
	TablaProcesoArchivo* registro = new_TablaProcesoArchivo(FDLocal, modoApertura,FD-3);
	guardar_registro_Tabla_Proceso_Archivo(tablaArchivoLocal, registro);

	return registro->FD;

}
void guardar_apertura_archivo_tabla_global_archivos(char* pathArchivo) {
	TablaGlobalArchivo* archivo = buscar_TablaGlobalArchivo_por_FILE(pathArchivo);
	if (archivo==NULL) {
		TablaGlobalArchivo* registro = new_TablaGlobalArchivo(pathArchivo);
		guardar_Tabla_Global_Archivo(registro);
	} else {
		archivo->open++;
	}
}

char* obtener_modo_apertura(bool flagCreate, bool flagRead, bool flagWrite) {
	char* modo = string_new();
	string_append(&modo, (flagCreate ? "C" : ""));
	string_append(&modo, (flagRead ? "L" : ""));
	string_append(&modo, (flagWrite ? "E" : ""));
	return modo;
}

TablaProcesoArchivo* buscar_registro_TablaProcesoArchivo(char* PID, int FD) {
	t_list* tabla = ((Proceso*) buscar_proceso_by_PID(PID))->tablaProcesoArchivo;
	if (tabla == NULL) {
		return NULL;
	}
	TablaProcesoArchivo* registro = buscar_TablaProcesoArchivo_por_FD(tabla, FD);
	return registro;
}

char* getNombreArchivo(int FD) {
	return ((TablaGlobalArchivo*) list_get(TABLA_GLOBAL_ARCHIVO, (FD - 3)))->file;
}

bool verificarFlag(char* flags, char flagBuscado) {
	int tamanio = strlen(flags);
	int i = 0;
	for (i = 0; i < tamanio; i++) {
		if (flags[i] == flagBuscado) {
			return true;
		}
	}
	return false;
}


void abrir_archivo(clienteCPU){
	char* PID = recibir_dato_serializado(clienteCPU);
	char* pathArchivo = recibir_dato_serializado(clienteCPU);
	char* flagCreate = recibir_dato_serializado(clienteCPU);
	char* flagRead = recibir_dato_serializado(clienteCPU);
	char* flagWrite = recibir_dato_serializado(clienteCPU);

	char* respuesta = CU_ABRIR_ARCHIVO(PID, pathArchivo, flagCreate, flagRead, flagWrite);

	enviar_dato_serializado(respuesta, clienteCPU);
}

void mover_cursor_archivo(clienteCPU){

	char* PID = recibir_dato_serializado(clienteCPU);
	int FD = atoi(recibir_dato_serializado(clienteCPU));
	int cursor_bloque = atoi(recibir_dato_serializado(clienteCPU));

	char* respuesta = CU_MOVER_CURSOR_ARCHIVO(PID, FD, cursor_bloque);

	enviar_dato_serializado(respuesta, clienteCPU);


}

void leer_archivo(clienteCPU){

	char* PID = recibir_dato_serializado(clienteCPU);
	int FD = atoi(recibir_dato_serializado(clienteCPU));
	int tamanio = atoi(recibir_dato_serializado(clienteCPU));

	char* respuesta = LEER_ARCHIVO(PID, FD, tamanio);

	enviar_dato_serializado(respuesta, clienteCPU);

}

void cerrar_archivo(clienteCPU){
	char* PID = recibir_dato_serializado(clienteCPU);
	int FD = atoi(recibir_dato_serializado(clienteCPU));

	char* respuesta = CERRAR_ARCHIVO(PID, FD);

	enviar_dato_serializado(respuesta, clienteCPU);
}

void borrar_archivo(clienteCPU){

	char* PID = recibir_dato_serializado(clienteCPU);
	char* rutaArchivo = atoi(recibir_dato_serializado(clienteCPU));

	char* respuesta = BORRAR_ARCHIVO(PID, rutaArchivo);

	enviar_dato_serializado(respuesta, clienteCPU);
}
