
#include <ctype.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

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

int get_line(char * text, char * line, int save_cursor){
	//esta funcion (horripilante, mil perdones) sería como un fgets(lee linea por linea) pero para un string
	static int pos = 0;
	int i = 0;
	while(text[pos] != '\0'){
		if(text[pos] == '\n'){
			if(save_cursor)
				pos++; //salteamos el \n
			else
				pos = 0;
			line[i] = '\0';
			return 1;
		}

		line[i] = text[pos];
		i++;
		pos++;
		if(text[pos] == '\0'){
			line[i] = '\0';
			return 1; //necesito que haga una iteracion mas para poder leer la ultima linea en un while
		}
	}
	pos = 0;
	line[i] = '\0';
	return 0;
}

char * obtener_codigo(char * path_archivo_fuente){
	FILE * archivo_fuente;
	char * literal;
	int size_buffer;

	archivo_fuente = fopen(path_archivo_fuente, "r");

	// Ahora necesitamos saber el tamaño del archivo.
	fseek(archivo_fuente, 0, SEEK_END);
	size_buffer = ftell(archivo_fuente);
	fseek(archivo_fuente, 0, SEEK_SET);

	//Para evitar segmentation fault allocamos memoria.
	literal = malloc(sizeof(char) * size_buffer);
	fread(literal, sizeof(char), size_buffer, archivo_fuente);

	return literal;
}
