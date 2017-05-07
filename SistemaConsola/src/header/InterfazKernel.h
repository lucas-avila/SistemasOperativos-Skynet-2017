/*
 * InterfazKernel.h
 *
 *  Created on: 1/4/2017
 *      Author: utnso
 */

#ifndef HEADER_INTERFAZKERNEL_H_
#define HEADER_INTERFAZKERNEL_H_

char * enviar_programa_ANSISOP(char * path_archivo_fuente, int kernel);
void CU_handshake_consola(int kernel);
void CU_handshake_programa(int kernel);

void enviar_solicitud_cancelar_proceso();

void recibir_mensajes_consola();

#endif /* HEADER_INTERFAZKERNEL_H_ */
