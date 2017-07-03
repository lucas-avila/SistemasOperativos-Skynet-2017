/*
 * ExitCodes.c
 *
 *  Created on: 29/4/2017
 *      Author: utnso
 */

#include <commons/string.h>
#include <stdlib.h>

#include "header/funcionesUtiles.h"

void mostrar_exit_code(int exit_code) {

	system("clear");
	string_append(&info_log, "EXIT CODE: ");
	switch (exit_code) {
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
		string_append(&info_log, "Excepción de memoria. Se intentó acceder a un proceso que no existe.\n");
		break;
	case -6:
		string_append(&info_log, "Finalizado a través de desconexión de consola.\n");
		break;
	case -7:
		string_append(&info_log, "Finalizado a través del comando Finalizar Programa de la Consola.\n");
		break;
	case -8:
		string_append(&info_log, "Se intentó reservar más memoria que el tamaño de una página.\n");
		break;
	case -9:
		string_append(&info_log, "No se pueden asignar más páginas al proceso.\n");
		break;
		// Adicionales
	case -10:
		string_append(&info_log, "El programa intentó crear un archivo que ya existe.\n");
		break;
	case -11:
		string_append(&info_log, "El programa intentó abrir un archivo sin explicitar los permisos.\n");
		break;
	case -12:
		string_append(&info_log, "El programa intentó hacer una operación sobre un archivo sin abrirlo previamente.\n");
		break;
	case -13:
		string_append(&info_log, "El programa intentó acceder a una variable compartida que no existe.\n");
		break;
	case -14:
		string_append(&info_log, "El programa intentó acceder a una página que no existe.\n");
		break;
	case -15:
		string_append(&info_log, "No hay más espacio en el File System.\n");
		break;
	case -16:
		string_append(&info_log, "El programa ha intentado acceder a un semáforo que no existe.\n");
		break;
	case -17:
		string_append(&info_log, "El programa intentó hacer un free a un bloque inexistente.\n");
		break;
	case -18:
		string_append(&info_log, "El programa intentó hacer un free a un bloque no ocupado por MALLOC.\n");
		break;
	case -19:
		string_append(&info_log, "El programa intentó eliminar un archivo que estaba abierto.\n");
		break;
	case -20:
		string_append(&info_log, "Error sin definición.\n");
		break;
	case -21:
		string_append(&info_log, "Se perdio conexion con el KERNEL.\n");
		break;
	case -22:
		string_append(&info_log, "Stack Overflow.\n");
		break;

	}
}
