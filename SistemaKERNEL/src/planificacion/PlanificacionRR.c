#include "Planificacion.h"
#include "PlanificacionRR.h"
#include "../header/AppConfig.h"
#include "../administrarPCB/EstadisticaProceso.h"
#include "../general/Socket.h"

void ejecutar_algoritmo_planificacion_RR() {
	PCB* pcb;
	CPUInfo* cpu;
	atender_clientes(0, planificador_mediano_plazo);
	while (configuraciones.planificacion_activa == 1) {

		pcb = obtener_proceso_de_cola_READY();
		//Si pcb es NULL, se desactivo la planificacion, volvemos
		if(pcb == NULL) return;

		//Seteamos QUANTUM
		pcb->RR=1;
		pcb->cantidad_rafagas =configuraciones.QUANTUM;
		pcb->quantum_sleep=configuraciones.QUANTUM_SLEEP;

		//Esperamos hasta obtener una CPU
		cpu = obtener_CPU_Disponible();

		//Si la cpu es NULL, se desactivo la planificacion, volvemos
		if(cpu == NULL) return;

		marcar_CPU_Ocupada(cpu);
		mover_PCB_de_cola(pcb, READY, EJECUTANDO);
		enviar_PCB_Serializado_a_CPU(cpu, pcb);

	}
}
