#include "TablaProcesoArchivo.h"

#include <commons/string.h>
#include <stdlib.h>
#include <string.h>

#include "../header/SolicitudesUsuario.h"

TablaProcesoArchivo* new_TablaProcesoArchivo(int FD, char* flags, int ProcesoFD) {
	TablaProcesoArchivo* nuevo = malloc(sizeof(TablaProcesoArchivo));
	nuevo->FD = FD;
	nuevo->GlobalFD = ProcesoFD;
	nuevo->cursor_bloque = 0;
	nuevo->flags = malloc(strlen(flags) + 1);
	strcpy(nuevo->flags, flags);
	return nuevo;
}
void guardar_registro_Tabla_Proceso_Archivo(t_list* tabla, TablaProcesoArchivo* registro) {
	list_add(tabla, registro);
}
void modificar_registro_Tabla_Proceso_Archivo(t_list* tabla, TablaProcesoArchivo* registro) {

}
void eliminar_registro_Tabla_Proceso_Archivo(t_list* tabla, TablaProcesoArchivo* registro) {
	list_remove(tabla, buscar_indice_TablaProcesoArchivo(tabla, registro->FD));
}
int buscar_indice_TablaProcesoArchivo(t_list* tabla, int FD) {
	int tam = list_size(tabla);
	int i = 0;
	for (i = 0; i < tam; i++) {
		TablaProcesoArchivo* element = list_get(tabla, i);
		if (element->FD == FD) {
			return i;
		}
	}

	return -1;
}
TablaProcesoArchivo* buscar_TablaProcesoArchivo_por_FD(t_list* tabla, int FD) {
	int tam = list_size(tabla);
	int i = 0;
	for (i = 0; i < tam; i++) {
		TablaProcesoArchivo* element = list_get(tabla, i);
		if (element->FD == FD) {
			return element;
		}
	}

	return NULL;
}

void mostrar_tabla_proceso_archivos(t_list* tablaProceso) {

	string_append(&info_log, "\n -----------------------------------------------------");
	string_append(&info_log, "\n TABLA PROCESO DE ARCHIVOS");
	string_append(&info_log, "\n -----------------------------------------------------");
	string_append(&info_log, "\n FD \t\t GlobalFD \t CursorBloque \t   FLAGS");
	string_append(&info_log, "\n -----------------------------------------------------");
	int tamanio = list_size(tablaProceso);
	int i = 0;
	char* informacion;
	for (i = 0; i < tamanio; i++) {
		TablaProcesoArchivo* registro = list_get(tablaProceso, i);
		informacion = malloc(string_from_format("\n %d \t\t %d  \t\t %d \t\t %s", registro->FD, registro->GlobalFD, registro->cursor_bloque, registro->flags));
		informacion = string_from_format("\n %d \t\t %d  \t\t %d \t\t %s", registro->FD, registro->GlobalFD, registro->cursor_bloque, registro->flags);
		string_append(&info_log, informacion);

	}

}
