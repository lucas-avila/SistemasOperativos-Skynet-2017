/*
 * FuncionesAuxiliares.h
 *
 *  Created on: 1/5/2017
 *      Author: utnso
 */

#ifndef PRIMITIVAS_FUNCIONESAUXILIARES_H_
#define PRIMITIVAS_FUNCIONESAUXILIARES_H_

//t_puntero generarPunteroMemoria(char* proceso, char*pagina, char* byteInicial) ;
t_puntero generarPunteroStack(char* filaStack, char*tipoVariable, char* filaTabla);
PunteroVariable* deserializarPunteroStack(t_puntero punteroDireccion);

t_puntero serializarMemoriaDinamica(char* pagina, char* byteInicial);
Variable* deserializarMemoriaDinamica(t_puntero memoria_serializada);
#endif /* PRIMITIVAS_FUNCIONESAUXILIARES_H_ */
