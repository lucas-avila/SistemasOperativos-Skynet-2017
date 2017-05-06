#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#include "PrimitivasFunciones.h"
#include "../interfaz/InterfazMemoria.h"
#include "../interfaz/InterfazKernel.h"
#include "FuncionesAuxiliares.h"
#include "Stack.h"
#include "commons/string.h"
#include "EstructurasDeDatosPrimitivas.h"

PCB* pcb;
const int TAMANIO_VARIABLE = 4;

void setearPCB(PCB* pcbEnviado) {
	pcb = pcbEnviado;
}

t_puntero DEFINIR_VARIABLE(t_nombre_variable variable) {
	// 1. Obtenemos la ultima fila de la tabla del stack(porque es la que estamos trabajando)
	IndiceStack* pila = obtener_Ultima_fila_Indice_Stack(pcb);

	// 2. Obtenemos la ultima variable creada, para obtener la ultima posicion de memoria utilizada
	Variable* ultima_variable = obtener_Ultima_variable_declarada(pcb->pila);
	int pagina;
	int byte_inicial;
	if (ultima_variable == NULL) {
		// 2.A Esto quiere decir que no se crearon variables
		pagina = pcb->pagina_inicial_stack;
		byte_inicial = 0;
	} else {
		// 2.B A partir de la ultima posicion de memoria de la variable, declaro la mia.
		pagina = ultima_variable->pagina;
		byte_inicial = ultima_variable->byte_inicial + TAMANIO_VARIABLE;
	}

	// 3. Creo la variable
	Variable* var_new = crear_variable(variable, pagina, byte_inicial, TAMANIO_VARIABLE);

	// 4. Almaceno la variable en la ultima fila de la tabla variable de la pila
	crear_variable_en_Indice_Stack(pila, var_new);

	// 5. Almaceno en memoria la variable declarada.
	almacenar_Bytes_de_Pagina(pcb->PID, string_itoa(pagina), string_itoa(byte_inicial), string_itoa(TAMANIO_VARIABLE), "    ");

	// 6. Genero el puntero de la variable al indice de Stack, poniendo el numero de fila del stack, si es argumento o variable, y el numero de fila dentro de la tabla
	// Por ejemplo la variable a, fue declarada en la primer fila del stack, es de tipo variable, y ocupa la posicon 7 de la tabla de variables.
	// El puntero para esto seria 99 0000 0001 0007
	PunteroVariable* direccionPuntero = buscar_posicion_variable_por_nombre(pila, variable);
	return generarPunteroStack(string_itoa(direccionPuntero->filaStack), string_itoa(direccionPuntero->esVariable), string_itoa(direccionPuntero->filaTabla));
}

t_puntero OBTENER_DIRECCION_DE_VARIABLE(t_nombre_variable variable) {
	IndiceStack* pila = obtener_Ultima_fila_Indice_Stack(pcb);
	PunteroVariable* direccionPuntero = buscar_posicion_variable_por_nombre(pila, variable);
	if (direccionPuntero != NULL) {
		return generarPunteroStack(string_itoa(direccionPuntero->filaStack), string_itoa(direccionPuntero->esVariable), string_itoa(direccionPuntero->filaTabla));
	}
	/** Vamos buscar Argumento **/
	direccionPuntero = buscar_posicion_variable_por_nombre(pila, variable);
	if (direccionPuntero != NULL) {
		return generarPunteroStack(string_itoa(direccionPuntero->filaStack), string_itoa(direccionPuntero->esVariable), string_itoa(direccionPuntero->filaTabla));
	}
	return -1;
}

void ASIGNAR_VARIABLE(t_puntero direccion_variable, t_valor_variable valor) {
	PunteroVariable* punteroVariable = deserializarPunteroStack(direccion_variable);
	char contenido_variable[5];
	char* varComplete = string_repeat(' ', (int) (TAMANIO_VARIABLE - strlen(string_itoa(valor))));
	strcpy(contenido_variable, varComplete);
	strcat(contenido_variable, string_itoa(valor));
	if (punteroVariable->esVariable == 1) {
		Variable* var = buscar_variable_por_stack_y_fila(pcb, punteroVariable->filaStack, punteroVariable->filaTabla);
		almacenar_Bytes_de_Pagina(pcb->PID, string_itoa(var->pagina), string_itoa(var->byte_inicial), string_itoa(4), contenido_variable);
	} else {
		Argumento* arg = buscar_argumento_por_stack_y_fila(pcb, punteroVariable->filaStack, punteroVariable->filaTabla);
		almacenar_Bytes_de_Pagina(pcb->PID, string_itoa(arg->pagina), string_itoa(arg->byte_inicial), string_itoa(4), contenido_variable);

	}

//IndiceStack* pila = obtener_Ultima_fila_Indice_Stack(pcb);
//	DireccionVariable* direccion_var = buscar_variable_por_nombre(pila, variable);

	/**DireccionVariable* direccion_var = OBTENER_DIRECCION_DE_VARIABLE(pcb, variable);

	 if (direccion_var == NULL) {
	 return;
	 }



	 char contenido_variable[5];
	 if (strlen(constante) > 4) {
	 return;

	 } else {
	 char* varComplete = string_repeat(' ', (int) (TAMANIO_VARIABLE - strlen(constante)));
	 strcpy(contenido_variable, varComplete);
	 strcat(contenido_variable, constante);

	 }

	 if (strcmp(almacenar_Bytes_de_Pagina(pcb->PID, string_itoa(direccion_var->pagina), string_itoa(direccion_var->byteInicial), string_itoa(TAMANIO_VARIABLE), contenido_variable), "OK") != 0) {
	 //TODO: PREGUNTAR AL AYUDANTE QUE PASA CUANDO ME QUEDO SIN PAGINA PARA ASIGNAR VARIABLES.
	 }**/
//	free(direccion_var);
}

t_valor_variable DEREFERENCIAR(t_puntero puntero) {

	PunteroVariable* punteroVariable = deserializarPunteroStack(puntero);

	if (punteroVariable->esVariable == 1) {
		Variable* var = buscar_variable_por_stack_y_fila(pcb, punteroVariable->filaStack, punteroVariable->filaTabla);
		return atoi(solicitar_bytes_memoria(pcb->PID, string_itoa(var->pagina), string_itoa(var->byte_inicial), string_itoa(4)));
	} else {
		Argumento* arg = buscar_argumento_por_stack_y_fila(pcb, punteroVariable->filaStack, punteroVariable->filaTabla);
		return atoi(solicitar_bytes_memoria(pcb->PID, string_itoa(arg->pagina), string_itoa(arg->byte_inicial), string_itoa(4)));
	}
}
t_puntero ALOCAR(t_valor_variable espacio) {

	Variable* variable_malloc = enviar_SYSCALL_solicitar_memoria_dinamica_a_kernel(pcb->PID, espacio);

	t_puntero memoria_serializada = serializarMemoriaDinamica(variable_malloc->pagina, variable_malloc->byte_inicial);

	return memoria_serializada;

}

void LIBERAR(t_puntero memoria_serializada) {

	Variable* variableLiberar = deserializarMemoriaDinamica(memoria_serializada);

	enviar_SYSCALL_liberar_memoria_dinamica_a_kernel(pcb->PID, variableLiberar);

}

/**DireccionVariable* obtener_dir_variable(char* variable) {
 if (variable[0] == '!') {
 //ir a buscar variable Compartida
 return obtener_valor_Variable_Compartida(conexionKernel, variable);
 } else if (variable[0] == '$') {
 //ir a buscar argumento
 IndiceStack* pila = obtener_Ultima_fila_Indice_Stack(pcb);
 return buscar_argumento_por_nombre(pila, variable);
 } else if ((variable[0] != '!') && (variable[0] != '$')) {
 //ir a buscar variable local
 IndiceStack* pila = obtener_Ultima_fila_Indice_Stack(pcb);
 return buscar_variable_por_nombre(pila, variable);
 }
 return NULL;
 }

 char* OBTENER_VALOR_VARIABLE(PCB* pcb, char* variable) {
 //DireccionVariable* direccion_var = obtener_dir_variable(variable);
 if (direccion_var == NULL) {
 return "NO_EXISTE";
 }
 return solicitar_bytes_memoria(pcb->PID, string_itoa(direccion_var->pagina), string_itoa(direccion_var->byteInicial), string_itoa(TAMANIO_VARIABLE));
 }
 **/

