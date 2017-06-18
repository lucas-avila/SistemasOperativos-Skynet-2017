/*
 ============================================================================
 Name        : FILE_SYSTEM.c
 Author      :
 Version     :
 Copyright   : Skynet :|
 Description : Say hello...good.
 ============================================================================
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Sharedlib/Socket.h>
#include <unistd.h>

#include "header/AppConfig.h"
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
	mostrar_menu_testing();


	/*crear_archivo("archivo_prueba.bin");
	crear_archivo("archi_hijo.bin");
	crear_archivo("");
	*/
	//while Provisorio...

	close(servidor);
	return EXIT_SUCCESS;
}

void escuchar_Conexiones_Kernel(int servidor){
	do {
			int cliente = aceptar_conexion_cliente(servidor);
			char* codigo_IDENTIFICACION = recibir_dato_serializado(cliente);
			pthread_t mihilo1;
			if (strcmp(codigo_IDENTIFICACION, "KERNEL") == 0) {
				pthread_create(&mihilo1, NULL, &CU_Recibir_Conexiones_Kernel, cliente);
				pthread_detach(&mihilo1);
			} else {
				close(cliente);
			}
		} while (1);
}

void CU_Recibir_Conexiones_Kernel(int clienteKernel){
	printf("Se conecto KERNEL\n");
	enviar_dato_serializado("FILESYSTEM", clienteKernel);
	close(clienteKernel);
}
