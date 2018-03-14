#include "pwm_driver.h"
#include "asf.h"


#define TC_PWM TC0
#define TC_PWM_CH 0
#define TC_PWM_PCM ID_TC0
#define FREC_MCK 84000000U

#define PWM_PIN PIO_PB25_IDX

void tc_pwm_init(unsigned int frec){

	pmc_enable_periph_clk(TC_PWM_PCM);
	tc_init(TC_PWM, TC_PWM_CH, 0 |
	TC_CMR_WAVE       |
	TC_CMR_ACPA_SET   |
	TC_CMR_ACPC_CLEAR |
	TC_CMR_CPCTRG);


  unsigned int rc = FREC_MCK/2/frec;

	tc_write_rc(TC_PWM,TC_PWM_CH,rc);
	tc_write_ra(TC_PWM,TC_PWM_CH,rc+1);
	tc_start(TC_PWM,TC_PWM_CH);
}

void tc_pwm_duty(int dc){
	if(dc < 0) dc = 0;
	if(dc > 1000) dc = 1000;
	unsigned int rc = tc_read_rc(TC_PWM,TC_PWM_CH);
	unsigned int ra = rc - (rc*dc)/1000;
	tc_write_ra(TC_PWM,TC_PWM_CH,ra+1);
}
