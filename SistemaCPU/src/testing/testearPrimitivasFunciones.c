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

#include "../procesador/Ejecucion.h"

PCB* procesoPrueba;
IndiceCodigo* crearIndiceCodigo(int programCounter, int byteInicial, int tamanio, int pagina) {
	IndiceCodigo* indice1 = malloc(sizeof(IndiceCodigo));
	indice1->byte_inicial_codigo = byteInicial;
	indice1->byte_final_codigo = byteInicial + tamanio;
	indice1->pagina = pagina;
	indice1->program_counter = programCounter;
	return indice1;
}

void crear_PCB_TEST() {
	procesoPrueba = malloc(sizeof(PCB));
	procesoPrueba->PID=1241;
	procesoPrueba->program_counter = 0;
	procesoPrueba->cantidad_paginas_codigo = 1;

	/** procesoPrueba->pila = list_create();

	IndiceStack* filaInicial = malloc(sizeof(IndiceStack));
	filaInicial->argumentos = list_create();
	filaInicial->variables = list_create();
	filaInicial->posicion = 0;
	list_add(procesoPrueba->pila, filaInicial);
	procesoPrueba->pagina_inicial_stack = atoi(asignar_Paginas_Programa(procesoPrueba->PID, "1"));

	procesoPrueba->RR = 0;


	int paginaSentencia = atoi(asignar_Paginas_Programa(procesoPrueba->PID, "1"));
	char sentencias[][40] = { "variables a, b, x,d,e,f,g,h,i,j,k,l", "a = 3", "b = 5", "a = b + 12", "alocar x 50 ", "liberar x", "alocar d 50 ", "liberar d", "alocar e 50 ", "liberar e", "alocar f 50 ", "liberar f", "alocar g 50 ", "liberar g", "alocar h 50 ", "liberar h", "alocar i 50 ", "liberar i", "alocar j 50 ", "liberar j", "alocar k 50 ", "liberar k", "alocar l 50 ", "liberar l" };/**

	int cantidadSentencias = 11;
	int i = 0;
	int indiceInicial = 0;
	procesoPrueba->codigo = list_create();
	for (i = 0; i < cantidadSentencias; i++) {
		IndiceCodigo* indiceNuevo = crearIndiceCodigo(i, indiceInicial, strlen(sentencias[i]), paginaSentencia);
		indiceInicial = indiceInicial + strlen(sentencias[i]);
		list_add(procesoPrueba->codigo, indiceNuevo);

		almacenar_Bytes_de_Pagina(procesoPrueba->PID, string_itoa(indiceNuevo->pagina), string_itoa(indiceNuevo->byte_inicial_codigo), string_itoa(indiceNuevo->byte_final_codigo - indiceNuevo->byte_inicial_codigo), sentencias[i]);
	}
*/
}

void casos_multiples_primitivas();

void mostrar_menu_primitivas() {
	char nombreVariable[20];
	int valor;
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

		printf("\n 9 - Probar ANALIZADOR SINTACTICO");

		printf("\n 10 - Probar Asginar Compartida");
		printf("\n 11 - Probar Buscar Compartida");

		printf("\n 20 - Salir");
		printf("\n Opcion: ");
		opcion_Salir = validarNumeroInput(0, 20);
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
		case 9:
			setPCBEjecucion(procesoPrueba);
			ejecutar_Programa();
			break;

		case 10:

			printf("\n Nombre Variable: ");
			do {
				scanf("%s", &nombre);
			} while (nombre == '\n');
			printf("\n Valor: ");
			valor = validarNumeroInput(0, 999999);
			ASIGNAR_VALOR_COMPARTIDA(nombre, valor);
			break;
		case 11:


			printf("\n Nombre Variable: ");
			do {
				scanf("%s", &nombre);
			} while (nombre == '\n');
			OBTENER_VALOR_COMPARTIDA(nombreVariable);
			break;
		}

	} while (opcion_Salir != 20);
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
