#include "Planificacion.h"

#include <commons/collections/list.h>
#include <commons/string.h>
#include <semaphore.h>
#include <string.h>

#include "../capaMEMORIA/AdministrarSemaforos.h"
#include "../general/Semaforo.h"
#include "../general/Socket.h"
#include "../header/AppConfig.h"
#include "PlanificacionFIFO.h"
#include "PlanificacionRR.h"

void EJECUTAR_ALGORITMO_PLANIFICACION() {
	atender_clientes(0, planificador_largo_plazo);
	if (strcmp(configuraciones.ALGORITMO, "FIFO") == 0) {
		dispatcher_FIFO();
	} else if (strcmp(configuraciones.ALGORITMO, "RR") == 0) {
		dispatcher_RR();
	}
}

void inicializar_colas_semaforos() {
	int i = 0;
	for(i; i < configuraciones.cantidad_sem; i++){
		dictionary_put(COLAS, configuraciones.SEM_IDS[i], queue_create());
	}
}

void inicializar_colas_5_estados() {
	COLAS = dictionary_create();
	dictionary_put(COLAS, NEW, queue_create());
	dictionary_put(COLAS, READY, queue_create());
	dictionary_put(COLAS, WAITING, queue_create());
	dictionary_put(COLAS, EXEC, queue_create());
	dictionary_put(COLAS, EXIT, queue_create());
	inicializar_colas_semaforos();
}

t_queue* cola(char * nombre) {
	return dictionary_get(COLAS, nombre);
}

void proceso_a_NEW(Proceso * p){
	string_append(&p->cola, NEW);
	queue_push(cola(NEW), p->pcb);
}

void mover_PCB_de_cola(PCB* pcb, char * origen, char * destino) {
	queue_pop(cola(origen));

	Proceso * p = proceso(pcb);

	//Si viene de algun waiting
	if(es_semaforo(origen)) {

	}

	//Si va a algun waiting
	if(es_semaforo(destino)){
		marcar_CPU_Disponible(p->cpu);
		enviar_dato_serializado("BLOQUEADO", p->cpu->numeroConexion);
		p->cpu = NULL;
	}else if(strcmp(destino, EXIT) == 0){
		marcar_CPU_Disponible(p->cpu);
		pcb->exit_code = 0;
		//finalizar_proceso(pcb);
	}

	char * cola_guardada = p->cola;
	strcpy(cola_guardada, "");
	string_append(&cola_guardada, destino);

	//Después de que se actualice el proceso lo metemos en la cola a la que iba
	queue_push(cola(destino), pcb);
}

CPUInfo* obtener_CPU_Disponible() {
	int i = 0;
	while (configuraciones.planificacion_activa == 1) {
		int tamanioLista = list_size(lista_CPUs);
		for (i = 0; i < tamanioLista; i++) {
			CPUInfo* cpu_element = list_get(lista_CPUs, i);
			if (cpu_element->disponible == 1) {
				return cpu_element;
			}
		}
	}
	return NULL;
}

int cantidad_en_WAITING(){
	int i = 0;
	int res = 0;
	while(i < configuraciones.cantidad_sem){
		res += queue_size(dictionary_get(COLAS, configuraciones.SEM_IDS[i]));
		i++;
	}
	return res;
}

int cantidad_procesos_en_memoria(){
	return (queue_size(cola(READY)) + queue_size(cola(EXEC)) + cantidad_en_WAITING());
}

void planificador_largo_plazo() {
	while (configuraciones.planificacion_activa == 1) {

		if (configuraciones.GRADO_MULTIPROG > cantidad_procesos_en_memoria()) {
			if (!queue_is_empty(cola(NEW))) {
				mover_PCB_de_cola(queue_peek(cola(NEW)), NEW, READY);
			}
		}
	}
}

PCB* obtener_proceso_de_cola_READY() {
	while (configuraciones.planificacion_activa == 1) {
		//SE CONTROLA LA MULTIPROGRAMACION DE ESTA MANERA
		sem_wait(&mutex_cola_READY);
		if (!queue_is_empty(cola(READY))) {

			PCB* pcb = queue_peek(cola(READY));
			sem_post(&mutex_cola_READY);
			return pcb;
		}
		sem_post(&mutex_cola_READY);
	}
	return NULL;
}

void enviar_PCB_Serializado_a_CPU(CPUInfo* cpu, PCB* pcb) {
	//TODO: Haria falta chequear que haya llegado bien?
	//para testear TODO: BORRAR ESTO

	proceso(pcb)->cpu = cpu;
	enviar_dato_serializado("TESTEAR_PLANIFICACION", cpu->numeroConexion);
	enviar_pcb(pcb, cpu->numeroConexion);
}

//TODO: VOLVER A VERIFICAR SI ESTA FUNCION ANDA BIEN, Y SI NECESITA SEMAFOROS.
void marcar_CPU_Ocupada(CPUInfo* cpu) {
	cpu->disponible = 0;
	return;
}

void marcar_CPU_Disponible(CPUInfo* cpu){
	cpu->disponible = 1;
	return;
}

void recepcion_PCB_en_COLA_EXIT() {
	if (strcmp(configuraciones.ALGORITMO, "FIFO") == 0) {
		recepcion_PCB_en_COLA_EXIT_FIFO();
	}else {
		recepcion_PCB_en_COLA_EXIT_RR();
	}
}

void recepcion_SIGNAL_semaforo_ansisop(char * nombre_sem){
	signal_semaforo_ansisop(nombre_sem);

	PCB * pcb = queue_peek(cola(nombre_sem));

	if(pcb != NULL)
		mover_PCB_de_cola(pcb, nombre_sem, READY);
}

void recibir_PCB_de_CPU(int clienteCPU, char * modo) {
	PCB* pcb = recibir_pcb(clienteCPU);

	if(strcmp(modo, "TERMINADO") == 0){
		mover_PCB_de_cola(pcb, EXEC, EXIT);
	} else if(strcmp(modo, "QUANTUM") == 0){
		mover_PCB_de_cola(pcb, EXEC, READY);
	} else if(strcmp(modo, "WAIT_SEM") == 0){
		char * nombre_sem = recibir_dato_serializado(clienteCPU);
		int resultado_sem = wait_semaforo_ansisop(nombre_sem);

		if(resultado_sem == 0)
			mover_PCB_de_cola(pcb, EXEC, nombre_sem);
		else
			enviar_dato_serializado("NO_BLOQUEADO", clienteCPU);
	}

}

