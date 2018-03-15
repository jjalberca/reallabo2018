#include "asf.h"
#include "main.h"
#include "stdio_serial.h"
#include <delay.h>
#include "pwm_driver.h"
#include "periodic_tc.h"
#include "encoder.h"
#include "driver_motor.h"

#define SAMPLE_LEN 1200

static void toggle_pin(void);
static void save_pos(void);

typedef struct{
	unsigned int time;
	int pos;
} sample;


static sample samples[SAMPLE_LEN];
static unsigned int p = 0;

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
		save_pos();
		toggle_pin();
	}
}
static void save_pos(void){


	if(p >= SAMPLE_LEN) return;

	samples[p].time = sys_ticks;
	samples[p].pos = encoder_getpos();

	++p;
}

static void toggle_pin(void){
	static unsigned int next = 0;
	unsigned int current = sys_ticks;
	if(current < next) return;

	pio_toggle_pin(PIO_PB27_IDX);
	next = current + 1000;
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
	periodic_init(1000);

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

		if(p == SAMPLE_LEN){
			for(int i=0; i<SAMPLE_LEN; i++){
				printf("%d \t %d\n\r", samples[i].time, samples[i].pos);
			}
			p = 0;
		}

		/*
		int val;
		int ret = scanf("%d", &val);
		if(ret){
			printf("In: %d\n\r", val);
			motor_set(val);
		}
		fflush(stdin);
		*/
	}
}
