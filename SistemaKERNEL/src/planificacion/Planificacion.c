#include "Planificacion.h"

#include <commons/collections/list.h>
#include <commons/string.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Sharedlib/Socket.h>

#include "../administrarPCB/EstadisticaProceso.h"
#include "../administrarPCB/PCBData.h"
#include "../capaMEMORIA/AdministrarSemaforos.h"
#include "../general/Semaforo.h"
#include "../header/AppConfig.h"
#include "../header/KERNEL.h"
#include "../header/SolicitudesUsuario.h"
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
	for (i=0; i < configuraciones.cantidad_sem; i++) {
		dictionary_put(COLAS, configuraciones.SEM_IDS[i], queue_create());
	}
}

void inicializar_buffer_codigo(){
	BUFFER_CODIGO = dictionary_create();
}

void inicializar_colas_5_estados() {
	COLAS = dictionary_create();
	dictionary_put(COLAS, NEW, queue_create());
	dictionary_put(COLAS, READY, queue_create());
	dictionary_put(COLAS, EXEC, list_create());
	dictionary_put(COLAS, WAITING, list_create());
	dictionary_put(COLAS, EXIT, queue_create());

	inicializar_colas_semaforos();
}

t_queue* cola(char * nombre) {
	return dictionary_get(COLAS, nombre);
}

void proceso_a_NEW(Proceso * p) {
	string_append(&p->cola, NEW);
	wait_cola(NEW);
	queue_push(cola(NEW), p->pcb);
	signal_cola(NEW);
	sem_post(&proceso_new);

	// Informar mediante log.
	informar_accion_en_log(p->PID, NEW);
}

void informar_accion_en_log(uint32_t PID, char * destino){
	sem_wait(&escribir_log);
	char * pid = string_new();
	pid = string_itoa(PID);
	string_append(&info_log, "Se movio el proceso (");
	string_append(&info_log, pid);
	string_append(&info_log, ") a la cola ");
	string_append(&info_log, destino);
	string_append(&info_log, ".");
	sem_post(&escribir_log);

	generar_log();
}

int mover_PCB_de_cola(PCB* pcb, char * origen, char * destino) {

	Proceso * p = proceso(pcb);

	//Validacion
	if(strcmp(p->cola, origen) != 0)
		return -1;

	wait_cola(origen);

	bool es_pcb_buscado(PCB * otro) {
		return otro->PID == pcb->PID;
	}

	if (strcmp(origen, EXEC) == 0) {

		list_remove_by_condition((t_list*) cola(EXEC), &es_pcb_buscado);
		//Marcamos el CPU que estaba usando como disponible
		marcar_CPU_Disponible(p->cpu);
		//Si va a algun waiting
		if (es_semaforo(destino))
			enviar_dato_serializado("BLOQUEADO", p->cpu->numeroConexion);

		p->cpu = NULL;
	} //Si viene de algun waiting
	else if (es_semaforo(origen)) {
		list_remove_by_condition((t_list*) cola(WAITING), &es_pcb_buscado);
		queue_pop(cola(origen));
	} else
		queue_pop(cola(origen));

	signal_cola(origen);

	if (strcmp(destino, EXIT) == 0){
		p->pcb=pcb;
		finalizar_proceso(p);
		sem_post(&grado_multiprogramacion);
	}

	char * cola_guardada = p->cola;
	strcpy(cola_guardada, "");
	string_append(&cola_guardada, destino);

	//Después de que se actualice el proceso lo metemos en la cola a la que iba
	wait_cola(destino);
	if (strcmp(destino, EXEC) == 0)
		list_add((t_list*) cola(destino), pcb);
	else if (es_semaforo(destino)) {
		queue_push(cola(destino), pcb);
		list_add((t_list*) cola(WAITING), pcb);
	} else
		queue_push(cola(destino), pcb);

	if(strcmp(destino, READY) == 0)
		sem_post(&proceso_ready);

	signal_cola(destino);
	informar_accion_en_log(pcb->PID, destino);
	return 0;
}

CPUInfo* obtener_CPU_Disponible() {
	int i = 0;
	while (configuraciones.planificacion_activa == 1) {
		sem_wait(&cpu_disponible);
		sem_wait(&mutex_lista_CPUs);
		int tamanioLista = list_size(lista_CPUs);
		for (i = 0; i < tamanioLista; i++) {
			CPUInfo* cpu_element = list_get(lista_CPUs, i);
			if (cpu_element->disponible == 1) {
				sem_post(&mutex_lista_CPUs);
				return cpu_element;
			}
		}
		sem_post(&mutex_lista_CPUs);
	}

	return NULL;
}

int cantidad_en_WAITING() {
	int i = 0;
	int res = 0;
	while (i < configuraciones.cantidad_sem) {
		res += queue_size(dictionary_get(COLAS, configuraciones.SEM_IDS[i]));
		i++;
	}
	return res;
}

int cantidad_procesos_en_memoria() {
	return (queue_size(cola(READY)) + list_size(cola(EXEC)) + cantidad_en_WAITING());
}

void planificador_largo_plazo() {
	while (configuraciones.planificacion_activa == 1) {
		sem_wait(&proceso_new);
		sem_wait(&grado_multiprogramacion);
		if (!queue_is_empty(cola(NEW))) {
			PCB * pcb = queue_peek(cola(NEW));
			char * PID = string_itoa(pcb->PID);
			char * codigo = dictionary_get(BUFFER_CODIGO, PID);
			procesar_programa(codigo, pcb);
			void my_free(char * elemento){
					free(elemento);
				}
			dictionary_remove_and_destroy(BUFFER_CODIGO, PID, &my_free);
			free(PID);
			mover_PCB_de_cola(pcb, NEW, READY);
		}

	}
}

PCB* obtener_proceso_de_cola_READY() {
	while (configuraciones.planificacion_activa == 1) {
		sem_wait(&proceso_ready);
		printf("\nproceso_ready es %d\n", proceso_ready);
		wait_cola(READY);
		if (!queue_is_empty(cola(READY))) {

			PCB* pcb = queue_peek(cola(READY));
			signal_cola(READY);
			return pcb;
		}
		signal_cola(READY);
	}
	return NULL;
}

void enviar_PCB_Serializado_a_CPU(CPUInfo* cpu, PCB* pcb) {
	//TODO: Haria falta chequear que haya llegado bien?
	//para testear TODO: BORRAR ESTO
	if(cpu == NULL) return;
	proceso(pcb)->cpu = cpu;
	//enviar_dato_serializado("TESTEAR_PLANIFICACION", cpu->numeroConexion);
	enviar_pcb(pcb, cpu->numeroConexion);
}

void marcar_CPU_Ocupada(CPUInfo* cpu) {
	if(cpu == NULL) return;
	cpu->disponible = 0;
	return;
}

void marcar_CPU_Disponible(CPUInfo* cpu) {
	if(cpu == NULL) return;
	sem_post(&cpu_disponible);
	cpu->disponible = 1;
	return;
}

int recepcion_SIGNAL_semaforo_ansisop(char * nombre_sem) {
	int res = signal_semaforo_ansisop(nombre_sem);

	if(res == -2)
		return res;


	PCB * pcb = queue_peek(cola(nombre_sem));

	if (pcb != NULL)
		mover_PCB_de_cola(pcb, nombre_sem, READY);

	return res;
}

void recibir_PCB_de_CPU(int clienteCPU, char * modo) {
	PCB* pcb = recibir_pcb(clienteCPU);

	if (strcmp(modo, "TERMINADO") == 0) {
		if(pcb->RR) incrementar_rafagas_ejecutadas(pcb->PID, 1);
		mover_PCB_de_cola(pcb, EXEC, EXIT);
	} else if (strcmp(modo, "QUANTUM") == 0) {
		incrementar_rafagas_ejecutadas(pcb->PID, 1);
		mover_PCB_de_cola(pcb, EXEC, READY);
	} else if (strcmp(modo, "WAIT_SEM") == 0) {
		char * nombre_sem = recibir_dato_serializado(clienteCPU);

		int resultado_sem = wait_semaforo_ansisop(nombre_sem);

		if (resultado_sem == -1){
			mover_PCB_de_cola(pcb, EXEC, nombre_sem);
		}else if(resultado_sem== -2){
			enviar_dato_serializado("SEMAFORO_NO_EXISTE", clienteCPU);
		}else{
			enviar_dato_serializado("NO_BLOQUEADO", clienteCPU);
		}
	}

}

void limpiar_procesos_CPU(int clienteCPU, char * modo){
	bool buscar_procesos_por_cola(Proceso * proceso) {
		if(proceso->cpu == NULL) return false;
		return (proceso->cpu->numeroConexion == clienteCPU);
	}
	sem_wait(&mutex_lista_PROCESOS);
	t_list * procesos_filtrados = list_filter(procesos, &buscar_procesos_por_cola);
	sem_post(&mutex_lista_PROCESOS);
	Proceso * buscado = list_get(procesos_filtrados, 0);
	if(buscado != NULL){
		if(strcmp(modo, "REPLANIFICAR") == 0){
			mover_PCB_de_cola(buscado->pcb, EXEC, READY);
		}
		else{
			buscado->pcb->exit_code = -23; // CPU_HEAVY_METAL
			mover_PCB_de_cola(buscado->pcb, EXEC, EXIT);
		}
	}
}

void wait_cola(char * cola) {
	if (strcmp(cola, NEW) == 0) {
		sem_wait(&mutex_cola_NEW);
	} else if (strcmp(cola, READY) == 0) {
		sem_wait(&mutex_cola_READY);
	} else if (strcmp(cola, EXEC) == 0) {
		sem_wait(&mutex_cola_EXEC);
	} else if (strcmp(cola, WAITING) == 0 || es_semaforo(cola)) {
		sem_wait(&mutex_cola_WAITING);
	} else if (strcmp(cola, EXIT) == 0) {
		sem_wait(&mutex_cola_EXIT);
	}
}

void signal_cola(char * cola) {
	if (strcmp(cola, NEW) == 0) {
		sem_post(&mutex_cola_NEW);
	} else if (strcmp(cola, READY) == 0) {
		sem_post(&mutex_cola_READY);
	} else if (strcmp(cola, EXEC) == 0) {
		sem_post(&mutex_cola_EXEC);
	} else if (strcmp(cola, WAITING) == 0 || es_semaforo(cola)) {
		sem_post(&mutex_cola_WAITING);
	} else if (strcmp(cola, EXIT) == 0) {
		sem_post(&mutex_cola_EXIT);
	}
}

