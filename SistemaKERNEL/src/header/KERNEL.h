/*
 * KERNEL.h
 *
 *  Created on: 28/4/2017
 *      Author: utnso
 */

#ifndef HEADER_KERNEL_H_
#define HEADER_KERNEL_H_

void inicializar_listas_globales();
void inicializar_listas_globales();
void inicializar_semaforos();
void inicializar_KERNEL();
void atender_solicitudes_de_usuario();
void mostrar_menu_usuario();
void CU_iniciar_programa(int consola);
void finalizar_proceso(Proceso * proceso);
void actualizar_exit_code(Proceso * proceso, int exit_code);

#endif /* HEADER_KERNEL_H_ */
