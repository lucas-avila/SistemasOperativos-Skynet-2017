/*
 * FCB.c
 *
 *  Created on: 14/6/2017
 *      Author: utnso
 */

#include "header/Archivo.h"
#include <stdint.h>
#include <stdlib.h>



Archivo * new_Archivo(){
	Archivo * archivo = malloc(sizeof(Archivo));
	archivo->tamanio = 0;
	archivo->bloques = (int *)malloc(sizeof(int));
	archivo->bloques[0] = -1;
	return archivo;
}
