#ifndef PERIODIC_TC_H
#define PERIODIC_TC_H

#include "asf.h"

#define PTC_INT TC0
#define PTC_INT_CH 1
#define PTC_PCM ID_TC1
#define PTC_IRQ TC1_IRQn
#define PTC_PRIORITY 9
#define PTC_ISR void TC1_Handler(void)

void periodic_init(unsigned int frec);


#endif //PERIODIC_TC_H
