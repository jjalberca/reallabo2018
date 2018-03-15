#include "periodic_tc.h"

#define FREC_MCK 84000000U

void periodic_init(unsigned int frec){
	// Encendemos el timer0
	pmc_enable_periph_clk(PTC_PCM);
	// Desactivamos interupciones antes de cambiar nada
	NVIC_DisableIRQ(PTC_IRQ);
	// Y borramos interrupciones pendientes
	NVIC_ClearPendingIRQ(PTC_IRQ);
	// Establecemos la prioridad (Cuanto mayor el número menor prioridad)
	NVIC_SetPriority(PTC_IRQ, PTC_PRIORITY);
	// Configuración del timer
	// Básicamente rellena el Channel Mode Register
	tc_init(PTC_INT, PTC_INT_CH, 0 |
		// Fuente de reloj MCK/2
		TC_CMR_TCCLKS_TIMER_CLOCK1 |
		// Activa el clear automático del timer cuando se alcanza RC
		TC_CMR_CPCTRG);
	// Activa las interrupciones para los registros
	// Básicamente rellena el Interrupt Enable Register
	tc_enable_interrupt(PTC_INT, PTC_INT_CH, 0 |
		TC_IER_CPAS | // RA
		TC_IER_CPBS | // RB
		TC_IER_CPCS); // RC
		// Mete valores en los registros de comparación
	tc_write_rc(PTC_INT,PTC_INT_CH,FREC_MCK/2/frec);
	tc_start(PTC_INT,PTC_INT_CH);
	// Permitimos las interrupciones
	NVIC_EnableIRQ(PTC_IRQ);
}
