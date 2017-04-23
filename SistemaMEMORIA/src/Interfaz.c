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
#include "header/MemoriaPrincipal.h"

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
			controlSeguir = 0;
		} else {
			enviar_dato_serializado("ERROR: CODIGO OPERACION INEXISTENTE", cliente);
		}
		//free(codigo_operacion);
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
			controlSeguir = 0;
		} else {
			enviar_dato_serializado("ERROR: CODIGO OPERACION INEXISTENTE", cliente);
		}
	} while (controlSeguir == 1);
}
/****************FUNCIONES QUE ACCEDEN LAS DOS INTERFACES **************/

void CU_Solicitar_Bytes_Memoria(int cliente) {
	char* PID = recibir_dato_serializado(cliente); //PID

	char* texto;
	texto = recibir_dato_serializado(cliente);
	int pagina = atoi(texto);
	free(texto);

	texto = recibir_dato_serializado(cliente);
	int byteInicial = atoi(texto);
	free(texto);

	texto = recibir_dato_serializado(cliente);
	int longitud = atoi(texto);
	free(texto);

	texto = solicitar_bytes_de_una_pagina(PID, pagina, byteInicial, longitud);
	enviar_dato_serializado(texto, cliente);

	//enviar_dato_serializado("OK JONY", cliente);
	free(PID);
}

void CU_Almacenar_Bytes_de_Pagina(int cliente) {
	char* PID = recibir_dato_serializado(cliente); //PID

	char* texto;
	texto = recibir_dato_serializado(cliente);
	int pagina = atoi(texto);
	free(texto);

	texto = recibir_dato_serializado(cliente);
	int byteInicial = atoi(texto);
	free(texto);

	texto = recibir_dato_serializado(cliente);
	int tamanio = atoi(texto);
	free(texto);

	char* contenido;
	contenido = recibir_dato_serializado(cliente);

	texto = almacenar_bytes_de_una_pagina(PID, pagina, byteInicial, tamanio, contenido);
	enviar_dato_serializado(texto, cliente);
free(contenido);

	//enviar_dato_serializado("OK JONY", cliente);
	free(PID);

}
void CU_Inicializar_Programa(int cliente) {
	char* PID = recibir_dato_serializado(cliente); //PID
	char* texto;
	texto = recibir_dato_serializado(cliente);
	int cantidad_paginas = atoi(texto);
	free(texto);

	texto = inicializar_programa(PID, cantidad_paginas);
	enviar_dato_serializado(texto, cliente);


	//enviar_dato_serializado("OK JONY", cliente);
	free(PID);

}
void CU_Asignar_Paginas_Programa(int cliente) {
	char* PID = recibir_dato_serializado(cliente); //PID
	char* texto;
	texto = recibir_dato_serializado(cliente);
	int cantidad_paginas = atoi(texto);
	free(texto);

	texto = asignar_paginas_a_proceso(PID, cantidad_paginas);
	enviar_dato_serializado(texto, cliente);


	//enviar_dato_serializado("OK JONY", cliente);
	free(PID);
}
void CU_Finalizar_Programa(int cliente) {
	char* PID = recibir_dato_serializado(cliente); //PID

	finalizar_programa(PID);
	enviar_dato_serializado("OK", cliente);


	free(PID);
}
