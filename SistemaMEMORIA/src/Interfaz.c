#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <pthread.h>

#include "header/Interfaz.h"
#include "general/Socket.h"

void CU_Recibir_conexiones(int servidor) {
	do {
		int cliente = aceptar_conexion_cliente(servidor);
		char* codigo_IDENTIFICACION = recibir_dato_serializado(cliente);
		pthread_t mihilo1;
		if (strcmp(codigo_IDENTIFICACION, "KERNEL") == 0) {
			pthread_create(&mihilo1, NULL, &CU_Recibir_Conexion_KERNEL, cliente);
			pthread_detach(&mihilo1);
		} else if (strcmp(codigo_IDENTIFICACION, "CPU") == 0) {
			pthread_create(&mihilo1, NULL, &CU_Recibir_Conexion_CPU, cliente);
			pthread_detach(&mihilo1);
		} else {
			close(cliente);
		}
	} while (1);
}

void CU_Recibir_Conexion_KERNEL(int cliente) {
	int controlSeguir = 1;
	char* codigo_operacion;
	do {

		codigo_operacion = recibir_dato_serializado(cliente);

		if (strcmp(codigo_operacion, "SOLICITAR_BYTE_MEMORIA") == 0) {
			CU_Solicitar_Bytes_Memoria(cliente);
		} else if (strcmp(codigo_operacion, "ALMACENAR_BYTE_MEMORIA") == 0) {
			CU_Almacenar_Bytes_de_Pagina(cliente);
		} else if (strcmp(codigo_operacion, "INICIALIZAR_PROGRAMA") == 0) {
			CU_Inicializar_Programa(cliente);
		} else if (strcmp(codigo_operacion, "ASIGNAR_PAGINAS_PROCESO") == 0) {
			CU_Asignar_Paginas_Programa(cliente);
		} else if (strcmp(codigo_operacion, "FINALIZAR_PROGRAMA") == 0) {
			CU_Finalizar_Programa(cliente);
		} else if (strcmp(codigo_operacion, "") == 0) {
			close(cliente);
			controlSeguir=0;
		} else {
			enviar_dato_serializado("ERROR: CODIGO OPERACION INEXISTENTE", cliente);
		}
	} while (controlSeguir == 1);
}

void CU_Recibir_Conexion_CPU(int cliente) {
	int controlSeguir = 1;
	char* codigo_operacion;
	do {
		codigo_operacion = recibir_dato_serializado(cliente);
		if (strcmp(codigo_operacion, "SOLICITAR_BYTE_MEMORIA") == 0) {
			CU_Solicitar_Bytes_Memoria(cliente);
		} else if (strcmp(codigo_operacion, "ALMACENAR_BYTE_MEMORIA") == 0) {
			CU_Almacenar_Bytes_de_Pagina(cliente);
		} else if (strcmp(codigo_operacion, "") == 0) {
			close(cliente);
			controlSeguir=0;
		} else{
			enviar_dato_serializado("ERROR: CODIGO OPERACION INEXISTENTE", cliente);
		}
	} while (controlSeguir == 1);
}
/****************FUNCIONES QUE ACCEDEN LAS DOS INTERFACES **************/

void CU_Solicitar_Bytes_Memoria(int cliente) {
	char* PID = recibir_dato_serializado(cliente); //PID
	int pagina = atoi(recibir_dato_serializado(cliente)); //pagina
	int byteInicial = atoi(recibir_dato_serializado(cliente)); //byte inicial de pagina
	int longitud = atoi(recibir_dato_serializado(cliente)); //longitud de bytes a pedir
	enviar_dato_serializado(solicitar_bytes_de_una_pagina(PID, pagina, byteInicial, longitud), cliente);
}

void CU_Almacenar_Bytes_de_Pagina(int cliente) {
	char* PID = recibir_dato_serializado(cliente); //PID
	int pagina = atoi(recibir_dato_serializado(cliente));
	int byteInicial = atoi(recibir_dato_serializado(cliente));
	int tamanio = atoi(recibir_dato_serializado(cliente));
	char* texto = recibir_dato_serializado(cliente);
	enviar_dato_serializado(almacenar_bytes_de_una_pagina(PID, pagina, byteInicial, tamanio, texto), cliente);
}
void CU_Inicializar_Programa(int cliente) {
	char* PID = recibir_dato_serializado(cliente); //PID
	int cantidad_paginas = atoi(recibir_dato_serializado(cliente));
	enviar_dato_serializado(inicializar_programa(PID, cantidad_paginas), cliente);
}
void CU_Asignar_Paginas_Programa(int cliente) {
	char* PID = recibir_dato_serializado(cliente); //PID
	int cantidad_paginas = atoi(recibir_dato_serializado(cliente));
	enviar_dato_serializado(asignar_paginas_a_proceso(PID, cantidad_paginas), cliente);
}
void CU_Finalizar_Programa(int cliente) {
	char* PID = recibir_dato_serializado(cliente); //PID
	finalizar_programa(PID);
	enviar_dato_serializado("OK", cliente);
}
