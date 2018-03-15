#include "asf.h"
#include "main.h"
#include "stdio_serial.h"
#include <delay.h>
#include "pwm_driver.h"
#include "periodic_tc.h"
#include "encoder.h"
#include "driver_motor.h"



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
		printf("Pasos: %d\r\n", encoder_getpos());
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

	encoder_init();

	motor_init();
/*
	int dc = 0;
	int s = 5;
	int u = 1;
	*/

	// Infinite loop
	while (1) {
		/*
		if(dc >= 1000) u = -1;
		else if(dc <= -1000) u = 1;
		dc+=s*u;
		motor_set(dc);
		delay_ms(15);
		*/

		encoder_fire();
	}
}
