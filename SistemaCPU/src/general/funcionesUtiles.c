#include <stdio.h>
#include <stdlib.h>
#include<commons/log.h>
#include<commons/string.h>
#include "../header/AppConfig.h"

int validarNumeroInput(int rangoMinimo, int rangoMaximo) {
	int numero = 0;
	do {
		scanf("%d", &numero);
		if ((numero < rangoMinimo || numero > rangoMaximo)) {
			printf("\n Numero invalido, por favor ingrese otro: ");
		}
	} while (numero < rangoMinimo || numero > rangoMaximo);
	return numero;
}

void logSO(char* mensajeLoguear) {

	/*va_list pa;

	va_start(pa, mensajeLoguear);
	char* mensaje = string_from_format(mensajeLoguear, pa);
	va_end(pa); */

	t_log* logger = log_create(configuraciones.PATH_ARCHIVO_LOG, "CPU", true, LOG_LEVEL_INFO);
	log_info(logger, "%s", mensajeLoguear);
	//free(mensajeLoguear);
	log_destroy(logger);
}
