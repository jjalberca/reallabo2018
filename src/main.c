#include "due_sam3x.h"


int main(void)
{
  /* The general init (clock, libc, watchdog ...) */
  init_controller();

  /* Board pin 13 == PB27 */
  PIO_Configure(PIOB, PIO_OUTPUT_1, PIO_PB27, PIO_DEFAULT);

  /* Main loop */
  while(1) {
    Sleep(500);
    if(PIOB->PIO_ODSR & PIO_PB27) {
    /* Set clear register */
      PIOB->PIO_CODR = PIO_PB27;
    } else {
      /* Set set register */
      PIOB->PIO_SODR = PIO_PB27;
    }
  }
  return 0;
}
