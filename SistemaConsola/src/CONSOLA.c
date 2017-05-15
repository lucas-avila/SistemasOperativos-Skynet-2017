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

#include "commons/collections/list.h"
#include "header/AppConfig.h"
#include "header/Socket.h"
#include "header/funcionesUtiles.h"
#include "header/ExitCodes.h"
#include "header/InterfazKernel.h"
#include "header/Estructuras.h"

void atender_solicitudes_de_usuario();
void iniciar_thread();
void CU_iniciar_programa();
void recibir_mensajes(int pid);
void finalizar_programa(int pid);
void eliminar_info_proceso(int pid);
void mostrar_info_proceso(pid);

int kernel;

int main(int argc, char *argv[]) {

	inicializar_configuracion(argv[1]);
	kernel = conectar_servidor(configuraciones.IP_KERNEL, configuraciones.PUERTO_KERNEL);
	CU_handshake_consola(kernel);

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
	char path_archivo_fuente[100];
	do {
		mostrar_menu_usuario();
		opcion = validarNumeroInput(0, 4);
		switch (opcion) {

		case 1:
			printf("Ingrese el PATH del archivo: ");
			scanf("%s", path_archivo_fuente);
			iniciar_thread(path_archivo_fuente);
			system("clear");
			break;
		case 2: {
			int pid;
			printf("\nIngrese el PID del programa que desea terminar: ");
			scanf("%d", &pid);
			enviar_dato_serializado("FINALIZAR_PROGRAMA", kernel);
			enviar_dato_serializado(string_itoa(pid), kernel);
		}
			break;
		case 3:

			break;
		case 4:
			system("clear");
			break;
		}
	} while (opcion != 0);
}

void iniciar_thread(char * path_archivo_fuente){

	pthread_t t_programa;

	pthread_create(&t_programa, NULL, &CU_iniciar_programa, path_archivo_fuente);
	pthread_detach(t_programa);

}

void CU_iniciar_programa(char * path_archivo_fuente){

	validarArchivo(path_archivo_fuente);
	CU_handshake_programa(kernel);

	int pid;
	pid = atoi(enviar_programa_ANSISOP(path_archivo_fuente, kernel));

	Info_ejecucion info_proceso;
	//Falta almacenar las cosas en un struct Info_ejecucion..
	//list_add(lista_procesos, info_proceso);

	recibir_mensajes(pid);
}

void recibir_mensajes(int pid){
	char * mensaje;

	mensaje = recibir_dato_serializado(kernel);
	while (strcmp(mensaje, "FIN_PROGRAMA") != 0){
		printf("El mensaje del Proceso (%d) es: %s\n", pid, mensaje);
		mensaje = recibir_dato_serializado(kernel);
	}

	finalizar_programa(pid);
}

void finalizar_programa(pid){

	int exit_code = atoi(recibir_dato_serializado(kernel));

	mostrar_exit_code(exit_code);
	mostrar_info_proceso(pid);
	eliminar_info_proceso(pid);
}

void eliminar_info_proceso(int pid){

	//list_remove_and_destroy_element();
}

void mostrar_info_proceso(pid){

	//list_find(lista_procesos, &pid_identicos());
}

