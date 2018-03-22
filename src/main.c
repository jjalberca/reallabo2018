#include "asf.h"
#include "main.h"
#include "stdio_serial.h"
#include <delay.h>
#include "pwm_driver.h"
#include "periodic_tc.h"
#include "encoder.h"
#include "driver_motor.h"
#include "position_control.h"

/** Auto Test **/
#define SAMPLE_LEN 1200
#define N_REP 10
#define STEPS 1,2,3,4,5,6,7,8,9,10,11,12
#define V_REF 12
/***************/


static void toggle_pin(void);
static void save_pos(void);
static void test_motor(void);
static void manual_speed(void);
static void position_control(void);
static void set_position(void);
static void set_position_move(void);


typedef struct{
	unsigned int time;
	int pos;
} sample;

enum {
	PCONTROL_DIS,
	PCONTROL_POS,
	PCONTROL_SPEED
};


static sample samples[SAMPLE_LEN];
static volatile unsigned int p = 0;
static unsigned int start_time = 0;

static volatile int pcontrol_en = PCONTROL_DIS;
static volatile int pcontrol_speed = 0;

// Mensaje de inicio
#define STRING_HEADER "-- RealLabo2018 Motor test --\r\n" \
		"-- Alejandro Vicario --\r\n" \
		"-- Juan Jose Alberca --\r\n" \
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
		position_control();
	}
}

static void save_pos(void){
	if(p >= SAMPLE_LEN) return;

	samples[p].time = sys_ticks-start_time;;
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

static void position_control(void){
	if(pcontrol_en == PCONTROL_DIS) return;

	if(pcontrol_en == PCONTROL_SPEED){
		pcontrol_move(pcontrol_speed);
	}

	motor_set(pcontrol_fire(encoder_getpos()));
}


/**
 * Application entry point
 */
int main(void) {
		/** Rutinas de configuración **/
	due_init();
	ioport_init();

	// Enciende la interfaz de I/O del puerto B
	pmc_enable_periph_clk(ID_PIOB);
	// Establece el pin 27 como salida
	pio_set_output(PIOB, PIO_PB27, LOW, DISABLE, ENABLE);
	// Rutina de configuración del timer
	periodic_init(1000);
	// Rutina de configuración del encoder
	encoder_init();
	// Rutina de configuración del motor
	motor_init();

	/** Imprime el mensaje de inicio **/
	puts(STRING_HEADER);

	/** Infinite loop **/
	while (1) {


		puts(
			"Choose:\n\r" \
			"-- Auto test:    1\n\r" \
			"-- Manual PWM: 2\n\r" \
			"-- Position Control: 3\n\r" \
			"-- Speed Control: 4\r\n\n\n" \
			"     * Any key to return\r"
		);


		int val;
		int ret = scanf("%d", &val);
		if(ret){
			printf("In: %d\n\r", val);
			switch (val) {
				case 1:
					test_motor();
					break;
				case 2:
					manual_speed();
					break;
				case 3:
					set_position();
					break;
				case 4:
					set_position_move();
					break;
				default:
					puts("Incorrect option\r");
			}
		}
		fflush(stdin);
	}
}

static void test_motor(void){
	const int steps[] = {STEPS};
	motor_set(0);

	for(unsigned int i=0; i < sizeof(steps)/sizeof(steps[0]); i++){

		for(int j=0; j < N_REP; j++){
			int pwm_value = (1000*steps[i])/12;
			printf("%dV, Test %d, PWM %d\n\r",
				steps[i], j+1, pwm_value);
			start_time = sys_ticks;
			encoder_clear();
			p=0;
			motor_set(pwm_value);
			delay_ms(600);
			motor_set(0);

			while(p < SAMPLE_LEN);

			for(int k=0; k<SAMPLE_LEN; ++k){
				printf("%d\t%d\n\r", samples[k].time, samples[k].pos);
			}
		}
	}
}

// TODO print position when manual pwm
static void manual_speed(void){
	puts("PWM [-1000, 1000]\r");
	while(1){
		int val;
		int ret = scanf("%d", &val);
		if(!ret) break;

		printf("PWM: %d\n\r", val);
		motor_set(val);
	}
	motor_set(0);

}

static void set_position(void){
	puts("Position Control\n\r" \
		"Enter Position (Counts)\r"
	);
	encoder_clear();
	pcontrol_en = PCONTROL_POS;
	while(1){
		int val;
		int ret = scanf("%d", &val);
		if(!ret) break;

		printf("Pos: %d\n\r", val);
		pcontrol_set(val);
	}
	pcontrol_en = PCONTROL_DIS;
	motor_set(0);
}

static void set_position_move(void){
	puts("Speed Control\n\r" \
		"Enter Speed (Counts/ms)\r"
	);
	encoder_clear();
	pcontrol_speed = 0;
	pcontrol_en = PCONTROL_SPEED;
	while(1){
		int val;
		int ret = scanf("%d", &val);
		if(!ret) break;

		printf("Speed: %d\n\r", val);
		pcontrol_speed = val;
	}
	pcontrol_en = DISABLE;
	motor_set(0);
}
