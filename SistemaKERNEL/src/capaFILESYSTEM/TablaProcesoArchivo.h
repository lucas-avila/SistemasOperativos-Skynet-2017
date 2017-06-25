/*
 * TablaProcesoArchivo.h
 *
 *  Created on: 7/5/2017
 *      Author: utnso
 */

#ifndef CAPAFILESYSTEM_TABLAPROCESOARCHIVO_H_
#define CAPAFILESYSTEM_TABLAPROCESOARCHIVO_H_

#include "commons/collections/list.h"
typedef struct {
	int FD;
	char* flags;
	int GlobalFD;

	//dato agregado que no debe mostrarse
	int cursor_bloque; //guardar el numero de bloque desde donde se desea hacer la operacion
} TablaProcesoArchivo;

TablaProcesoArchivo* new_TablaProcesoArchivo(int FD, char* flags, int ProcesoFD);
void guardar_registro_Tabla_Proceso_Archivo(t_list* tabla, TablaProcesoArchivo* registro);
void modificar_registro_Tabla_Proceso_Archivo(t_list* tabla, TablaProcesoArchivo* registro);
void eliminar_registro_Tabla_Proceso_Archivo(t_list* tabla, TablaProcesoArchivo* registro);
int buscar_indice_TablaProcesoArchivo(t_list* tabla, int FD);
TablaProcesoArchivo* buscar_TablaProcesoArchivo_por_FD(t_list* tabla, int FD);
void mostrar_tabla_proceso_archivos(t_list* tablaProceso, char* info_log);
#endif /* CAPAFILESYSTEM_TABLAPROCESOARCHIVO_H_ */
