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
		char contenido[1000];
		do {
			printf("Testing --> Tiene efecto de lado\n");
			printf("\n1- Crear archivo");
			printf("\n2- Leer archivo");
			printf("\n3- Mostrar bitmap");
			printf("\n4- Wipe data");
			printf("\n5- Borrar Archivo.");
			printf("\n6- Guardar datos.");
			printf("\n7- Test lectura de bloques.");
			printf("\n0- Salir\n");
			printf("Opcion: \n");
			opcion = validarNumeroInput(0, 7);

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
			case 4:{
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
			}
				break;
			case 5:{
				printf("Ingrese el nombre del archivo: ");
				scanf("%s", path);
				borrar(path);
			}
				break;
			case 6:{
				int* offset = malloc(sizeof(int));
				int* size = malloc(sizeof(int));
				printf("Ingrese el nombre del archivo: ");
				scanf("%s", path);
				printf("Ingrese el offset en bytes desde donde escribir: ");
				scanf("%d", offset);
				printf("Ingrese el size en bytes a escribir: ");
				scanf("%d", size);
				printf("Ingrese el contenido del archivo (test max 1000): ");
				scanf("%s", contenido);
				guardar_datos(path, *offset, *size, contenido);
				/*
				FILE * f = fopen("/home/utnso/workspace/tp-2017-1c-Skynet/SistemaFILESYSTEM/pruebita.txt", "r+");
				if(f == NULL)
					perror("NO EXISTE");
				char * buff = read_Archivo(f);
				guardar_datos(path, *offset, *size, buff);
				fclose(f);*/
				free(offset);
				free(size);
			}
			break;
			case 7:{
				char * path = generar_path_absoluto(PATH_ARCHIVOS, "pepe.bin");
				t_config * config = config_create(path);
				char ** bloques = config_get_array_value(config, "BLOQUES");
				obtener_bloques_de_config(bloques, 2);
			}
			break;
			}
		} while (opcion != 0);

		exit(0);

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


