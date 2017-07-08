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
#include "../general/funcionesUtiles.h"
void setPCBEjecucion(PCB* pcb) {
	pcbEjecutar = pcb;
	setearPCB(pcb);
}
char* solicitar_sentencia_ejecutar() {
	IndiceCodigo* indiceCodigo = obtener_Indice_codigo(pcbEjecutar->codigo, pcbEjecutar->program_counter);
	if (indiceCodigo == NULL) {
		return "FIN";
	} else {
		//printf("\n PC %d  - Pagina %d - BI %d - BD %d",indiceCodigo->program_counter,indiceCodigo->pagina,indiceCodigo->byte_inicial_codigo,indiceCodigo->byte_final_codigo);

		int longitud = indiceCodigo->byte_final_codigo - indiceCodigo->byte_inicial_codigo;
		return solicitar_bytes_memoria(string_itoa(pcbEjecutar->PID), string_itoa(indiceCodigo->pagina), string_itoa(indiceCodigo->byte_inicial_codigo), string_itoa(longitud));
	}
}

long retardo;
void ejecutar_Programa() {
	esFinPrograma = false;
	programaBloqueado = false;
	retardo = pcbEjecutar->quantum_sleep * 1000000;
	bool esRoundRobin = (pcbEjecutar->RR == 1);
	if (!esRoundRobin) {
		logSO(string_from_format("PID : %d. Ejecutando con Algoritmo FIFO", pcbEjecutar->PID));
		ejecutar_programa_por_FIFO();
	} else {
		logSO(string_from_format("PID : %d. Ejecutando con Algoritmo RR", pcbEjecutar->PID));
		ejecutar_programa_por_RR();
	}

}

void ejecutar_programa_por_FIFO() {

	//printf("\n Comienzo de Ejecucion");
	while (!esFinPrograma && !programaBloqueado) {
		//printf("\n Comienzo de Solicitar sentencia...");
		char* sentencia = solicitar_sentencia_ejecutar();
		//printf("\n Sentencia SOlicitada %s: ", sentencia);
		esFinPrograma = (strcmp(sentencia, "FIN") == 0);
		if (!esFinPrograma && !programaBloqueado) { //Este if tiene que sacarse, es solo para probar ahora

			analizadorLinea(sentencia, funciones, kernel);
			if (hubo_excepcion) {
				hubo_excepcion = false;
				enviar_PCB_a_kernel(pcbEjecutar, "TERMINADO");
				return;
			}

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
	logSO(string_from_format("PID : %d. Algoritmo RR. Quantum Establecido: %d", pcbEjecutar->PID, topeEjecucion));
	//printf("\n Comienzo de Ejecucion");
	while (!esFinPrograma && cantidadEjecutada < topeEjecucion && !programaBloqueado) {
		//printf("\n Comienzo de Solicitar sentencia...");
		char* sentencia = solicitar_sentencia_ejecutar();
		//printf("\n Sentencia SOlicitada %s: ", sentencia);
		esFinPrograma = (strcmp(sentencia, "FIN") == 0);
		if (!esFinPrograma && !programaBloqueado) { //Este if tiene que sacarse, es solo para probar ahora
			analizadorLinea(sentencia, funciones, kernel);
			if (hubo_excepcion) {
				hubo_excepcion = false;
				enviar_PCB_a_kernel(pcbEjecutar, "TERMINADO");
				return;
			}

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

void lanzar_excepcion(char * mensaje) {

	if (strcmp(mensaje, "ERROR_PROCESO_NO_EXISTE") == 0) {
		pcbEjecutar->exit_code = -5;
	} else if (strcmp(mensaje, "ERROR - ARCHIVO EXISTE") == 0) {
		pcbEjecutar->exit_code = -10;
	} else if (strcmp(mensaje, "ERROR_ARCHIVO_NO_EXISTE") == 0) {
		pcbEjecutar->exit_code = -2;
	} else if (strcmp(mensaje, "ERROR - FALTA MODO APERTURA") == 0) {
		pcbEjecutar->exit_code = -11;
	} else if (strcmp(mensaje, "ERROR_ARCHIVO_NO_ABIERTO") == 0) {
		pcbEjecutar->exit_code = -12;
	} else if (strcmp(mensaje, "ERROR_FALTA_MODO_LECTURA") == 0) {
		pcbEjecutar->exit_code = -3;
	} else if (strcmp(mensaje, "ERROR_FALTA_MODO_ESCRITURA") == 0) {
		pcbEjecutar->exit_code = -4;
	} else if (strcmp(mensaje, "No existe la variable compartida solicitada") == 0) {
		pcbEjecutar->exit_code = -13;
	} else if (strcmp(mensaje, "ERROR_ARCHIVO_ABIERTO") == 0) {
		pcbEjecutar->exit_code = -19;
	} else if (strcmp(mensaje, "PAGINA_NO_EXISTE") == 0) {
		pcbEjecutar->exit_code = -14;
	} else if (strcmp(mensaje, "MALLOC_EXCEDE_TAMANIO_DE_PAGINA") == 0) {
		pcbEjecutar->exit_code = -8;
	} else if (strcmp(mensaje, "FALTA ESPACIO") == 0) {
		pcbEjecutar->exit_code = -9;
	} else if (strcmp(mensaje, "ERROR - SIN_ESPACIO EN FS") == 0) {
		pcbEjecutar->exit_code = -15;
	} else if (strcmp(mensaje, "SEMAFORO_NO_EXISTE") == 0) {
		pcbEjecutar->exit_code = -16;
	} else if (strcmp(mensaje, "BLOQUE_INEXISTENTE") == 0) {
		pcbEjecutar->exit_code = -17;
	} else if (strcmp(mensaje, "BLOQUE_NO_OCUPADO") == 0) {
		pcbEjecutar->exit_code = -18;
	} else if (strcmp(mensaje, "STACKOVERFLOW") == 0) {
		pcbEjecutar->exit_code = -22;
	}

	hubo_excepcion = true;
	//printf("\n\n\n Error %s",mensaje);

}

