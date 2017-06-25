#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include "parser/parser.h"

#include "commons/string.h"

#include "Ejecucion.h"

#include "../../../Sharedlib/Sharedlib/PCB.h"
#include "../primitivas/PrimitivasFunciones.h"
#include "../primitivas/IndiceCodigo.h"
#include "../interfaz/InterfazKernel.h"
#include "../interfaz/InterfazMemoria.h"

void setPCBEjecucion(PCB* pcb) {
	pcbEjecutar = pcb;
	setearPCB(pcb);
}
char* solicitar_sentencia_ejecutar() {
	IndiceCodigo* indiceCodigo = obtener_Indice_codigo(pcbEjecutar->codigo, pcbEjecutar->program_counter);
	if (indiceCodigo == NULL) {
		return "FIN";
	} else {

		int longitud = indiceCodigo->byte_final_codigo - indiceCodigo->byte_inicial_codigo;
		return solicitar_bytes_memoria(string_itoa(pcbEjecutar->PID), string_itoa(indiceCodigo->pagina), string_itoa(indiceCodigo->byte_inicial_codigo), string_itoa(longitud));
	}
}


long retardo;
void ejecutar_Programa() {
	esFinPrograma = false;
	programaBloqueado = false;
	retardo= 	pcbEjecutar->quantum_sleep * 1000000 ;
	bool esRoundRobin = (pcbEjecutar->RR == 1);
	if (!esRoundRobin) {
		ejecutar_programa_por_FIFO();
	} else {
		ejecutar_programa_por_RR();
	}

}

void ejecutar_programa_por_FIFO() {

	printf("\n Comienzo de Ejecucion");
	while (!esFinPrograma && !programaBloqueado) {
		printf("\n Comienzo de Solicitar sentencia...");
		char* sentencia = solicitar_sentencia_ejecutar();
		printf("\n Sentencia SOlicitada %s: ", sentencia);
		esFinPrograma = (strcmp(sentencia, "FIN") == 0);
		if (!esFinPrograma && !programaBloqueado) { //Este if tiene que sacarse, es solo para probar ahora

			analizadorLinea(sentencia, funciones, kernel);


			pcbEjecutar->program_counter++;
			nanosleep(retardo); //Retardo en tiempo de ejecucion

		}

	}

	if (esFinPrograma)
		enviar_PCB_a_kernel(pcbEjecutar, "TERMINADO");

	esFinPrograma = false;
	programaBloqueado = false;

}

void ejecutar_programa_por_RR() {
	int topeEjecucion = pcbEjecutar->cantidad_rafagas;
	int cantidadEjecutada = 0;
	printf("\n Comienzo de Ejecucion");
	while (!esFinPrograma && cantidadEjecutada < topeEjecucion && !programaBloqueado) {
		printf("\n Comienzo de Solicitar sentencia...");
		char* sentencia = solicitar_sentencia_ejecutar();
		printf("\n Sentencia SOlicitada %s: ", sentencia);
		esFinPrograma = (strcmp(sentencia, "FIN") == 0);
		if (!esFinPrograma && !programaBloqueado) { //Este if tiene que sacarse, es solo para probar ahora
			analizadorLinea(sentencia, funciones, kernel);

			pcbEjecutar->program_counter++;
			cantidadEjecutada++;
			nanosleep(retardo); //Retardo en tiempo de ejecucion
		}
	}

	if (esFinPrograma)
		enviar_PCB_a_kernel(pcbEjecutar, "TERMINADO");
	else if (!programaBloqueado) {
		pcbEjecutar->cantidad_rafagas_ejecutadas++;
		enviar_PCB_a_kernel(pcbEjecutar, "QUANTUM");
	}

	esFinPrograma = false;
	programaBloqueado = false;
}

void inicializar_contexto_ejecucion() {
	funciones = malloc(sizeof(AnSISOP_funciones));
	kernel = malloc(sizeof(AnSISOP_kernel));

	funciones->AnSISOP_definirVariable = DEFINIR_VARIABLE;
	funciones->AnSISOP_obtenerPosicionVariable = OBTENER_DIRECCION_DE_VARIABLE;
	funciones->AnSISOP_dereferenciar = DEREFERENCIAR;
	funciones->AnSISOP_asignar = ASIGNAR_VARIABLE;
	funciones->AnSISOP_asignarValorCompartida = ASIGNAR_VALOR_COMPARTIDA;
	funciones->AnSISOP_obtenerValorCompartida = OBTENER_VALOR_COMPARTIDA;
	funciones->AnSISOP_irAlLabel = IR_A_LABEL;
	funciones->AnSISOP_llamarConRetorno = LLAMAR_CON_RETORNO;
	funciones->AnSISOP_llamarSinRetorno = LLAMAR_SIN_RETORNO;
	funciones->AnSISOP_retornar = RETORNAR;
	funciones->AnSISOP_finalizar = FINALIZAR;

	kernel->AnSISOP_wait = WAIT;
	kernel->AnSISOP_signal = SIGNAL;
	kernel->AnSISOP_reservar = ALOCAR;
	kernel->AnSISOP_liberar = LIBERAR;
	kernel->AnSISOP_abrir = ABRIR_ARCHIVO_PRIM;
	kernel->AnSISOP_borrar = BORRAR_ARCHIVO_PRIM;
	kernel->AnSISOP_cerrar = CERRAR_ARCHIVO_PRIM;
	kernel->AnSISOP_moverCursor = MOVER_CURSOR_PRIM;
	kernel->AnSISOP_escribir = ESCRIBIR_ARCHIVO_PRIM;
	kernel->AnSISOP_leer = LEER_ARCHIVO_PRIM;
}

void marcarFinDePrograma() {
	esFinPrograma = true;
}

void marcarBloqueado() {
	programaBloqueado = true;
}

