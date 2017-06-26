/*
 * signals.c
 *
 *  Created on: 11/6/2017
 *      Author: utnso
 */

#include <stdlib.h>

#include "../../../Sharedlib/Sharedlib/Socket.h"
#include "../header/AppConfig.h"

void recibir_seniales_de_linux(int signum){
    //Terminar de ejecutar y salir
	printf("\na\n");
	CU_Terminar_ejecucion_y_finalizar();
}



