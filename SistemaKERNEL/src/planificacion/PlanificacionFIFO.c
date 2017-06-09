#include "Planificacion.h"
#include "PlanificacionFIFO.h"
#include "../header/AppConfig.h"
#include "../administrarPCB/EstadisticaProceso.h"
#include "../general/Socket.h"

void dispatcher_FIFO() {
	static int n = 1;
	PCB* pcb;
	CPUInfo* cpu;
	while (configuraciones.planificacion_activa == 1) {

		printf("Pase por 1\n", n);
		pcb = obtener_proceso_de_cola_READY();
		printf("Pase por 2 %d veces\n", n);
		//Si pcb es NULL, se desactivo la planificacion, volvemos
		if(pcb == NULL) return;

		//Seteamos RR a false
		pcb->RR = 0;

		//Esperamos hasta obtener una CPU
		cpu = obtener_CPU_Disponible();
		printf("Pase por 3 %d veces\n", n);
		//Si la cpu es NULL, se desactivo la planificacion, volvemos
		if(cpu == NULL) return;

		marcar_CPU_Ocupada(cpu);

		printf("Pase por 4 %d veces\n", n);
		mover_PCB_de_cola(pcb, READY, EXEC);
		printf("Pase por 5 %d veces\n", n);
		enviar_PCB_Serializado_a_CPU(cpu, pcb);
		printf("Pase por 6 %d veces\n", n);
		n++;
	}
}

void recepcion_PCB_en_COLA_EXIT_FIFO() {
	PCB* pcb;
	pcb = queue_pop(cola(EXIT));
	//INFORMACION ESTADISTICA
	//incrementar_rafagas_ejecutadas();
	queue_push(cola(EXIT), pcb);
}
