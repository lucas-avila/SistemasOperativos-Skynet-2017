#include "TestingMemoriaCache.h"

#include "../header/MemoriaPrincipal.h"
#include "commons/string.h"

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
void cargar_paginas_de_proceso(char *PID, int cantidadPagina, char* contenidoQueQuieras) {
	int paginaInicial = atoi(asignar_paginas_a_proceso(PID, cantidadPagina));
	int i = 0;
	for (i = 0; i < cantidadPagina; i++) {
		char* contenidoDinamico = malloc(strlen(contenidoQueQuieras) + 5 + 1);
		strcpy(contenidoDinamico, contenidoQueQuieras);
		strcat(contenidoDinamico, " ");
		strcat(contenidoDinamico, string_itoa(i));
		almacenar_bytes_de_una_pagina(PID, string_itoa((i + paginaInicial)), "0", string_itoa(strlen(contenidoDinamico)), contenidoDinamico);
		free(contenidoDinamico);
	}
}

void solicitar_paginas_de_proceso(char *PID, int pagina, int cantidadVecesPedir) {
	int i = 0;
	for (i = 0; i < cantidadVecesPedir; i++) {
		solicitar_bytes_de_una_pagina(PID, pagina, 0, 10);
	}
}
