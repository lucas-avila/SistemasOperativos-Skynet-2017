/*
 * TestingMemoriaCache.h
 *
 *  Created on: 7/5/2017
 *      Author: utnso
 */

#ifndef TESTING_TESTINGMEMORIACACHE_H_
#define TESTING_TESTINGMEMORIACACHE_H_

void cargar_paginas_de_proceso(char *PID,int cantidadPagina, char* contenidoQueQuieras);

void solicitar_paginas_de_proceso(char *PID,int pagina,int cantidadVecesPedir);



#endif /* TESTING_TESTINGMEMORIACACHE_H_ */
