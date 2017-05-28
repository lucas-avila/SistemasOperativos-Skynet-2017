/*
 * AdministrarSemaforos.c
 *
 *  Created on: 27/5/2017
 *      Author: utnso
 */

int obtener_valor_semaforo(char * nombre_sem){

	int i = 0;
	while(i < configuraciones.cantidad_sem){
		if(strcmp(configuraciones.SEM_IDS[i], nombre_sem) == 0){

		}
		i++;
	}

}
