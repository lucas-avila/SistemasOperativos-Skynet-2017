/*
 * InterfazConsola.h
 *
 *  Created on: 1/4/2017
 *      Author: utnso
 */

#ifndef HEADER_INTERFAZCONSOLA_H_
#define HEADER_INTERFAZCONSOLA_H_

void iniciar_conexion_servidor_programa();
void escuchar_Conexiones_Programa(int servidorConsola) ;
void CU_Recibir_Conexiones_Programa(int clientePrograma);
void notificar_exit_code(int exit_code, int socket);

#endif /* HEADER_INTERFAZCONSOLA_H_ */
