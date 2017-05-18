#include <commons/collections/list.h>
#include <commons/string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "administrarPCB/EstadisticaProceso.h"
#include "administrarPCB/PCBData.h"
#include "administrarProcesos/Proceso.h"
#include "capaFILESYSTEM/TablaGlobalArchivo.h"
#include "capaMEMORIA/GestMemoriaFuncionesAux.h"
#include "general/Semaforo.h"
#include "general/Socket.h"
#include "header/AppConfig.h"
#include "header/Estructuras.h"
#include "header/PCB.h"
#include "header/SolicitudesUsuario.h"
#include "interfaz/InterfazConsola.h"
#include "interfaz/InterfazCPU.h"
#include "interfaz/InterfazMemoria.h"
#include "planificacion/Planificacion.h"

void inicializar_listas_globales();
void inicializar_semaforos();

void CU_iniciar_programa(int consola);

void inicializar_KERNEL();

int main(int argc, char *argv[]) {
	//inicializar_configuracion(argv[1]);
	inicializar_configuracion("/home/utnso/Escritorio/tp-2017-1c-Skynet/SistemaKERNEL/resource/config.cfg");
	inicializar_KERNEL();
	iniciar_conexion_servidor_consola();
	iniciar_conexion_servidor_cpu();
	iniciar_conexion_servidor_memoria();
//	iniciar_conexion_servidor_FS();
	atender_solicitudes_de_usuario();
	return EXIT_SUCCESS;
}

void inicializar_KERNEL() {
	//inicializar_semaforos();
	inicializar_listas_globales();
	inicializar_tabla_proceso_estadistica();
	inicializar_tabla_proceso_memoria();
	inicializar_tabla_global_archivo();
	inicializar_lista_procesos();
	//   PLanificacion
	inicializar_colas_5_estados();
	atender_clientes(0, &EJECUTAR_ALGORITMO_PLANIFICACION);
}

void inicializar_semaforos(){
	inicializar_semaforo(mutex_pids);
}

void inicializar_listas_globales() {
	lista_consolas = list_create();
	lista_CPUs = list_create();
}

void CU_iniciar_programa(int consola) {
	char * codigo = recibir_dato_serializado(consola);
	PCB * pcb_nuevo = crear_pcb();
	procesar_programa(codigo, pcb_nuevo); //aca adentro se llena el pcb y se envia el programa a memoria

	Proceso * proceso_nuevo = new_Proceso(pcb_nuevo);

	list_add(procesos, proceso_nuevo);


	enviar_dato_serializado(string_itoa(pcb_nuevo->PID), consola);
}

void finalizar_proceso(Proceso * proceso){

	char * respuesta = finalizar_Programa_memoria(string_itoa(proceso->PID));
	// TODO finalizar_programa_memoria no devuelve OK?
	if (strcmp(respuesta, "OK") == 0) {

		notificar_exit_code(proceso->pcb->exit_code, proceso->consola);
	} else {
		printf("No se pudo finalizar el programa\n");
		//TODO ver qué mierda hacemos acá...
	}

}

/* Se asume que cada vez que se quiera terminar un proceso se
 * actualizará previamente su exit_code y luego se llamará a
 * la función para finalizar el proceso. Al actualizar el exit_code
 * borraremos directamente el proceso de la lista de procesos
 * (donde se encuentran todos ellos) y retornaremos el Proceso.
 */

void actualizar_exit_code(Proceso * proceso, int exit_code){

	int buscar_proceso(Proceso * elem_proceso){
		return elem_proceso->PID == proceso->PID;
	}
	list_remove_by_condition(procesos, &buscar_proceso);
	proceso->pcb->exit_code = exit_code;

}

void notificar_exit_code(int exit_code, int consola){
	enviar_dato_serializado("FIN_PROGRAMA", consola);
	enviar_dato_serializado(string_itoa(exit_code), consola);
}
