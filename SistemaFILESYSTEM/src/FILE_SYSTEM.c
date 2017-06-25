/*
 ============================================================================
 Name        : FILE_SYSTEM.c
 Author      :
 Version     :
 Copyright   : Skynet :|
 Description : Say hello...good.
 ============================================================================
 */

#include <stdlib.h>
#include <string.h>
#include <Sharedlib/Socket.h>
#include <unistd.h>

#include "header/AppConfig.h"
#include "header/FileManager.h"
#include "header/InterfazKernel.h"
#include "testing/testing.h"

void escuchar_Conexiones_Kernel(int servidor);
void CU_Recibir_Conexiones_Kernel(int clienteKernel);

int main(int argc, char * argv[]) {

	inicializar_configuracion(argv[1]);
	/*
	 servidor = crear_servidor(configuraciones.PUERTO, configuraciones.CANTIDAD_MAXIMA_CONCURRENCIA);
	 atender_clientes(servidor, &escuchar_Conexiones_Kernel);
	 */
	inicializar_estructuras_administrativas();
	atender_clientes(-1, &mostrar_menu_testing);

	escuchar_Conexiones_Kernel(servidor);
	close(servidor);

	return EXIT_SUCCESS;
}

void escuchar_Conexiones_Kernel(int servidor) {

	int cliente = -1;
	while((cliente = aceptar_conexion_cliente(servidor)) == -1);
	char* codigo_IDENTIFICACION = recibir_dato_serializado(cliente);

	if (strcmp(codigo_IDENTIFICACION, "KERNEL") == 0) {
		clienteKernel = cliente;
		CU_Recibir_Conexiones_Kernel(cliente);
	}else{
		close(cliente);
	}

}

void CU_Recibir_Conexiones_Kernel(int cliente) {
	enviar_dato_serializado("FILESYSTEM", cliente);
	char * codigo_operacion;
	int controlSeguir = 1;
	do {
		codigo_operacion = recibir_dato_serializado(cliente);
		char * path = recibir_dato_serializado(cliente);

		if (strcmp(codigo_operacion, "VALIDAR_ARCHIVO") == 0) {
			validar_archivo(path);
		} else if (strcmp(codigo_operacion, "CREAR_ARCHIVO") == 0) {
			crear_archivo(path);
		} else if (strcmp(codigo_operacion, "OBTENER_DATOS") == 0) {
			int offset = atoi(recibir_dato_serializado(cliente));
			int size = atoi(recibir_dato_serializado(cliente));
			obtener_datos(path, offset, size);
		} else if (strcmp(codigo_operacion, "GUARDAR_DATOS") == 0) {
			int offset = atoi(recibir_dato_serializado(cliente));
			int size = atoi(recibir_dato_serializado(cliente));
			char * buffer = recibir_dato_serializado(cliente);
			guardar_datos(path, offset, size,buffer);
		} else if (strcmp(codigo_operacion, "BORRAR_ARCHIVO") == 0) {
			borrar(path);
		} else if (strcmp(codigo_operacion, "") == 0) {
			close(cliente);
			controlSeguir = 0;
		} else {
			enviar_dato_serializado("ERROR: CODIGO OPERACION INEXISTENTE", cliente);
		}
		free(path);
	} while (controlSeguir == 1);
	free(codigo_operacion);
	close(cliente);
}
