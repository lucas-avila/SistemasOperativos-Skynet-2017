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
	//printf("El valor es %s\n", nombreVarComp);
	int valor;
	int i = 0;
	char * varCompartida = string_new();
	if((configuraciones.cantidad_var_comp > 0) && string_starts_with(configuraciones.VAR_COMP[0], "!") && !string_starts_with(nombreVarComp, "!")){
		string_append(&varCompartida, "!");
	}
	string_append(&varCompartida, nombreVarComp);
	while (i < configuraciones.cantidad_var_comp){
		if (strcmp(configuraciones.VAR_COMP[i], varCompartida)== 0){

			valor = VECTOR_VAR_COM[i];
			enviar_dato_serializado("OK",conexionCPU);
			enviar_dato_serializado(string_itoa(valor), conexionCPU);
			free(nombreVarComp);
			free(varCompartida);
			return;
		}

		i++;
	}
	free(nombreVarComp);
	free(varCompartida);
	enviar_dato_serializado("No existe la variable compartida solicitada\n",conexionCPU);
	return;
}

void asignar_valor_var_comp (int conexionCPU){
	char* nombreVarComp = recibir_dato_serializado(conexionCPU);
	int valorVarComp = atoi(recibir_dato_serializado(conexionCPU));
	int i = 0;
	char * varCompartida = string_new();
	if((configuraciones.cantidad_var_comp > 0) && string_starts_with(configuraciones.VAR_COMP[0], "!") && !string_starts_with(nombreVarComp, "!")){
		string_append(&varCompartida, "!");
	}
	string_append(&varCompartida, nombreVarComp);
	while (i < configuraciones.cantidad_var_comp){
			if (strcmp(configuraciones.VAR_COMP[i], varCompartida)== 0){
				VECTOR_VAR_COM[i] = valorVarComp;
				enviar_dato_serializado("OK",conexionCPU);
				free(varCompartida);
				free(nombreVarComp);
				return;
			}
			i++;

	}
	free(varCompartida);
	free(nombreVarComp);
	enviar_dato_serializado("No existe la variable compartida solicitada\n",conexionCPU);
	return;
}
