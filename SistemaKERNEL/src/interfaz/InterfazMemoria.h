/*
 * InterfazMemoria.h
 *
 *  Created on: 1/4/2017
 *      Author: utnso
 */

#ifndef HEADER_INTERFAZMEMORIA_H_
#define HEADER_INTERFAZMEMORIA_H_

#include <parser/metadata_program.h>

#include "../header/PCB.h"

int servidor_Memoria;
int tamanio_pagina_memoria;

void iniciar_conexion_servidor_memoria();

char* solicitar_bytes_memoria(char* PID, char* pagina, char* byteInicial, char* longitud);

char* almacenar_Bytes_de_Pagina(char* PID, char* pagina, char* byteInicial, char* longitud, char* texto);

char* inicializar_Programa_memoria(char* PID, char* cantidad_paginas);

char* asignar_Paginas_Programa(char* PID, char* cantidad_paginas);

char* liberar_pagina_proceso(char* PID, char* numeroPagina);

char* finalizar_Programa_memoria(char* PID);

IndiceCodigo* crear_IndiceCodigo(int programCounter, int byteInicial, int tamanio, int pagina);

int enviar_programa_memoria(t_metadata_program * meta, PCB * pcb, char * programa);

int is_valid_line(char* line);

#endif /* HEADER_INTERFAZMEMORIA_H_ */
