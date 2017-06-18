/*
 * InterfazKernel.h
 *
 *  Created on: 1/4/2017
 *      Author: utnso
 */

#ifndef HEADER_INTERFAZKERNEL_H_
#define HEADER_INTERFAZKERNEL_H_

int servidor;

void crear_archivo(char* path);
void obtener_datos(char * path, int offset, int size);

#endif /* HEADER_INTERFAZKERNEL_H_ */
