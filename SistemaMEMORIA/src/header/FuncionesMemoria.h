/*
 * FuncionesMemoria.h
 *
 *  Created on: 6/4/2017
 *      Author: utnso
 */
#define TAMANIO 4
#define VACIO -1

#ifndef HEADER_FUNCIONESMEMORIA_H_
#define HEADER_FUNCIONESMEMORIA_H_

typedef struct {
	char frame[TAMANIO];
	char PID[TAMANIO];
	char pagina[TAMANIO];
} Tabla_Pagina_Invertida;

struct t_list *tablaPaginasInvertidas = NULL;


Tabla_Pagina_Invertida* buscar_pagina_disponible();
int indiceInicialPaginas = 0; /*los bloques de administracion ocupan los primeros n bloques de la memoria, este almacena la cantidad total que ocupa esa estructura administrativa*/

char* inicializar_programa(char* PID, int cantidad_paginas_requeridas);

char* solicitar_bytes_de_una_pagina(char* PID, int pagina, int byteInicial, int longitud);

void almacenar_bytes_de_una_pagina(char* PID, int pagina, int byteInicial, int longitud, char*contenido);

char* asignar_paginas_a_proceso(char *PID, int cantidad_paginas_requeridas);

void finalizar_programa(char *PID);

void actualizar_tabla_pagina( Tabla_Pagina_Invertida *registro );

#endif /* HEADER_FUNCIONESMEMORIA_H_ */