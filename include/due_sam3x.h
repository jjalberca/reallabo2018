#ifndef DUE_SAM3x_H
#define	DUE_SAM3x_H

#ifdef	__cplusplus
extern "C" {
#endif

#define __SAM3X8E__

#ifdef __SAM3X8E__
#define MAIN_OSC_FREQ (12000000)
#define MASTER_CLK_FREQ (84000000)
#endif

#include "chip.h"
#include "sam.h"


extern void init_controller(void);

#ifdef	__cplusplus
}
#endif

#endif //DUE_SAM3x_H




