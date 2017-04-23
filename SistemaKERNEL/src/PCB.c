#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <commons/collections/list.h>

#include "header/PCB.h"

#define MIN_PIDS 1000

int pids_reg = MIN_PIDS;

PCB * crear_pcb(){
	PCB * pcb = malloc(sizeof(PCB));
	pcb->pid = pids_reg++; //pid se puede pasar a string con string_itoa de las commons
	pcb->cantidad_paginas = 0;

	return pcb;
}

