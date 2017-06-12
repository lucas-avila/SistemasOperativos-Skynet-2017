/*
 * ExitCodes.c
 *
 *  Created on: 29/4/2017
 *      Author: utnso
 */

#include <commons/string.h>
#include <stdlib.h>

#include "funcionesUtiles.h"

void mostrar_exit_code(int exit_code){

	system("clear");
	string_append(&info_log, "EXIT CODE: ");
	switch(exit_code){
		case 0:
			string_append(&info_log, "El programa finalizo correctamente.\n");
			break;
		case -1:
			string_append(&info_log, "No se pudieron reservar recursos para ejecutar el programa.\n");
			break;
		case -2:
			string_append(&info_log, "El programa intentó acceder a un archivo que no existe.\n");
			break;
		case -3:
			string_append(&info_log, "El programa intentó leer un archivo sin permisos.\n");
			break;
		case -4:
			string_append(&info_log, "El programa intentó escribir un archivo sin permisos.\n");
			break;
		case -5:
			string_append(&info_log, "Excepción de memoria.\n");
			break;
		case -6:
			string_append(&info_log, "Finalizado a través de desconexión de consola.\n");
			break;
		case -7:
			string_append(&info_log, "Finalizado a través del comando Finalizar Programa de la consola.\n");
			break;
		case -8:
			string_append(&info_log, "Se intentó reservar más memoria que el tamaño de una página.\n");
			break;
		case -9:
			string_append(&info_log, "No se pueden asignar más páginas al proceso.\n");
			break;
		// Adicionales
		case -10:
			string_append(&info_log, "No se pudo finalizar el proceso en memoria.\n");
			break;
		case -20:
			string_append(&info_log, "Error sin definición.\n");
	}
}
