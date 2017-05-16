/*
 * SolicitudesUsuario.h
 *
 *  Created on: 1/4/2017
 *      Author: utnso
 */

#ifndef HEADER_SOLICITUDESUSUARIO_H_
#define HEADER_SOLICITUDESUSUARIO_H_

void atender_solicitudes_de_usuario();
void mostrar_menu_usuario() ;
void mostrar_cola(COLA cola);
void mostrar_menu_colas();
void mostrar_info_procesos(t_list * procesos_lista);
void listar_procesos_por_cola();
void listar_procesos();
void mostrar_menu_listado_procesos();

#endif /* HEADER_SOLICITUDESUSUARIO_H_ */
