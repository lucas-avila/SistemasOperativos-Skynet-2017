/*
 * FuncionesAuxiliares.h
 *
 *  Created on: 1/5/2017
 *      Author: utnso
 */

#ifndef PRIMITIVAS_FUNCIONESAUXILIARES_H_
#define PRIMITIVAS_FUNCIONESAUXILIARES_H_

//t_puntero generarPunteroMemoria(char* proceso, char*pagina, char* byteInicial) ;



t_puntero serializarMemoriaDinamica(char* pagina, char* byteInicial);
DireccionMemoriaDinamica* deserializarMemoriaDinamica(char* proceso, t_puntero memoria_serializada);
#endif /* PRIMITIVAS_FUNCIONESAUXILIARES_H_ */
