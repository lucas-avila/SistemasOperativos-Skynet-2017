
#include <semaphore.h>
#include <signal.h>

void inicializar_semaforo(sem_t* variable_semaforo) {
	sem_init(variable_semaforo, 0, 1); //-->Inicializo un semaforo
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
