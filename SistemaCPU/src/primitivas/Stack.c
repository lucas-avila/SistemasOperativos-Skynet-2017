#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include "Stack.h"
#include "commons/collections/list.h"
#include "../interfaz/InterfazMemoria.h"
#include "PrimitivasFunciones.h"
void insertar_nueva_fila_Indice_Stack(PCB* pcb) {
	IndiceStack* filaInicial = malloc(sizeof(IndiceStack));
	filaInicial->argumentos = list_create();
	filaInicial->variables = list_create();
	list_add(pcb->pila, filaInicial);
}

void eliminar_fila_Indice_Stack(PCB* pcb, IndiceStack* pila) {

	/**
	 * Funcion que  elimina la fila recibida por parametro de la pila del pcb
	 */
}

IndiceStack* obtener_Ultima_fila_Indice_Stack(PCB* pcb) {
	return list_get(pcb->pila, list_size(pcb->pila) - 1);
}

IndiceStack* crear_argumento_en_Indice_Stack(IndiceStack* pila, Argumento* argumento) {
	list_add(pila->argumentos, argumento);
	return pila;
}

IndiceStack* crear_variable_en_Indice_Stack(IndiceStack* pila, Variable* variable) {
	list_add(pila->variables, variable);
	return pila;

}

IndiceStack* crear_variable_retorno_en_Indice_Stack(IndiceStack* pila, ReturnVariable* variable_retorno) {
	pila->retVar = variable_retorno;
	return pila;
}

Variable* crear_variable(char id, unsigned int pagina, unsigned int byte_inicial, unsigned int tamanio, int dinamica) {
	Variable* varNueva = malloc(sizeof(Variable));
	varNueva->id = id;
	//strcpy(varNueva->id, id);
	varNueva->pagina = pagina;
	varNueva->byte_inicial = byte_inicial;
	varNueva->tamanio = tamanio;
	varNueva->dinamica = dinamica;
	return varNueva;
}

ReturnVariable* crear_return_variable(unsigned int pagina, unsigned int byte_inicial, unsigned int tamanio) {
	ReturnVariable* returnVariable = malloc(sizeof(ReturnVariable));
	returnVariable->pagina = pagina;
	returnVariable->byte_inicial = byte_inicial;
	returnVariable->tamanio = tamanio;
	return returnVariable;
}

Argumento* crear_argumento(char id, unsigned int pagina, unsigned int byte_inicial, unsigned int tamanio) {
	Argumento* argumento = malloc(sizeof(Argumento));

	argumento->id= id;
	argumento->pagina = pagina;
	argumento->byte_inicial = byte_inicial;
	argumento->tamanio = tamanio;
	return argumento;

}

Variable* obtener_Ultima_variable_declarada(t_list* pila) {
	IndiceStack* indiceStack;
	t_list* listaVariables;
	int i = 0;
	for (i = list_size(pila) - 1; i >= 0; i--) {
		indiceStack = list_get(pila, i);
		listaVariables = indiceStack->variables;
		if (listaVariables != NULL && list_size(listaVariables) > 0) {
			int tamanioLista = list_size(listaVariables);
			int j = tamanioLista - 1;
			for (j = tamanioLista - 1; j >= 0; j--) {
				Variable* var = list_get(listaVariables, j);
				if (var->dinamica == 0 && var->byte_inicial != -1) {
					return var;
				}
			}
			return NULL;
			//return list_get(listaVariables, list_size(listaVariables) - 1);
		} else {
			return NULL;
		}
	}

}
/**
 DireccionVariable* buscar_variable_por_nombre(IndiceStack* pila, t_nombre_variable nombre_variable) {
 t_list* listaVariables = pila->variables;
 Variable* var;
 int i = 0;
 for (i = list_size(listaVariables) - 1; i >= 0; i--) {
 var = list_get(listaVariables, i);
 if (var->id == nombre_variable) {
 DireccionVariable* dirVariable = malloc(sizeof(DireccionVariable));
 dirVariable->pagina = var->pagina;
 dirVariable->byteInicial = var->byte_inicial;
 return dirVariable;
 }
 }
 return NULL;
 }

 DireccionVariable* buscar_argumento_por_nombre(IndiceStack* pila, t_nombre_variable nombre_argumento) {
 t_list* listaArgumentos = pila->argumentos;
 Argumento* argument;
 int i = 0;
 for (i = list_size(listaArgumentos) - 1; i >= 0; i--) {
 argument = list_get(listaArgumentos, i);
 if (argument->id == nombre_argumento) {
 DireccionVariable* dirVariable = malloc(sizeof(DireccionVariable));
 dirVariable->pagina = argument->pagina;
 dirVariable->byteInicial = argument->byte_inicial;
 return dirVariable;
 }
 }
 return NULL;
 }
 **/
PunteroVariable* buscar_posicion_variable_por_nombre(IndiceStack* pila, t_nombre_variable variable) {
	PunteroVariable* punteroVariable = malloc(sizeof(PunteroVariable));
	punteroVariable->filaStack = pila->posicion;

	t_list* listaVariables = pila->variables;
	Variable* var;
	int i = 0;
	for (i = list_size(listaVariables) - 1; i >= 0; i--) {
		var = list_get(listaVariables, i);
		if (var->id == variable) {
			punteroVariable->esVariable = 1;
			punteroVariable->filaTabla = i;
			return punteroVariable;
		}
	}

	t_list* listaArgumentos = pila->argumentos;
	Argumento* argument;
	for (i = list_size(listaArgumentos) - 1; i >= 0; i--) {
		argument = list_get(listaArgumentos, i);
		if (argument->id == variable) {
			punteroVariable->esVariable = 0;
			punteroVariable->filaTabla = i;
			return punteroVariable;
		}
	}

	return NULL;
}
/*
 void actualizar_variable_stack(PCB* pcb,Variable *variable){
 PunteroVariable* punt= buscar_posicion_variable_por_nombre(variable->id);
 punt->filaStack


 } */

Variable* buscar_variable_por_stack_y_fila(PCB* pcb, unsigned int stack, unsigned int filaTabla) {
	IndiceStack* pila = list_get(pcb->pila, stack);
	return list_get(pila->variables, filaTabla);

}

Argumento* buscar_argumento_por_stack_y_fila(PCB* pcb, unsigned int stack, unsigned int filaTabla) {
	IndiceStack* pila = list_get(pcb->pila, stack);
	return list_get(pila->argumentos, filaTabla);
}
const int TAMANIO_VARIABLE_D = 4;
void asingar_espacio_memoria_variable(PCB* pcb, Variable* var, int filaStack, int filaTabla) {
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
		byte_inicial = ultima_variable->byte_inicial + TAMANIO_VARIABLE_D;
	}
	// 5. Almaceno en memoria la variable declarada.
	almacenar_Bytes_de_Pagina(string_itoa(pcb->PID), string_itoa(pagina), string_itoa(byte_inicial), string_itoa(TAMANIO_VARIABLE_D), "    ");

	var->pagina = pagina;
	var->tamanio = TAMANIO_VARIABLE_D;
	var->byte_inicial = byte_inicial;

}
