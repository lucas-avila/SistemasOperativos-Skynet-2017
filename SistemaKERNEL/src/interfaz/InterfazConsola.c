#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "../general/Socket.h"
#include "InterfazConsola.h"
#include<pthread.h>

void escuchar_Conexiones_Consola(int servidorConsola) {
	do {
		int cliente = aceptar_conexion_cliente(servidorConsola);
		char* codigo_IDENTIFICACION = recibir_dato_serializado(cliente);
		pthread_t mihilo1;
		if (strcmp(codigo_IDENTIFICACION, "CONSOLA") == 0) {
			pthread_create(&mihilo1, NULL, &CU_Recibir_Conexiones_Consola, cliente);
			pthread_detach(&mihilo1);
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

void CU_iniciar_programa(int filesystem){
	void CU_iniciar_programa(int filesystem){
	char * respuesta = recibir_dato_serializado(filesystem);
printf("Archivo: %s", respuesta);
	/**enviar_dato_serializado("KERNEL", filesystem);

	char * respuesta = recibir_dato_serializado(filesystem);

	if(strcmp(respuesta, "FILESYSTEM") == 0){
		printf("Handshake exitoso\n");

	}
	close(filesystem);**/
}
}
