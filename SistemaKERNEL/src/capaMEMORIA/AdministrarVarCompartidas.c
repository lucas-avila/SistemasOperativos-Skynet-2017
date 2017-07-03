/*
 * AdministrarVarCompartidas.c
 *
 *  Created on: 15/5/2017
 *      Author: utnso
 */
#include <stdio.h>
#include <string.h>
#include "AdministrarVarCompartidas.h"
#include "../header/AppConfig.h"

void obtener_valor_var_comp(int conexionCPU){
	char* nombreVarComp = recibir_dato_serializado(conexionCPU);
	int valor;
	int i = 0;
	while (i < configuraciones.cantidad_var_comp){
		if (strcmp(configuraciones.VAR_COMP[i], nombreVarComp)== 0){

			valor = VECTOR_VAR_COM[i];
			enviar_dato_serializado("OK",conexionCPU);
			enviar_dato_serializado(string_itoa(valor), conexionCPU);
			return;
		}

		i++;
	}

	 enviar_dato_serializado("No existe la variable compartida solicitada\n",conexionCPU);
	 return;
}

void asignar_valor_var_comp (int conexionCPU){
	char* nombreVarComp = recibir_dato_serializado(conexionCPU);
	int valorVarComp = atoi(recibir_dato_serializado(conexionCPU));
	int i = 0;
	while (i < configuraciones.cantidad_var_comp){
			if (strcmp(configuraciones.VAR_COMP[i], nombreVarComp)== 0){
				VECTOR_VAR_COM[i] = valorVarComp;
				enviar_dato_serializado("OK",conexionCPU);
				return;
			}
			i++;

	}
	enviar_dato_serializado("No existe la variable compartida solicitada\n",conexionCPU);
	return;
}
