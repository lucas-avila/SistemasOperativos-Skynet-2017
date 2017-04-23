/*
 * funcionesUtiles.c
 *
 *  Created on: 1/4/2017
 *      Author: utnso
 */


#include <ctype.h>

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

int is_white_line(char * str){
	int i=0;
	while(str[i] != '\0'){
		if(!isspace(str[i]))
			return 0;
		i++;
	}
	return 1;
}

char first_char(char * str){
	int i=0;
	while(str[i] != '\0'){
		if(!isspace(str[i]))
			return str[i];
		i++;
	}
	return '\0';
}
