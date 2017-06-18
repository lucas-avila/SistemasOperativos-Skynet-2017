/*
 * FCB.c
 *
 *  Created on: 14/6/2017
 *      Author: utnso
 */

#include "header/Archivo.h"
#include <stdint.h>
#include <stdlib.h>



Archivo * new_Archivo(uint32_t tamanio){
	Archivo * archivo = malloc(sizeof(Archivo));
	archivo->tamanio = 0;
	archivo->bloques[0] = malloc(sizeof(uint32_t));
	return archivo;
}
