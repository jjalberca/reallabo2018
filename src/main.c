#include "due_sam3x.h"

#define LED_CH (0)
#define TC0_PERIOD (0xffff)

int main(void)
{
  /* The general init (clock, libc, watchdog disable) */
  init_controller();

  /* Use Timer/Counter 0 (enable its clock) */
  pmc_enable_periph_clk(ID_TC0);

  TC_Configure(TC0, LED_CH, 0   /* Channel 0 on TC0                           */
  /* Configure as PWM using upward counter */
    |TC_CMR_TCCLKS_TIMER_CLOCK2 /* Use TCLK2 as source === MCLK/8             */
    |TC_CMR_WAVE                /* Waveform mode ("PWM")                      */
    |TC_CMR_WAVSEL_UP_RC        /* Count upwards to register C (==RC)         */
    |TC_CMR_BCPB_CLEAR          /* Switch TIOB off when reaching RB Compare   */
    |TC_CMR_BCPC_SET            /* Switch TIOB on  when reaching RC Compare   */
    |TC_CMR_EEVT_XC0
  );

  PIO_Configure(PIOB, PIO_PERIPH_B, PIO_PB27B_TIOB0, PIO_DEFAULT);
  TC_SetRC(TC0, LED_CH, TC0_PERIOD);/* Period: CLK/65536 == 10Hz              */
  TC_SetRB(TC0, 0, 0);              /* Initial duty cycle: 0                  */
  TC_Start(TC0, LED_CH);            /* Start timer (for channel 0)            */


  /* Main loop */
  int16_t d = 1;
  int16_t duty_cycle = 25;

  while(1) {
    Sleep(25);
    duty_cycle += d;
    if(duty_cycle >= 48 || duty_cycle <= 2) d = -d;
    TC_SetRB(TC0, 0, duty_cycle * TC0_PERIOD / 50);
  }
  return 0;
}
