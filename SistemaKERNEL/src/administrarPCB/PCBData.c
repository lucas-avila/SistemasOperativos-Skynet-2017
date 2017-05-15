#include <parser/metadata_program.h>

/*
	typedef struct {
		t_puntero_instruccion	start;
		t_size		offset;
	} t_intructions;

	typedef struct {
		t_puntero_instruccion	instruccion_inicio;	//El numero de la primera instruccion (Begin)
		t_size			instrucciones_size;				// Cantidad de instrucciones
		t_intructions*	instrucciones_serializado; 		// Instrucciones del programa

		t_size			etiquetas_size;					// Tamaño del mapa serializado de etiquetas
		char*			etiquetas;							// La serializacion de las etiquetas

		int				cantidad_de_funciones;
		int				cantidad_de_etiquetas;
	} t_metadata_program;
 */
void procesar_programa(char * programa){
	t_metadata_program * meta = metadata_desde_literal(programa);
	int program_counter = 0;

	while(program_counter < meta->instrucciones_size){
		char * instruccion = string_substring(programa, meta->instrucciones_serializado[program_counter].start, meta->instrucciones_serializado[program_counter].offset);
		instruccion[meta->instrucciones_serializado[program_counter].offset] = '\0';

		printf("%s\n", instruccion);
		program_counter++;
	}


	int debugeame = 0;
}



