#include <stdio.h>
#include <stdlib.h>

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
	printf("\n %s",mensajeLoguear);


	//t_log* logger = log_create(configuraciones.PATH_ARCHIVO_LOG, "MEMORIA", true, LOG_LEVEL_INFO);
	//log_info(logger, "%s", mensajeLoguear);

	//log_destroy(logger);
}
