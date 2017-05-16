#include <commons/collections/list.h>
#include <commons/collections/queue.h>
#include <commons/string.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "administrarPCB/EstadisticaProceso.h"
#include "administrarPCB/PCBData.h"
#include "capaFILESYSTEM/TablaGlobalArchivo.h"
#include "capaMEMORIA/GestMemoriaFuncionesAux.h"

#include "parser/metadata_program.h"
#include "general/Socket.h"
#include "header/AppConfig.h"
#include "header/Estructuras.h"
#include "header/PCB.h"
#include "interfaz/InterfazConsola.h"
#include "interfaz/InterfazCPU.h"
#include "interfaz/InterfazMemoria.h"
#include "planificacion/Planificacion.h"

#include "administrarProcesos/Proceso.h"

#include "header/SolicitudesUsuario.h"
void inicializar_listas_globales();

void CU_iniciar_programa(int consola);

void inicializar_KERNEL();

int main(int argc, char *argv[]) {
	inicializar_configuracion(argv[1]);
	//inicializar_configuracion("/home/utnso/Escritorio/tp-2017-1c-Skynet/SistemaKERNEL/resource/config.cfg");
	inicializar_KERNEL();
	iniciar_conexion_servidor_consola();
	iniciar_conexion_servidor_cpu();
	iniciar_conexion_servidor_memoria();
//	iniciar_conexion_servidor_FS();
	atender_solicitudes_de_usuario();
	return EXIT_SUCCESS;
}

void inicializar_KERNEL() {
	inicializar_listas_globales();
	inicializar_tabla_proceso_estadistica();
	inicializar_tabla_proceso_memoria();
	inicializar_tabla_global_archivo();
	inicializar_lista_procesos();
	//   PLanificacion
	inicializar_colas_5_estados();
	atender_clientes(0, &EJECUTAR_ALGORITMO_PLANIFICACION);
}

void inicializar_listas_globales() {
	lista_consolas = list_create();
	lista_CPUs = list_create();
}

void CU_iniciar_programa(int consola) {
	char * codigo = recibir_dato_serializado(consola);
	PCB * pcb_nuevo = crear_pcb();
	Proceso * proceso_nuevo = new_Proceso(pcb_nuevo);

	list_add(procesos, proceso_nuevo);
	int resultado = enviar_programa_memoria(codigo, string_itoa(pcb_nuevo->PID));
	//TODO enviar_programa_memoria deberia devolver algo que no sea 1 .-.
	/* La variable RESULTADO es para saber si se le pudo
	 * asignar memoria o no. En el caso de que SI el
	 * resultado va a ser mayor a 0 y se utilizará para
	 * actualizar el valor de cantidad_paginas del PCB.
	 * En el caso de que NO, entonces su valor sera menor
	 * a 0 y se empleara como EXIT_CODE.
	 */

	enviar_dato_serializado(string_itoa(pcb_nuevo->PID), consola);
	if (resultado > 0) {
		t_metadata_program * info_codigo = metadata_desde_literal(codigo);
		llenar_PCB(proceso_nuevo, resultado, info_codigo, consola);
		printf("--El PCB fue creado exitosamente--\n");
	} else {
		notificar_exit_code(resultado, consola);
	}
}

void llenar_PCB(Proceso * proceso, int paginas_codigo, t_metadata_program * info_codigo, int consola){

	proceso->consola = consola;
	proceso->pcb->cantidad_paginas_codigo = paginas_codigo;
	//FALTA TERMINAR

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

Proceso * actualizar_exit_code(int exit_code, int pid){
	Proceso * proceso_a_actualizar;
	int buscar_proceso(Proceso * proceso){
		return proceso->PID == pid;
	}
	proceso_a_actualizar = list_find(procesos, &buscar_proceso);
	list_remove_by_condition(procesos, &buscar_proceso);
	proceso_a_actualizar->pcb->exit_code = exit_code;

	return proceso_a_actualizar;
}

void notificar_exit_code(int exit_code, int consola){
	enviar_dato_serializado("FIN_PROGRAMA", consola);
	enviar_dato_serializado(string_itoa(exit_code), consola);
}
