/*
 * InterfazConsola.h
 *
 *  Created on: 1/4/2017
 *      Author: utnso
 */

#ifndef HEADER_INTERFAZCONSOLA_H_
#define HEADER_INTERFAZCONSOLA_H_

void escuchar_Conexiones_Consola(int servidorConsola) ;
void CU_Recibir_Conexiones_Consola(int clienteConsola);
void agregar_consola_global(int numeroConexion, pthread_t hilo);
#endif /* HEADER_INTERFAZCONSOLA_H_ */
