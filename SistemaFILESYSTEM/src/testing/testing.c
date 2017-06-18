/*
 * testing.c
 *
 *  Created on: 17/6/2017
 *      Author: utnso
 */

#include <commons/bitarray.h>
#include <stdio.h>
#include <stdlib.h>

#include "testing.h"
#include "../header/FileManager.h"
#include "../header/InterfazKernel.h"

void mostrar_menu_testing(){
	int opcion = -1;
		char path[100];
		do {
			printf("Testing --> Tiene efecto de lado\n");
			printf("\n1- Crear archivo");
			printf("\n2- Leer archivo");
			printf("\n3- Mostrar bitmap");
			printf("\n4- Wipe data");
			printf("\n0- Salir\n");
			printf("Opcion: \n");
			opcion = validarNumeroInput(0, 4);

			switch (opcion) {
			case 1:
				printf("Ingrese el nombre del archivo: ");
				scanf("%s", path);
				system("clear");
				crear_archivo(path);
				break;
			case 2: {
				int* offset = malloc(sizeof(int));
				int* size = malloc(sizeof(int));
				printf("Ingrese el nombre del archivo: ");
				scanf("%s", path);
				printf("Ingrese el offset en bytes desde donde leer: ");
				scanf("%d", offset);
				printf("Ingrese el size en bytes a leer: ");
				scanf("%d", size);
				obtener_datos(path, *offset, *size);
				free(offset);
				free(size);
			}
				break;
			case 3:{
				system("clear");
				printf("El bitmap actual es: \n");
				mostrar_bitmap();
				printf("\n");
			}
				break;
			case 4:
				system("clear");
				int* block_size = malloc(sizeof(int));
				int* block_cant = malloc(sizeof(int));
				printf("Ingrese el tamaño de bloque: ");
				scanf("%d", block_size);
				printf("Ingrese la cantidad de bloques: ");
				scanf("%d", block_cant);
				printf("Borrando todo y creando nuevamente...\n");
				wipe_data(*block_size, *block_cant);
				free(block_size);
				free(block_cant);
				break;
			}
		} while (opcion != 0);
}

void mostrar_bitmap(){
	int i = 0;
	int size = bitarray_get_max_bit(bitmap);
	while(i < size){
		if(bitarray_test_bit(bitmap, i))
			printf("1|");
		else
			printf("0|");
		i++;
	}
}

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


