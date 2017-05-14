#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include <commons/collections/list.h>
#include "../header/AppConfig.h"
#include "../general/Socket.h"
#include "InterfazCPU.h"
#include "../header/PCB.h"
#include "../header/Estructuras.h"
#include "../capaMEMORIA/GestionMemoriaDinamica.h"

#include "../planificacion/Planificacion.h"

int servidor_CPU = 0;

void iniciar_conexion_servidor_cpu() {
	servidor_CPU = crear_servidor(configuraciones.PUERTO_CPU, configuraciones.CANTIDAD_MAXIMA_CONCURRENCIA);
	atender_clientes(servidor_CPU, &escuchar_Conexiones_CPU); // asincronico - multihilo
}

void escuchar_Conexiones_CPU(int servidorCPU) {
	do {
		int cliente = aceptar_conexion_cliente(servidorCPU);
		char* codigo_IDENTIFICACION = recibir_dato_serializado(cliente);
		pthread_t mihilo1;
		if (strcmp(codigo_IDENTIFICACION, "CPU") == 0) {
			agregar_CPU_global(cliente, mihilo1);
			pthread_create(&mihilo1, NULL, &CU_Recibir_Conexiones_CPU, cliente);
			pthread_detach(&mihilo1);
			//retirar_CPU_global(cliente);
		} else {
			close(cliente);
		}
	} while (1);
}

void CU_Recibir_Conexiones_CPU(int clienteCPU) {
	int controlSeguir = 1;
	char* codigo_operacion;

	do {
		codigo_operacion = recibir_dato_serializado(clienteCPU);
		if (strcmp(codigo_operacion, "GESTION_MEMORIA") == 0) {
			CU_Gestionar_HEAP(clienteCPU);
		} else if (strcmp(codigo_operacion, "RECIBIR_CPU") == 0) {
			recibir_PCB_de_CPU(clienteCPU);
		} else if (strcmp(codigo_operacion, "") == 0) {
			close(clienteCPU);
			controlSeguir = 0;
		} else {
			enviar_dato_serializado("ERROR: CODIGO OPERACION INEXISTENTE", clienteCPU);
		}
	} while (controlSeguir == 1);

	close(clienteCPU);
}

int index_of_CPU(int numeroConexion) {
	if (numeroConexion < 0)
		return -1;
	int i = 0;
	CPUInfo * aux = malloc(sizeof(CPUInfo));
	while (aux = list_get(lista_CPUs, i)) {
		if (aux->numeroConexion == numeroConexion)
			return i;
		i++;
	}
}

void agregar_CPU_global(int numeroConexion, pthread_t hilo) {
	CPUInfo * cpu = malloc(sizeof(CPUInfo));
	cpu->numeroConexion = numeroConexion;
	cpu->hilo = hilo;
	cpu->disponible = 1;
	list_add(lista_CPUs, cpu);
}

void retirar_CPU_global(int numeroConexion) {
	printf("Se elimino a CPU en posicion %d de la lista\n", index_of_CPU(numeroConexion));
	list_remove(lista_CPUs, index_of_CPU(numeroConexion));
}
