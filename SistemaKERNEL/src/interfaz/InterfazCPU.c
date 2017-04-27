#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include "../header/AppConfig.h"
#include "../general/Socket.h"
#include "InterfazCPU.h"
#include "../header/PCB.h"

int servidor_CPU = 0;

void iniciar_conexion_servidor_cpu(){
	servidor_CPU = crear_servidor(configuraciones.PUERTO_CPU, configuraciones.CANTIDAD_MAXIMA_CONCURRENCIA);
	atender_clientes(servidor_CPU, &escuchar_Conexiones_CPU); // asincronico - multihilo
}

void escuchar_Conexiones_CPU(int servidorCPU) {
	do {
		int cliente = aceptar_conexion_cliente(servidorCPU);
		char* codigo_IDENTIFICACION = recibir_dato_serializado(cliente);
		pthread_t mihilo1;
		if (strcmp(codigo_IDENTIFICACION, "CPU") == 0) {
			pthread_create(&mihilo1, NULL, &CU_Recibir_Conexiones_CPU, cliente);
			pthread_detach(&mihilo1);
		} else {
			close(cliente);
		}
	} while (1);
}


void CU_Recibir_Conexiones_CPU(int clienteCPU) {
	int controlSeguir = 1;
	char* codigo_operacion;

	PCB * pcb = crear_pcb();
	enviar_pcb(pcb, clienteCPU);

	/*
	do {
		codigo_operacion = recibir_dato_serializado(clienteCPU);

		if (strcmp(codigo_operacion, "NOMBRE") == 0) {

		}else if (strcmp(codigo_operacion, "") == 0) {
			close(clienteCPU);
			controlSeguir = 0;
		} else {
			enviar_dato_serializado("ERROR: CODIGO OPERACION INEXISTENTE", clienteCPU);
		}
	} while (controlSeguir == 1);
	*/
	close(clienteCPU);
}
