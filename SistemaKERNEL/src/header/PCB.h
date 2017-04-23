#ifndef HEADER_PCB_H_
#define HEADER_PCB_H_

typedef struct{
   int pid;
   unsigned int cantidad_paginas;
} PCB;

PCB * crear_pcb();

#endif /* HEADER_PCB_H_ */
