#include <commons/collections/list.h>
#include <commons/collections/queue.h>
#include <commons/string.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../administrarPCB/EstadisticaProceso.h"
#include "../administrarPCB/PCBData.h"
#include "../capaFILESYSTEM/TablaGlobalArchivo.h"
#include "../capaMEMORIA/GestMemoriaFuncionesAux.h"
#include "../general/funcionesUtiles.h"
#include "../general/Socket.h"
#include "../header/AppConfig.h"
#include "../header/Estructuras.h"
#include "../header/PCB.h"
#include "../interfaz/InterfazConsola.h"
#include "../interfaz/InterfazCPU.h"
#include "../interfaz/InterfazMemoria.h"
#include "../planificacion/Planificacion.h"

#include "../administrarProcesos/Proceso.h"

void mostrar_menu_testing_general() {
	int opcion = 0;
	do {

		printf("\n1 - Crear Proceso");
		printf("\n2 - Serializar PCB");
		printf("\n3 - Agregar CPU y CONSOLA en tabla GLOBAL");
		//printf("\n4 - Testear COSAS GENERALES");
		printf("\n6 - Enviar PCB hardcodeado a CPU(tiene q estar conectada)");
		printf("\n7 - Enviar programa a procesar_programa");
		printf("\n8 - Salir");
		printf("\nOpcion: ");

		opcion = validarNumeroInput(1, 8);
		switch (opcion) {

		case 1: {
			//testeando crear PCB
			PCB * pcb_nuevo = crear_pcb();

			//JONY: esto lo agrego para probar la planificacion
			queue_push(COLA_NEW, pcb_nuevo);
			crear_Proceso_en_tabla(string_itoa(pcb_nuevo->PID));
			Proceso* proc = new_Proceso(pcb_nuevo);
			agregar_proceso(proc);

			printf("PCB creado, PID es : %d\n", pcb_nuevo->PID);
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

			agregar_consola_global(7);
			ConsolaInfo * consola = list_get(lista_consolas, 0);
			printf("ConsolaInfo es %d\n", consola->consola_ID);

		}

			break;
		case 4: {
			CPUInfo * cpu = list_get(lista_CPUs, 0);
			printf("CPUInfo es %d, %d\n", cpu->numeroConexion, cpu->disponible);
		}

			break;
		case 5: {
			PCB * pcb = hardcodear_pcb();
			enviar_pcb_deb(pcb);
		}
		case 6: {
			PCB * pcb = hardcodear_pcb();
			CPUInfo * cpu0 = list_get(lista_CPUs, 0);
			enviar_pcb(pcb, cpu0->numeroConexion);
		}
			break;
		case 7: {
			char * codigo = obtener_codigo("/home/utnso/Escritorio/tp-2017-1c-Skynet/SistemaConsola/resource/completo.ansisop");
			PCB * pcb = crear_pcb();
			procesar_programa(codigo, pcb);
			int debugeame = 111;
		}
			break;

		}
	} while (opcion != 8);
}
