/*
 * GestionArchivoFuncAux.h
 *
 *  Created on: 7/5/2017
 *      Author: utnso
 */

#ifndef CAPAFILESYSTEM_GESTIONARCHIVOFUNCAUX_H_
#define CAPAFILESYSTEM_GESTIONARCHIVOFUNCAUX_H_
#include<stdbool.h>
char* grabar_apertura_en_tablas_archivos(char* pathArchivo, char* PID, bool flagCreate, bool flagRead, bool flagWrite) ;
int guardar_archivo_tabla_local_archivo(char* PID,char*  pathArchivo,char*  modoApertura);
void guardar_apertura_archivo_tabla_global_archivos(char*  pathArchivo);
char* obtener_modo_apertura(bool flagCreate, bool flagRead, bool flagWrite);
#endif /* CAPAFILESYSTEM_GESTIONARCHIVOFUNCAUX_H_ */
