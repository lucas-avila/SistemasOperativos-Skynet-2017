
#include <semaphore.h>
#include <signal.h>
#include <pthread.h>

void inicializar_semaforo(sem_t* variable_semaforo, int valor) {
	sem_init(variable_semaforo, 0, valor); //-->Inicializo un semaforo
	signal(SIGPIPE, SIG_IGN);
}
void destruir_semaforo(sem_t* variable_semaforo) {
	sem_destroy(variable_semaforo);
}

void activar_semaforo(sem_t* variable_semaforo) {
	sem_wait(variable_semaforo);
}
void desactivar_semaforo(sem_t* variable_semaforo) {
	sem_post(variable_semaforo);
}
