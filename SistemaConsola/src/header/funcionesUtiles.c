/*
 * funcionesUtiles.c
 *
 *  Created on: 1/4/2017
 *      Author: utnso
 */
#include <unistd.h>

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

void validarArchivo(char * path){

	if(access(path, F_OK) == -1) {
			perror("Archivo Inexistente.");
			exit(-2);
		}
}
