/*
 * funcionesUtiles.h
 *
 *  Created on: 1/4/2017
 *      Author: utnso
 */

#ifndef HEADER_FUNCIONESUTILES_H_
#define HEADER_FUNCIONESUTILES_H_

#include <stdint.h>

char * info_log;

int validarNumeroInput(int rangoMinimo, int rangoMaximo) ;
int validarArchivo(char * path);
char * obtener_codigo(char * path_archivo_fuente);
int pertenece_a_la_consola(uint32_t pid);
void incializar_log();
void generar_log();
char * diferencia_entre_tiempos(char * tiempo_A, char * tiempo_B);

#endif /* HEADER_FUNCIONESUTILES_H_ */
