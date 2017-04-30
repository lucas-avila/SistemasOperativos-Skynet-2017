/*
 * InterfazMemoria.h
 *
 *  Created on: 1/4/2017
 *      Author: utnso
 */

#ifndef HEADER_INTERFAZMEMORIA_H_
#define HEADER_INTERFAZMEMORIA_H_

void iniciar_conexion_servidor_memoria();

char* solicitar_bytes_memoria(char* PID, char* pagina, char* byteInicial, char* longitud);

char* almacenar_Bytes_de_Pagina(char* PID, char* pagina, char* byteInicial, char* longitud, char* texto);

char* inicializar_Programa_memoria(char* PID, char* cantidad_paginas);

char* asignar_Paginas_Programa(char* PID, char* cantidad_paginas);

char* finalizar_Programa_memoria(char* PID);



int enviar_programa_memoria(char * path_programa, int pid);

int is_valid_line(char* line);

#endif /* HEADER_INTERFAZMEMORIA_H_ */
