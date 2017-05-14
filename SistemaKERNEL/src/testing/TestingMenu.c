#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "TestingInterfazMemoria.h"
#include "TestingCapaFileSystem.h"

void menu_principal_testing() {
	int opcion = 0;
	do {
		system("clear");
		printf("\n1 - Testear Capa FilyeSYSTEM");
		printf("\n2 - Testear Capa MEMORIA");
		printf("\n3 - Testear PLANIFICACION");
		printf("\n4 - Testear COSAS GENERALES");
		printf("\n5 - Volver al Menu Principal");
		printf("\nOpcion: ");
		opcion = validarNumeroInput(1, 5);
		system("clear");
		switch (opcion) {
		case 1:
			mostrar_menu_testing_filesystem();
			break;
		case 2:
			testear_intefaz();
			break;
		case 3:
			mostrar_estado_colas() ;
			break;
		case 4:
			mostrar_menu_testing_general();
			break;

		}
	} while (opcion != 5);
}
