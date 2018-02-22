#include "due_sam3x.init.h"

void init_controller(void)
{
  /*
   * SAM System init: Initializes the PLL / clock.
   * Defined in CMSIS/ATMEL/sam3xa/source/system_sam3xa.c
   */
  SystemInit();
  /*
   * Config systick interrupt timing, core clock is in microseconds --> 1ms
   * Defined in CMSIS/CMSIS/include/core_cm3.h
   */
  if (SysTick_Config(SystemCoreClock / 1000)) while (1);

  /*
   * No watchdog now
   *
   */
  WDT_Disable(WDT);

  /*
   * GCC libc init, also done in Reset_Handler()
   */
   __libc_init_array();
}


/********************************************************************************
 * C++ catching pure virtual methods ( virtual myMethod = 0)
 ********************************************************************************/

void __cxa_pure_virtual(void)
{ while(1); }

