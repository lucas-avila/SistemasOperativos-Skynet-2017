/*
 ============================================================================
 Name        : CPU.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "header/AppConfig.h"
#include "header/Socket.h"


void CU_Procesar_PCB_a_ejecutar(int kernel);


int main(int argc, char *argv[]) {
	puts(argv[1]);
	inicializar_configuracion(argv[1]);
	controlEjecucionPrograma =false;
	int kernel = iniciar_conexion_servidor(configuraciones.IP_KERNEL, configuraciones.PUERTO_KERNEL);
    //Parametro de Identificacion
	enviar_dato_serializado("CPU", kernel);
	bool controlSeguir = true;
	do{
       char *operacion = recibir_dato_serializado(kernel);
       //SEÑAL ENVIADA POR KERNEL PARA SALIR
       if(strcmp(operacion,"SIGUSR1")==0){
    	   while(controlEjecucionPrograma==true){
    	   }
    	   controlSeguir=false;
       }else if(strcmp(operacion,"RECIBIR_PCB")==0){
           CU_Procesar_PCB_a_ejecutar(kernel);
       }
	}while(controlSeguir);
	return EXIT_SUCCESS;
}

void CU_Procesar_PCB_a_ejecutar(int kernel){

}
