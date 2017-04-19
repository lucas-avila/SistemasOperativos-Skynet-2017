#include "PruebasUnitarias.h"

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "../header/MemoriaCache.h"
#include "../header/AppConfig.h"
#include "../general/funcionesUtiles.h"
#include "../general/Socket.h"

#include "../header/MemoriaPrincipal.h"

#include <pthread.h>
void pruebas_concurrencia();

void pruebas_Basicas();
void proceso_1241();

void asignarPaginas_Test();
void almacenarPaginas_Test();
void solicitarBytesPagina_Test();
void finalizarProceso_Test();
void testIntegrador_1();

void menu_pruebas() {
	int opcion = 0;
	do {

		printf("\nMenu de Pruebas");
		printf("\n1 - Asignar Paginas a Proceso");
		printf("\n2 - Almacenar Bytes en Pagina");
		printf("\n3 - Solicitar Bytes de Pagina");
		printf("\n4 - Finalizar proceso (elimina paginas asignadas)");
		printf("\n5 - Test Integrador 1 - Prueba de Concurrencia");
		printf("\n6 - Salir del Menu de Pruebas");
		printf("\nOpcion: ");
		opcion = validarNumeroInput(1, 6);

		switch (opcion) {
		case 1:
			asignarPaginas_Test();
			break;
		case 2:
			almacenarPaginas_Test();
			break;
		case 3:
			solicitarBytesPagina_Test();
			break;
		case 4:
			finalizarProceso_Test();
			break;
		case 5:
			testIntegrador_1();
			break;
		}

	} while (opcion != 6);
}

void pruebas_concurrencia() {
	atender_clientes(0, &pruebas_Basicas);
	atender_clientes(0, &proceso_1241);
}

void proceso_1241() {
	asignar_paginas_a_proceso("1241", 5);
	printf("\nTest 1 - Prueba Asignar 5 paginas a proceso 1241");
	almacenar_bytes_de_una_pagina("1241", 0, 0, strlen("00000printf('Hola Mundo');00000a=a+1;00000printf('Resultado de a es' + a);00000"), "00000printf('Hola Mundo');00000a=a+1;00000printf('Resultado de a es' + a);00000");
	printf("\nTest 1 - Escribir pagina 1 proceso 1241");
	almacenar_bytes_de_una_pagina("1241", 1, 0, strlen("00000printf('Hola Mundo');00000a=a+1;00000printf('Resultado de a es' + a);00000"), "00000printf('Hola Mundo');00000a=a+1;00000printf('Resultado de a es' + a);00000");
	printf("\nTest 1 - Escribir pagina 2 proceso 1241");
	almacenar_bytes_de_una_pagina("1241", 2, 0, strlen("00000printf('Hola Mundo');00000a=a+1;00000printf('Resultado de a es' + a);00000"), "00000printf('Hola Mundo');00000a=a+1;00000printf('Resultado de a es' + a);00000");
	printf("\nTest 1 - Escribir pagina 3 proceso 1241");
	almacenar_bytes_de_una_pagina("1241", 3, 0, strlen("00000printf('Hola Mundo');00000a=a+1;00000printf('Resultado de a es' + a);00000"), "00000printf('Hola Mundo');00000a=a+1;00000printf('Resultado de a es' + a);00000");
	printf("\nTest 1 - Escribir pagina 4 proceso 1241");
	almacenar_bytes_de_una_pagina("1241", 4, 0, strlen("00000printf('Hola Mundo');00000a=a+1;00000printf('Resultado de a es' + a);00000"), "00000printf('Hola Mundo');00000a=a+1;00000printf('Resultado de a es' + a);00000");

	//int i = 0;
	//for (i = 0; i < 20; i++) {
	//	printf("\n Contenido pagina: %s", solicitar_bytes_de_una_pagina("1241", 4, 0, 10 + i));
	//}
	//printf("\nContenido PID: 1241 - pagina 5 - inicio 0 longitud 10: %s ", solicitar_bytes_de_una_pagina("1241", 0, 0, 512));

	//mostrar_tabla_memoria_cache();
	MEMORIA_CACHE fila;
	strcpy(fila.PID, "1241");
	fila.contenidoPagina = malloc(configuraciones.MARCO_SIZE);
	fila.nroPagina = 20;

	strcpy(fila.contenidoPagina, "HOLA MUNDO");
	reemplazar_linea_aplicando_algoritmo(fila);
	printf("\nTest 1 - Prueba memoria cache - algoritmo LRU");

	//mostrar_tabla_memoria_cache();
	printf("\nFInalize test 1");
}

void pruebas_Basicas() {

	asignar_paginas_a_proceso("1241", 5);
	printf("\nTest 2 - Prueba Asignar 5 paginas a proceso 1241");

	almacenar_bytes_de_una_pagina("1241", 1, 0, 10, "jonathan10");
	printf("\nTest 2 - Contenido PID: 1241 - pagina 1 - inicio 0 longitud 10: %s ", solicitar_bytes_de_una_pagina("1241", 1, 0, 10));

	almacenar_bytes_de_una_pagina("1241", 3, 0, 10, "jonathan10");
	printf("\nTest 2 - Contenido PID: 1241 - pagina 3 - inicio 0 longitud 10: %s ", solicitar_bytes_de_una_pagina("1241", 3, 0, 10));

	almacenar_bytes_de_una_pagina("1241", 5, 0, 10, "jonathan10");
	printf("\nTest 2 - Contenido PID: 1241 - pagina 5 - inicio 0 longitud 10: %s ", solicitar_bytes_de_una_pagina("1241", 5, 0, 10));

	asignar_paginas_a_proceso("1371", 3);
	almacenar_bytes_de_una_pagina("1371", 1, 0, 10, "jonathan20");
	almacenar_bytes_de_una_pagina("1371", 2, 0, 10, "jonathan20");

	asignar_paginas_a_proceso("1241", 2);
	almacenar_bytes_de_una_pagina("1241", 6, 0, 10, "jonathan10");

	finalizar_programa("1241");
	inicializar_programa("1189", 9);

	inicializar_programa("1858", 10);
	finalizar_programa("1858");

	inicializar_programa("1858", 2);
	inicializar_programa("1859", 2);
	inicializar_programa("1860", 2);

	finalizar_programa("1859");
	printf("\nFInalize test 2");
}

void asignarPaginas_Test() {
	char proceso[4];
	int cantidadPaginas;
	printf("\n Menu: ");
	printf("\n Proceso: ");
	scanf("%*c%[^\n]s", proceso);
	printf("\n Cantidad Paginas: ");
	scanf("%d", &cantidadPaginas);
	printf("\n Resultado: %s", asignar_paginas_a_proceso(proceso, cantidadPaginas));
}
void almacenarPaginas_Test() {
	char proceso[4];
	int pagina;
	int byteInicial;
	char* contenido = malloc(strlen("printf('Hola como estas tanto tiempo jony');")+1);
	strcpy(contenido, "printf('Hola como estas tanto tiempo jony');");
	printf("\n Proceso: ");
	scanf("%s", proceso);

	printf("\n Pagina: ");
	scanf("%d", &pagina);
	printf("\n Byte Inicial: ");
	scanf("%d", &byteInicial);
	almacenar_bytes_de_una_pagina(proceso, pagina, byteInicial, strlen(contenido), contenido);
}
void solicitarBytesPagina_Test() {
	char proceso[4];
	int pagina;
	int byteInicial;
	int longitud;
	printf("\n Proceso: ");
	scanf("%*c%[^\n]s", proceso);
	printf("\n Pagina: ");
	scanf("%d", &pagina);
	printf("\n Byte Inicial: ");
	scanf("%d", &byteInicial);
	printf("\n Longitud Solicitada: ");
	scanf("%d", &longitud);
	printf("\n Resultado: %s" , solicitar_bytes_de_una_pagina(proceso, pagina, byteInicial, longitud));
}
void finalizarProceso_Test() {
	char proceso[4];
	printf("\n Proceso: ");
	scanf("%*c%[^\n]s", proceso);
	finalizar_programa(proceso);
}
void testIntegrador_1() {
	atender_clientes(0, &pruebas_concurrencia);
}
