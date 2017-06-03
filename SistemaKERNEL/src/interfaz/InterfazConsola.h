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
void agregar_consola_global(int consola_ID);
int index_of_consola(int numeroConexion);
void retirar_consola_global(int numeroConexion);

#endif /* HEADER_INTERFAZCONSOLA_H_ */
