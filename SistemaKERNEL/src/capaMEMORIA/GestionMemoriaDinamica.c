#include<stdlib.h>
#include<stdio.h>
#include <string.h>
#include<stdbool.h>
#include <commons/collections/list.h>
#include <commons/string.h>

#include "../header/AppConfig.h"
//#include "../general/Socket.h"
#include "../../../Sharedlib/Sharedlib/Socket.h"

#include <commons/config.h>
#include "GestionMemoriaDinamica.h"
#include "GestMemoriaFuncionesAux.h"
#include "../general/Semaforo.h"
#include "../administrarPCB/EstadisticaProceso.h"

void CU_Gestionar_HEAP(int conexionCPU) {
	char* tipoAccionMemoria = recibir_dato_serializado(conexionCPU);
	if (strcmp(tipoAccionMemoria, "MALLOC_MEMORIA") == 0) {
		sem_wait(&mutex_MALLOC);
		CU_Reservar_memoria_MALLOC(conexionCPU);
		sem_post(&mutex_MALLOC);
	} else if (strcmp(tipoAccionMemoria, "FREE") == 0) {
		sem_wait(&mutex_FREE);
		CU_Liberar_memoria_FREE(conexionCPU);
		sem_post(&mutex_FREE);
	}
}

void CU_Reservar_memoria_MALLOC(int conexionCPU) {

	char* PID = recibir_dato_serializado(conexionCPU);
	unsigned tamanioMALLOC = atoi(recibir_dato_serializado(conexionCPU));
	unsigned byteInicial = 0;
	informar_accion_en_log(string_from_format("RESERVAR MEMORIA: Proceso %s, pide %d de memoria",PID,tamanioMALLOC), atoi(PID));

	TABLA_MEMORIA_PROCESO* ultima_pagina_asignada = buscar_ultima_pagina_asignada_a_proceso(PID);
	if (ultima_pagina_asignada == NULL) {
		ultima_pagina_asignada = solicitar_nueva_pagina_memoria(PID);
		// Si ya no me asigna otra pagina, la memoria se quedo sin espacio.
		if (ultima_pagina_asignada == NULL) {
			informar_accion_en_log(string_from_format("Resultado RESERVAR MEMORIA: Proceso %s, FALTA ESPACIO EN MEMORIA",PID), atoi(PID));
			enviar_dato_serializado("FALTA ESPACIO", conexionCPU);


			return;
		}
		guardar_registro_tabla_memoria(ultima_pagina_asignada);
	}
	int resultadoVerificacion = verificar_si_malloc_entra_en_pagina(ultima_pagina_asignada, tamanioMALLOC);
	if (resultadoVerificacion == 3) {
		informar_accion_en_log(string_from_format("Resultado RESERVAR MEMORIA: Proceso %s, MALLOC_EXCEDE_TAMANIO_DE_PAGINA",PID), atoi(PID));
		enviar_dato_serializado("MALLOC_EXCEDE_TAMANIO_DE_PAGINA", conexionCPU);

		return;
	} else if (resultadoVerificacion == 2) {
		//No hay espacio en la pagina pido otra
		ultima_pagina_asignada = solicitar_nueva_pagina_memoria(PID);
		// Si ya no me asigna otra pagina, la memoria se quedo sin espacio.
		if (ultima_pagina_asignada == NULL) {
			informar_accion_en_log(string_from_format("Resultado RESERVAR MEMORIA: Proceso %s, FALTA ESPACIO EN MEMORIA",PID), atoi(PID));
			enviar_dato_serializado("FALTA ESPACIO", conexionCPU);

			return;
		}
		guardar_registro_tabla_memoria(ultima_pagina_asignada);
		//Asigno Espacio
		byteInicial = reservar_espacio_memoria_en_pagina(ultima_pagina_asignada, tamanioMALLOC);
		enviar_datos_respuesta(conexionCPU, ultima_pagina_asignada->nroPagina, ultima_pagina_asignada->PID, byteInicial);
	} else if (resultadoVerificacion == 1) {
		//Asigno Espacio
		byteInicial = reservar_espacio_memoria_en_pagina(ultima_pagina_asignada, tamanioMALLOC);
		if (byteInicial == -1) {
			//No hay espacio en la pagina pido otra
			ultima_pagina_asignada = solicitar_nueva_pagina_memoria(PID);
			// Si ya no me asigna otra pagina, la memoria se quedo sin espacio.
			if (ultima_pagina_asignada == NULL) {
				informar_accion_en_log(string_from_format("Resultado RESERVAR MEMORIA: Proceso %s, FALTA ESPACIO EN MEMORIA",PID), atoi(PID));
				enviar_dato_serializado("FALTA ESPACIO", conexionCPU);

				return;
			}
			guardar_registro_tabla_memoria(ultima_pagina_asignada);
			//Asigno Espacio
			byteInicial = reservar_espacio_memoria_en_pagina(ultima_pagina_asignada, tamanioMALLOC);
		}

		enviar_datos_respuesta(conexionCPU, ultima_pagina_asignada->nroPagina, ultima_pagina_asignada->PID, byteInicial);
	}
	//Lleno informacion estadistica
	incrementar_MALLOC(atoi(PID), tamanioMALLOC);
	informar_accion_en_log("Resultado RESERVO memoria (MALLOC)", atoi(PID));

}

void CU_Liberar_memoria_FREE(int conexionCPU) {
	char* PID = recibir_dato_serializado(conexionCPU);
	unsigned pagina = atoi(recibir_dato_serializado(conexionCPU));
	unsigned byteInicial = atoi(recibir_dato_serializado(conexionCPU));
	TABLA_MEMORIA_PROCESO* pagina_Buscada = buscar_pagina_por_PID_NroPagina(PID, pagina);

	int resultado = liberar_pagina_encontrada(pagina_Buscada, byteInicial);

	if (resultado == 1) {
		informar_accion_en_log("LIBERO memoria (FREE)", atoi(PID));
		enviar_dato_serializado("OK", conexionCPU);
		aplicar_algoritmo_Desfragmentacion_Interna(pagina_Buscada);
		return;
	} else if (resultado == 2) {
		enviar_dato_serializado("BLOQUE_INEXISTENTE", conexionCPU);
		return;
	} else if (resultado == 3) {
		enviar_dato_serializado("BLOQUE_NO_OCUPADO", conexionCPU);
		return;
	}

}

void enviar_datos_respuesta(int conexionCPU, unsigned pagina, char *PID, int byteInicial) {
	enviar_dato_serializado("OK", conexionCPU);
	//enviar_dato_serializado(PID, conexionCPU);
	char * page = string_itoa(pagina);
	char * byteIni = string_itoa(byteInicial);

	enviar_dato_serializado(page, conexionCPU);
	enviar_dato_serializado(byteIni, conexionCPU);

	free(page);
	free(byteIni);
}
