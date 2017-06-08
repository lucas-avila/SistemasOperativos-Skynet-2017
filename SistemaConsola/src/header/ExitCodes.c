/*
 * ExitCodes.c
 *
 *  Created on: 29/4/2017
 *      Author: utnso
 */

void mostrar_exit_code(int exit_code){

	switch(exit_code){
		case 0:
			printf("\nEl programa finalizo correctamente.\n");
			break;
		case -1:
			printf("\nNo se pudieron reservar recursos para ejecutar el programa.\n");
			break;
		case -2:
			printf("\nEl programa intentó acceder a un archivo que no existe.\n");
			break;
		case -3:
			printf("\nEl programa intentó leer un archivo sin permisos.\n");
			break;
		case -4:
			printf("\nEl programa intentó escribir un archivo sin permisos.\n");
			break;
		case -5:
			printf("\nExcepción de memoria.\n");
			break;
		case -6:
			printf("\nFinalizado a través de desconexión de consola.\n");
			break;
		case -7:
			printf("\nFinalizado a través del comando Finalizar Programa de la consola.\n");
			break;
		case -8:
			printf("\nSe intentó reservar más memoria que el tamaño de una página.\n");
			break;
		case -9:
			printf("\nNo se pueden asignar más páginas al proceso.\n");
			break;
		// Se pueden agregar más.
		case -20:
			printf("\nError sin definición.\n");
	}
}
