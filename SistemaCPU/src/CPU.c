/*
 ============================================================================
 Name        : CPU.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "header/AppConfig.h"
#include "header/Socket.h"

void CU_Procesar_PCB_a_ejecutar(int kernel);
void solicitar_bytes_memoria();

int main(int argc, char *argv[]) {

	inicializar_configuracion(argv[1]);
	controlEjecucionPrograma = false;
	int kernel = conectar_servidor("127.0.0.1", configuraciones.PUERTO_KERNEL);

	//Parametro de Identificacion
	enviar_dato_serializado("CPU", kernel);
	bool controlSeguir = true;
	char *operacion;
	do {
		operacion = recibir_dato_serializado(kernel);
		//SEÑAL ENVIADA POR KERNEL PARA SALIR
		if (strcmp(operacion, "SIGUSR1") == 0) {
			while (controlEjecucionPrograma == true) {
			}
			controlSeguir = false;
		} else if (strcmp(operacion, "RECIBIR_PCB") == 0) {
			CU_Procesar_PCB_a_ejecutar(kernel);
		}
	} while (controlSeguir);

	close(kernel);
	return EXIT_SUCCESS;
}

void CU_Procesar_PCB_a_ejecutar(int kernel) {
	//solicitar_bytes_memoria();
}

void solicitar_bytes_memoria() {
	int serverMemoria = crear_servidor(configuraciones.IP_MEMORIA, configuraciones.PUERTO_MEMORIA);
	/**
	 * Enviar datos para solicitar memoria
	 */
	enviar_dato_serializado("CPU", serverMemoria); //identificacion
	enviar_dato_serializado("SOLICITAR_BYTE_MEMORIA", serverMemoria); //operacion

	enviar_dato_serializado("1", serverMemoria); //pagina
	enviar_dato_serializado("0", serverMemoria); //byte inicial de pagina
	enviar_dato_serializado("20", serverMemoria); //longitud de bytes a pedir

	//recibir datos
	char* dato = recibir_dato_serializado(serverMemoria);
	printf("DATO RECIBIDO DE MEMORIA : %s", dato);
	close(serverMemoria);
}

