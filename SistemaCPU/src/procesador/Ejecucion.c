
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include "parser/parser.h"

#include "commons/string.h"

#include "Ejecucion.h"

#include "../header/PCB.h"
#include "../primitivas/PrimitivasFunciones.h"
#include "../primitivas/IndiceCodigo.h"

#include "../interfaz/InterfazMemoria.h"
PCB* pcbEjecutar;

void setPCBEjecucion(PCB* pcb) {
	pcbEjecutar = pcb;
}
char* solicitar_sentencia_ejecutar() {
	IndiceCodigo* indiceCodigo = obtener_Indice_codigo(pcbEjecutar->codigo, pcbEjecutar->program_counter);
	if (indiceCodigo == NULL) {
		return "FIN";
	} else {
		return solicitar_bytes_memoria(pcbEjecutar->PID, string_itoa(indiceCodigo->pagina), string_itoa(indiceCodigo->byte_inicial_codigo), string_itoa((indiceCodigo->byte_final_codigo - indiceCodigo->byte_inicial_codigo)));
	}
}
void ejecutar_Programa() {
	bool esRoundRobin = (pcbEjecutar->RR == 1);
	if (!esRoundRobin) {
		ejecutar_programa_por_FIFO();
	} else {
		ejecutar_programa_por_RR();
	}

}

void ejecutar_programa_por_FIFO() {
	bool esFinPrograma = false;
	while (!esFinPrograma) {
		char* sentencia = solicitar_sentencia_ejecutar();
		esFinPrograma = (strcmp(sentencia, "FIN") == 0);
		if (!esFinPrograma) { //Este if tiene que sacarse, es solo para probar ahora
			analizadorLinea(sentencia, funciones, kernel);

			pcbEjecutar->program_counter++;
		}

	}

}

void ejecutar_programa_por_RR() {
	bool esFinPrograma = false;
	int topeEjecucion = pcbEjecutar->cantidad_rafagas;
	int cantidadEjecutada = 0;

}

void inicializar_contexto_ejecucion() {
	funciones = malloc(sizeof(AnSISOP_funciones));
	kernel = malloc(sizeof(AnSISOP_kernel));

	funciones->AnSISOP_definirVariable = DEFINIR_VARIABLE;
	funciones->AnSISOP_obtenerPosicionVariable = OBTENER_DIRECCION_DE_VARIABLE;
	funciones->AnSISOP_dereferenciar = DEREFERENCIAR;
	funciones->AnSISOP_asignar = ASIGNAR_VARIABLE;

//	funciones->AnSISOP_irAlLabel = irAlLabel;

	kernel->AnSISOP_reservar = ALOCAR;
	kernel->AnSISOP_liberar = LIBERAR;
//	kernel->AnSISOP_abrir = abrir;
//	kernel->AnSISOP_borrar = borrar;
//	kernel->AnSISOP_cerrar = cerrar;
//	kernel->AnSISOP_moverCursor = moverCursor;
//	kernel->AnSISOP_escribir = escribir;
//	kernel->AnSISOP_leer = leer;
}


