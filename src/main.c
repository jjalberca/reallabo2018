#include "asf.h"
#include "main.h"
#include "stdio_serial.h"
#include <delay.h>



// Mensaje de inicio
#define STRING_HEADER "-- Blink LED Example --\r\n" \
		"-- "BOARD_NAME" --\r\n" \
		"-- Compiled: "__DATE__" "__TIME__" --\r"


/**
 * Configuración de la interrupción periódica
 */
static void tc_config (void){
	// Encendemos el timer0
	pmc_enable_periph_clk(ID_TC0);
	// Desactivamos interupciones antes de cambiar nada
	NVIC_DisableIRQ(TC0_IRQn);
	// Y borramos interrupciones pendientes
	NVIC_ClearPendingIRQ(TC0_IRQn);
	// Establecemos la prioridad (Cuanto mayor el número menor prioridad)
	NVIC_SetPriority(TC0_IRQn, 9);

	// Configuración del timer
	// Básicamente rellena el Channel Mode Register
	tc_init(TC0, 0, 0 |
		// Fuente de reloj MCK/2
		TC_CMR_TCCLKS_TIMER_CLOCK1 |
		// Permite el registro A
		TC_CMR_WAVE |
		// Permite el registro B
		// (Aunque no sé por qué)
		TC_CMR_EEVT_XC0 |
		// Activa el clear automático del timer cuando se alcanza RC
		TC_CMR_CPCTRG);

	// Activa las interrupciones para los registros
	// Básicamente rellena el Interrupt Enable Register
	tc_enable_interrupt(TC0, 0, 0 |
		TC_IER_CPAS | // RA
		TC_IER_CPBS | // RB
		TC_IER_CPCS); // RC

	// Mete valores en los registros de comparación
	tc_write_rc(TC0,0,42000000U);
	tc_write_rb(TC0,0,21000000U);
	tc_write_ra(TC0,0,10500000U);
	tc_start(TC0,0);

	// Permitimos las interrupciones
	NVIC_EnableIRQ(TC0_IRQn);
}

// Rutina de atención a la interrupción
void TC0_Handler(void) {
	// Leemos el registro de estado
	// Para saber qué ha causado la interrupción
	uint32_t tc_status = tc_get_status(TC0, 0);
	if (tc_status & TC_SR_CPAS)
	{
		printf("AA: %lu\r\n", sys_ticks);
	}
	if (tc_status & TC_SR_CPBS)
	{
		printf("BB: %lu\r\n", sys_ticks);
	}
	if (tc_status & TC_SR_CPCS)
	{
		printf("CC: %lu\r\n", sys_ticks);
		pio_toggle_pin(PIO_PB27_IDX);
	}
	if (tc_status & TC_SR_COVFS)
	{
		// Nunca se llega aqui:
		// No está activada la interrupción de overflow
		// No puede llegar a hacer overflow porque se reinicia en RC
	}
}

static void pwm_config(void){
	pwm_channel_t pwm_channel_instance;
	pmc_enable_periph_clk(ID_PWM);
	pwm_channel_disable(PWM, PWM_CHANNEL_0);
	pwm_clock_t clock_setting = {
	    .ul_clka = 1000 * 200,
	    .ul_clkb = 0,
	    .ul_mck = sysclk_get_cpu_hz()
	};
	pwm_init(PWM, &clock_setting);
		/* Initialize PWM channel for LED0 */
		/* Period is left-aligned */
		pwm_channel_instance.alignment = PWM_ALIGN_LEFT;
		/* Output waveform starts at a low level */
		pwm_channel_instance.polarity = PWM_HIGH;
		/* Use PWM clock A as source clock */
		pwm_channel_instance.ul_prescaler = PWM_CMR_CPRE_CLKA;
		/* Period value of output waveform */
		pwm_channel_instance.ul_period = 200;
		/* Duty cycle value of output waveform */
		pwm_channel_instance.ul_duty = 50;
		pwm_channel_instance.channel = PWM_CHANNEL_0;
	pwm_channel_init(PWM, &pwm_channel_instance);

	//pwm_channel_enable_interrupt(PWM, PWM_CHANNEL_0, 0);
	pwm_channel_enable(PWM, PWM_CHANNEL_0);
}


/**
 * Application entry point
 */
int main(void) {
	// Rutina de configuración
	due_init();

	ioport_init();

	pio_configure_pin(PIO_PC3_IDX, 0 |
		PIO_TYPE_PIO_OUTPUT_1 |
		PIO_DEFAULT);
	pio_configure_pin(PIO_PC3_IDX, 0 |
		PIO_PERIPH_B |
		PIO_DEFAULT);

	// Imprime el mensaje de inicio
	puts(STRING_HEADER);

	// Enciende la interfaz de I/O del puerto B
	pmc_enable_periph_clk(ID_PIOB);
	// Establece el pin 27 como salida
	pio_set_output(PIOB, PIO_PB27, LOW, DISABLE, ENABLE);



	// Rutina de configuración del timer
	tc_config();

	pwm_config();

	// Infinite loop
	while (1) {

	}
}
