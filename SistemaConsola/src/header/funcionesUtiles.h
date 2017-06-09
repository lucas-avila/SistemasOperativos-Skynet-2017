/*
 * funcionesUtiles.h
 *
 *  Created on: 1/4/2017
 *      Author: utnso
 */

#ifndef HEADER_FUNCIONESUTILES_H_
#define HEADER_FUNCIONESUTILES_H_

#include <stdint.h>

int validarNumeroInput(int rangoMinimo, int rangoMaximo) ;
void validarArchivo(char * path);
char * obtener_codigo(char * path_archivo_fuente);
int pertenece_a_la_consola(uint32_t pid);
//int pid_identicos(Info_ejecucion info_proceso);

#endif /* HEADER_FUNCIONESUTILES_H_ */
