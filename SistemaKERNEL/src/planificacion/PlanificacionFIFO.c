#include "Planificacion.h"
#include "PlanificacionFIFO.h"
#include "../header/AppConfig.h"
#include "../administrarPCB/EstadisticaProceso.h"
#include "../general/Socket.h"

void dispatcher_FIFO() {
	PCB* pcb;
	CPUInfo* cpu;
	while (configuraciones.planificacion_activa == 1) {
		pcb = obtener_proceso_de_cola_READY();
		//Si pcb es NULL, se desactivo la planificacion, volvemos
		if(pcb == NULL) return;

		//Seteamos RR a false
		pcb->RR = 0;

		//Esperamos hasta obtener una CPU
		cpu = obtener_CPU_Disponible();

		//Si la cpu es NULL, se desactivo la planificacion, volvemos
		if(cpu == NULL) return;

		marcar_CPU_Ocupada(cpu);
		mover_PCB_de_cola(pcb, READY, EXEC);
		enviar_PCB_Serializado_a_CPU(cpu, pcb);
	}
}

void recepcion_PCB_en_COLA_EXIT_FIFO() {
	PCB* pcb;
	pcb = queue_pop(cola(EXIT));
	//INFORMACION ESTADISTICA
	//incrementar_rafagas_ejecutadas();
	queue_push(cola(EXIT), pcb);
}
