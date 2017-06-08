
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <semaphore.h>
#include <signal.h>

void inicializar_semaforo_en(sem_t* variable_semaforo, int value) {
	sem_init(variable_semaforo, 0, value); //-->Inicializo un semaforo
	signal(SIGPIPE, SIG_IGN);
}

void inicializar_semaforo(sem_t* variable_semaforo) {
	sem_init(variable_semaforo, 0, 1); //-->Inicializo un semaforo
	signal(SIGPIPE, SIG_IGN);
}
void destruir_semaforo(sem_t* variable_semaforo) {
	sem_destroy(variable_semaforo);
}

void activar_semaforo(sem_t* variable_semaforo) {
	//printf("\n Active Semaforo",variable_semaforo->__align );
	sem_wait(variable_semaforo);
}
void desactivar_semaforo(sem_t* variable_semaforo) {
//	printf("\n Desactive Semaforo",variable_semaforo->__align );
	sem_post(variable_semaforo);
}
