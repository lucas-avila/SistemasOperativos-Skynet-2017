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

	//Provisorio, necesito avisarle a la CPU que terminamos
	enviar_dato_serializado("DESCONECTAR", servidor_kernel);
    exit(0);
}



