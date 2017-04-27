/*
 ============================================================================
 Name        : CONSOLA.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "header/AppConfig.h"
#include "header/Socket.h"
#include "header/funcionesUtiles.h"

void atender_solicitudes_de_usuario();
void CU_handshake(int kernel);
void iniciar_thread();
void CU_iniciar_programa();
int enviar_programa_ANSISOP();
char * obtener_codigo(char * path_archivo_fuente);

int kernel;

int main(int argc, char *argv[]) {

	inicializar_configuracion(argv[1]);
	kernel = conectar_servidor(configuraciones.IP_KERNEL, configuraciones.PUERTO_KERNEL);
	enviar_dato_serializado("CONSOLA", kernel);


	pthread_t t_interfaz;
	pthread_create(&t_interfaz, NULL, &atender_solicitudes_de_usuario, NULL);
	pthread_join(t_interfaz, NULL);

	return EXIT_SUCCESS;
}

void mostrar_menu_usuario() {
	printf("\n******* Bienvenido a Esther ******");
	printf("\n 1 - Iniciar programa -> ruta al programa (Handshake Kernel)");
	printf("\n 2 - Finalizar programa -> PID");
	printf("\n 3 - Desconectar consola");
	printf("\n 4 - Limpiar mensajes");
	printf("\n 0 - Salir");
	printf("\n Opcion: ");
}

void atender_solicitudes_de_usuario() {
	int opcion = -1;
	do {
		mostrar_menu_usuario();
		opcion = validarNumeroInput(0, 4);
		switch (opcion) {

		case 1:
			iniciar_thread();
			break;
		case 2:

			break;
		case 3:

			break;
		case 4:

			break;
		}
	} while (opcion != 0);
}

void iniciar_thread(){

	pthread_t t_programa;

	pthread_create(&t_programa, NULL, &CU_iniciar_programa, NULL);
	pthread_join(t_programa, NULL);

	//falta modificar para que se puedan ejecutar varios hilos-programa en paralelo.

}

void CU_iniciar_programa(){
	char path_archivo_fuente[100]; // No creo que exista un path muy largo...
	int pid;

	printf("Ingrese el PATH del archivo: ");
	scanf("%s", path_archivo_fuente);
	validarArchivo(path_archivo_fuente);
	//CU_handshake(kernel);

	pid = enviar_programa_ANSISOP(&path_archivo_fuente);
}

void CU_handshake(int kernel){
	enviar_dato_serializado("CONSOLA", kernel);

	//char * respuesta = recibir_dato_serializado(kernel);
	//if(strcmp(respuesta, "KERNEL") == 0){
	//	printf("--Handshake exitoso--\n");
	//}
}

int enviar_programa_ANSISOP(char * path_archivo_fuente){

	char * literal;
	literal = obtener_codigo(path_archivo_fuente);
	enviar_dato_serializado("INICIAR_PROGRAMA",kernel);
	enviar_dato_serializado(literal, kernel);
	//Todavía en testing...
	return 3;
}

char * obtener_codigo(char * path_archivo_fuente){
	FILE * archivo_fuente;
	char * literal;
	int size_buffer;

	archivo_fuente = fopen(path_archivo_fuente, "r");

	// Ahora necesitamos saber el tamaño del archivo.
	fseek(archivo_fuente, 0, SEEK_END);
	size_buffer = ftell(archivo_fuente);
	fseek(archivo_fuente, 0, SEEK_SET);

	//Para evitar segmentation fault allocamos memoria.
	literal = malloc(sizeof(char) * size_buffer);
	fread(literal, sizeof(char), size_buffer, archivo_fuente);

	return literal;
}
