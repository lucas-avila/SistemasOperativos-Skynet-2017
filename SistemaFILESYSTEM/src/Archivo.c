/*
 * FCB.c
 *
 *  Created on: 14/6/2017
 *      Author: utnso
 */

#include "header/Archivo.h"
#include <stdint.h>
#include <stdlib.h>



Archivo * crear_Archivo(uint32_t tamanio){
	Archivo * archivo = malloc(sizeof(Archivo));
	archivo->tamanio = 0;
	archivo->bloques[0] = malloc(sizeof(uint32_t));
	archivo->bloques[0] = obtener_BLOQUE_libre();
	return archivo;
}
