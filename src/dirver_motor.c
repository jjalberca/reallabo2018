#include "driver_motor.h"
#include "pwm_driver.h"
#include "asf.h"

void motor_init(void){
	pmc_enable_periph_clk(MOT_EN_PCM);
	pio_set_output(MOT_EN_PORT, MOT_EN_MASK, LOW, DISABLE, ENABLE);
	pmc_enable_periph_clk(MOT_I1_PCM);
	pio_configure_pin(MOT_I1_IDX, 0 |
		PIO_PERIPH_B |
		PIO_DEFAULT);
	tc_pwm_init(MOT_PWM);
	tc_pwm_duty(0);
	pmc_enable_periph_clk(MOT_I2_PCM);
	pio_set_output(MOT_I2_PORT, MOT_I2_MASK, LOW, DISABLE, ENABLE);


	pio_set(MOT_EN_PORT, MOT_EN_MASK);
}

void motor_dir(int dir){
	if(dir == 0){
		pio_clear(MOT_I2_PORT, MOT_I2_MASK);
	} else if(dir > 0){
		pio_clear(MOT_I2_PORT, MOT_I2_MASK);
	} else if(dir < 0){
		pio_set(MOT_I2_PORT, MOT_I2_MASK);
	}
}

void motor_set(int speed){

	if(speed >= 0){
		tc_pwm_duty(speed);
	} else {
		tc_pwm_duty(1000+speed);
	}

	motor_dir(speed);
}
