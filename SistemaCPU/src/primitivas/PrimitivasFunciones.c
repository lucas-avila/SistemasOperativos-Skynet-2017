#include "PrimitivasFunciones.h"

#include <commons/collections/list.h>
#include <commons/string.h>
#include <ctype.h>
#include <parser/metadata_program.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../header/PCB.h"
#include "../interfaz/InterfazKernel.h"
#include "../interfaz/InterfazMemoria.h"
#include "../procesador/Ejecucion.h"
#include "EstructurasDeDatosPrimitivas.h"
#include "FuncionesAuxiliares.h"
#include "Stack.h"

PCB* pcb;
const int TAMANIO_VARIABLE = 4;

void setearPCB(PCB* pcbEnviado) {
	pcb = pcbEnviado;
}

t_puntero DEFINIR_VARIABLE(t_nombre_variable variable) {
	// 1. Obtenemos la ultima fila de la tabla del stack(porque es la que estamos trabajando)
	IndiceStack* pila = obtener_Ultima_fila_Indice_Stack(pcb);
	if(isalpha(variable)){
	// 3. Creo la variable
	Variable* var_new = crear_variable(variable, -1, -1, sizeof(variable), 0);

	// 4. Almaceno la variable en la ultima fila de la tabla variable de la pila
	crear_variable_en_Indice_Stack(pila, var_new);

	// 6. Genero el puntero de la variable al indice de Stack, poniendo el numero de fila del stack, si es argumento o variable, y el numero de fila dentro de la tabla
	// Por ejemplo la variable a, fue declarada en la primer fila del stack, es de tipo variable, y ocupa la posicon 7 de la tabla de variables.
	// El puntero para esto seria 99 0000 0001 0007
	PunteroVariable* direccionPuntero = buscar_posicion_variable_por_nombre(pila, variable);

	//printf("La nueva variable %s tiene la direccion:\n",variable);
	return generarPunteroStack(string_itoa(direccionPuntero->filaStack), string_itoa(direccionPuntero->esVariable), string_itoa(direccionPuntero->filaTabla));
	}
	else{
		Argumento* arg_new = crear_argumento(variable,-1,-1,sizeof(variable));

		crear_argumento_en_Indice_Stack(pila, arg_new);

		PunteroVariable* direccionPuntero = buscar_posicion_variable_por_nombre(pila, variable);

		return generarPunteroStack(string_itoa(direccionPuntero->filaStack), string_itoa(direccionPuntero->esVariable), string_itoa(direccionPuntero->filaTabla));
	}

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
	if (strlen(string_itoa(valor)) > 4) {
		Variable* var = buscar_variable_por_stack_y_fila(pcb, punteroVariable->filaStack, punteroVariable->filaTabla);
		DireccionMemoriaDinamica* variableDiamica = deserializarMemoriaDinamica(string_itoa(pcb->PID), valor);
		var->byte_inicial = variableDiamica->byteInicial;
		var->pagina = variableDiamica->pagina;

		int tamanio = atoi(solicitar_bytes_memoria(string_itoa(pcb->PID), string_itoa(var->pagina), string_itoa(var->byte_inicial + 1), string_itoa(4)));

		var->tamanio = tamanio;
		var->dinamica = 1;
		return;
	}

	if (punteroVariable->esVariable == 1) {
		Variable* var = buscar_variable_por_stack_y_fila(pcb, punteroVariable->filaStack, punteroVariable->filaTabla);

		if (var->byte_inicial == -1) {
			//No tiene espacio de memoria asginado, se lo asignamos.
			asingar_espacio_memoria_variable(pcb, var, punteroVariable->filaStack, punteroVariable->filaTabla);
		}

		char* contenido_variable = malloc(var->tamanio + 1);
		char* varComplete = string_repeat(' ', (int) (var->tamanio - strlen(string_itoa(valor))));
		strcpy(contenido_variable, varComplete);
		strcat(contenido_variable, string_itoa(valor));

		almacenar_Bytes_de_Pagina(string_itoa(pcb->PID), string_itoa(var->pagina), string_itoa(var->byte_inicial), string_itoa(strlen(contenido_variable)), contenido_variable);
	} else {
		Argumento* arg = buscar_argumento_por_stack_y_fila(pcb, punteroVariable->filaStack, punteroVariable->filaTabla);

		char* contenido_variable = malloc(arg->tamanio + 1);
		char* varComplete = string_repeat(' ', (int) (arg->tamanio - strlen(string_itoa(valor))));
		strcpy(contenido_variable, varComplete);
		strcat(contenido_variable, string_itoa(valor));

		almacenar_Bytes_de_Pagina(string_itoa(pcb->PID), string_itoa(arg->pagina), string_itoa(arg->byte_inicial), string_itoa(strlen(contenido_variable)), contenido_variable);

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

	 if (strcmp(almacenar_Bytes_de_Pagina( string_itoa(pcb->PID), string_itoa(direccion_var->pagina), string_itoa(direccion_var->byteInicial), string_itoa(TAMANIO_VARIABLE), contenido_variable), "OK") != 0) {
	 //TODO: PREGUNTAR AL AYUDANTE QUE PASA CUANDO ME QUEDO SIN PAGINA PARA ASIGNAR VARIABLES.
	 }**/
//	free(direccion_var);
}

void WAIT(t_nombre_semaforo identificador_semaforo){
	//Incremento el program counter para pasar a la siguiente linea del wait
	//Si se bloqueó, ya se mando al kernel con el program counter apuntando a la siguiente sentencia para que cuando vuelva
	//A una cpu no ejecute de nuevo el wait
	//Si resulta que no se bloqueó, retrocedemos el program counter y el ++ se hace en Ejecucion.c luego de analizarLinea
	pcb->program_counter++;

	int result = enviar_SYSCALL_wait_semaforo_a_kernel(identificador_semaforo, pcb);
	if(result == 1)
		marcarBloqueado();
	else if(result == -1 || result == -2)
		exit(result);
	else
		pcb->program_counter--;

	//Si da error terminamos la cpu
	//TODO: Chequear si esta bien finalizar cuando ocurren esos errores
	//TODO: Chequear en el kernel tambien que esta cpu crasheó
}

void SIGNAL(t_nombre_semaforo identificador_semaforo){
	enviar_SYSCALL_signal_semaforo_a_kernel(identificador_semaforo);
}

t_valor_variable DEREFERENCIAR(t_puntero puntero) {

	PunteroVariable* punteroVariable = deserializarPunteroStack(puntero);
	int valor = 0;
	if (punteroVariable->esVariable == 1) {
		Variable* var = buscar_variable_por_stack_y_fila(pcb, punteroVariable->filaStack, punteroVariable->filaTabla);

		valor = atoi(solicitar_bytes_memoria(string_itoa(pcb->PID), string_itoa(var->pagina), string_itoa(var->byte_inicial), string_itoa(4)));
	} else {
		Argumento* arg = buscar_argumento_por_stack_y_fila(pcb, punteroVariable->filaStack, punteroVariable->filaTabla);
		valor = atoi(solicitar_bytes_memoria(string_itoa(pcb->PID), string_itoa(arg->pagina), string_itoa(arg->byte_inicial), string_itoa(4)));
	}
	return valor;
}
t_puntero ALOCAR(t_valor_variable espacio) {

	char* mensaje = enviar_SYSCALL_solicitar_memoria_dinamica_a_kernel(pcb->PID, espacio);

	if (strcmp(mensaje, "ERROR") == 0) {

	}
	int numero = atoi(mensaje);
	return numero;

}

void LIBERAR(t_puntero memoria_serializada) {

	//DireccionMemoriaDinamica* variableLiberar = deserializarMemoriaDinamica( string_itoa(pcb->PID), memoria_serializada);
	PunteroVariable* punteroVariable = deserializarPunteroStack(memoria_serializada);
	Variable* var = buscar_variable_por_stack_y_fila(pcb, punteroVariable->filaStack, punteroVariable->filaTabla);

	DireccionMemoriaDinamica* variableLiberar = malloc(sizeof(DireccionMemoriaDinamica));
	variableLiberar->pid = pcb->PID;
	variableLiberar->pagina = var->pagina;
	variableLiberar->byteInicial = var->byte_inicial;
	char* resultado = enviar_SYSCALL_liberar_memoria_dinamica_a_kernel(variableLiberar);
	printf("\nResultado de LIBERAR el puntero %d: %s", memoria_serializada, resultado);

}
void IR_A_LABEL(t_nombre_etiqueta nombre_etiqueta){

	pcb->program_counter = metadata_buscar_etiqueta(nombre_etiqueta, pcb->etiquetas, pcb->etiquetas_size);

}

void RETORNAR(t_valor_variable variableRetorno){

	IndiceStack* stackDeFuncion = list_get(pcb->pila,(list_size(pcb->pila) - 1));
	ASIGNAR_VARIABLE(serializarMemoriaDinamica(stackDeFuncion->retVar->pagina,stackDeFuncion->retVar->byte_inicial), variableRetorno);

}

void FINALIZAR(){
	//FALTA DETERMINAR SI DEBE CAMBIAR EL EXIT_CODE DEL PCB (SI TERMINO EL PROCESO)
	if(list_size(pcb->pila) != 1){
		IndiceStack* stackDeFuncion = list_get(pcb->pila,(list_size(pcb->pila) - 1));
		pcb->program_counter = stackDeFuncion->retPos;
		list_remove(pcb->pila,(list_size(pcb->pila) - 1));

	}
	else{

		pcb->exit_code = 0;

	}
}

void LLAMAR_SIN_RETORNO(t_nombre_etiqueta nombre_etiqueta ){
	IndiceStack* stackDeFuncion = malloc(sizeof(IndiceStack));
	list_add(pcb->pila, stackDeFuncion);
	stackDeFuncion->retPos = pcb->program_counter;
	// tiene que buscar la etiqueta en la lista de etiquetas y conseguir la direccion
	t_puntero_instruccion direccionEtiqueta = metadata_buscar_etiqueta(nombre_etiqueta, pcb->etiquetas, pcb->etiquetas_size);
	pcb->program_counter = direccionEtiqueta;

}

void LLAMAR_CON_RETORNO(t_nombre_etiqueta nombre_etiqueta, t_puntero direccionRetorno ){

	IndiceStack* stackDeFuncion = malloc(sizeof(IndiceStack));
	list_add(pcb->pila,stackDeFuncion);
	stackDeFuncion->retPos = pcb->program_counter;
	stackDeFuncion->retVar = direccionRetorno;
	// tiene que buscar la etiqueta en la lista de etiquetas y conseguir la direccion
	t_puntero_instruccion direccionEtiqueta = metadata_buscar_etiqueta(nombre_etiqueta, pcb->etiquetas, pcb->etiquetas_size);
		pcb->program_counter = direccionEtiqueta;

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
 return solicitar_bytes_memoria( string_itoa(pcb->PID), string_itoa(direccion_var->pagina), string_itoa(direccion_var->byteInicial), string_itoa(TAMANIO_VARIABLE));
 }
 **/

t_valor_variable OBTENER_VALOR_COMPARTIDA(t_nombre_compartida variable){
	t_valor_variable valorVariable;
	if ((obtener_valor_de_variable_compartida_en_kernel(variable, &valorVariable )) == 0){
	return valorVariable;
	}
	else{
		printf("Ocurrio un error al buscar la variable %s\n", variable);
		return -1;
		}
}

t_valor_variable ASIGNAR_VALOR_COMPARTIDA(t_nombre_compartida variable, t_valor_variable valor) {
	if ((asignar_valor_a_variable_compartida_en_kernel(variable, valor)) == 0){
		printf("Asignacion exitosa!!\n");
		return valor;
	}
	else {
		printf("Ocurrio un error al asiignar el valor a la variable\n");
		return valor;
	}
}

