/*
 * FuncionesMemoria.h
 *
 *  Created on: 6/4/2017
 *      Author: utnso
 */
#define TAMANIO 5
#define VACIO "NNNN"
#include <stdbool.h>
#ifndef HEADER_FUNCIONESMEMORIA_H_
#define HEADER_FUNCIONESMEMORIA_H_

typedef struct {
	char frame[TAMANIO];
	char PID[TAMANIO];
	char pagina[TAMANIO];

} Tabla_Pagina_Invertida;

int buscar_frame();

char* inicializar_programa(char* PID, int cantidad_paginas_requeridas);

char* solicitar_bytes_de_una_pagina(char* PID, int pagina, int byteInicial, int longitud);

char* almacenar_bytes_de_una_pagina(char PID[4], int pagina, int byteInicial, int longitud, char* contenido, bool cacheIr );

char* asignar_paginas_a_proceso(char *PID, int cantidad_paginas_requeridas);

char* liberar_pagina(char PID[4], int pagina);

void finalizar_programa(char *PID);

void actualizar_tabla_pagina(Tabla_Pagina_Invertida registro);

void generar_Reporte_Estructura_de_Memoria();

void reservar_memoria_principal();

void actualizar_informacion_memoria_principal(Tabla_Pagina_Invertida registro, int indiceRegistro);

void mostrar_reporte_proceso();

void mostrar_reporte_memoria();

Tabla_Pagina_Invertida busqueda_secuencial(int frame_start, int frame_finish, char * comparar_con);

Tabla_Pagina_Invertida evitar_colisiones(int frame);

/***
 * FUNCION LOOKUP
 */
int frame_lookup(char*PID, int pagina);


#endif /* HEADER_FUNCIONESMEMORIA_H_ */
