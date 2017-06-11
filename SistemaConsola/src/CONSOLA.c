/*
 ============================================================================
 Name        : CONSOLA.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <commons/collections/list.h>
#include <commons/string.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "header/AppConfig.h"
#include "header/Estructuras.h"
#include "header/ExitCodes.h"
#include "header/funcionesUtiles.h"
#include "header/InterfazKernel.h"
#include "header/Socket.h"

void atender_solicitudes_de_usuario();
void iniciar_thread();
void CU_iniciar_programa();
void recibir_mensajes(int pid, int kernel_programa);
void finalizar_programa(int pid, int kernel_programa);
void mostrar_info_proceso(uint32_t pid);

int main(int argc, char *argv[]) {

	inicializar_configuracion(argv[1]);
	inicializar_lista_Info_procesos();
	incializar_log();

	pthread_t t_interfaz;
	pthread_create(&t_interfaz, NULL, &atender_solicitudes_de_usuario, NULL);
	pthread_join(t_interfaz, NULL);

	return EXIT_SUCCESS;
}

void mostrar_menu_usuario() {
	printf("\n******* Bienvenido a Esther ******");
	printf("\n 1 - Iniciar programa.");
	printf("\n 2 - Finalizar programa.");
	printf("\n 3 - Desconectar consola.");
	printf("\n 4 - Limpiar mensajes.");
	printf("\n 0 - Salir.");
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
			if(pertenece_a_la_consola(pid)){
				Info_ejecucion * programa = buscar_info_por_PID(pid);
				solicitar_fin_programa(programa->pid, programa->socket);
			} else {
				printf("El PID ingresado no pertenece a un proceso de esta consola.\n");
			}
		}
			break;
		case 3:
			printf("Se finalizaran todos los programas activos.\n");
			int tamanio = list_size(Info_procesos);
			int i = 0;
			Info_ejecucion* info_ejecucion;
			for (i; i < tamanio; i++) {
				info_ejecucion = list_get(Info_procesos, i);
				solicitar_desconexion(info_ejecucion->pid, info_ejecucion->socket);
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
	finalizar_programa(pid, kernel_programa);
	close(kernel_programa);
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

}

void finalizar_programa(int pid, int kernel_programa) {

	int exit_code = atoi(recibir_dato_serializado(kernel_programa));

	mostrar_exit_code(exit_code);
	mostrar_info_proceso(pid);
	eliminar_info_proceso(buscar_info_por_PID(pid));
}

// Fecha y hora de fin de ejecucion, fecha y hora de inicio de ejecucion,
// tiempo total de ejecucion, cantidad de impresiones por pantalla.
void mostrar_info_proceso(uint32_t pid) {

	Info_ejecucion* info_proceso = buscar_info_por_PID(pid);
	char * textoInicio = ctime(&(info_proceso->fecha_inicio));
	time_t fecha_fin = time(NULL);
	char * textoFin = ctime(&fecha_fin);

	unsigned int tiempoTotal = difftime(fecha_fin, info_proceso->fecha_inicio);
	unsigned int horas = tiempoTotal / 3600;
	unsigned int minutos = (tiempoTotal % 3600) / 60;
	unsigned int segundos = (tiempoTotal % 3600) % 60;

	string_append(&info_log, "El Proceso(");
	string_append(&info_log, string_itoa(info_proceso->pid));
	string_append(&info_log, ") ha finalizado, los siguientes son sus datos estadisticos: \n");
	string_append(&info_log, "Fecha de inicio de ejecucion: ");
	string_append(&info_log, textoInicio);
	string_append(&info_log, "Fecha de fin de ejecucion: ");
	string_append(&info_log, textoFin);
	string_append(&info_log, "Tiempo total de ejecucion: (");
	string_append(&info_log, string_itoa(horas));
	string_append(&info_log, ") horas, (");
	string_append(&info_log, string_itoa(minutos));
	string_append(&info_log, ") minutos, (");
	string_append(&info_log, string_itoa(segundos));
	string_append(&info_log, ") segundos.\n");
	string_append(&info_log, "Cantidad de impresiones por pantalla: ");
	string_append(&info_log, string_itoa(info_proceso->cant_impresiones));
	generar_log();
	mostrar_menu_usuario();
}

