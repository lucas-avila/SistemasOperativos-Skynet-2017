#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include <commons/collections/list.h>
#include "../general/Socket.h"
#include "InterfazConsola.h"
#include "../header/Estructuras.h"
#include "../header/KERNEL.h"
#include "../header/AppConfig.h"
#include "../header/PCB.h"


void iniciar_conexion_servidor_consola() {
	int servidor_Consola = crear_servidor(configuraciones.PUERTO_PROG, configuraciones.CANTIDAD_MAXIMA_CONCURRENCIA);
	atender_clientes(servidor_Consola, &escuchar_Conexiones_Consola); // asincronico - multihilo

}

void escuchar_Conexiones_Consola(int servidorConsola) {
	do {
		int cliente = aceptar_conexion_cliente(servidorConsola);
		char* codigo_IDENTIFICACION = recibir_dato_serializado(cliente);
		pthread_t thread_programa;
		if (strcmp(codigo_IDENTIFICACION, "CONSOLA") == 0) {
			agregar_consola_global(cliente);
			enviar_dato_serializado(string_itoa(cliente), cliente);
			//pthread_create(&mihilo1, NULL, &CU_Recibir_Conexiones_Consola, cliente);
			//pthread_detach(&mihilo1);
			//retirar_consola_global(cliente);
		} else if (strcmp(codigo_IDENTIFICACION, "INICIAR_PROGRAMA") == 0) {
			int consola_ID = atoi(recibir_dato_serializado(cliente));
			pthread_create(&thread_programa, NULL, &CU_Recibir_Conexiones_Programa, cliente);
			pthread_detach(&thread_programa);
			CU_iniciar_programa(cliente);
			ConsolaInfo * consola = list_get(lista_consolas, index_of_consola(consola_ID));
			printf("HOLA\n");
			consola->programas = malloc(sizeof(thread_programa));
			list_add(consola->programas, thread_programa);
		} else {
			close(cliente);
		}
	} while (1);
}

void CU_Recibir_Conexiones_Programa(int clientePrograma) {
	int controlSeguir = 1;
	char* codigo_operacion;
	do {
		codigo_operacion = recibir_dato_serializado(clientePrograma);
		if (strcmp(codigo_operacion, "FINALIZAR_PROGRAMA") == 0) {
			int pid = atoi(recibir_dato_serializado(clientePrograma));
			// Proceso* Proceso_a_eliminar = buscar_proceso_by_PID(pid);
			verificar_estado(pid);
		} else if (strcmp(codigo_operacion, "") == 0) {
			close(clientePrograma);
			controlSeguir = 0;
		} else {
			enviar_dato_serializado("ERROR: CODIGO OPERACION INEXISTENTE", clientePrograma);
		}
	} while (controlSeguir == 1);
	close(clientePrograma);
}

int index_of_consola(int consola_ID) {
	if (consola_ID < 0)
		return -1;
	int i = 0;
	ConsolaInfo * aux = malloc(sizeof(ConsolaInfo));
	while (aux = list_get(lista_consolas, i)) {
		if (aux->consola_ID == consola_ID)
			return i;
		i++;
	}
}

void agregar_consola_global(int consola_ID) {
	ConsolaInfo * consola = malloc(sizeof(ConsolaInfo));
	consola->consola_ID = consola_ID;
	list_add(lista_consolas, consola);
}

void retirar_consola_global(int numeroConexion) {
	printf("Se elimino a consola con posicion %d de la lista\n", index_of_consola(numeroConexion));
	list_remove(lista_consolas, index_of_consola(numeroConexion));
}

