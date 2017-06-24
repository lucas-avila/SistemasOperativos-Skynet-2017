#include "InterfazFS.h"

#include <commons/collections/list.h>
#include <commons/string.h>
#include <string.h>
#include <Sharedlib/PCB.h>
#include <Sharedlib/Socket.h>

#include "../administrarPCB/EstadisticaProceso.h"
#include "../administrarProcesos/Proceso.h"
#include "../header/AppConfig.h"



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

void iniciar_conexion_servidor_FS(){
	servidor_filesystem = conectar_servidor(configuraciones.IP_FS, configuraciones.PUERTO_FS);
	enviar_dato_serializado("KERNEL", servidor_filesystem);
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
	enviar_dato_serializado("VALIDAR_ARCHIVO", servidor_filesystem);
	enviar_dato_serializado(path, servidor_filesystem);
	return recibir_dato_serializado(servidor_filesystem);
}
char* crear_archivo(char *path) {
	/*list_add(listaArchivos, path);
	t_list* contenido3 = list_create();
	list_add(contenido3, "contenido nuevo 1");
	list_add(contenido3, "contenido nuevo 2");
	list_add(contenido3, "contenido nuevo 3");
	list_add(contenido3, "contenido nuevo 4");
	list_add(contenidoArchivo, contenido3);
	*/
	enviar_dato_serializado("CREAR_ARCHIVO", servidor_filesystem);
	enviar_dato_serializado(path, servidor_filesystem);
	return recibir_dato_serializado(servidor_filesystem);
}
char* borrar(char* path) {
	enviar_dato_serializado("BORRAR_ARCHIVO", servidor_filesystem);
	enviar_dato_serializado(path, servidor_filesystem);
	/*
	list_remove(contenidoArchivo, buscarIndice(path));
	list_remove(listaArchivos, buscarIndice(path));
	*/
	return recibir_dato_serializado(servidor_filesystem);
}
char* obtenerDatos(char* path, int offset, int size) {
	enviar_dato_serializado("OBTENER_DATOS", servidor_filesystem);
	enviar_dato_serializado(path, servidor_filesystem);
	enviar_dato_serializado(string_itoa(offset), servidor_filesystem);
	enviar_dato_serializado(string_itoa(size), servidor_filesystem);
	/*t_list* contenido = list_get(contenidoArchivo, buscarIndice(path));
	return string_substring(((char*) list_get(contenido, offset)), 0, size);*/
	return recibir_dato_serializado(servidor_filesystem);
}
char* guardarDatos(char* path, int offset, int size, char* contenido) {
	enviar_dato_serializado("GUARDAR_DATOS", servidor_filesystem);
	enviar_dato_serializado(path, servidor_filesystem);
	enviar_dato_serializado(string_itoa(offset), servidor_filesystem);
	enviar_dato_serializado(string_itoa(size), servidor_filesystem);
	enviar_dato_serializado(contenido, servidor_filesystem);
	//t_list* contenidoe = list_get(contenidoArchivo, buscarIndice(path));
	//list_replace(list_get(contenidoArchivo, buscarIndice(path)), bloqueInicial, contenido);
	return recibir_dato_serializado(servidor_filesystem);
}
