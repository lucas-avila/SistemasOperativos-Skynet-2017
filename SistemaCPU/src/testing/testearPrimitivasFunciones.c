#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include "commons/collections/list.h"

#include "../primitivas/PrimitivasFunciones.h"
#include "../general/funcionesUtiles.h"
#include "../interfaz/InterfazMemoria.h"
#include "../header/PCB.h"
#include "../primitivas/EstructurasDeDatosPrimitivas.h"
#include "../primitivas/FuncionesAuxiliares.h"
PCB* procesoPrueba;

void crear_PCB_TEST() {
	procesoPrueba = malloc(sizeof(PCB));
	strcpy(procesoPrueba->PID, "1241");
	procesoPrueba->program_counter = 0;
	procesoPrueba->cantidad_paginas_codigo = 1;

	procesoPrueba->pila = list_create();

	IndiceStack* filaInicial = malloc(sizeof(IndiceStack));
	filaInicial->argumentos = list_create();
	filaInicial->variables = list_create();
	filaInicial->posicion = 0;
	list_add(procesoPrueba->pila, filaInicial);
	procesoPrueba->pagina_inicial_stack = atoi(asignar_Paginas_Programa(procesoPrueba->PID, "1"));

}

void casos_multiples_primitivas();

void mostrar_menu_primitivas() {
	char nombre;
	int punterosAlocar[3];
	crear_PCB_TEST();
	setearPCB(procesoPrueba);
	int opcion_Salir = 0;
	do {
		printf("\n 1 - Probar DECLARAR VARIABLE");
		printf("\n 2 - Probar OBTENER_DIRECCION_DE_VARIABLE");
		printf("\n 3 - Probar ASIGNAR_VARIABLE");
		printf("\n 4 - Probar DEFERENCIAR VARIABLE");
		printf("\n 5 - Probar MALLOC");
		printf("\n 6 - Probar FREE");
		printf("\n 7 - Probar CASOS MULTIPLES (ANTERIORES)");

		printf("\n 8 - Probar FRAGMENTACION INTERNA (MALLOC Y FREE)");
		printf("\n 9 - Salir");
		printf("\n Opcion: ");
		opcion_Salir = validarNumeroInput(0, 9);
		switch (opcion_Salir) {
		case 1:
			printf("\n Nombre Variable: ");
			do {
				scanf("%c", &nombre);
			} while (nombre == '\n');

			printf("\n Puntero Variable: %d", DEFINIR_VARIABLE(nombre));
			break;
		case 2:
			printf("\n Nombre Variable: ");
			do {
				scanf("%c", &nombre);
			} while (nombre == '\n');
			printf("\n Puntero Variable %c: %d", nombre, OBTENER_DIRECCION_DE_VARIABLE(nombre));
			break;
		case 3:
			printf("\n Nombre Variable: ");

			do {
				scanf("%c", &nombre);
			} while (nombre == '\n');
			printf("\n Valor: ");
			int valor = validarNumeroInput(0, 9999);
			ASIGNAR_VARIABLE(OBTENER_DIRECCION_DE_VARIABLE(nombre), valor);
			printf("\n Valor Asignado. ");
			break;
		case 4:
			printf("\n Nombre Variable: ");

			do {
				scanf("%c", &nombre);
			} while (nombre == '\n');
			printf("\n Valor de Variable %c: %d", nombre, DEREFERENCIAR(OBTENER_DIRECCION_DE_VARIABLE(nombre)));
			break;

		case 5:
			printf("\n Cantidad de Bytes Reservar: ");
			valor = validarNumeroInput(0, 9999);
			printf("\n Puntero %d", ALOCAR(valor));
			break;
		case 6:
			printf("\n Puntero Liberar: ");
			valor = validarNumeroInput(0, 99999999);
			LIBERAR(valor);
			break;
		case 8:

			punterosAlocar[0] = ALOCAR(10);
			punterosAlocar[1] = ALOCAR(20);
			punterosAlocar[2] = ALOCAR(30);

			DireccionMemoriaDinamica* dir = deserializarMemoriaDinamica(procesoPrueba->PID, punterosAlocar[0]);
			printf("\n\nMemoria Despues del MALLOC: \n %s", solicitar_bytes_memoria(procesoPrueba->PID, string_itoa(dir->pagina), string_itoa(dir->byteInicial), "512"));

			LIBERAR(punterosAlocar[0]);
			LIBERAR(punterosAlocar[1]);
			printf("\n Probando Fragmentacion... ");
			sleep(5);
			printf("\n Fragmentacion Realizada ");
			printf("\n\nMemoria Despues del FREE: \n %s", solicitar_bytes_memoria(procesoPrueba->PID, string_itoa(dir->pagina), string_itoa(dir->byteInicial), "512"));


			break;

		case 7:
			casos_multiples_primitivas();
			break;

		}
	} while (opcion_Salir != 9);
}

void casos_multiples_primitivas() {
	char variables[] = { 'a', 'b', 'c', 'd', 'e', 'f' };
	int numeros[] = { 10, 20, 30, 400, 5000, 6000 };
	int punteroA = ALOCAR(50);
	printf("\n Puntero A: %d ", punteroA);
	int punteroB = ALOCAR(20);
	printf("\n Puntero B: %d ", punteroB);
	int punteroC = ALOCAR(10);
	printf("\n Puntero C: %d ", punteroC);
	LIBERAR(punteroA);
//	DireccionVariable* dirVar;
	int i = 0;
	for (i = 0; i < 6; i++) {
		DEFINIR_VARIABLE(variables[i]);
		t_puntero ee = OBTENER_DIRECCION_DE_VARIABLE(variables[i]);
		ASIGNAR_VARIABLE(ee, numeros[i]);
		printf("\n Valor de Variable %c en MEMORIA es de %d", variables[i], DEREFERENCIAR(ee));
	}

}
