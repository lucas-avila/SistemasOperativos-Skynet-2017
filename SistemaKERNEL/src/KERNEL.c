#include <commons/collections/list.h>
#include <commons/collections/queue.h>
#include <commons/string.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "administrarPCB/EstadisticaProceso.h"
#include "capaFILESYSTEM/TablaGlobalArchivo.h"
#include "capaMEMORIA/GestMemoriaFuncionesAux.h"
#include "general/funcionesUtiles.h"
#include "general/Socket.h"
#include "header/AppConfig.h"
#include "header/Estructuras.h"
#include "header/PCB.h"
#include "interfaz/InterfazConsola.h"
#include "interfaz/InterfazCPU.h"
#include "interfaz/InterfazMemoria.h"
#include "planificacion/Planificacion.h"
#include "testing/TestingInterfazMemoria.h"

void inicializar_listas_globales();
void atender_solicitudes_de_usuario();

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
	//   PLanificacion
	inicializar_colas_5_estados();
	atender_clientes(0, &EJECUTAR_ALGORITMO_PLANIFICACION);
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
		case 6: {
//			IndiceCodigo * in1 = malloc(sizeof(IndiceCodigo));
//			IndiceCodigo * in2 = malloc(sizeof(IndiceCodigo));
//			in1->program_counter = 15;
//			in1->byte_inicial_codigo = 2;
//			in1->byte_final_codigo = 355;
//			in1->pagina = 4;
//			in2->program_counter = 14;
//			in2->byte_inicial_codigo = 3;
//			in2->byte_final_codigo = 356;
//			in2->pagina = 5;
//			t_list * lista = list_create();
//			list_add(lista, in1);
//			list_add(lista, in2);
//
//			char * buffer = serializar_con_header(lista, "LISTA_CODIGO");
//
//			t_list * lista_deserializada = deserializar_con_header(buffer, "LISTA_CODIGO");
//			IndiceCodigo * elemento = list_get(lista_deserializada, 0);
//			printf("Resultado : %d, %d, %d, %d \n", elemento->program_counter, elemento->byte_inicial_codigo, elemento->byte_final_codigo, elemento->pagina);
//			elemento = list_get(lista_deserializada, 1);
//			printf("Resultado : %d, %d, %d, %d \n", elemento->program_counter, elemento->byte_inicial_codigo, elemento->byte_final_codigo, elemento->pagina);

			ReturnVariable * retVar = malloc(sizeof(ReturnVariable));
			retVar->byte_inicial = 2;
			retVar->pagina = 9;
			retVar->tamanio = 80;

			t_list * l_args = list_create();
			Argumento * arg1 = malloc(sizeof(Argumento));
			arg1->id = 1;
			arg1->pagina = 2;
			arg1->byte_inicial = 3;
			arg1->tamanio = 4;
			list_add(l_args, arg1);

			t_list * l_vars = list_create();
			Variable * var1 = malloc(sizeof(Variable));
			var1->id = 1;
			var1->pagina = 2;
			var1->byte_inicial = 3;
			var1->tamanio = 4;
			list_add(l_vars, var1);

			IndiceStack * in = malloc(sizeof(IndiceStack));
			in->posicion = 5;
			in->argumentos = l_args;
			in->variables = l_vars;
			in->retPos = 6;
			in->retVar = retVar;

			t_list * lista = list_create();
			list_add(lista, in);
			list_add(lista, in);
			list_add(lista, in);


			LISTA_SERIALIZADA * buffer = serializar_con_header(lista, "LISTA_STACK");
			t_list * lista_des = deserializar_con_header(buffer->buffer, "LISTA_STACK");
			IndiceStack * e = malloc(sizeof(IndiceStack));
			int i=0;
			for(i; i < list_size(lista_des); i++){
				e = list_get(lista_des, i);
				printf("Elemento %d del stack:\n posicion: %d\n", i, e->posicion);
				int x=0;
				for(x; x < list_size(e->argumentos); x++){
					Argumento * a = list_get(e->argumentos, x);
					printf("argumentos:\n--id:%d\n--pagina:%d\n--byte_inicial:%d\n--tamanio:%d\n", a->id, a->pagina, a->byte_inicial, a->tamanio);
				}
				for(x=0; x < list_size(e->variables); x++){
					Variable * v = list_get(e->variables, x);
					printf("variables:\n--id:%d\n--pagina:%d\n--byte_inicial:%d\n--tamanio:%d\n", v->id, v->pagina, v->byte_inicial, v->tamanio);
				}
				printf("retPos: %d\nretVar:\n--byte_inicial:%d\n--pagina:%d\n--tamanio:%d\n", e->retPos, e->retVar->byte_inicial, e->retVar->pagina, e->retVar->tamanio);
			}
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

