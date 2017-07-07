#include <commons/collections/list.h>
#include <commons/string.h>
#include <semaphore.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Sharedlib/PCB.h>
#include <Sharedlib/Socket.h>
#include <unistd.h>

#include "administrarPCB/EstadisticaProceso.h"
#include "administrarPCB/PCBData.h"
#include "administrarProcesos/Proceso.h"
#include "capaFILESYSTEM/TablaGlobalArchivo.h"
#include "capaMEMORIA/GestMemoriaFuncionesAux.h"
#include "general/Semaforo.h"
#include "header/AppConfig.h"
#include "header/Estructuras.h"
#include "header/SolicitudesUsuario.h"
#include "interfaz/InterfazConsola.h"
#include "interfaz/InterfazCPU.h"
#include "interfaz/InterfazFS.h"
#include "interfaz/InterfazMemoria.h"
#include "planificacion/Planificacion.h"

void inicializar_KERNEL();

int main(int argc, char *argv[]) {
	inicializar_configuracion(argv[1]);
	//inicializar_configuracion("..//resource//config.cfg");
	inicializar_KERNEL();
	iniciar_conexion_servidor_programa();
	iniciar_conexion_servidor_cpu();
	iniciar_conexion_servidor_memoria();
	//inicializar();//sacar esto despues ESPARA PROBAR FILE SYSTEM
	int conectado_FS = iniciar_conexion_servidor_FS();
	if(!conectado_FS) printf("\n\t --> FILESYSTEM SIN CONECTAR\n");
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
	inicializar_semaforo(&mutex_pids, 1);
	inicializar_semaforo(&mutex_lista_PROCESOS, 1);
	inicializar_semaforo(&mutex_lista_CPUs, 1);
	//PLANIFICACION
	inicializar_semaforo(&mutex_cola_NEW, 1);
	inicializar_semaforo(&mutex_cola_READY, 1);
	inicializar_semaforo(&mutex_cola_EXEC, 1);
	inicializar_semaforo(&mutex_cola_WAITING, 1);
	inicializar_semaforo(&mutex_cola_EXIT, 1);

	inicializar_semaforo(&grado_multiprogramacion, configuraciones.GRADO_MULTIPROG);
	inicializar_semaforo(&cpu_disponible, 0);
	inicializar_semaforo(&proceso_new, 0);
	inicializar_semaforo(&proceso_ready, 0);
	//OTROS
	inicializar_semaforo(&mutex_memoria, 1);
	inicializar_semaforo(&mutex_tabla_estadistica, 1);
	inicializar_semaforo(&mutex_tabla_estadistica_busqueda, 1);
	inicializar_semaforo(&buffer_codigo, 1);
	inicializar_semaforo(&escribir_log, 1);
}

void inicializar_listas_globales() {
	lista_consolas = list_create();
	lista_CPUs = list_create();
	inicializar_vec_variables_compartidas();
	inicializar_dict_semaforos_ansisop();
	inicializar_buffer_codigo();
}

void CU_iniciar_programa(int programa_socket) {
	char * codigo = recibir_dato_serializado(programa_socket);
	PCB * pcb_nuevo = crear_pcb();
	Proceso * proceso_nuevo = new_Proceso(pcb_nuevo);

	sem_wait(&buffer_codigo);
	dictionary_put(BUFFER_CODIGO, string_itoa(pcb_nuevo->PID), codigo);
	sem_post(&buffer_codigo);

<<<<<<< HEAD
	crear_Proceso_en_tabla(proceso_nuevo->PID);
	proceso_nuevo->socket = programa_socket;
	agregar_proceso(proceso_nuevo);
=======
	agregar_proceso(proceso_nuevo);
	crear_Proceso_en_tabla(proceso_nuevo->PID);
	proceso_nuevo->socket = programa_socket;
>>>>>>> f617bed21251d98d85e5040cd6193ea58c7e8e5b
	proceso_a_NEW(proceso_nuevo);

	char * pid = string_itoa(pcb_nuevo->PID);
	enviar_dato_serializado(pid, programa_socket);
	free(pid);
}

void 	mostrar_por_pantalla_memory_leaks(uint32_t PID){
	EstadisticaProceso* estadistica = buscar_registro_por_PID(PID);
	printf("\n Ha finalizado el proceso PID %d ",PID);
	int diferencia = estadistica->tamanio_Total_Alocar == estadistica->tamanio_Total_Liberar;

	if(estadistica->tamanio_Total_Alocar==0){
		return;
	}

	if(diferencia==0){
		printf("\n El proceso ha liberado toda la memoria alocada.");
	}else{
		printf("\n El proceso no ha liberado toda la memoria alocada.");
	}
}

void finalizar_proceso(Proceso * proceso){

	sem_wait(&mutex_memoria);
	char * respuesta = finalizar_Programa_memoria(string_itoa(proceso->PID));
	sem_post(&mutex_memoria);

	mostrar_por_pantalla_memory_leaks(proceso->PID);

	if (strcmp(respuesta, "OK") == 0) {
		proceso->activo = 0;
		notificar_exit_code(proceso->pcb->exit_code, proceso->socket);
		close(proceso->socket);
	}

}

void actualizar_exit_code(Proceso * proceso, int exit_code){
	proceso->pcb->exit_code = exit_code;
}
