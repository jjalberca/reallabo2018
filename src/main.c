#include "asf.h"
#include "main.h"
#include "stdio_serial.h"
#include <delay.h>
#include "pwm_driver.h"
#include "periodic_tc.h"
#include "encoder.h"



// Mensaje de inicio
#define STRING_HEADER "-- Blink LED Example --\r\n" \
		"-- "BOARD_NAME" --\r\n" \
		"-- Compiled: "__DATE__" "__TIME__" --\r"



// Rutina de atención a la interrupción
PTC_ISR {
	// Leemos el registro de estado
	// Para saber qué ha causado la interrupción
	uint32_t tc_status = tc_get_status(PTC_INT, PTC_INT_CH);
	if (tc_status & TC_SR_CPCS){
		printf("CC: %lu\r\n", sys_ticks);
		pio_toggle_pin(PIO_PB27_IDX);
	}
}


/**
 * Application entry point
 */
int main(void) {
	// Rutina de configuración
	due_init();

	ioport_init();

	// Imprime el mensaje de inicio
	puts(STRING_HEADER);

	// Enciende la interfaz de I/O del puerto B
	pmc_enable_periph_clk(ID_PIOB);
	// Establece el pin 27 como salida
	pio_set_output(PIOB, PIO_PB27, LOW, DISABLE, ENABLE);
	// Rutina de configuración del timer
	periodic_init();
	// El pin de salida del pwm
	pio_configure_pin(PIO_PB25_IDX, 0 |
		PIO_PERIPH_B |
		PIO_DEFAULT);

	tc_pwm_init(5000);

	encoder_init();

	int dc = 0;
	int s = 10;
	int u = 1;

	// Infinite loop
	while (1) {
		if(dc >= 1000) u = -1;
		else if(dc <= 0) u = 1;
		dc+=s*u;
		tc_pwm_duty(dc);
		delay_ms(15);
	}
}
