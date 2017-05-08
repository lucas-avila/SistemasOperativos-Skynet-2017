#include "Planificacion.h"
#include "PlanificacionFIFO.h"
#include "../header/AppConfig.h"
#include "../administrarPCB/EstadisticaProceso.h"

void ejecutar_algoritmo_planificacion_FIFO() {
	PCB* pcb;
	CPUInfo* cpu;
	while (configuraciones.planificacion_activa == 1) {
		pcb = obtener_proceso_de_cola_NEW();
		mover_PCB_de_cola(pcb, NEW, READY);
		cpu = obtener_CPU_Disponible();
		marcar_CPU_Ocupada(cpu);
		mover_PCB_de_cola(pcb, READY, EJECUTANDO);
		enviar_PCB_Serializado_a_CPU(cpu, pcb);
	}
}

void recepcion_PCB_en_COLA_EXIT_FIFO() {
	PCB* pcb;
	pcb = queue_pop(COLA_EXIT);

	//INFORMACION ESTADISTICA
	//incrementar_rafagas_ejecutadas();

	queue_push(COLA_EXIT, pcb);
}
