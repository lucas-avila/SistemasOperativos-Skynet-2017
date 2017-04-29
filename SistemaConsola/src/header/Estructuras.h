/*
 * Estructuras.h
 *
 *  Created on: 29/4/2017
 *      Author: utnso
 */

#ifndef HEADER_ESTRUCTURAS_H_
#define HEADER_ESTRUCTURAS_H_

typedef struct {

	int pid;
	char fecha_inicio[9];
	char * hora_inicio;
	char fecha_final[9];
	char * hora_final;
	int cant_impresiones;

} Info_ejecucion;

#endif /* HEADER_ESTRUCTURAS_H_ */
