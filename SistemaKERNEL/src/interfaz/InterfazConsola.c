#include "InterfazConsola.h"

#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../administrarProcesos/Proceso.h"
#include "../general/Socket.h"
#include "../header/AppConfig.h"
#include "../header/KERNEL.h"
#include "../header/SolicitudesUsuario.h"


void iniciar_conexion_servidor_programa() {
	int servidor_Programa = crear_servidor(configuraciones.PUERTO_PROG, configuraciones.CANTIDAD_MAXIMA_CONCURRENCIA);
	atender_clientes(servidor_Programa, &escuchar_Conexiones_Programa); // asincronico - multihilo

}

void escuchar_Conexiones_Programa(int servidorPrograma) {
	do {
		int cliente = aceptar_conexion_cliente(servidorPrograma);
		char* codigo_IDENTIFICACION = recibir_dato_serializado(cliente);
		pthread_t thread_programa;
		if (strcmp(codigo_IDENTIFICACION, "INICIAR_PROGRAMA") == 0) {
			pthread_create(&thread_programa, NULL, &CU_Recibir_Conexiones_Programa, cliente);
			pthread_detach(&thread_programa);
			CU_iniciar_programa(cliente);
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
			Proceso* Proceso_a_eliminar = buscar_proceso_by_PID(pid);
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
