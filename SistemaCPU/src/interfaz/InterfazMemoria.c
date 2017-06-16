#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include "../header/AppConfig.h"
#include "../../../Sharedlib/Sharedlib/Socket.h"
#include "InterfazMemoria.h"
#include "../general/funcionesUtiles.h"

void iniciar_conexion_servidor_memoria() {
	servidor_Memoria = conectar_servidor(configuraciones.IP_MEMORIA, configuraciones.PUERTO_MEMORIA);
	enviar_dato_serializado("CPU", servidor_Memoria);
	//tamanio_pagina_memoria = atoi(recibir_dato_serializado(servidor_Memoria));

}

char* solicitar_bytes_memoria(char* PID, char* pagina, char* byteInicial, char* longitud) {
	enviar_dato_serializado("SOLICITAR_BYTE_MEMORIA", servidor_Memoria);
	enviar_dato_serializado(PID, servidor_Memoria);
	enviar_dato_serializado(pagina, servidor_Memoria);
	enviar_dato_serializado(byteInicial, servidor_Memoria);
	enviar_dato_serializado(longitud, servidor_Memoria);
	return recibir_dato_serializado(servidor_Memoria);
}

char* almacenar_Bytes_de_Pagina(char* PID, char* pagina, char* byteInicial, char* longitud, char* texto) {
	enviar_dato_serializado("ALMACENAR_BYTE_MEMORIA", servidor_Memoria);
	enviar_dato_serializado(PID, servidor_Memoria);
	enviar_dato_serializado(pagina, servidor_Memoria);
	enviar_dato_serializado(byteInicial, servidor_Memoria);
	enviar_dato_serializado(longitud, servidor_Memoria);
	enviar_dato_serializado(texto, servidor_Memoria);
	return recibir_dato_serializado(servidor_Memoria);
}

char* asignar_Paginas_Programa(char* PID, char* cantidad_paginas) {
	enviar_dato_serializado("ASIGNAR_PAGINAS_PROCESO", servidor_Memoria);
	enviar_dato_serializado(PID, servidor_Memoria);
	enviar_dato_serializado(cantidad_paginas, servidor_Memoria);
	return recibir_dato_serializado(servidor_Memoria);
}
