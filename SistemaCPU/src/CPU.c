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
#include "general/Socket.h"

#include "interfaz/InterfazMemoria.h"
#include "interfaz/InterfazKernel.h"
#include "testing/testearPrimitivasFunciones.h"

void CU_Procesar_PCB_a_ejecutar(int kernel);

int kernel;

int main(int argc, char *argv[]) {

	inicializar_configuracion(argv[1]);
	controlEjecucionPrograma = false;
	kernel = conectar_servidor(configuraciones.IP_KERNEL, configuraciones.PUERTO_KERNEL);

	iniciar_conexion_servidor_memoria();


	//Parametro de Identificacion
	enviar_dato_serializado("CPU", conexionKernel);
	bool controlSeguir = true;

	atender_clientes(0, mostrar_menu_primitivas);

	char *operacion;
	do {
		operacion = recibir_dato_serializado(conexionKernel);
		//SEÑAL ENVIADA POR KERNEL PARA SALIR
		if (strcmp(operacion, "SIGUSR1") == 0) {
			while (controlEjecucionPrograma == true) {
			}
			controlSeguir = false;
		} else if (strcmp(operacion, "RECIBIR_PCB") == 0) {
			CU_Procesar_PCB_a_ejecutar(conexionKernel);
		}
	} while (controlSeguir);

	close(conexionKernel);
	return EXIT_SUCCESS;
}

void CU_Procesar_PCB_a_ejecutar(int kernel) {

}

