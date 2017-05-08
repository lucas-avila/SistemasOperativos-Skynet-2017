#include<stdlib.h>
#include<stdio.h>
#include <string.h>
#include<stdbool.h>
#include <commons/collections/list.h>
#include <commons/string.h>

#include "../header/AppConfig.h"
#include "GestionArchivoFuncAux.h"

char* grabar_apertura_en_tablas_archivos(char* pathArchivo, char* PID, bool flagCreate, bool flagRead, bool flagWrite) {
	guardar_apertura_archivo_tabla_global_archivos(pathArchivo);
	char* modoApertura = obtener_modo_apertura(flagCreate, flagRead, flagWrite);
	int FD = guardar_archivo_tabla_local_archivo(PID, pathArchivo, modoApertura);
	return string_itoa(FD);
}

int guardar_archivo_tabla_local_archivo(char* PID, char* pathArchivo, char* modoApertura) {
	return -1;
}
void guardar_apertura_archivo_tabla_global_archivos(char* pathArchivo) {

}

char* obtener_modo_apertura(bool flagCreate, bool flagRead, bool flagWrite) {
	return "";
}

