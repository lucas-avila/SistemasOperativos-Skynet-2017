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
char* almacenar_Bytes_de_Pagina(char* PID, char* pagina, char* byteInicial, char* longitud, int texto);
char* asignar_Paginas_Programa(char* PID, char* cantidad_paginas);
int tamanio_pagina_memoria;
#endif /* HEADER_INTERFAZMEMORIA_H_ */
