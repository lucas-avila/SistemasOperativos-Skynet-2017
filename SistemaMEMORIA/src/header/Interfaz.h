/*
 * Interfaz.h
 *
 *  Created on: 12/4/2017
 *      Author: utnso
 */

#ifndef HEADER_INTERFAZ_H_
#define HEADER_INTERFAZ_H_

void CU_Recibir_Conexion_KERNEL(int cliente);
void CU_Recibir_Conexion_CPU(int cliente);
void CU_Recibir_conexiones(int servidor);



void CU_Solicitar_Bytes_de_Pagina(int cliente);
void CU_Almacenar_Bytes_de_Pagina(int cliente);
void CU_Inicializar_Programa(int cliente);
void CU_Asignar_Paginas_Programa(int cliente);
void CU_Finalizar_Programa(int cliente);
void CU_Liberar_Pagina(int cliente);
#endif /* HEADER_INTERFAZ_H_ */
