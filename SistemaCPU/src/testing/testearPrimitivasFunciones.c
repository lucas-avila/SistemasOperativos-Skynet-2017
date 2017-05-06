#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include "commons/collections/list.h"

#include "../primitivas/PrimitivasFunciones.h"
#include "../general/funcionesUtiles.h"
#include "../interfaz/InterfazMemoria.h"
#include "../header/PCB.h"

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
	list_add(procesoPrueba->pila, filaInicial);
	procesoPrueba->pagina_inicial_stack = atoi(asignar_Paginas_Programa(procesoPrueba->PID, "1"));

}

void casos_multiples_primitivas();

void mostrar_menu_primitivas() {
	char nombre;
	crear_PCB_TEST();
	setearPCB(procesoPrueba);
	int opcion_Salir = 0;
	do {
		printf("\n 1 - Probar DECLARAR VARIABLE");
		printf("\n 2 - Probar OBTENER_DIRECCION_DE_VARIABLE");
		printf("\n 3 - Probar ASIGNAR_VARIABLE");
		printf("\n 4 - Probar CASOS MULTIPLES (ANTERIORES)");
		printf("\n 7 - Salir");
		printf("\n Opcion: ");
		opcion_Salir = validarNumeroInput(0, 7);
		switch (opcion_Salir) {
		case 1:
			printf("\n Nombre Variable: ");
			fflush(stdin);
			scanf("%c", &nombre);
			printf("\n Puntero Variable: %l", DEFINIR_VARIABLE(nombre));
			break;
		case 2:
			//OBTENER_DIRECCION_DE_VARIABLE('a');
			break;
		case 3:
			printf("\n Nombre Variable: ");
			fflush(stdin);
			scanf("%c", &nombre);
			printf("\n Valor: ");
			int valor = validarNumeroInput(0, 9999);
			ASIGNAR_VARIABLE(nombre, string_itoa(valor));
			break;
		case 4:
			casos_multiples_primitivas();
			break;

		}
	} while (opcion_Salir != 7);
}

void casos_multiples_primitivas() {
	char variables[] = { 'a', 'b', 'c', 'd', 'e', 'f' };
	int numeros[] = { 10, 20, 30, 400, 5000, 6000 };

//	DireccionVariable* dirVar;
	int i = 0;
	for (i=0; i < 6; i++) {
		DEFINIR_VARIABLE(variables[i]);
		t_puntero ee = OBTENER_DIRECCION_DE_VARIABLE(variables[i]);
		ASIGNAR_VARIABLE(ee,  numeros[i]);
	}
}
