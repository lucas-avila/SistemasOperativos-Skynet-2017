#include <commons/collections/list.h>
#include <commons/string.h>
#include <ctype.h>
#include <parser/metadata_program.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../../Sharedlib/Sharedlib/PCB.h"
#include "../interfaz/InterfazKernel.h"
#include "../interfaz/InterfazMemoria.h"
#include "../procesador/Ejecucion.h"
#include "EstructurasDeDatosPrimitivas.h"
#include "FuncionesAuxiliares.h"
#include "Stack.h"
#include "PrimitivasFunciones.h"

PCB* pcb;
const int TAMANIO_VARIABLE = 4;

void setearPCB(PCB* pcbEnviado) {
	pcb = pcbEnviado;
}

void buscarPaginaDisponibleStack(PCB* pcbRecibido) {
	if (pcb->posicion_pagina_stack + sizeof(uint32_t) > tamanio_pagina_memoria) {
		pcb->posicion_pagina_stack = sizeof(uint32_t);
		pcb->pagina_inicial_stack++;
	} else {
		pcb->posicion_pagina_stack += sizeof(uint32_t);
	}

}

t_puntero DEFINIR_VARIABLE(t_nombre_variable variable) {
	IndiceStack* pila = obtener_Ultima_fila_Indice_Stack(pcb);

	int paginaDiponible = 0, offsetDisp = 0;
	buscarPaginaDisponibleStack(pcb);

	if (isalpha(variable)) {

		Variable* var_new = crear_variable(variable, pcb->pagina_inicial_stack, pcb->posicion_pagina_stack - sizeof(uint32_t), sizeof(uint32_t), 0);
		crear_variable_en_Indice_Stack(pila, var_new);



		return serializarPuntero(var_new->pagina, var_new->byte_inicial, tamanio_pagina_memoria);
	} else {
		Argumento* arg_new = crear_argumento(variable, pcb->pagina_inicial_stack, pcb->posicion_pagina_stack - sizeof(uint32_t), sizeof(uint32_t));

		crear_argumento_en_Indice_Stack(pila, arg_new);

		return serializarPuntero(arg_new->pagina, arg_new->byte_inicial, tamanio_pagina_memoria);
	}

}

t_puntero OBTENER_DIRECCION_DE_VARIABLE(t_nombre_variable variable) {

	if (isalpha(variable)) {

		Variable* var = buscar_variable_stack_por_nombre(pcb, variable);
		return serializarPuntero(var->pagina, var->byte_inicial, tamanio_pagina_memoria);
	} else {
		Argumento* arg = buscar_argumento_stack_por_nombre(pcb, variable);
		return serializarPuntero(arg->pagina, arg->byte_inicial, tamanio_pagina_memoria);
	}

}

char* n_char;

char* intToChar4(int num) {
	//RECORDAR: liberar el puntero con free()
	if (n_char == NULL) {
		n_char = malloc(4 + 1);
	}

	//num = 1537;
	int *pb; // pb deklariert als pointer auf int
	pb = &num; // & ist Adress operator, liefert Adresse von b
	strcpy(n_char, "");

	memcpy(n_char, pb, sizeof(int));
	return n_char;
}

int char4ToInt(char* chars) {
	int a;
	if (strcmp(chars, " ") != 0) {
		string_trim(&chars);
	}

	memcpy(&a, chars, sizeof(int));
	return a;
}

void ASIGNAR_VARIABLE(t_puntero direccion_variable, t_valor_variable valor) {
	int pagina = 0, offset = 0;
	deserializar_puntero(direccion_variable, &pagina, &offset, tamanio_pagina_memoria);

	almacenar_Bytes_de_Pagina(string_itoa(pcb->PID), string_itoa(pagina), string_itoa(offset), string_itoa(sizeof(uint32_t)), intToChar4(valor));

}

void WAIT(t_nombre_semaforo identificador_semaforo) {
	//Incremento el program counter para pasar a la siguiente linea del wait
	//Si se bloqueó, ya se mando al kernel con el program counter apuntando a la siguiente sentencia para que cuando vuelva
	//A una cpu no ejecute de nuevo el wait
	//Si resulta que no se bloqueó, retrocedemos el program counter y el ++ se hace en Ejecucion.c luego de analizarLinea
	pcb->program_counter++;

	int result = enviar_SYSCALL_wait_semaforo_a_kernel(identificador_semaforo, pcb);
	if (result == 1)
		marcarBloqueado();
	else if (result == -1 || result == -2)
		exit(result);
	else
		pcb->program_counter--;

	//Si da error terminamos la cpu
	//TODO: Chequear si esta bien finalizar cuando ocurren esos errores
	//TODO: Chequear en el kernel tambien que esta cpu crasheó
}

void SIGNAL(t_nombre_semaforo identificador_semaforo) {
	enviar_SYSCALL_signal_semaforo_a_kernel(identificador_semaforo);
}

t_valor_variable DEREFERENCIAR(t_puntero puntero) {
	int pagina = 0, offset = 0;
	deserializar_puntero(puntero, &pagina, &offset, tamanio_pagina_memoria);
	char* resultado = solicitar_bytes_memoria(string_itoa(pcb->PID), string_itoa(pagina), string_itoa(offset), string_itoa(sizeof(uint32_t)));

	return char4ToInt(resultado);

}
t_puntero ALOCAR(t_valor_variable espacio) {
	int pagina, offset;
	char* mensaje = enviar_SYSCALL_solicitar_memoria_dinamica_a_kernel(pcb->PID, espacio, &pagina, &offset);

	if (strcmp(mensaje, "OK") == 0) {
		return serializarPuntero(pagina, offset, tamanio_pagina_memoria);
	}
	else {
		hubo_excepcion = true;

	}
	//TODO: aca se tendria que bloquear el programa y lanzar excepcion

	return 0;

}

void LIBERAR(t_puntero memoria_serializada) {

	int punteroLiberar = DEREFERENCIAR(memoria_serializada);
	int pagina = 0, offset = 0;
	int* ptPagina =&pagina;
	int* ptoffset =&offset;

	// punteroLiberar = 1536;
	deserializar_puntero(punteroLiberar, ptPagina, ptoffset, tamanio_pagina_memoria);
	DireccionMemoriaDinamica* varDinamica = malloc(sizeof(DireccionMemoriaDinamica));
	varDinamica->pid = pcb->PID;
	varDinamica->pagina = pagina;
	varDinamica->byteInicial = offset;

	char* resultado = enviar_SYSCALL_liberar_memoria_dinamica_a_kernel(varDinamica);
	printf("\nResultado de LIBERAR el puntero %d: %s", memoria_serializada, resultado);

}

void IR_A_LABEL(t_nombre_etiqueta nombre_etiqueta) {
	if (nombre_etiqueta[strlen(nombre_etiqueta) - 1] == '\n') {
		nombre_etiqueta[strlen(nombre_etiqueta) - 1] = '\0';	//esto es porque las etiquetas vienen con el \n al final
	}

	pcb->program_counter = metadata_buscar_etiqueta(nombre_etiqueta, pcb->etiquetas, pcb->etiquetas_size);

}

void RETORNAR(t_valor_variable variableRetorno) {

	IndiceStack* stackDeFuncion = list_get(pcb->pila, (list_size(pcb->pila) - 1));
	int punteroVariableRetorno = serializarPuntero(stackDeFuncion->retVar->pagina, stackDeFuncion->retVar->byte_inicial, tamanio_pagina_memoria);
	ASIGNAR_VARIABLE(punteroVariableRetorno, variableRetorno);

}

void FINALIZAR() {
	//FALTA DETERMINAR SI DEBE CAMBIAR EL EXIT_CODE DEL PCB (SI TERMINO EL PROCESO)
	if (list_size(pcb->pila) != 1) {
		IndiceStack* stackDeFuncion = list_get(pcb->pila, (list_size(pcb->pila) - 1));
		pcb->program_counter = stackDeFuncion->retPos;
		list_remove(pcb->pila, (list_size(pcb->pila) - 1));

	} else {
		esFinPrograma = true;

		pcb->exit_code = 0;

	}
}

void LLAMAR_SIN_RETORNO(t_nombre_etiqueta nombre_etiqueta) {
	insertar_nueva_fila_Indice_Stack(pcb);
	IndiceStack * pila = obtener_Ultima_fila_Indice_Stack(pcb);
	pila->retPos = pcb->program_counter;

	// tiene que buscar la etiqueta en la lista de etiquetas y conseguir la direccion
	t_puntero_instruccion direccionEtiqueta = metadata_buscar_etiqueta(nombre_etiqueta, pcb->etiquetas, pcb->etiquetas_size);
	pcb->program_counter = direccionEtiqueta;

}

void LLAMAR_CON_RETORNO(t_nombre_etiqueta nombre_etiqueta, t_puntero direccionRetorno) {

	insertar_nueva_fila_Indice_Stack(pcb);
	IndiceStack * pila = obtener_Ultima_fila_Indice_Stack(pcb);
	pila->retPos = pcb->program_counter;
	//pila->retVar
	//TODO:Aca hay que poner la fucking RETVAR
	// tiene que buscar la etiqueta en la lista de etiquetas y conseguir la direccion
	t_puntero_instruccion direccionEtiqueta = metadata_buscar_etiqueta(nombre_etiqueta, pcb->etiquetas, pcb->etiquetas_size);
	IndiceCodigo* indiceCodigo = list_get(pcbEjecutar->codigo, direccionEtiqueta);

	pcb->program_counter = indiceCodigo->program_counter - 1;

	int pagina = 0, offset = 0;
	deserializar_puntero(direccionRetorno, &pagina, &offset, tamanio_pagina_memoria);

	pila->retVar = malloc(sizeof(ReturnVariable));
	pila->retVar->byte_inicial = offset;
	pila->retVar->pagina = pagina;
	pila->retVar->tamanio = 4;

}

t_valor_variable OBTENER_VALOR_COMPARTIDA(t_nombre_compartida variable) {
	string_substring(variable, 0, strlen(variable) - 2);
	t_valor_variable valorVariable;
	if ((obtener_valor_de_variable_compartida_en_kernel(variable, &valorVariable)) == 0) {
		return valorVariable;
	} else {
		printf("Ocurrio un error al buscar la variable %s\n", variable);
		return -1;
	}
}

t_valor_variable ASIGNAR_VALOR_COMPARTIDA(t_nombre_compartida variable, t_valor_variable valor) {
	if ((asignar_valor_a_variable_compartida_en_kernel(variable, valor)) == 0) {
		printf("Asignacion exitosa!!\n");
		return valor;
	} else {
		printf("Ocurrio un error al asiignar el valor a la variable\n");
		return valor;
	}
}

t_descriptor_archivo ABRIR_ARCHIVO_PRIM(t_direccion_archivo direccion, t_banderas flags) {
	char* mensaje = abrir_archivo(string_itoa(pcb->PID), direccion, flags.creacion, flags.lectura, flags.escritura);
	if (strcmp("OK", mensaje) != 0) {
		return atoi(mensaje);
	}
	else {

	}
	return 0;
	//TODO: Genera error
}

void BORRAR_ARCHIVO_PRIM(t_descriptor_archivo descriptor_archivo) {
	char* mensaje = borrar_archivo(string_itoa(pcb->PID), string_itoa(descriptor_archivo));
	if (strcmp("OK", mensaje) != 0) {
		//TODO: Genera error

	}

}

void CERRAR_ARCHIVO_PRIM(t_descriptor_archivo descriptor_archivo) {
	char* mensaje = cerrar_archivo(string_itoa(pcb->PID), descriptor_archivo);
	if (strcmp("OK", mensaje) != 0) {
		//TODO: Genera error

	}
}

void MOVER_CURSOR_PRIM(t_descriptor_archivo descriptor_archivo, t_valor_variable posicion) {
	char* mensaje = mover_cursor_archivo(string_itoa(pcb->PID), descriptor_archivo, posicion);
	if (strcmp("OK", mensaje) != 0) {
		//TODO: Genera error

	}
}

void ESCRIBIR_ARCHIVO_PRIM(t_descriptor_archivo descriptor_archivo, void* informacion, t_valor_variable tamanio) {
	char* mensaje = escribir_archivo(string_itoa(pcb->PID), descriptor_archivo, tamanio, informacion);
	if (strcmp("OK", mensaje) != 0) {
		//TODO: Genera error

	}
}

void LEER_ARCHIVO_PRIM(t_descriptor_archivo descriptor_archivo, t_puntero informacion, t_valor_variable tamanio) {
	char* mensaje = leer_archivo(string_itoa(pcb->PID), descriptor_archivo, tamanio);
	if (strcmp("OK", mensaje) != 0) {
		//TODO: Genera error

	}
}
