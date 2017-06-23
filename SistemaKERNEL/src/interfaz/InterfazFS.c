#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "commons/collections/list.h"

#include "../administrarPCB/EstadisticaProceso.h"
#include "../administrarPCB/PCBData.h"
#include "../capaFILESYSTEM/TablaGlobalArchivo.h"
#include "../capaMEMORIA/GestMemoriaFuncionesAux.h"
#include "../general/funcionesUtiles.h"
#include "../../../Sharedlib/Sharedlib/Socket.h"
#include "../header/AppConfig.h"
#include "../header/Estructuras.h"
#include "../../../Sharedlib/Sharedlib/PCB.h"
#include "../interfaz/InterfazConsola.h"
#include "../interfaz/InterfazCPU.h"
#include "../interfaz/InterfazMemoria.h"
#include "../planificacion/Planificacion.h"

#include "../administrarProcesos/Proceso.h"



t_list* listaArchivos;
t_list* contenidoArchivo;
void inicializar() {

	listaArchivos = list_create();
	list_add(listaArchivos, "dia.dia");
	list_add(listaArchivos, "archivo");
	list_add(listaArchivos, "gdd.zip");

	contenidoArchivo = list_create();

	t_list* contenido1 = list_create();
	list_add(contenido1, "contenido 1");
	list_add(contenido1, "contenido 2");
	list_add(contenido1, "contenido 3");
	list_add(contenido1, "contenido 4");
	list_add(contenido1, "contenido 5");

	t_list* contenido2 = list_create();
	list_add(contenido2, "contenido 1 1");
	list_add(contenido2, "contenido 1 2");
	list_add(contenido2, "contenido 1 3");
	list_add(contenido2, "contenido 1 4");
	list_add(contenido2, "contenido 1 5");

	t_list* contenido3 = list_create();
	list_add(contenido3, "contenido 2 1");
	list_add(contenido3, "contenido 2 2");
	list_add(contenido3, "contenido 2 3");
	list_add(contenido3, "contenido 2 4");
	list_add(contenido3, "contenido 2 5");

	list_add(contenidoArchivo, contenido1);

	list_add(contenidoArchivo, contenido2);

	list_add(contenidoArchivo, contenido3);

	//testeando crear PCB
	PCB * pcb_nuevo = crear_pcb();
	pcb_nuevo->PID= 1024;
	crear_Proceso_en_tabla(string_itoa(pcb_nuevo->PID));
	Proceso* proc = new_Proceso(pcb_nuevo);
	//JONY: esto lo agrego para probar la planificacion

	//ESTO CAGA TODA LA PLANIFICACION LO COMENTO XD
	//proceso_a_NEW(proc);
	agregar_proceso(proc);
}
int buscarIndice(char* path) {
	int tam = list_size(listaArchivos);
	int i = 0;
	for (i = 0; i < tam; i++) {
		char* elemen = list_get(listaArchivos, i);
		if (strcmp(elemen, path) == 0) {
			return i;
		}
	}
	return -1;
}

char* validar_archivo(char *path) {

	return buscarIndice(path) == -1 ? "ERROR_NO_EXISTE" : "OK";
}
char* crear_archivo(char *path) {
	list_add(listaArchivos, path);
	t_list* contenido3 = list_create();
	list_add(contenido3, "contenido nuevo 1");
	list_add(contenido3, "contenido nuevo 2");
	list_add(contenido3, "contenido nuevo 3");
	list_add(contenido3, "contenido nuevo 4");
	list_add(contenidoArchivo, contenido3);

	return "OK";
}
char* borrar(char* path) {

	list_remove(contenidoArchivo, buscarIndice(path));
	list_remove(listaArchivos, buscarIndice(path));
	return "OK";
}
char* obtenerDatos(char* path, int bloqueInicial, int size) {
	t_list* contenido = list_get(contenidoArchivo, buscarIndice(path));
	return string_substring(((char*) list_get(contenido, bloqueInicial)), 0, size);
}
char* guardarDatos(char* path, int bloqueInicial, int size, char* contenido) {
	//t_list* contenidoe = list_get(contenidoArchivo, buscarIndice(path));
	list_replace(list_get(contenidoArchivo, buscarIndice(path)), bloqueInicial, contenido);
	return "OK";
}
