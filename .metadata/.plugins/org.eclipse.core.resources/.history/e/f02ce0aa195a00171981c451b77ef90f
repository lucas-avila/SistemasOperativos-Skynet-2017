/*
 * Estructuras.h
 *
 *  Created on: 29/4/2017
 *      Author: utnso
 */

#ifndef HEADER_ESTRUCTURAS_H_
#define HEADER_ESTRUCTURAS_H_

#include <commons/collections/list.h>
#include <time.h>
#include <stdint.h>

typedef struct {

	int pid;
	int socket;
	time_t fecha_inicio;
	int cant_impresiones;

} Info_ejecucion;

t_list* Info_procesos;

void inicializar_lista_Info_procesos();

Info_ejecucion* new_Info_ejecucion(int pid, int socket);

void agregar_proceso(Info_ejecucion* info_ejecucion);
void eliminar_todas_infos();
void eliminar_info_proceso(Info_ejecucion* info_ejecucion);
int buscar_indice_Info_proceso(uint32_t PID);

Info_ejecucion* buscar_info_por_PID(uint32_t PID);



#endif /* HEADER_ESTRUCTURAS_H_ */
