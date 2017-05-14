#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "../capaFILESYSTEM/GestionArchivos.h"

#include "../capaFILESYSTEM/TablaGlobalArchivo.h"
#include "../capaFILESYSTEM/TablaProcesoArchivo.h"
#include "../capaFILESYSTEM/GestionArchivoFuncAux.h"
#include "commons/string.h"
#include "../administrarProcesos/Proceso.h"

#include "../interfaz/InterfazFS.h"
void inicializar_test() {

	PCB* pcb;
	Proceso* proc;

	pcb = crear_pcb();
	strcpy(pcb->PID, "1241");
	proc = new_Proceso(pcb);
	agregar_proceso(proc);

	pcb = crear_pcb();
	strcpy(pcb->PID, "1371");
	proc = new_Proceso(pcb);
	agregar_proceso(proc);

	pcb = crear_pcb();
	strcpy(pcb->PID, "1189");
	proc = new_Proceso(pcb);
	agregar_proceso(proc);

	inicializar();

}
void destruir_test() {
	eliminar_todos_registros();
}

void mostrar_menu_testing_filesystem() {
	inicializar_test();
	int opcion = 0;
	do {
		system("clear");
		printf("\n-----------------------------------");
		printf("\n 1 - Abrir Archivo (MANUAL) ");
		printf("\n 2 - Mover Cursor Archivo (MANUAL) ");
		printf("\n 3 - Leer Archivo (MANUAL) ");
		printf("\n 4 - Escribir Archivo (MANUAL) ");
		printf("\n 5 - Cerrar Archivo (MANUAL) ");
		printf("\n 6 - Borrar Archivo (MANUAL) ");
		printf("\n-----------------------------------");
		printf("\n 7 - Abrir Archivo (AUTOMATICO) ");
		printf("\n 8 - Mover Cursor Archivo (AUTOMATICO) ");
		printf("\n 9 - Leer Archivo (AUTOMATICO) ");
		printf("\n 10 - Escribir Archivo (AUTOMATICO) ");
		printf("\n 11 - Cerrar Archivo (AUTOMATICO) ");
		printf("\n 12 - Borrar Archivo (AUTOMATICO) ");
		printf("\n-----------------------------------");
		printf("\n 13 - Volver al MENU PRINCIPAL ");
		printf("\n-----------------------------------");
		printf("\n 14 - VER TABLA PROCESO ");
		printf("\n-----------------------------------");
		printf("\n Opcion: ");
		opcion = validarNumeroInput(1, 14);
		system("clear");
		switch (opcion) {

		case 1:
			TEST_Manual_CU_ABRIR_ARCHIVO();
			break;
		case 2:
			TEST_Manual_CU_MOVER_CURSOR_ARCHIVO();
			break;
		case 3:
			TEST_Manual_CU_LEER_ARCHIVO();
			break;
		case 4:
			TEST_Manual_CU_ESCRIBIR_ARCHIVO();
			break;
		case 5:
			TEST_Manual_CU_CERRAR_ARCHIVO();
			break;
		case 6:
			TEST_Manual_CU_BORRAR_ARCHIVO();
			break;
		case 7:
			TEST_Auto_CU_ABRIR_ARCHIVO();
			break;
		case 8:
			TEST_Auto_CU_MOVER_CURSOR_ARCHIVO();
			break;
		case 9:
			TEST_Auto_CU_LEER_ARCHIVO();
			break;
		case 10:
			TEST_Auto_CU_ESCRIBIR_ARCHIVO();
			break;
		case 11:
			TEST_Auto_CU_CERRAR_ARCHIVO();
			break;
		case 12:
			TEST_Auto_CU_BORRAR_ARCHIVO();
			break;
		case 14:
			TEST_VER_TABLA_PROCESO();
			break;
		}
		while (getchar() == '\n')
			;
	} while (opcion != 13);
	destruir_test();
}

char* recibir_INPUT(char* textoMostrar, int longitudMaxima) {
	char dato[250];
	printf("\n %s: ", textoMostrar);
	do {
		scanf("%s", &dato);
	} while (dato[0] == '\n');
	return string_substring(dato, 0, longitudMaxima);
}

void TEST_Manual_CU_ABRIR_ARCHIVO() {
	char* PID = recibir_INPUT("Ingrese PID: ", 4);
	char* pathArchivo = recibir_INPUT("Ingrese Path Archivo: ", 200);
	char* modoApertura = recibir_INPUT("Ingrese Modo Apertura: ", 3);

	bool flagCreate = verificarFlag(modoApertura, 'C');
	bool flagRead = verificarFlag(modoApertura, 'L');
	bool flagWrite = verificarFlag(modoApertura, 'E');
	printf("\n Resultado: %s \n", CU_ABRIR_ARCHIVO(PID, pathArchivo, flagCreate, flagRead, flagWrite));

}
void TEST_Manual_CU_MOVER_CURSOR_ARCHIVO() {
	char* PID = recibir_INPUT("Ingrese PID: ", 4);
	int FD = atoi(recibir_INPUT("Ingrese FD ", 6));
	int cursor = atoi(recibir_INPUT("Ingrese CURSOR: ", 6));

	printf("\n Resultado: %s \n", CU_MOVER_CURSOR_ARCHIVO(PID, FD, cursor));

}
void TEST_Manual_CU_LEER_ARCHIVO() {
	char* PID = recibir_INPUT("Ingrese PID: ", 4);
	int FD = atoi(recibir_INPUT("Ingrese FD: ", 6));
	int tamanio = atoi(recibir_INPUT("Ingrese SIZE leer: ", 6));

	printf("\n Resultado: %s \n", CU_LEER_ARCHIVO(PID, FD, tamanio));

}
void TEST_Manual_CU_ESCRIBIR_ARCHIVO() {
	char* PID = recibir_INPUT("Ingrese PID: ", 4);
	int FD = atoi(recibir_INPUT("Ingrese FD: ", 6));
	int tamanio = atoi(recibir_INPUT("Ingrese SIZE escribir: ", 6));
	char* contenido = recibir_INPUT("Ingrese Contenido: ", 1000);

	printf("\n Resultado: %s \n", CU_ESCRIBIR_ARCHIVO(PID, FD, tamanio, contenido));

}
void TEST_Manual_CU_CERRAR_ARCHIVO() {
	char* PID = recibir_INPUT("Ingrese PID: ", 4);
	int FD = atoi(recibir_INPUT("Ingrese FD: ", 6));
	printf("\n Resultado: %s \n", CU_CERRAR_ARCHIVO(PID, FD));

}
void TEST_Manual_CU_BORRAR_ARCHIVO() {
	char* PID = recibir_INPUT("Ingrese PID: ", 4);
	char* pathArchivo = recibir_INPUT("Ingrese Path Archivo: ", 200);
	printf("\n Resultado: %s \n", CU_BORRAR_ARCHIVO(PID, pathArchivo));

}

void TEST_Auto_CU_ABRIR_ARCHIVO() {

	CU_ABRIR_ARCHIVO("1241", "nuevoArchivo1241.zip", true, false, false);
	CU_ABRIR_ARCHIVO("1241", "nuevoArchivo1241.zip", false, true, false);
	CU_ABRIR_ARCHIVO("1241", "nuevoArchivo1241.zip", false, false, true);

	CU_ABRIR_ARCHIVO("1371", "dia.dia", false, true, false);
	CU_ABRIR_ARCHIVO("1371", "dia.dia", false, false, true);
	CU_ABRIR_ARCHIVO("1371", "dia.dia", false, true, false);
	CU_ABRIR_ARCHIVO("1371", "dia.dia", false, false, true);

	CU_ABRIR_ARCHIVO("1189", "gdd.zip", false, false, true);
	CU_ABRIR_ARCHIVO("1189", "dia.dia", false, true, false);
	CU_ABRIR_ARCHIVO("1189", "nuevoArchivo1241.zip", false, false, true);
}
void TEST_Auto_CU_MOVER_CURSOR_ARCHIVO() {

}
void TEST_Auto_CU_LEER_ARCHIVO() {

}
void TEST_Auto_CU_ESCRIBIR_ARCHIVO() {

}
void TEST_Auto_CU_CERRAR_ARCHIVO() {

}
void TEST_Auto_CU_BORRAR_ARCHIVO() {

}

void TEST_VER_TABLA_PROCESO() {
	char* PID = recibir_INPUT("Ingrese PID: ", 4);
	mostrar_tabla_Global_archivos();
	t_list* listaArchivos = buscar_proceso_by_PID(PID)->tablaProcesoArchivo;
	mostrar_tabla_proceso_archivos(listaArchivos);

}
