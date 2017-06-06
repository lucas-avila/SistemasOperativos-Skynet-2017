#include "PlanificacionRR.h"

#include <stddef.h>

#include "../administrarProcesos/Proceso.h"
#include "../general/Socket.h"
#include "../header/AppConfig.h"
#include "../header/Estructuras.h"
#include "../header/PCB.h"
#include "Planificacion.h"

void dispatcher_RR() {
	PCB* pcb;
	CPUInfo* cpu;
	while (configuraciones.planificacion_activa == 1) {

		pcb = obtener_proceso_de_cola_READY();
		//Si pcb es NULL, se desactivo la planificacion, volvemos
		if(pcb == NULL) return;

		//Seteamos QUANTUM
		pcb->RR=1;
		pcb->cantidad_rafagas = configuraciones.QUANTUM;
		pcb->quantum_sleep = configuraciones.QUANTUM_SLEEP;

		//Esperamos hasta obtener una CPU
		cpu = obtener_CPU_Disponible();

		//Si la cpu es NULL, se desactivo la planificacion, volvemos
		if(cpu == NULL) return;

		marcar_CPU_Ocupada(cpu);
		mover_PCB_de_cola(pcb, READY, EXEC);
		enviar_PCB_Serializado_a_CPU(cpu, pcb);

	}
}

void recepcion_PCB_en_COLA_EXIT_RR(){
	PCB* pcb;
	pcb = queue_pop(cola(EXIT));
	//INFORMACION ESTADISTICA
	//incrementar_rafagas_ejecutadas();
	queue_push(cola(EXIT), pcb);
}
