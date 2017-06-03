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
#include "procesador/Ejecucion.h"

void CU_Procesar_PCB_a_ejecutar();
void CU_Escribir_Pantalla_AnSISOP();

int main(int argc, char *argv[]) {

	inicializar_configuracion(argv[1]);
	//inicializar_configuracion("/home/utnso/Escritorio/tp-2017-1c-Skynet/SistemaCPU/resource/config.cfg");
	controlEjecucionPrograma = false;
	servidor_kernel = conectar_servidor(configuraciones.IP_KERNEL, configuraciones.PUERTO_KERNEL);

	 iniciar_conexion_servidor_memoria();


	 inicializar_contexto_ejecucion();

	//Parametro de Identificacion
	enviar_dato_serializado("CPU", servidor_kernel);
	bool controlSeguir = true;
	//mostrar_menu_primitivas();
	atender_clientes(0, mostrar_menu_primitivas);

	char *operacion;
	do {
		operacion = recibir_dato_serializado(servidor_kernel);
		//SEÑAL ENVIADA POR KERNEL PARA SALIR
		if (strcmp(operacion, "SIGUSR1") == 0) {
			while (controlEjecucionPrograma == true) {
			}
			controlSeguir = false;
		} else if (strcmp(operacion, "RECIBIR_PCB") == 0) {
			CU_Procesar_PCB_a_ejecutar();
		}
	} while (controlSeguir);

	close(servidor_kernel);
	return EXIT_SUCCESS;
}

void CU_Procesar_PCB_a_ejecutar() {
	PCB* pcb = recibir_PCB_de_kernel();
	setPCBEjecucion(pcb);
	ejecutar_Programa();
}

void CU_Escribir_Pantalla_AnSISOP(char* mensaje) {
	enviar_dato_serializado("IMPRIMIR_POR_PANTALLA", servidor_kernel);

	char* pidMensaje = string_itoa(pcbEjecutar->PID);
	enviar_dato_serializado(pidMensaje, servidor_kernel);
	enviar_dato_serializado(mensaje, servidor_kernel);
}
