/*
 ============================================================================
 Name        : KERNEL.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <semaphore.h>
#include <signal.h>

#include <commons/string.h>

#include "general/Socket.h"
#include "header/SolicitudesUsuario.h"

#include "header/PCB.h"
#include "header/Estructuras.h"
#include "header/AppConfig.h"

#include "capaMEMORIA/GestMemoriaFuncionesAux.h"

#include "general/funcionesUtiles.h"
#include "interfaz/InterfazMemoria.h"
#include "interfaz/InterfazConsola.h"
#include "testing/TestingInterfazMemoria.h"
//#include "parser/metadata_program.h"

#include "planificacion/Planificacion.h"
#include "administrarPCB/EstadisticaProceso.h"

void inicializar_listas_globales();
void atender_solicitudes_de_usuario();

void escuchar_Conexiones_CPU(int servidorCPU);
void escuchar_Conexiones_Consola(int servidorConsola);
void CU_iniciar_programa(int consola);

int main(int argc, char *argv[]) {
	inicializar_configuracion(argv[1]);
	inicializar_listas_globales();
	inicializar_tabla_proceso_estadistica();

	inicializar_tabla_proceso_memoria();

	int servidor_Consola = crear_servidor(configuraciones.PUERTO_PROG, configuraciones.CANTIDAD_MAXIMA_CONCURRENCIA);
	atender_clientes(servidor_Consola, &escuchar_Conexiones_Consola); // asincronico - multihilo

	iniciar_conexion_servidor_cpu();

	iniciar_conexion_servidor_memoria();

	//PLanificacion
	inicializar_colas_5_estados();
	atender_clientes(0, &EJECUTAR_ALGORITMO_PLANIFICACION);

	atender_solicitudes_de_usuario();

	return EXIT_SUCCESS;
}

void mostrar_menu_usuario() {
	printf("\n******* MENU KERNEL ******");
	printf("\n 1 - Obtener listado de procesos del Sistema.");
	printf("\n 2 - Obtener Informacion de un proceso");
	printf("\n 3 - Obtener la tabla global de archivos");
	printf("\n 4 - Modificar grado de multiprogramacion");
	printf("\n 5 - Finalizar proceso");
	printf("\n 6 - Detener la planificacion");
	printf("\n 7 - Salir");
	printf("\n Opcion: ");
}

void atender_solicitudes_de_usuario() {
	int opcion = 0;
	do {
		mostrar_menu_usuario();
		opcion = validarNumeroInput(1, 10);
		switch (opcion) {

		case 1: {
			//testeando crear PCB
			PCB * pcb_nuevo = crear_pcb();

			//JONY: esto lo agrego para probar la planificacion
			queue_push(COLA_NEW, pcb_nuevo);
			crear_Proceso_en_tabla(pcb_nuevo->PID);

			printf("PCB creado, PID es : %s\n", pcb_nuevo->PID);
			break;
		}
		case 2: {
			//testeando serializador con header
			IndiceCodigo * in1 = malloc(sizeof(IndiceCodigo));
			IndiceCodigo * in2 = malloc(sizeof(IndiceCodigo));
			in1->program_counter = 1;
			in1->byte_inicial_codigo = 2;
			in1->byte_final_codigo = 3;
			in1->pagina = 4;
			in2->program_counter = 5;
			in2->byte_inicial_codigo = 6;
			in2->byte_final_codigo = 7;
			in2->pagina = 8;
			t_list * lista = list_create();
			list_add(lista, in1);
			list_add(lista, in2);
			char * result = serializar_con_header(lista, "LISTA_CODIGO");

			deserializar_con_header(result, "LISTA_CODIGO");
			break;
		}
		case 3: {
			//testeando agregar cpu a lista global
			pthread_t th;
			int i = 4;
			agregar_CPU_global(i, th);
			CPUInfo * cpu = list_get(lista_CPUs, 0);
			printf("CPUInfo es %d, %d\n", cpu->numeroConexion, cpu->disponible);

			agregar_consola_global(7, th);
			ConsolaInfo * consola = list_get(lista_consolas, 0);
			printf("ConsolaInfo es %d\n", consola->numeroConexion);

		}

			break;
		case 4: {
			CPUInfo * cpu = list_get(lista_CPUs, 0);
			printf("CPUInfo es %d, %d\n", cpu->numeroConexion, cpu->disponible);
		}

			break;
		case 5: {
			CPUInfo * cpu = list_get(lista_CPUs, 1);
			printf("CPUInfo es %d, %d\n", cpu->numeroConexion, cpu->disponible);
		}

			break;
		case 6:{
			int i = 2;
			int x = 2000;
			IndiceCodigo * in1 = malloc(sizeof(IndiceCodigo));
			IndiceCodigo * in2 = malloc(sizeof(IndiceCodigo));
			in1->program_counter = 13;
			in1->byte_inicial_codigo = 2;
			in1->byte_final_codigo = 355;
			in1->pagina = 4;
			char * Buffer = malloc(sizeof(char) * sizeof(IndiceCodigo));
			int offset = 0;
			memcpy(Buffer + offset, &in1->program_counter, sizeof(in1->program_counter));
			offset += sizeof(in1->program_counter);
			memcpy(Buffer + offset, &in1->byte_inicial_codigo, sizeof(in1->byte_inicial_codigo));
			offset += sizeof(in1->byte_inicial_codigo);
			memcpy(Buffer + offset, &in1->byte_final_codigo, sizeof(in1->byte_final_codigo));
			offset += sizeof(in1->byte_final_codigo);
			memcpy(Buffer + offset, &in1->pagina, sizeof(in1->pagina));
			offset += sizeof(in1->pagina);

			IndiceCodigo * resultado = malloc(sizeof(IndiceCodigo));
			memcpy(resultado, Buffer, sizeof(IndiceCodigo));
			printf("Resultado str : %s\n", Buffer);
			printf("Resultado : %d, %d, %d, %d \n", resultado->program_counter, resultado->byte_inicial_codigo, resultado->byte_final_codigo, resultado->pagina);
		}

			break;
		case 8:
			//provisorio para el checkpoint
			CU_iniciar_programa(conectar_servidor(configuraciones.IP_FS, configuraciones.PUERTO_FS));
			break;
		case 10:
			testear_intefaz();
			break;

		}
	} while (opcion != 7);
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

