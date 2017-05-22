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
#include <time.h>

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
void recibir_mensajes(int pid, int kernel_programa);
void finalizar_programa(int pid, int kernel_programa);
void mostrar_info_proceso( pid);

int kernel;

int main(int argc, char *argv[]) {

	inicializar_configuracion(argv[1]);
	inicializar_lista_Info_procesos();
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
			Info_ejecucion * programa = buscar_info_por_PID(pid);
			enviar_dato_serializado("FINALIZAR_PROGRAMA", programa->socket);
			enviar_dato_serializado(string_itoa(pid), programa->socket);
		}
			break;
		case 3:
			printf("Se finalizaran todos los programas activos.\n");

			int tamanio = list_size(Info_procesos);
			int i = 0;
			Info_ejecucion* info_ejecucion;
			for (i; i < tamanio; i++) {
				info_ejecucion = list_get(Info_procesos, i);
				finalizar_programa(info_ejecucion->pid, info_ejecucion->socket);
			}
			break;
		case 4:
			system("clear");
			break;
		}
	} while (opcion != 0);
}

void iniciar_thread(char * path_archivo_fuente) {

	pthread_t t_programa;

	pthread_create(&t_programa, NULL, &CU_iniciar_programa, path_archivo_fuente);
	pthread_detach(t_programa);

}

void CU_iniciar_programa(char * path_archivo_fuente) {

	validarArchivo(path_archivo_fuente);
	int kernel_programa = conectar_servidor(configuraciones.IP_KERNEL, configuraciones.PUERTO_KERNEL);
	CU_handshake_programa(kernel_programa);

	int pid;
	pid = atoi(enviar_programa_ANSISOP(path_archivo_fuente, kernel_programa));

	Info_ejecucion * info_proceso = new_Info_ejecucion(pid, kernel_programa);
	agregar_proceso(info_proceso);

	recibir_mensajes(pid, kernel_programa);
}

void recibir_mensajes(int pid, int kernel_programa) {
	char * mensaje;
	mensaje = recibir_dato_serializado(kernel_programa);

	while (strcmp(mensaje, "FIN_PROGRAMA") != 0) {
		printf("El mensaje del Proceso (%d) es: %s\n", pid, mensaje);
		Info_ejecucion* info_proceso = buscar_info_por_PID(pid);
		info_proceso->cant_impresiones = info_proceso->cant_impresiones + 1;
		mensaje = recibir_dato_serializado(kernel_programa);
	}

	finalizar_programa(pid, kernel_programa);
}

void finalizar_programa(int pid, int kernel_programa) {

	enviar_dato_serializado("FINALIZAR_PROGRAMA", kernel_programa);
	enviar_dato_serializado(string_itoa(pid), kernel_programa);

	int exit_code = atoi(recibir_dato_serializado(kernel_programa));

	mostrar_exit_code(exit_code);
	mostrar_info_proceso(pid);
	eliminar_info_proceso(buscar_info_por_PID(pid));
}

// Fecha y hora de fin de ejecucion, fecha y hora de inicio de ejecucion, tiempo total de ejecucion, cantidad de impresiones por pantalla.
void mostrar_info_proceso(uint32_t pid) {
	Info_ejecucion* info_proceso = buscar_info_por_PID(pid);
	char* textoInicio = ctime(info_proceso->fecha_inicio);
	time_t fecha_fin = time(NULL);
	char* textoFin = ctime(fecha_fin);

	unsigned int tiempoTotal = difftime(fecha_fin, info_proceso->fecha_inicio);
	unsigned int horas = tiempoTotal / 3600;
	unsigned int minutos = (tiempoTotal % 3600) / 60;
	unsigned int segundos = (tiempoTotal % 3600) % 60;

	printf("El Proceso (%d) ha finalizado, los siguientes son sus datos estadisticos: \n", info_proceso->pid);
	printf("Fecha de inicio de ejecucion: (%s)\n", textoInicio);
	printf("Fecha de fin de ejecucion: (%s)\n", textoFin);
	printf("Tiempo total de ejecucion: (%d) horas, (%d) minutos, (%d) segundos.\n", horas, minutos, segundos);
	printf("Cantidad de impresiones por pantalla: (%d)", info_proceso->cant_impresiones);

	return;
}

