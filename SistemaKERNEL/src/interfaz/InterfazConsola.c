#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include <commons/collections/list.h>
#include "../general/Socket.h"
#include "InterfazConsola.h"
#include "../header/Estructuras.h"
#include "../header/KERNEL.h"

void escuchar_Conexiones_Consola(int servidorConsola) {
	do {
		int cliente = aceptar_conexion_cliente(servidorConsola);
		char* codigo_IDENTIFICACION = recibir_dato_serializado(cliente);
		pthread_t mihilo1;
		if (strcmp(codigo_IDENTIFICACION, "CONSOLA") == 0) {
			agregar_consola_global(cliente, mihilo1);
			pthread_create(&mihilo1, NULL, &CU_Recibir_Conexiones_Consola, cliente);
			pthread_detach(&mihilo1);
			//retirar_consola_global(cliente);

		} else {
			close(cliente);
		}
	} while (1);
}

void CU_Recibir_Conexiones_Consola(int clienteConsola) {
	int controlSeguir = 1;
	char* codigo_operacion;
	do {
		codigo_operacion = recibir_dato_serializado(clienteConsola);
		if (strcmp(codigo_operacion, "INICIAR_PROGRAMA") == 0) {
			CU_iniciar_programa(clienteConsola);
		} else if (strcmp(codigo_operacion, "") == 0) {
			close(clienteConsola);
			controlSeguir = 0;
		} else {
			enviar_dato_serializado("ERROR: CODIGO OPERACION INEXISTENTE", clienteConsola);
		}
	} while (controlSeguir == 1);
	close(clienteConsola);
}

int index_of_consola(int numeroConexion){
	if(numeroConexion < 0) return -1;
	int i = 0;
	ConsolaInfo * aux = malloc(sizeof(ConsolaInfo));
	while(aux = list_get(lista_consolas, i)){
		if(aux->numeroConexion == numeroConexion)
			return i;
		i++;
	}
}

void agregar_consola_global(int numeroConexion, pthread_t hilo){
	ConsolaInfo * consola = malloc(sizeof(ConsolaInfo));
	consola->numeroConexion = numeroConexion;
	consola->hilo = hilo;
	list_add(lista_consolas, consola);
}


void retirar_consola_global(int numeroConexion){
	printf("Se elimino a consola con posicion %d de la lista\n", index_of_consola(numeroConexion));
	list_remove(lista_consolas, index_of_consola(numeroConexion));
}


