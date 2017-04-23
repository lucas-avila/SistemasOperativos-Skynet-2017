#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <pthread.h>

#include <commons/string.h>

#include "general/Socket.h"
#include "general/Semaforo.h"
#include "general/funcionesUtiles.h"
#include "header/Interfaz.h"
#include "header/AppConfig.h"
#include "header/SolicitudesUsuario.h"
#include "testing/PruebasUnitarias.h"

//#define TAMANIO 4

void atender_solicitudes_de_usuario();
void finalizar_sistema_memoria(int servidor);
int main(int argc, char *argv[]) {
	inicializar_configuracion(argv[1]);
	reservar_memoria_principal();
	int servidor = crear_servidor(configuraciones.PUERTO, configuraciones.CANTIDAD_MAXIMA_CONCURRENCIA);



	atender_clientes(servidor, &CU_Recibir_conexiones);
	atender_solicitudes_de_usuario();
	finalizar_sistema_memoria(servidor);
	return EXIT_SUCCESS;
}

void finalizar_sistema_memoria(int servidor) {
	close(servidor);
	free(MEMORIA_PRINCIPAL);
	destruir_semaforo(&semaforo_Tabla_MEMORY);
	destruir_semaforo(&semaforo_Tabla_CACHE);
	destruir_semaforo(&semaforo_Proceso_Asignar_Pagina);
	destruir_semaforo(&semaforo_Proceso_Finalizar_Programa);
}

void mostrar_menu_usuario() {
	printf("\n******* MENU PRINCIPAL MEMORIA ******");
	printf("\n 1 - Modifcar Retardo");
	printf("\n 2 - Generar Reporte");
	printf("\n 3 - Limpiar CACHE");
	printf("\n 4 - Mostrar Tamaño");
	printf("\n 5 - Salir");
	printf("\n 6 - DESARROLLO PROBAR MEMORIA");
	printf("\n Opcion: ");
}

void atender_solicitudes_de_usuario() {
	int opcion = 0;
	do {
		system("clear");
		mostrar_menu_usuario();
		opcion = validarNumeroInput(1, 6);
		system("clear");
		switch (opcion) {

		case 1:
			CU_Modificar_Retardo();
			break;
		case 2:
			CU_Generar_Reporte();
			break;
		case 3:
			CU_Limpiar_CACHE();
			break;
		case 4:
			CU_Mostrar_Tamanio_MEMORIA_PID();
			break;
		case 6:
			menu_pruebas();
			break;
		}
		if (opcion != 5) {
			printf("\n\nPress '0' to continue: ... ");
			fflush(stdin);
			while (getchar() != '0')
				;
		}
	} while (opcion != 5);
}

