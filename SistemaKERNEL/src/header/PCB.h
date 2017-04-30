#ifndef HEADER_PCB_H_
#define HEADER_PCB_H_

#include "parser/metadata_program.h"

typedef struct{
   int pid;
   unsigned int cantidad_paginas;
   t_metadata_program * info_codigo;
} PCB;

int enviar_pcb(PCB * pcb, int s_destino);
PCB * recibir_pcb(int s_origen);
PCB * crear_pcb();


#endif /* HEADER_PCB_H_ */
