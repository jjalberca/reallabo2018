#include "asf.h"
#include "main.h"
#include "stdio_serial.h"
#include <delay.h>



// Mensaje de inicio
#define STRING_HEADER "-- Blink LED Example --\r\n" \
		"-- "BOARD_NAME" --\r\n" \
		"-- Compiled: "__DATE__" "__TIME__" --\r"


#define TC_INT TC0
#define TC_INT_CH 1
#define TC_IRQ TC1_IRQn

#define TC_PWM TC0
#define TC_PWM_CH 0

/**
 * Configuración de la interrupción periódica
 */
static void tc_config (void){
	// Encendemos el timer0
	pmc_enable_periph_clk(ID_TC1);
	// Desactivamos interupciones antes de cambiar nada
	NVIC_DisableIRQ(TC_IRQ);
	// Y borramos interrupciones pendientes
	NVIC_ClearPendingIRQ(TC_IRQ);
	// Establecemos la prioridad (Cuanto mayor el número menor prioridad)
	NVIC_SetPriority(TC_IRQ, 9);

	// Configuración del timer
	// Básicamente rellena el Channel Mode Register
	tc_init(TC_INT, TC_INT_CH, 0 |
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
	tc_enable_interrupt(TC_INT, TC_INT_CH, 0 |
		TC_IER_CPAS | // RA
		TC_IER_CPBS | // RB
		TC_IER_CPCS); // RC

	// Mete valores en los registros de comparación
	tc_write_rc(TC_INT,TC_INT_CH,42000000U);
	tc_write_rb(TC_INT,TC_INT_CH,21000000U);
	tc_write_ra(TC_INT,TC_INT_CH,10500000U);
	tc_start(TC_INT,TC_INT_CH);

	// Permitimos las interrupciones
	NVIC_EnableIRQ(TC_IRQ);
}

static void tc_pwm_config(void){
	pmc_enable_periph_clk(ID_TC0);
	tc_init(TC_PWM, TC_PWM_CH, 0 |
		TC_CMR_WAVE       |
		TC_CMR_ACPA_SET   |
		TC_CMR_ACPC_CLEAR |
		TC_CMR_CPCTRG);


	// Mete valores en los registros de comparación
	tc_write_rc(TC_PWM,TC_PWM_CH,42000U);
	tc_write_ra(TC_PWM,TC_PWM_CH,10500U);
	tc_start(TC_PWM,TC_PWM_CH);


}

// Rutina de atención a la interrupción
void TC1_Handler(void) {
	// Leemos el registro de estado
	// Para saber qué ha causado la interrupción
	uint32_t tc_status = tc_get_status(TC_INT, TC_INT_CH);
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

	printf("FF: %lu\r\n", sys_ticks);
}
/*
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
		pwm_channel_instance.alignment = PWM_ALIGN_LEFT;
		pwm_channel_instance.polarity = PWM_HIGH;
		pwm_channel_instance.ul_prescaler = PWM_CMR_CPRE_CLKA;
		pwm_channel_instance.ul_period = 200;
		pwm_channel_instance.ul_duty = 50;
		pwm_channel_instance.channel = PWM_CHANNEL_0;
	pwm_channel_init(PWM, &pwm_channel_instance);

	pwm_channel_enable(PWM, PWM_CHANNEL_0);
}
*/

/**
 * Application entry point
 */
int main(void) {
	// Rutina de configuración
	due_init();
	/*
	//pmc_enable_periph_clk(ID_PIOC);
	ioport_init();

	pio_configure_pin(PIO_PC3_IDX, 0 |
		PIO_TYPE_PIO_OUTPUT_1 |
		PIO_DEFAULT);
	pio_configure_pin(PIO_PC3_IDX, 0 |
		PIO_PERIPH_B |
		PIO_DEFAULT);
*/

	ioport_init();

	pio_configure_pin(PIO_PB25_IDX, 0 |
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
	tc_pwm_config();

	//pwm_config();

	// Infinite loop
	while (1) {

	}
}
