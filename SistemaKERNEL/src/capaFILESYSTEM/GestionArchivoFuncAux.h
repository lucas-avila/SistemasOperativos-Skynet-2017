/*
 * GestionArchivoFuncAux.h
 *
 *  Created on: 7/5/2017
 *      Author: utnso
 */

#ifndef CAPAFILESYSTEM_GESTIONARCHIVOFUNCAUX_H_
#define CAPAFILESYSTEM_GESTIONARCHIVOFUNCAUX_H_
#include<stdbool.h>
#include "../capaFILESYSTEM/TablaProcesoArchivo.h"
#include "../capaFILESYSTEM/TablaGlobalArchivo.h"
#include "../administrarProcesos/Proceso.h"
char* grabar_apertura_en_tablas_archivos(char* pathArchivo,  Proceso* proc, bool flagCreate, bool flagRead, bool flagWrite);
int guardar_archivo_tabla_local_archivo( Proceso* proc, char* pathArchivo, char* modoApertura);
void guardar_apertura_archivo_tabla_global_archivos(char* pathArchivo);
char* obtener_modo_apertura(bool flagCreate, bool flagRead, bool flagWrite);

TablaProcesoArchivo* buscar_registro_TablaProcesoArchivo(char* PID, int FD);
char* getNombreArchivo(int FD);
bool verificarFlag(char* flags, char flagBuscado);
#endif /* CAPAFILESYSTEM_GESTIONARCHIVOFUNCAUX_H_ */
