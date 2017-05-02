#include "header/MemoriaPrincipal.h"

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include "header/AppConfig.h"
#include "header/MemoriaCache.h"
#include<commons/log.h>
#include "general/Semaforo.h"

#include<commons/string.h>

int indiceInicialPaginas = 0;
Tabla_Pagina_Invertida* TABLA_MEMORY;

int frame_lookup(char*PID, int pagina) {

	int tope = configuraciones.MARCOS;
	Tabla_Pagina_Invertida registro;
	int numeroIndice = rand() % tope;
	int controlSeguir = 0;
	int i = numeroIndice;
	do {
		for (i = 0; i < tope; i++) {
			Tabla_Pagina_Invertida registro = TABLA_MEMORY[i];
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

	int i = 0;
	int tope = configuraciones.MARCOS;
	char valorAux[4];
	for (i = 0; i < tope; i++) {
		Tabla_Pagina_Invertida registro = TABLA_MEMORY[i];
		strcpy(valorAux, "");
		sprintf(valorAux, "%d", pagina);
		if ((strcmp(registro.PID, PID) == 0) && (strcmp(registro.pagina, valorAux) == 0)) {
			return atoi(registro.frame);
		}
	}
	return -1;

	//return frame_lookup(PID, pagina);
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
	return asignar_paginas_a_proceso(PID, cantidad_paginas_requeridas);

}

char* solicitar_bytes_de_una_pagina(char* PID, int pagina, int byteInicial, int longitud) {
	//1. Primero Busco en memoria Cache
	char *contenidoPaginaBuscada = buscar_valor_en_cache(PID, pagina);
	if (strcmp(contenidoPaginaBuscada, "No existe en Cache") != 0) {

		// Espera un tiempo configurable
		printf("\n cantidad milisegundo %d", configuraciones.RETARDO_MEMORIA);
		sleep(configuraciones.RETARDO_MEMORIA);
		printf("\n fin cantidad milisegundo %d", configuraciones.RETARDO_MEMORIA);
		//1 a. Existe en Memoria Cache
		return string_substring(contenidoPaginaBuscada, byteInicial, longitud);
	} else {
		//1 b. No Existe en Memoria Cache
		int numeroFrame = getFrame(PID, pagina);
		if (numeroFrame < 0) {
			return "PAGINA_NO_EXISTE"; //getframe retorna negativo cuando no encuentra
		}
		int numeroInicial = indiceInicialPaginas + configuraciones.MARCO_SIZE * numeroFrame + byteInicial;

		contenidoPaginaBuscada = string_substring(MEMORIA_PRINCIPAL, numeroInicial, longitud);
		// Almacenar pagina en memoria cache porque no existe
		ingresar_valor_en_cache(PID, pagina, string_substring(MEMORIA_PRINCIPAL, indiceInicialPaginas + configuraciones.MARCO_SIZE * getFrame(PID, pagina) + byteInicial, configuraciones.MARCO_SIZE));
		return contenidoPaginaBuscada;
	}
}

char* almacenar_bytes_de_una_pagina(char PID[4], int pagina, int byteInicial, int longitud, char* contenido) {

	activar_semaforo(&semaforo_Tabla_MEMORY);

	/**
	 * el indiceInicialPaginas me indica desde que posición comienzan las paginas, ya que antes
	 * se encuentran las estructuras de administracion de la memoria.
	 * Se invoca getFrame, ya que me va devolver el numero de frame de la memoria donde se encuentra
	 * el proceso asociado con su pagina.
	 * El anterior resultado lo multiplico por la cantidad de bytes que ocupa cada frame en la memoria
	 * obteniendo justo la posición inicial del frame a modificar.
	 * por ultimo le sumo el byte inicial enviado por parametro, ya que es desde ahi que yo tengo
	 * que empezar a escribir en la memoria.
	 */
	int numeroFrame = getFrame(PID, pagina);
	if (numeroFrame < 0) {
		desactivar_semaforo(&semaforo_Tabla_MEMORY);
		return "PAGINA_NO_EXISTE"; //getframe retorna negativo cuando no encuentra
	} else if ((byteInicial > configuraciones.MARCO_SIZE || (byteInicial + longitud) > configuraciones.MARCO_SIZE)) {
		desactivar_semaforo(&semaforo_Tabla_MEMORY);
		return "CONTENIDO_NO_ENTRA_EN_PAGINA";
	}
	int numeroInicial = indiceInicialPaginas + (numeroFrame * configuraciones.MARCO_SIZE) + byteInicial;

	/**
	 * Sumo la longitud que va ocupar el nuevo contenido al numero de byte obtenido antes.
	 */
	int numeroFinal = numeroInicial + longitud;
	/***
	 * Obtengo la primer parte de todo el bloque de memoria desde la poscion cero
	 * hasta la posición donde voy a comenzar a modificar.
	 */
	char* primeraParte = string_substring(MEMORIA_PRINCIPAL, 0, numeroInicial);
	char* textoMedio = contenido;
	/***
	 * Obtengo la segunda parte del bloque de memoria, desde la posición que corresponde
	 * al final de espacio de memoria que voy a modificar hasta el final del string.
	 */
	char* segundaParte = string_substring_from(MEMORIA_PRINCIPAL, numeroFinal);
	//string_capitalized(MEMORIA_PRINCIPAL);
	//MEMORIA_PRINCIPAL = string_new();
	/**
	 * Concateno primero la primera parte
	 * Luego el contenido a modificar
	 * y por ultimo el resto del bloque de memoria
	 * De esta manera me queda conformada nuevamente la memoria principal con el contenido
	 * modificado.
	 */

	strcpy(MEMORIA_PRINCIPAL, "");
	strcpy(MEMORIA_PRINCIPAL, primeraParte);
	strcat(MEMORIA_PRINCIPAL, textoMedio);
	strcat(MEMORIA_PRINCIPAL, segundaParte);
	free(primeraParte);
	//free(textoMedio);
	free(segundaParte);
	//string_append(&MEMORIA_PRINCIPAL, primeraParte);
	//string_append(&MEMORIA_PRINCIPAL, textoMedio);
	//string_append(&MEMORIA_PRINCIPAL, segundaParte);
	//string_capitalized(primeraParte);
	//string_capitalized(segundaParte);

	desactivar_semaforo(&semaforo_Tabla_MEMORY);
	/**CACHE **/
	//ingresar_valor_en_cache(PID, pagina, solicitar_bytes_de_una_pagina(PID, pagina, 0, configuraciones.MARCO_SIZE));
	return "OK";
}

char* asignar_paginas_a_proceso(char *PID, int cantidad_paginas_requeridas) {
	int cantidad_paginas_pedidas = 0;
	char* numero_pagina_inicial;
	/**
	 * Voy recorriendo toda la tabla de administracion de la memoria
	 * en busca de registros que esten vacios. Cuando los encuentra
	 * les asigna como refencia el PID del proceso, y su nueva pagina correspondiente.
	 */
	while (cantidad_paginas_pedidas < cantidad_paginas_requeridas) {
		activar_semaforo(&semaforo_Proceso_Asignar_Pagina);

		Tabla_Pagina_Invertida registro = buscar_pagina_disponible();
		if (strcmp(registro.frame, VACIO) == 0) {
			desactivar_semaforo(&semaforo_Proceso_Asignar_Pagina);
			return "FALTA ESPACIO";
		}
		strcpy(registro.PID, PID);
		strcpy(registro.pagina, obtenerNumeroPaginaNew(PID));

		if (cantidad_paginas_pedidas == 0) {
			numero_pagina_inicial = registro.pagina;
		}

		actualizar_tabla_pagina(registro);

		cantidad_paginas_pedidas++;

		desactivar_semaforo(&semaforo_Proceso_Asignar_Pagina);
	}
	return numero_pagina_inicial;

}

/***
 * FUNCION DETONANTE
 */
void actualizar_tabla_pagina(Tabla_Pagina_Invertida registro) {

	activar_semaforo(&semaforo_Tabla_MEMORY);
	int bloqueAdmin = (TAMANIO - 1) * 3; //tamaño que ocupara en bytes de memoria 3 atributos por el tamaño que ocupa cada uno

	/**
	 * La estrategia es muy simila a almacenar bytes en memoria
	 */
	int numeroInicial = (atoi(registro.frame)) * bloqueAdmin;
	int numeroFinal = atoi(registro.frame) == 0 ? bloqueAdmin : atoi(registro.frame) * bloqueAdmin + bloqueAdmin;
	char* primeraParte = string_substring(MEMORIA_PRINCIPAL, 0, numeroInicial);
	char* segundaParte = string_substring_from(MEMORIA_PRINCIPAL, numeroFinal);

//	char* paginaAdministrativa = string_new();
	char* paginaAdministrativa = malloc(bloqueAdmin + 1);
	strcpy(paginaAdministrativa, "");
	/**comienzo TODO: mejorar esta parte del codigo ya que tiene muchas cosas
	 * que se pueden meter en una funcion **/

	char* cerosLlenar = malloc(TAMANIO - 1);
	strcpy(cerosLlenar, "");
	int j;
	for (j = 0; j < TAMANIO - strlen(registro.frame) - 1; j++) {
		strcat(cerosLlenar, " ");
	}
	strcat(cerosLlenar, registro.frame);
	strcat(paginaAdministrativa, cerosLlenar);
	//string_capitalized(cerosLlenar);

	strcpy(cerosLlenar, "");
	for (j = 0; j < TAMANIO - strlen(registro.pagina) - 1; j++) {
		strcat(cerosLlenar, " ");
	}
	strcat(cerosLlenar, registro.pagina);
	strcat(paginaAdministrativa, cerosLlenar);
	//string_capitalized(cerosLlenar);

	strcpy(cerosLlenar, "");
	for (j = 0; j < TAMANIO - strlen(registro.PID) - 1; j++) {
		strcat(cerosLlenar, " ");
	}
	strcat(cerosLlenar, registro.PID);
	strcat(paginaAdministrativa, cerosLlenar);
	//string_capitalized(cerosLlenar);

	free(cerosLlenar);
	/**fin TODO: mejorar esta parte del codigo ya que tiene muchas cosas
	 * que se pueden meter en una funcion **/

	//string_capitalized(MEMORIA_PRINCIPAL);
	//MEMORIA_PRINCIPAL = string_new();
	//string_append(&MEMORIA_PRINCIPAL, primeraParte);
	//string_append(&MEMORIA_PRINCIPAL, paginaAdministrativa);
	//string_append(&MEMORIA_PRINCIPAL, segundaParte);
	//string_capitalized(paginaAdministrativa);
	//string_capitalized(primeraParte);
	//string_capitalized(segundaParte);
	//free()
	//MEMORIA_PRINCIPAL = malloc(strlen(primeraParte)+strlen(paginaAdministrativa)+strlen(segundaParte) );
	strcpy(MEMORIA_PRINCIPAL, "");
	strcpy(MEMORIA_PRINCIPAL, primeraParte);
	strcat(MEMORIA_PRINCIPAL, paginaAdministrativa);
	strcat(MEMORIA_PRINCIPAL, segundaParte);

	free(primeraParte);
	free(paginaAdministrativa);
	free(segundaParte);

	strcpy(TABLA_MEMORY[atoi(registro.frame)].frame, registro.frame);
	strcpy(TABLA_MEMORY[atoi(registro.frame)].PID, registro.PID);
	strcpy(TABLA_MEMORY[atoi(registro.frame)].pagina, registro.pagina);

	desactivar_semaforo(&semaforo_Tabla_MEMORY);
}

/**
 * TODO: DISEÑAR FUNCION HASHING PARA ACELERAR TIEMPOS DE BUSQUEDA Y ASIGNACION
 * DE PAGINA DISPONIBLE puede ser un numero random del 0 al numero tope, y preguntar
 * si esa pagina esta ocupada.
 */
Tabla_Pagina_Invertida buscar_pagina_disponible() {

	activar_semaforo(&semaforo_Tabla_MEMORY);

	int i = 0;
	int tope = configuraciones.MARCOS;
	Tabla_Pagina_Invertida registro;

	for (i = 0; i < tope; i++) {
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
char* pagina_Blanco = NULL;
void finalizar_programa(char *PID) {

	if (pagina_Blanco == NULL) {
		pagina_Blanco = string_repeat('-', configuraciones.MARCO_SIZE);
	}

	activar_semaforo(&semaforo_Proceso_Finalizar_Programa);
	int i = 0;
	int tope = configuraciones.MARCOS;
	for (i = 0; i < tope; i++) {
		Tabla_Pagina_Invertida registro = TABLA_MEMORY[i];
		if (strcmp(registro.PID, PID) == 0) {
			almacenar_bytes_de_una_pagina(PID, atoi(registro.pagina), 0, configuraciones.MARCO_SIZE, pagina_Blanco);

			strcpy(registro.PID, VACIO);
			strcpy(registro.pagina, VACIO);
			TABLA_MEMORY[i] = registro;
			actualizar_tabla_pagina(registro);
			//printf("\nFinalizar Proceso Frame %s",registro.frame);

			//actualizar_informacion_memoria_principal(registro, i);
		}
	}
	desactivar_semaforo(&semaforo_Proceso_Finalizar_Programa);
	/**Liberar memoria cache de ese proceso**/
	//eliminar_filas_de_procesos_en_cache(PID);
}

char* crear_tabla_memory_principal() {

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
	return bloqueMemoriaAdministrativa;
}

void crear_memoria_principal(char* bloqueMemoriaAdministrativa) {

	int tamanio = configuraciones.MARCOS * configuraciones.MARCO_SIZE;
	char* bloqueMemoria = string_repeat('-', tamanio);
	MEMORIA_PRINCIPAL = malloc(strlen(bloqueMemoriaAdministrativa) + strlen(bloqueMemoria));
	strcpy(MEMORIA_PRINCIPAL, bloqueMemoriaAdministrativa);
	strcat(MEMORIA_PRINCIPAL, bloqueMemoria);
	indiceInicialPaginas = (( TAMANIO - 1) * 3) * configuraciones.MARCOS;

}

void reservar_memoria_principal() {
	activar_semaforo(&semaforo_Tabla_MEMORY);
	char* bloqueMemoriaAdministrativa = crear_tabla_memory_principal();
	crear_memoria_principal(bloqueMemoriaAdministrativa);
	inicializar_memoria_cache(configuraciones.ENTRADAS_CACHE, configuraciones.MARCO_SIZE, configuraciones.CACHE_X_PROC);
	desactivar_semaforo(&semaforo_Tabla_MEMORY);
}

void generar_Reporte_Estructura_de_Memoria() {
	t_log* logger = log_create(configuraciones.PATH_ARCHIVO_LOG, "MEMORIA", false, LOG_LEVEL_INFO);
	char* textoLoguear = string_new();
	string_append(&textoLoguear, "\nContenido Administrativo de la Memoria: ");
	string_append(&textoLoguear, "\n Estructura de Memoria \n");
	string_append(&textoLoguear, "\n FRAME \t PAGINA PID ");
	string_append(&textoLoguear, "\n---------------------");

	printf("\n Estructura de Memoria \n");
	printf("\n FRAME \t PAGINA PID ");
	printf("\n---------------------");
	int i = 0;
	int tope = configuraciones.MARCOS;
	for (i = 0; i < tope; i++) {
		Tabla_Pagina_Invertida registro = TABLA_MEMORY[i];
		if (strcmp(registro.PID, VACIO) != 0) {
			printf("\n %s \t %s \t %s ", registro.frame, registro.pagina, registro.PID);
			printf("\n---------------------");

			string_append(&textoLoguear, string_from_format("\n %s \t %s \t %s ", registro.frame, registro.pagina, registro.PID));
			string_append(&textoLoguear, "\n---------------------");
		}
	}

	log_info(logger, "\n%s", textoLoguear);
	log_destroy(logger);
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
	printf("\n-----------------------------");
	printf("\n Cantidad de Frames Libres: %d ", totalFrameLibres);
	printf("\n Cantidad de Frames Ocupados: %d ", totalFrameOcupados);
	printf("\n-----------------------------");
	printf("\n Cantidad de Total de Frames: %d ", totalFrame);
	printf("\n-----------------------------");
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
	printf("\n-----------------------------");
	printf("\n Cantidad de Frames Total del Proceso %s : %d ", pidConsultar, cantidadTotal);
	printf("\n-----------------------------");
}

