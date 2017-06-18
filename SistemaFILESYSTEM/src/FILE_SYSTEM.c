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
#include <unistd.h>

#include "header/AppConfig.h"
#include "header/Archivo.h"
#include "header/FileManager.h"
#include "header/InterfazKernel.h"
#include "../../Sharedlib/Sharedlib/Socket.h"

void escuchar_Conexiones_Kernel(int servidor);
void CU_Recibir_Conexiones_Kernel(int clienteKernel);

int main(int argc, char * argv[]) {

inicializar_configuracion(argv[1]);
	/*
	servidor = crear_servidor(configuraciones.PUERTO, configuraciones.CANTIDAD_MAXIMA_CONCURRENCIA);
	atender_clientes(servidor, &escuchar_Conexiones_Kernel);
	*/
	inicializar_estructuras_administrativas();

	/*FILE * f = fopen("/home/utnso/Escritorio/tp-2017-1c-Skynet/mnt/SADICA_FS/Archivos/archivo_prueba.bin", "rb");
	char *buffer;
	int filelen;

	fseek(f, 0, SEEK_END);          // Jump to the end of the file
	filelen = ftell(f);             // Get the current byte offset in the file
	rewind(f);                      // Jump back to the beginning of the file

	buffer = (char *)malloc((filelen+1)*sizeof(char)); // Enough memory for file + \0
	fread(buffer, filelen, 1, f); // Read in the entire file

	Archivo * arch = deserializar_archivo(buffer);

	printf("El tamanio es : %d\n", arch->tamanio);
	printf("El primer bloque es : %d\n", arch->bloques[0]);

	fclose(f); // Close the file
		*/

	/*crear_archivo("archivo_prueba.bin");
	crear_archivo("archi_hijo.bin");
	crear_archivo("");
	*/
	//while Provisorio...
	while(1);

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
