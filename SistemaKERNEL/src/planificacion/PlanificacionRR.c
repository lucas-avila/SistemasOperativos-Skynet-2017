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
		cpu = obtener_CPU_Disponible();
		marcar_CPU_Ocupada(cpu);
		mover_PCB_de_cola(pcb, READY, EJECUTANDO);
		enviar_PCB_Serializado_a_CPU(cpu, pcb);
	}
}
