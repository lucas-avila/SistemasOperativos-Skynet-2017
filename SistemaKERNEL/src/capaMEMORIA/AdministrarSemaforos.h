/*
 * AdministrarSemaforos.h
 *
 *  Created on: 27/5/2017
 *      Author: utnso
 */

#ifndef CAPAMEMORIA_ADMINISTRARSEMAFOROS_H_
#define CAPAMEMORIA_ADMINISTRARSEMAFOROS_H_

int obtener_valor_semaforo(char * nombre_sem);

int signal_semaforo_ansisop(char * nombre_sem);

int wait_semaforo_ansisop(char * nombre_sem);

int semaforo_ansisop_index(char * nombre_sem);

#endif /* CAPAMEMORIA_ADMINISTRARSEMAFOROS_H_ */
