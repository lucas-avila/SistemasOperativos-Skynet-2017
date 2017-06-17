/*
 ============================================================================
 Name        : CPU.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../../Sharedlib/Sharedlib/Socket.h"
#include "header/AppConfig.h"
#include "header/PCB.h"
#include "interfaz/InterfazKernel.h"
#include "interfaz/InterfazMemoria.h"
#include "interfaz/signals.h"
#include "procesador/Ejecucion.h"
#include "testing/testearPrimitivasFunciones.h"

void CU_Procesar_PCB_a_ejecutar();

void testear_planificacion(servidor_kernel); //borrame

bool controlSeguir = true;

int main(int argc, char *argv[]) {
	//TODO: Agregar adentro de esta funcion, que espere a que termine de ejecutar, lo mande al kernel y DESPUES mandar el desconectar y finalizar el proceso
	signal(SIGINT, recibir_seniales_de_linux);

	inicializar_configuracion(argv[1]);
	//inicializar_configuracion("/home/utnso/Escritorio/tp-2017-1c-Skynet/SistemaCPU/resource/config.cfg");
	controlEjecucionPrograma = false;
	servidor_kernel = conectar_servidor(configuraciones.IP_KERNEL, configuraciones.PUERTO_KERNEL);
	iniciar_conexion_servidor_memoria();

	inicializar_contexto_ejecucion();

	//Parametro de Identificacion
	enviar_dato_serializado("CPU", servidor_kernel);

	//mostrar_menu_primitivas();
	atender_clientes(0, mostrar_menu_primitivas);

	char *operacion;
	do {
		operacion = recibir_dato_serializado(servidor_kernel);
		if (strcmp(operacion, "RECIBIR_PCB") == 0) {
			printf("\nPCB RECIBIDO:\n ");
			CU_Procesar_PCB_a_ejecutar();
		}else if (strcmp(operacion, "TESTEAR_PLANIFICACION") == 0){
			recibir_dato_serializado(servidor_kernel);
			testear_planificacion(servidor_kernel);
		}
	} while (controlSeguir);

	enviar_dato_serializado("DESCONECTAR", servidor_kernel);
	close(servidor_kernel);
	return EXIT_SUCCESS;
}

void CU_Procesar_PCB_a_ejecutar() {
	PCB* pcb = recibir_PCB_de_kernel();

	setPCBEjecucion(pcb);
	ejecutar_Programa();
}

void CU_Terminar_ejecucion_y_finalizar() {
	controlSeguir = false;
}










int n = 0;
void testear_planificacion(servidor_kernel){
	printf("\nDEBUG ---> Llego a testing -->  ");
	PCB* pcb = recibir_PCB_de_kernel();



	if(n <= 3){
		enviar_SYSCALL_wait_semaforo_a_kernel("mutex1", pcb);
	}else{
		printf("\n\nDEBUG ---> Se está por llegar al grado maximo de multiprogramacion, empezamos a hacer signals\n\n");
		sleep(1);
		enviar_SYSCALL_signal_semaforo_a_kernel("mutex1");
		enviar_PCB_a_kernel(pcb, "TERMINADO");
	}

	n++;
}


