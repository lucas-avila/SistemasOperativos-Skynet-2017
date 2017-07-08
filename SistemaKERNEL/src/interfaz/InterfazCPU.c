#include "InterfazCPU.h"

#include <commons/collections/list.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Sharedlib/Socket.h>
#include <unistd.h>

#include "../administrarProcesos/Proceso.h"
#include "../capaFILESYSTEM/GestionArchivoFuncAux.h"
#include "../capaMEMORIA/AdministrarVarCompartidas.h"
#include "../capaMEMORIA/GestionMemoriaDinamica.h"
#include "../general/Semaforo.h"
#include "../header/AppConfig.h"
#include "../header/Estructuras.h"
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
			informar_handshake_log("CPU");
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
		}
		else if (strcmp(codigo_operacion, "QUANTUM") == 0) {
			if(strcmp(recibir_dato_serializado(clienteCPU), "RECIBIR_PCB") == 0)
				recibir_PCB_de_CPU(clienteCPU, "QUANTUM");
		}
		else if (strcmp(codigo_operacion, "TERMINADO") == 0) {
			if(strcmp(recibir_dato_serializado(clienteCPU), "RECIBIR_PCB") == 0)
				recibir_PCB_de_CPU(clienteCPU, "TERMINADO");
		}
		else if(strcmp(codigo_operacion, "ERROR") == 0) {

		}
		else if(strcmp(codigo_operacion, "WAIT_SEM") == 0){
			if(strcmp(recibir_dato_serializado(clienteCPU), "RECIBIR_PCB") == 0)
				recibir_PCB_de_CPU(clienteCPU, "WAIT_SEM");
		}
		else if (strcmp(codigo_operacion, "SIGNAL_SEM")== 0) {
			char * nombre_sem = recibir_dato_serializado(clienteCPU);


			int res = recepcion_SIGNAL_semaforo_ansisop(nombre_sem);
			if(res == -2)
				enviar_dato_serializado("SEMAFORO_NO_EXISTE", clienteCPU);
			else
				enviar_dato_serializado("SEMAFORO_OK", clienteCPU);
		}
		else if (strcmp(codigo_operacion, "ASIGNAR_VAR_COMP") == 0) {
			asignar_valor_var_comp(clienteCPU);
		}
		else if (strcmp(codigo_operacion, "BUSCAR_VAL_VAR_COMP") == 0) {
			obtener_valor_var_comp(clienteCPU);
		}
		else if (strcmp(codigo_operacion, "ABRIR_ARCHIVO") == 0) {
			abrir_archivo(clienteCPU);
		}
		else if (strcmp(codigo_operacion, "MOVER_CURSOR_ARCHIVO") == 0) {
			mover_cursor_archivo(clienteCPU);
		}
		else if (strcmp(codigo_operacion, "LEER_ARCHIVO") == 0) {
			leer_archivo(clienteCPU);
		}
		else if (strcmp(codigo_operacion, "CERRAR_ARCHIVO") == 0) {
			cerrar_archivo(clienteCPU);
		}
		else if (strcmp(codigo_operacion, "ESCRIBIR_ARCHIVO") == 0) {
			escribir_archivo(clienteCPU);
		}
		else if (strcmp(codigo_operacion, "BORRAR_ARCHIVO") == 0) {
			borrar_archivo(clienteCPU);
		}
		else if (strcmp(codigo_operacion, "IMPRIMIR_POR_PANTALLA") == 0) {
			int proceso = atoi(recibir_dato_serializado(clienteCPU));
			char *mensaje = recibir_dato_serializado(clienteCPU);
			CU_Atender_Solicitud_Escritura_Por_Pantalla(clienteCPU,proceso,mensaje);
		}
		else if(strcmp(codigo_operacion, "DESCONEXION_PROXIMA") == 0){
			retirar_CPU_global(clienteCPU);
		}else if(strcmp(codigo_operacion, "DESCONECTAR") == 0) {
			limpiar_procesos_CPU(clienteCPU, "REPLANIFICAR");
			retirar_CPU_global(clienteCPU);
			controlSeguir = 0;
		}else if(strcmp(codigo_operacion, "") == 0) {
			limpiar_procesos_CPU(clienteCPU, "ELIMINAR");
			retirar_CPU_global(clienteCPU);
			controlSeguir = 0;
		}else {
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
	sem_wait(&mutex_lista_CPUs);
	while (aux = list_get(lista_CPUs, i)) {
		if (aux->numeroConexion == numeroConexion){
			sem_post(&mutex_lista_CPUs);
			return i;
		}
		i++;
	}
	sem_post(&mutex_lista_CPUs);
	return -1;
}

void agregar_CPU_global(int numeroConexion, pthread_t hilo) {
	CPUInfo * cpu = malloc(sizeof(CPUInfo));
	cpu->numeroConexion = numeroConexion;
	cpu->hilo = hilo;
	marcar_CPU_Disponible(cpu);
	sem_wait(&mutex_lista_CPUs);
	list_add(lista_CPUs, cpu);
	sem_post(&mutex_lista_CPUs);
}

void retirar_CPU_global(int numeroConexion) {
	int index = index_of_CPU(numeroConexion);
	if(index == -1) return;
	printf("Se elimino a CPU en posicion %d de la lista\n", index);
	sem_wait(&mutex_lista_CPUs);
	list_remove(lista_CPUs, index);
	sem_post(&mutex_lista_CPUs);
}

CPUInfo * obtener_CPU(int numeroConexion){
	int index = index_of_CPU(numeroConexion);
	sem_wait(&mutex_lista_CPUs);
	CPUInfo * cpu = list_get(lista_CPUs, index);
	sem_post(&mutex_lista_CPUs);
	return cpu;
}

// En esta funcion se usan funciones de Proceso.c. Tal vez falte incluir acceso. Revisar funcion atoi.
void CU_Atender_Solicitud_Escritura_Por_Pantalla(int cliente_CPU, uint32_t PID, char * mensaje) {
	Proceso* procesoSolicitante = buscar_proceso_by_PID(PID);
	int socketDelProceso = procesoSolicitante->socket;
	enviar_dato_serializado(mensaje, socketDelProceso);
}
