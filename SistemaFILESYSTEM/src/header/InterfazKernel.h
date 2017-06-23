/*
 * InterfazKernel.h
 *
 *  Created on: 1/4/2017
 *      Author: utnso
 */

#ifndef HEADER_INTERFAZKERNEL_H_
#define HEADER_INTERFAZKERNEL_H_

int servidor;

char * bloques_a_chars(int * bloques, int size);
void crear_archivo(char* path);
void obtener_datos(char * path, int offset, int size);

#endif /* HEADER_INTERFAZKERNEL_H_ */
