#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include "../general/Socket.h"
#include "../header/PCB.h"
#include "../header/AppConfig.h"
#include "../primitivas/PrimitivasFunciones.h"
#include "../primitivas/FuncionesAuxiliares.h"
#include "commons/string.h"
/* se encarga de recibir y llenar toda la estructura struct PCB */
void recibir_PCB_de_kernel(int conexionKernel, PCB *pcb) {
}

/* Se encarga de devolver al kernel el PCB con sus modificaciones por la ejecucion*/
void enviar_PCB_a_kernel(int conexionKernel, PCB pcb) {
}

char* enviar_SYSCALL_solicitar_memoria_dinamica_a_kernel(char* PID, int espacio) {
	enviar_dato_serializado("GESTION_MEMORIA", servidor_kernel);
	enviar_dato_serializado("MALLOC_MEMORIA", servidor_kernel);
	enviar_dato_serializado(PID, servidor_kernel);
	enviar_dato_serializado(string_itoa(espacio), servidor_kernel);

	char* resultado = recibir_dato_serializado(servidor_kernel);
	if (strcmp(resultado, "OK") == 0) {
		char* PIDR = recibir_dato_serializado(servidor_kernel);
		char* pagina = recibir_dato_serializado(servidor_kernel);
		char* byteInicial = recibir_dato_serializado(servidor_kernel);

		t_puntero memoria_serializada = serializarMemoriaDinamica( pagina, byteInicial);
		return string_itoa(memoria_serializada);
		//return "0001";
	} else {

		printf("\n Error en MALLOC: %s", resultado);
		return "ERROR";
	}
}

char* enviar_SYSCALL_liberar_memoria_dinamica_a_kernel(	DireccionMemoriaDinamica* varDinamica) {
	enviar_dato_serializado("GESTION_MEMORIA", servidor_kernel);
	enviar_dato_serializado("FREE", servidor_kernel);
	enviar_dato_serializado(string_itoa(varDinamica->pid), servidor_kernel);
	enviar_dato_serializado(string_itoa(varDinamica->pagina), servidor_kernel);
	enviar_dato_serializado(string_itoa(varDinamica->byteInicial), servidor_kernel);
	return recibir_dato_serializado(servidor_kernel);
}

void enviar_SYSCALL_operacion_crear_archivo_a_kernel(int conexionKernel, PCB pcb) {
}

void enviar_SYSCALL_operacion_leer_archivo_a_kernel(int conexionKernel, PCB pcb) {
}

void enviar_SYSCALL_operacion_guardar_datos_archivo_a_kernel(int conexionKernel, PCB pcb) {
}

void enviar_SYSCALL_operacion_borrar_archivo_a_kernel(int conexionKernel, PCB pcb) {
}

DireccionVariable* obtener_valor_Variable_Compartida(int conexionKernel, char *nombreVariableCompartida) {
	return NULL;
}
