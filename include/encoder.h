#ifndef ENCODER_H
#define ENCODER_H

#define ENC_A_PIN PIO_PB17
#define ENC_A_PORT PIOB
#define ENC_A_ID ID_PIOB
#define ENC_A_IRQ PIOB_IRQn

#define ENC_B_PIN PIO_PB18
#define ENC_B_PORT PIOB
#define ENC_B_ID ID_PIOB
#define ENC_B_IRQ PIOB_IRQn
#define ENC_PRIORITY 9

void encoder_init(void);
int encoder_getpos(void);
int encoder_clean(void);

#endif
