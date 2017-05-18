/*
 * PCBData.h
 *
 *  Created on: 14/5/2017
 *      Author: utnso
 */

#ifndef ADMINISTRARPCB_PCBDATA_H_
#define ADMINISTRARPCB_PCBDATA_H_

#include "../header/PCB.h"

IndiceCodigo* crear_IndiceCodigo(int programCounter, int byteInicial, int tamanio, int pagina);
void procesar_programa(char * programa);

#endif /* ADMINISTRARPCB_PCBDATA_H_ */
