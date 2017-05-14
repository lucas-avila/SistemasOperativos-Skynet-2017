/*
 ============================================================================
 Name        : CPU.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "header/AppConfig.h"
#include "general/Socket.h"

#include "interfaz/InterfazMemoria.h"
#include "interfaz/InterfazKernel.h"
#include "testing/testearPrimitivasFunciones.h"
#include "procesador/Ejecucion.h"

void CU_Procesar_PCB_a_ejecutar(int kernel);



int main(int argc, char *argv[]) {

	//inicializar_configuracion(argv[1]);
	inicializar_configuracion("/home/utnso/Escritorio/tp-2017-1c-Skynet/SistemaCPU/resource/config.cfg");
	controlEjecucionPrograma = false;
	servidor_kernel = conectar_servidor(configuraciones.IP_KERNEL, configuraciones.PUERTO_KERNEL);
	/*
	iniciar_conexion_servidor_memoria();


	inicializar_contexto_ejecucion();*/

	//Parametro de Identificacion
	enviar_dato_serializado("CPU", servidor_kernel);
	bool controlSeguir = true;
	//mostrar_menu_primitivas();
	//atender_clientes(0, mostrar_menu_primitivas);

	char *operacion;
	do {
		operacion = recibir_dato_serializado(servidor_kernel);
		//SEÑAL ENVIADA POR KERNEL PARA SALIR
		if (strcmp(operacion, "SIGUSR1") == 0) {
			while (controlEjecucionPrograma == true) {
			}
			controlSeguir = false;
		} else if (strcmp(operacion, "RECIBIR_PCB") == 0) {
			CU_Procesar_PCB_a_ejecutar(servidor_kernel);
		}
	} while (controlSeguir);

	close(servidor_kernel);
	return EXIT_SUCCESS;
}

void CU_Procesar_PCB_a_ejecutar(int kernel) {

	PCB * pcb = recibir_pcb(kernel);

	printf("El fucking PCB es :\n");
	printf("PID : %d\n", pcb->PID);
	printf("program counter : %d\n", pcb->program_counter);
	printf("cantidad paginas codigo : %d\n", pcb->cantidad_paginas_codigo);

	printf("Lista de indices de codigo:\n\n");
	IndiceCodigo * c = malloc(sizeof(IndiceCodigo));
	int i=0;
	for(i; i < list_size(pcb->codigo); i++){
		c = list_get(pcb->codigo, i);
		printf("Indice %d\n", i);
		printf("--program counter:%d\n--byte inicial:%d\n--byte final:%d\n--pagina:%d\n", c->program_counter, c->byte_inicial_codigo, c->byte_final_codigo, c->pagina);
	}

	printf("cantidad codigo : %d\n", pcb->cantidad_codigo);

	printf("Lista de indices de la pila:\n\n");
	IndiceStack * e = malloc(sizeof(IndiceStack));
	for(i=0; i < list_size(pcb->pila); i++){
		e = list_get(pcb->pila, i);
		printf("Elemento %d del stack:\n posicion: %d\n", i, e->posicion);
		int x=0;
		for(x; x < list_size(e->argumentos); x++){
			Argumento * a = list_get(e->argumentos, x);
			printf("argumentos:\n--id:%d\n--pagina:%d\n--byte_inicial:%d\n--tamanio:%d\n", a->id, a->pagina, a->byte_inicial, a->tamanio);
		}
		for(x=0; x < list_size(e->variables); x++){
			Variable * v = list_get(e->variables, x);
			printf("variables:\n--id:%d\n--pagina:%d\n--byte_inicial:%d\n--tamanio:%d\n--dinamica:%d\n", v->id, v->pagina, v->byte_inicial, v->tamanio, v->dinamica);
		}
		printf("retPos: %d\nretVar:\n--byte_inicial:%d\n--pagina:%d\n--tamanio:%d\n", e->retPos, e->retVar->byte_inicial, e->retVar->pagina, e->retVar->tamanio);
	}

	printf("Indice etiqueta:\n");
	printf("--id funcion: %s\n--nombre etiqueta: %s\n--valor program counter: %d\n", pcb->etiqueta->identificador_funcion, pcb->etiqueta->nombre_etiqueta, pcb->etiqueta->valor_program_counter);

	printf("Cantidad etiqueta: %d\n", pcb->cantidad_etiqueta);
	printf("Exit code: %d\n", pcb->exit_code);
	printf("Pagina inicial de la pila: %d\n", pcb->pagina_inicial_stack);

	printf("Cosas de planificacion\n");
	printf("RR (0->fifo, 1->RR): %d\n", pcb->RR);
	printf("Cantidad de rafagas para RR: %d\n", pcb->cantidad_rafagas);
	printf("Cantidad de rafagas ejecutadas: %d\n", pcb->cantidad_rafagas_ejecutadas);
}

