/*
 * InterfazKernel.c
 *
 *  Created on: 1/4/2017
 *      Author: utnso
 */

#include <stdlib.h>
#include "header/InterfazKernel.h"
#include "header/Socket.h"

PCB * recibir_pcb(int s_origen){
	PCB pcb;
	enviar_dato_serializado("ENVIAR_PCB", s_origen);

	int pid = atoi(recibir_dato_serializado(s_origen));
	int cantidad_paginas_codigo = atoi(recibir_dato_serializado(s_origen));


	//pid
	pcb.pid = pid;
	//cantidad paginas codigo
	pcb.cantidad_paginas = cantidad_paginas_codigo;

	return &pcb;
}
