/*
 * EstructurasDeDatosPrimitivas.h
 *
 *  Created on: 1/5/2017
 *      Author: utnso
 */

#ifndef PRIMITIVAS_ESTRUCTURASDEDATOSPRIMITIVAS_H_
#define PRIMITIVAS_ESTRUCTURASDEDATOSPRIMITIVAS_H_

typedef struct {
	unsigned int pagina;
	unsigned int byteInicial;

} DireccionVariable;

typedef struct {
	unsigned int filaStack;
	unsigned int esVariable; //1 si , 0 no
	unsigned int filaTabla; //Fila de la tabla, tanto en argumento como en variable

} PunteroVariable;

//Tipos de datos
typedef u_int32_t t_puntero;
typedef u_int32_t t_size;
typedef t_puntero t_puntero_instruccion;
typedef t_puntero t_descriptor_archivo;

typedef char t_nombre_variable;
typedef int t_valor_variable;

typedef t_nombre_variable* t_nombre_semaforo;
typedef t_nombre_variable* t_nombre_etiqueta;
typedef t_nombre_variable* t_nombre_compartida;
typedef t_nombre_variable* t_direccion_archivo;


#endif /* PRIMITIVAS_ESTRUCTURASDEDATOSPRIMITIVAS_H_ */
