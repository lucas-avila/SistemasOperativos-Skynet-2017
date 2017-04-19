#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<commons/log.h>

#include "header/SolicitudesUsuario.h"

#include "general/funcionesUtiles.h"
#include "header/AppConfig.h"
#include "header/MemoriaCache.h"

void CU_Modificar_Retardo() {
	printf("\nIngresar nuevo retardo: ");
	configuraciones.RETARDO_MEMORIA = validarNumeroInput(0, 100000);
}

void CU_Limpiar_CACHE() {
	vaciar_tabla_memoria_cache(configuraciones.MARCO_SIZE);
}

void CU_Generar_Reporte() {
	printf("\n1 - Cache");
	printf("\n2 - Estructuras de Memoria");
	printf("\n3 - Contenido de Memoria");
	printf("\nOpcion: ");
	int opcion = validarNumeroInput(1, 3);
	system("clear");
	switch (opcion) {
	case 1:
		generar_Reporte_Cache();
		break;
	case 2:
		generar_Reporte_Estructura_de_Memoria();
		break;
	case 3:
		generar_Reporte_Contenido_de_Memoria();
		break;
	}
}

void generar_Reporte_Cache() {
     mostrar_tabla_memoria_cache();
}

void generar_Reporte_Contenido_de_Memoria() {
	printf("\n%s", MEMORIA_PRINCIPAL);
    t_log* logger = log_create(configuraciones.PATH_ARCHIVO_LOG, "MEMORIA",false, LOG_LEVEL_INFO);
    log_info(logger, "\nContenido de la Memoria %s", MEMORIA_PRINCIPAL);
    log_destroy(logger);
}

void CU_Mostrar_Tamanio_MEMORIA_PID() {
	printf("\n1 - Memory");
	printf("\n2 - PID");
	printf("\nOpcion: ");
	int opcion = validarNumeroInput(1, 2);
	system("clear");
	switch (opcion) {
	case 1:
		mostrar_reporte_memoria();
		break;
	case 2:
		mostrar_reporte_proceso();
		break;
	}
}



