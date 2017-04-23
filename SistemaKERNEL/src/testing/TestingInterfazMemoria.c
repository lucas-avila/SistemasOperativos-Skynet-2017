#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "../interfaz/InterfazMemoria.h"
#include "commons/string.h"
void testear_intefaz(){
    inicializar_Programa("1241","2");
    inicializar_Programa("1241","2");
    inicializar_Programa("1241","2");
    inicializar_Programa("1241","2");
    solicitar_bytes_memoria("1241","0","0","10");
    almacenar_Bytes_de_Pagina("1241","0","0",string_itoa(strlen("jonathan10jonathan10jonathan10jonathan10jonathan10jonathan10jonathan10")),"jonathan10jonathan10jonathan10jonathan10jonathan10jonathan10jonathan10");
    asignar_Paginas_Programa("1241","1");
    almacenar_Bytes_de_Pagina("1241","2","0",string_itoa(strlen("jonathan10")),"jonathan10");
    finalizar_Programa("1241");

    inicializar_Programa("1371","4");
        inicializar_Programa("1371","5");
        almacenar_Bytes_de_Pagina("1371","5","0",string_itoa(strlen("jonathan10")),"jonathan10");
        solicitar_bytes_memoria("1371","5","0","20");
}
