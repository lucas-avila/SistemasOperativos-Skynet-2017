#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <commons/collections/list.h>
#include <commons/string.h>

#include "header/PCB.h"

#define MIN_PIDS 1000

int pids_reg = MIN_PIDS;

PCB * crear_pcb(){
	PCB * pcb = malloc(sizeof(PCB));
	pcb->pid = pids_reg++; //pid se puede pasar a string con string_itoa de las commons
	pcb->cantidad_paginas = 0;

	int status = enviar_pcb(pcb, 0);
	return pcb;
}

int enviar_pcb(PCB * pcb, int s_destino){
	//Se envia el PCB descomponiendo el struct en string y enviandolo por paquetes
	enviar_dato_serializado("ENVIAR_PCB", s_destino);
	char * respuesta = recibir_dato_serializado(s_destino);

	if(strcmp(respuesta, "RECIBIR_PCB") != 0) return -1;

	char * pid = string_itoa(pcb->pid);
	enviar_dato_serializado(pid, s_destino);
	char * cantidad_paginas_codigo = string_itoa(pcb->cantidad_paginas);
	enviar_dato_serializado(pid, s_destino);
}

