/*
 * InterfazKernel.h
 *
 *  Created on: 1/4/2017
 *      Author: utnso
 */

#ifndef HEADER_INTERFAZKERNEL_H_
#define HEADER_INTERFAZKERNEL_H_

int servidor;

void validar_archivo(char * path);
void crear_archivo(char* path);
void guardar_datos(char * path, int offset, int size, char * buffer);
void obtener_datos(char * path, int offset, int size);
void borrar(char * path);

char * convertir_bloques_a_array_chars(int * bloques, int size);

#endif /* HEADER_INTERFAZKERNEL_H_ */
