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

	int resultado = enviar_programa_memoria(codigo, pcb_nuevo->PID);
	/* La variable RESULTADO es para saber si se le pudo
	 * asignar memoria o no. En el caso de que SI el
	 * resultado va a ser mayor a 0 y se utilizará para
	 * actualizar el valor de cantidad_paginas del PCB.
	 * En el caso de que NO, entonces su valor sera menor
	 * a 0 y se empleara como EXIT_CODE.
	 */

	enviar_dato_serializado(pcb_nuevo->PID, consola);
	if (resultado > 0) {
		pcb_nuevo->cantidad_paginas_codigo = resultado;
		//pcb_nuevo->info_codigo = metadata_desde_literal(codigo);
		/*Info_codigo va a almacenar toda la informacion util del codigo
		 * como por ejemplo cantidad de etiquetas, de funciones,
		 * las etiquetas, el puntero de inicio, etc. Ver en libreria del
		 * parser.
		 */
		//FALTA agregar el PCB a la lista NEW.
		printf("--El PCB fue creado exitosamente--\n");
		//--PROBLEMA-- El metada_programa.h no implementa las funciones de su .c.
	} else {
		enviar_dato_serializado("FIN_PROGRAMA", consola);
		enviar_dato_serializado(string_itoa(resultado), consola);
	}
}

