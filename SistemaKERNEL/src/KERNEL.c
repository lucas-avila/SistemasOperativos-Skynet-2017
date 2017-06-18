#include <commons/collections/list.h>
#include <commons/string.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "administrarPCB/EstadisticaProceso.h"
#include "administrarPCB/PCBData.h"
#include "administrarProcesos/Proceso.h"
#include "capaFILESYSTEM/TablaGlobalArchivo.h"
#include "capaMEMORIA/GestMemoriaFuncionesAux.h"
#include "general/Semaforo.h"
#include "../Sharedlib/Sharedlib/Socket.h"
#include "header/AppConfig.h"
#include "header/Estructuras.h"
#include "header/PCB.h"
#include "header/SolicitudesUsuario.h"
#include "interfaz/InterfazConsola.h"
#include "interfaz/InterfazCPU.h"
#include "interfaz/InterfazFS.h"
#include "interfaz/InterfazMemoria.h"
#include "planificacion/Planificacion.h"

void inicializar_listas_globales();
void inicializar_semaforos();

void CU_iniciar_programa(int consola);

void inicializar_KERNEL();

int main(int argc, char *argv[]) {
	inicializar_configuracion(argv[1]);
	//inicializar_configuracion("..//resource//config.cfg");
	inicializar_KERNEL();
	iniciar_conexion_servidor_programa();
	iniciar_conexion_servidor_cpu();
	iniciar_conexion_servidor_memoria();
	inicializar();//sacar esto despues ESPARA PROBAR FILE SYSTEM
//	iniciar_conexion_servidor_FS();
	atender_solicitudes_de_usuario();
	return EXIT_SUCCESS;
}

void inicializar_KERNEL() {
	inicializar_semaforos();
	inicializar_listas_globales();
	inicializar_tabla_proceso_estadistica();
	inicializar_tabla_proceso_memoria();
	inicializar_tabla_global_archivo();
	inicializar_lista_procesos();
	//   PLanificacion
	inicializar_colas_5_estados();
	atender_clientes(0, &EJECUTAR_ALGORITMO_PLANIFICACION);

	info_log = string_new();
}

void inicializar_semaforos(){
	inicializar_semaforo(&mutex_pids);
	inicializar_semaforo(&mutex_lista_PROCESOS);
	inicializar_semaforo(&mutex_lista_CPUs);
	//PLANIFICACION
	inicializar_semaforo(&mutex_cola_READY);
	//OTROS
	inicializar_semaforo(&mutex_memoria);
}

void inicializar_listas_globales() {
	lista_consolas = list_create();
	lista_CPUs = list_create();
	inicializar_vec_variables_compartidas();
	inicializar_dict_semaforos_ansisop();
}

void CU_iniciar_programa(int programa_socket) {
	char * codigo = recibir_dato_serializado(programa_socket);
	PCB * pcb_nuevo = crear_pcb();
	procesar_programa(codigo, pcb_nuevo); //aca adentro se llena el pcb y se envia el programa a memoria

	Proceso * proceso_nuevo = new_Proceso(pcb_nuevo);
	crear_Proceso_en_tabla(proceso_nuevo->PID);
	proceso_nuevo->socket = programa_socket;
	agregar_proceso(proceso_nuevo);

	proceso_a_NEW(proceso_nuevo);

	enviar_dato_serializado(string_itoa(pcb_nuevo->PID), programa_socket);
}

void finalizar_proceso(Proceso * proceso){

	sem_wait(&mutex_memoria);
	char * respuesta = finalizar_Programa_memoria(string_itoa(proceso->PID));
	sem_post(&mutex_memoria);
	if (strcmp(respuesta, "OK") == 0) {

		int buscar_proceso(Proceso * elem_proceso){
			return elem_proceso->PID == proceso->PID;
		}
		sem_wait(&mutex_lista_PROCESOS);
		list_remove_by_condition(procesos, &buscar_proceso);
		sem_post(&mutex_lista_PROCESOS);
		notificar_exit_code(proceso->pcb->exit_code, proceso->socket);
		close(proceso->socket);
		free(proceso);
	} else {
		actualizar_exit_code(proceso, -10);
		notificar_exit_code(proceso->pcb->exit_code, proceso->socket);
		//TODO ver a dónde mandamos el proceso si sucede esto...aunque no debería suceder.
	}

}

void actualizar_exit_code(Proceso * proceso, int exit_code){

	proceso->pcb->exit_code = exit_code;
}

void notificar_exit_code(int exit_code, int socket){
	enviar_dato_serializado("FIN_PROGRAMA", socket);
	enviar_dato_serializado(string_itoa(exit_code), socket);
}
