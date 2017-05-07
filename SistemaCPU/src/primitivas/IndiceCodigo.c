#include "../header/PCB.h"
#include "commons/collections/list.h"
#include "IndiceCodigo.h"
#include<stdio.h>

IndiceCodigo* obtener_Indice_codigo(t_list* listaIndiceCodigo, int program_counter) {
	int tamanio = list_size(listaIndiceCodigo);
	int i = 0;
	for (i = 0; i < tamanio; i++) {
		IndiceCodigo* element = list_get(listaIndiceCodigo, i);
		if (element->program_counter == program_counter) {
			return element;
		}
	}
	return NULL;
}
