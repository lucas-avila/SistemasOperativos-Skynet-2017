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


