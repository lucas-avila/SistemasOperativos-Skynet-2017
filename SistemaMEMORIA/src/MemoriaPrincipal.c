#include "header/MemoriaPrincipal.h"

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include "header/AppConfig.h"
#include "header/MemoriaCache.h"
#include "general/funcionesUtiles.h"
#include "general/Semaforo.h"
#include <math.h>
#include <unistd.h>
#include<commons/string.h>

//int indiceInicialPaginas = 0;

int frame_lookup(char*PID, int pagina) {

	int tope = configuraciones.MARCOS;
	Tabla_Pagina_Invertida registro;
	int numeroIndice = rand() % tope;
	int controlSeguir = 0;
	int i = numeroIndice;
	do {
		for (i = 0; i < tope; i++) {
			registro = TABLA_MEMORY[i];
			char valorAux[4];
			sprintf(valorAux, "%d", pagina);
			if ((strcmp(registro.PID, PID) == 0) && (strcmp(registro.pagina, valorAux) == 0)) {
				return atoi(registro.frame);
			}
		}
		tope = i + 1;
		i = 0;
		controlSeguir++;
	} while (controlSeguir < 2);

	return -1;

}

/**
 * Función que busca el frame que contiene la pagina y el PID enviados por parametro.
 * Pre-condicion: el PID y pagina existe
 * Post-Condicion: En caso de encontrar retorna el nro de frame. en caso de error retorna -1
 */
int getFrame(char*PID, int pagina) {

	int frame = buscar_frame(PID, pagina);
	int tope_repetido = frame;
	int frame_buscado = 0;
	int continuar = 1;
	Tabla_Pagina_Invertida registro;

	do {
		registro = TABLA_MEMORY[frame];
		if (atoi(registro.pagina) == pagina && strcmp(PID, registro.PID) == 0) {
			frame_buscado = frame;
			continuar = 0;
		}
		frame++;
		if (frame >= configuraciones.MARCOS)
			frame = 0;
		if (frame == tope_repetido) {
			frame_buscado = -1;
			continuar = 0;
		}

	} while (continuar);

	return frame_buscado;
}

/**
 * Función que busca el siguiente numero de pagina para asignarle a un proceso.
 * Estrategia: busca el numero mayor de pagina que tiene asignado ese proceso y lo incrementa en 1.
 */
char* obtenerNumeroPaginaNew(char* PID) {
	activar_semaforo(&semaforo_Tabla_MEMORY);

	int i = 0;
	int max = 0;
	int control = 0;
	int tope = configuraciones.MARCOS;
	for (i = 0; i < tope; i++) {
		Tabla_Pagina_Invertida registro = TABLA_MEMORY[i];
		if (strcmp(registro.PID, PID) == 0) {
			control = 1;
			if (max < atoi(registro.pagina)) {
				max = atoi(registro.pagina);
			}
		}
	}

	desactivar_semaforo(&semaforo_Tabla_MEMORY);
	char valorAux[4];
	sprintf(valorAux, "%d", control == 0 ? 0 : max + 1);
	return valorAux;
}

char* inicializar_programa(char* PID, int cantidad_paginas_requeridas) {
	logSO(string_from_format("Iniciando Programa con PID %s. Cantidad de Paginas Requeridas: %d", PID, cantidad_paginas_requeridas));
	return asignar_paginas_a_proceso(PID, cantidad_paginas_requeridas);

}

char* solicitar_bytes_de_una_pagina(char* PID, int pagina, int byteInicial, int longitud) {
	logSO(string_from_format("PID %s. Solicitar bytes. Pagina: %d | Byte Incial: %d | Longitud solicitada: %d .", PID, pagina, byteInicial, longitud));

	//1. Primero Busco en memoria Cache
	char *paginaEnCache = buscar_valor_en_cache(PID, pagina);
	//Inicializo contenido con todos \0 dejandole uno extra al final
	char * contenido = calloc(longitud + 1, sizeof(char));
	if (strcmp(paginaEnCache, "No existe en Cache") != 0) {
		memcpy(contenido, paginaEnCache + byteInicial, longitud);
		logSO(string_from_format("PID %s. Resultado en CACHE Solicitar bytes. Contenido: %s", PID, contenido));
		return contenido;
	} else {
		//1 b. No Existe en Memoria Cache
		int numeroFrame = getFrame(PID, pagina);
		if (numeroFrame < 0) {
			logSO(string_from_format("PID %s. Resultado Solicitar bytes. Pagina: %d | Byte Incial: %d | Longitud solicitada: %d | Contenido : %s .", PID, pagina, byteInicial, longitud, "PAGINA_NO_EXISTE"));

			return "PAGINA_NO_EXISTE"; //getframe retorna negativo cuando no encuentra
		}
		int numeroInicial = configuraciones.MARCO_SIZE * numeroFrame + byteInicial;

		memcpy(contenido, MEMORIA_PRINCIPAL + numeroInicial, longitud);

		//memcpy(contenidoPaginaBuscada, MEMORIA_PRINCIPAL + numeroInicial, sizeof(char) * longitud);
		// Almacenar pagina en memoria cache porque no existe
		ingresar_valor_en_cache(PID, pagina, MEMORIA_PRINCIPAL + configuraciones.MARCO_SIZE * numeroFrame);
		logSO(string_from_format("PID %s. Pagina: %d No esta en CACHE. Tiempo de Retardo %d ", PID, pagina, configuraciones.RETARDO_MEMORIA));

		int retardo = configuraciones.RETARDO_MEMORIA / 1000;
		if (retardo > 0) {
			sleep(retardo);
		}

		//printf("\n Contenido Enviado: %s Pagina %d", contenidoPaginaBuscada,pagina);
		logSO(string_from_format("PID %s. Resultado Solicitar bytes. Pagina: %d | Byte Incial: %d | Longitud solicitada: %d | Contenido : %s .", PID, pagina, byteInicial, longitud, contenido));

		return contenido;
	}
}

char* almacenar_bytes_de_una_pagina(char PID[4], int pagina, int byteInicial, int longitud, char* contenido, bool cacheIr) {
	if (!cacheIr)
		logSO(string_from_format("PID %s. Almacenar bytes. Pagina: %d | Byte Incial: %d | Longitud: %d | Contenido: %s.", PID, pagina, byteInicial, longitud, contenido));

	activar_semaforo(&semaforo_Tabla_MEMORY);
	int numeroFrame = getFrame(PID, pagina);
	if (numeroFrame < 0) {
		desactivar_semaforo(&semaforo_Tabla_MEMORY);
		logSO(string_from_format("PID %s. Resultado Almacenar bytes. Pagina: %d | Byte Incial: %d | Longitud: %d | Contenido: %s. | Resultado: %s", PID, pagina, byteInicial, longitud, contenido, "PAGINA_NO_EXISTE"));
		return "PAGINA_NO_EXISTE";
	} else if ((byteInicial > configuraciones.MARCO_SIZE || (byteInicial + longitud) > configuraciones.MARCO_SIZE)) {
		desactivar_semaforo(&semaforo_Tabla_MEMORY);
		logSO(string_from_format("PID %s. Resultado Almacenar bytes. Pagina: %d | Byte Incial: %d | Longitud: %d | Contenido: %s. | Resultado: %s", PID, pagina, byteInicial, longitud, contenido, "CONTENIDO_NO_ENTRA_EN_PAGINA"));
		return "CONTENIDO_NO_ENTRA_EN_PAGINA";
	}
	int numeroInicial = (numeroFrame * configuraciones.MARCO_SIZE) + byteInicial;
	char* punteroAPosicionDondePegar = (char*) (MEMORIA_PRINCIPAL + numeroInicial);
	memcpy(punteroAPosicionDondePegar, contenido, sizeof(char) * longitud);
	desactivar_semaforo(&semaforo_Tabla_MEMORY);
	/**CACHE **/
	if (cacheIr == true) {
		ingresar_valor_en_cache(PID, pagina, MEMORIA_PRINCIPAL + configuraciones.MARCO_SIZE * numeroFrame);
		logSO(string_from_format("PID %s. Resultado Almacenar bytes. Pagina: %d | Byte Incial: %d | Longitud: %d | Contenido: %s. | Resultado: %s", PID, pagina, byteInicial, longitud, contenido, "Almacenado Correctamente."));
	}
	return "OK";
}
char* liberar_pagina(char PID[4], int pagina) {
	logSO(string_from_format("PID %s. Liberar Pagina. Pagina: %d ", PID, pagina));

	almacenar_bytes_de_una_pagina(PID, pagina, 0, configuraciones.MARCO_SIZE, string_repeat('-', configuraciones.MARCO_SIZE), false);

	int ind = 0;
	for (ind = 0; ind < configuraciones.MARCOS; ind++) {
		if (strcmp(TABLA_MEMORY[ind].PID, PID) == 0 && atoi(TABLA_MEMORY[ind].pagina) == pagina) {
			actualizar_tabla_pagina(TABLA_MEMORY[ind]);
			strcpy(TABLA_MEMORY[ind].pagina, VACIO);
			strcpy(TABLA_MEMORY[ind].PID, VACIO);

			ind = configuraciones.MARCOS;
		}
	}
	logSO(string_from_format("PID %s. Resultado Liberar Pagina. Pagina: %d | Resultado: %s ", PID, pagina, "OK"));
	return "OK";
}

int obtener_cantidad_paginas_proceso(char * PID) {
	int i = 0;
	int cant_frames = configuraciones.MARCOS;
	int cant_paginas_asignadas = 0;
	for (i = 0; i < cant_frames; i++) {
		Tabla_Pagina_Invertida registro = TABLA_MEMORY[i];
		if (strcmp(registro.PID, PID) == 0)
			cant_paginas_asignadas++;
	}

	return cant_paginas_asignadas;
}

char* asignar_paginas_a_proceso(char *PID, int cantidad_paginas_requeridas) {
	logSO(string_from_format("PID %s. Asignar Paginas Requeridas. | Cantidad Paginas: %d", PID, cantidad_paginas_requeridas));
	int cantidad_paginas_pedidas = 0;
	int numero_pagina_a_asignar = obtener_cantidad_paginas_proceso(PID) + 1;
	char* numero_pagina_inicial;
	int frame;
	activar_semaforo(&semaforo_Proceso_Asignar_Pagina);
	while (cantidad_paginas_pedidas < cantidad_paginas_requeridas) {

		frame = buscar_frame(PID, numero_pagina_a_asignar);
		Tabla_Pagina_Invertida registro = evitar_colisiones(frame);
		if (strcmp(registro.frame, VACIO) == 0) {
			desactivar_semaforo(&semaforo_Proceso_Asignar_Pagina);
			logSO(string_from_format("PID %s. Resultado Asignar Paginas Requeridas. | Cantidad Paginas: %d | Resultado: %s", PID, cantidad_paginas_requeridas, "FALTA ESPACIO"));
			return "FALTA ESPACIO";
		}
		strcpy(registro.PID, PID);
		strcpy(registro.pagina, obtenerNumeroPaginaNew(PID));

		if (cantidad_paginas_pedidas == 0) {
			numero_pagina_inicial = malloc(strlen(registro.pagina) + 1);
			strcpy(numero_pagina_inicial, registro.pagina);

		}

		actualizar_tabla_pagina(registro);

		cantidad_paginas_pedidas++;
		numero_pagina_a_asignar++;

	}
	desactivar_semaforo(&semaforo_Proceso_Asignar_Pagina);
	logSO(string_from_format("PID %s. Resultado Asignar Paginas Requeridas. | Cantidad Paginas: %d | Resultado: %s ", PID, cantidad_paginas_requeridas, "Paginas Asignadas Correctamente."));

	return numero_pagina_inicial;

}

void actualizar_tabla_pagina(Tabla_Pagina_Invertida registro) {

	int bloqueAdmin = (TAMANIO - 1) * 3;
	int numeroInicial = (atoi(registro.frame)) * bloqueAdmin;
	//int numeroFinal = atoi(registro.frame) == 0 ? bloqueAdmin : atoi(registro.frame) * bloqueAdmin + bloqueAdmin;
	//char* primeraParte = string_substring(MEMORIA_PRINCIPAL, 0, numeroInicial);
	//char* segundaParte = string_substring_from(MEMORIA_PRINCIPAL, numeroFinal);

	char* paginaAdministrativa = malloc(bloqueAdmin + 1);
	strcpy(paginaAdministrativa, "");

	char* cerosLlenar = malloc(TAMANIO - 1);
	strcpy(cerosLlenar, "");
	int j;
	for (j = 0; j < TAMANIO - strlen(registro.frame) - 1; j++) {
		strcat(cerosLlenar, " ");
	}
	strcat(cerosLlenar, registro.frame);
	strcat(paginaAdministrativa, cerosLlenar);

	strcpy(cerosLlenar, "");
	for (j = 0; j < TAMANIO - strlen(registro.pagina) - 1; j++) {
		strcat(cerosLlenar, " ");
	}
	strcat(cerosLlenar, registro.pagina);
	strcat(paginaAdministrativa, cerosLlenar);

	strcpy(cerosLlenar, "");
	for (j = 0; j < TAMANIO - strlen(registro.PID) - 1; j++) {
		strcat(cerosLlenar, " ");
	}
	strcat(cerosLlenar, registro.PID);
	strcat(paginaAdministrativa, cerosLlenar);

	free(cerosLlenar);

	//strcpy(MEMORIA_PRINCIPAL, "");
	//strcpy(MEMORIA_PRINCIPAL, primeraParte);
	//strcat(MEMORIA_PRINCIPAL, paginaAdministrativa);
	//strcat(MEMORIA_PRINCIPAL, segundaParte);
	activar_semaforo(&semaforo_Tabla_MEMORY);
	char* punteroAPosicionDondePegar = (char*) (MEMORIA_PRINCIPAL + numeroInicial);
	memcpy(punteroAPosicionDondePegar, paginaAdministrativa, sizeof(char) * bloqueAdmin);
	desactivar_semaforo(&semaforo_Tabla_MEMORY);
	//free(primeraParte);
	free(paginaAdministrativa);
	//free(segundaParte);

	strcpy(TABLA_MEMORY[atoi(registro.frame)].frame, registro.frame);
	strcpy(TABLA_MEMORY[atoi(registro.frame)].PID, registro.PID);
	strcpy(TABLA_MEMORY[atoi(registro.frame)].pagina, registro.pagina);

}

int aplicar_hashing(char * PID, int numero_pagina) {
	int i = 0;
	int hash_code = 0;
	int limit = string_length(PID);

	for (i = 0; i < limit; i++) {
		hash_code += PID[i];
		hash_code += (hash_code << 10);
		hash_code ^= (hash_code >> 6);
	}

	if (numero_pagina > 0)
		hash_code *= numero_pagina;

	return hash_code;
}

Tabla_Pagina_Invertida evitar_colisiones(int frame) {

	Tabla_Pagina_Invertida registro = busqueda_secuencial(frame, configuraciones.MARCOS);

	if (strcmp(registro.PID, VACIO) == 0)
		return registro;
	// else...
	registro = busqueda_secuencial(0, frame);

	return registro;
}

Tabla_Pagina_Invertida busqueda_secuencial(int frame_start, int frame_finish) {

	int i = 0;
	Tabla_Pagina_Invertida registro;

	activar_semaforo(&semaforo_Tabla_MEMORY);
	for (i = frame_start; i < frame_finish; i++) {
		registro = TABLA_MEMORY[i];
		if (strcmp(registro.PID, VACIO) == 0) {
			desactivar_semaforo(&semaforo_Tabla_MEMORY);
			return registro;
		}
	}
	strcpy(registro.frame, VACIO);
	desactivar_semaforo(&semaforo_Tabla_MEMORY);
	return registro;
}

int buscar_frame(char * PID, int numero_pagina) {

	int hash_code;
	int frame;
	hash_code = aplicar_hashing(PID, numero_pagina);
	frame = hash_code % configuraciones.MARCOS;

	if (frame < 0)
		frame += configuraciones.MARCOS;

	return frame;
}

char* pagina_Blanco = NULL;
void finalizar_programa(char *PID) {
	logSO(string_from_format("PID %s. Finalizar Programa", PID));

	if (pagina_Blanco == NULL) {
		pagina_Blanco = string_repeat('-', configuraciones.MARCO_SIZE);
	}

	activar_semaforo(&semaforo_Proceso_Finalizar_Programa);
	int i = 0;
	int tope = configuraciones.MARCOS;
	for (i = 0; i < tope; i++) {
		Tabla_Pagina_Invertida registro = TABLA_MEMORY[i];
		if (strcmp(registro.PID, PID) == 0) {
			almacenar_bytes_de_una_pagina(PID, atoi(registro.pagina), 0, configuraciones.MARCO_SIZE, pagina_Blanco, false);

			strcpy(registro.PID, VACIO);
			strcpy(registro.pagina, VACIO);
			TABLA_MEMORY[i] = registro;
			actualizar_tabla_pagina(registro);

		}
	}
	desactivar_semaforo(&semaforo_Proceso_Finalizar_Programa);
	/**Liberar memoria cache de ese proceso**/
	eliminar_filas_de_procesos_en_cache(PID);
	logSO(string_from_format("PID %s. Programa Finalizado Correctamente", PID));
}

char* crear_tabla_memory_principal() {
	logSO(string_from_format("Creando Memoria Principal..."));
	char* paginaAdministrativa;
	char* cerosLlenar;
	char* valorAux;
	int bloqueAdmin = (TAMANIO - 1) * 3;
	char* bloqueMemoriaAdministrativa = malloc((bloqueAdmin * configuraciones.MARCOS) + 1);
	strcpy(bloqueMemoriaAdministrativa, "");
	int i = 0;

	TABLA_MEMORY = malloc(configuraciones.MARCOS * sizeof(Tabla_Pagina_Invertida));

	paginaAdministrativa = malloc(bloqueAdmin + 1);
	cerosLlenar = malloc(TAMANIO - 1);
	valorAux = malloc(sizeof(int));
	for (i = 0; i < configuraciones.MARCOS; i++) {
		strcpy(valorAux, "");
		sprintf(valorAux, "%d", i);
		int j = 0;

		strcpy(cerosLlenar, "");
		for (j = 0; j < TAMANIO - strlen(valorAux) - 1; j++) {
			strcat(cerosLlenar, " ");
		}
		strcat(cerosLlenar, valorAux);

		strcpy(TABLA_MEMORY[i].pagina, VACIO);
		strcpy(TABLA_MEMORY[i].PID, VACIO);
		strcpy(TABLA_MEMORY[i].frame, cerosLlenar);

		strcpy(paginaAdministrativa, "");
		strcat(paginaAdministrativa, TABLA_MEMORY[i].frame);
		strcat(paginaAdministrativa, TABLA_MEMORY[i].PID);
		strcat(paginaAdministrativa, TABLA_MEMORY[i].pagina);

		strcat(bloqueMemoriaAdministrativa, paginaAdministrativa);

	}
	free(valorAux);
	free(paginaAdministrativa);
	free(cerosLlenar);

	logSO(string_from_format("Memoria Principal creada correctamente."));
	logSO(string_from_format("Creando Estructuras administrativas..."));
	logSO(string_from_format("Estructuras administrativas creadas correctamente."));
	return bloqueMemoriaAdministrativa;
}

void crear_memoria_principal(char* bloqueMemoriaAdministrativa) {

	int tamanioEstructuraAdministrativo = strlen(bloqueMemoriaAdministrativa);
	int tam = (int) (tamanioEstructuraAdministrativo / configuraciones.MARCO_SIZE);
	float result = ((float) tamanioEstructuraAdministrativo / (float) configuraciones.MARCO_SIZE);
	int cantidadPaginas = 0;
	if (tam != result) {
		cantidadPaginas = tam + 1;
	} else {
		cantidadPaginas = tam;
	}

	int cantidadEspacioVacio = cantidadPaginas * configuraciones.MARCO_SIZE - strlen(bloqueMemoriaAdministrativa);

	int tamanio = (configuraciones.MARCOS - cantidadPaginas) * configuraciones.MARCO_SIZE;
	char* bloqueMemoria = string_repeat('-', tamanio);

	string_append(&bloqueMemoriaAdministrativa, string_repeat('-', cantidadEspacioVacio));

	MEMORIA_PRINCIPAL = malloc(strlen(bloqueMemoriaAdministrativa) + cantidadEspacioVacio + strlen(bloqueMemoria) + 1);
	strcpy(MEMORIA_PRINCIPAL, bloqueMemoriaAdministrativa);
	strcat(MEMORIA_PRINCIPAL, bloqueMemoria);

	int i = 0;
	for (i = 0; i < cantidadPaginas; i++) {
		Tabla_Pagina_Invertida registro = evitar_colisiones(0);
		strcpy(registro.PID, "ADM");
		strcpy(registro.pagina, "ADM");
		actualizar_tabla_pagina(registro);
	}
}

void reservar_memoria_principal() {

	char* bloqueMemoriaAdministrativa = crear_tabla_memory_principal();
	crear_memoria_principal(bloqueMemoriaAdministrativa);
	inicializar_memoria_cache(configuraciones.ENTRADAS_CACHE, configuraciones.MARCO_SIZE, configuraciones.CACHE_X_PROC);

}

void generar_Reporte_Estructura_de_Memoria() {

	char* textoLoguear = string_new();
	string_append(&textoLoguear, "\nContenido Administrativo de la Memoria: ");
	string_append(&textoLoguear, "\n Estructura de Memoria \n");
	string_append(&textoLoguear, "\n FRAME \t PAGINA PID ");
	string_append(&textoLoguear, "\n---------------------");

	int i = 0;
	int tope = configuraciones.MARCOS;
	for (i = 0; i < tope; i++) {
		Tabla_Pagina_Invertida registro = TABLA_MEMORY[i];
		if (strcmp(registro.PID, VACIO) != 0) {

			string_append(&textoLoguear, string_from_format("\n %s \t %s \t %s ", registro.frame, registro.pagina, registro.PID));
			string_append(&textoLoguear, "\n---------------------");
		}
	}
	logSO(textoLoguear);
}

void mostrar_reporte_memoria() {
	int totalFrame = 0;
	int totalFrameOcupados = 0;
	int totalFrameLibres = 0;
	int i = 0;
	int tope = configuraciones.MARCOS;
	for (i = 0; i < tope; i++) {
		Tabla_Pagina_Invertida registro = TABLA_MEMORY[i];
		totalFrame++;
		if (strcmp(registro.PID, VACIO) == 0) {

			totalFrameLibres++;
		} else {
			totalFrameOcupados++;
		}
	}
	logSO("-----------------------------");
	logSO(string_from_format(" Cantidad de Frames Libres: %d ", totalFrameLibres));
	logSO(string_from_format(" Cantidad de Frames Ocupados: %d ", totalFrameOcupados));
	logSO("-----------------------------");
	logSO(string_from_format(" Cantidad de Total de Frames: %d ", totalFrame));
	logSO("-----------------------------");
}

void mostrar_reporte_proceso() {
	int cantidadTotal = 0;
	char pidConsultar[TAMANIO];
	printf("\nIngrese PID a consultar: ");
	scanf("%s", &pidConsultar);
	int i = 0;
	int tope = configuraciones.MARCOS;
	for (i = 0; i < tope; i++) {
		Tabla_Pagina_Invertida registro = TABLA_MEMORY[i];
		if (strcmp(registro.PID, pidConsultar) == 0) {
			cantidadTotal++;
		}
	}
	logSO("\n-----------------------------");
	logSO(string_from_format("\n Cantidad de Frames Total del Proceso %s : %d ", pidConsultar, cantidadTotal));
	logSO("\n-----------------------------");
}

