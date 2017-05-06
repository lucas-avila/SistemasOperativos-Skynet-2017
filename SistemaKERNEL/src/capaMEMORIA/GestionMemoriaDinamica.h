/*
 * GestionMemoriaDinamica.h
 *
 *  Created on: 1/5/2017
 *      Author: utnso
 */

#ifndef CAPAMEMORIA_GESTIONMEMORIADINAMICA_H_
#define CAPAMEMORIA_GESTIONMEMORIADINAMICA_H_



void CU_Gestionar_HEAP(int conexionCPU);
void CU_Reservar_memoria_MALLOC(int conexionCPU);
void CU_Liberar_memoria_FREE(int conexionCPU);
void enviar_datos_respuesta(int conexionCPU, unsigned pagina, char*PID, int byteInicial) ;





#endif /* CAPAMEMORIA_GESTIONMEMORIADINAMICA_H_ */
