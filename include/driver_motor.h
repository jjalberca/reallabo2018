#ifndef DRIVER_MOTOR_H
#define DRIVER_MOTOR_H

#define MOT_EN_MASK PIO_PB25
#define MOT_EN_PORT PIOB
#define MOT_EN_IDX PIO_PB25_IDX
#define MOT_EN_PCM ID_PIOB
#define MOT_I1_MASK PIO_PC26
#define MOT_I1_PORT PIOC
#define MOT_I1_PCM ID_PIOC
#define MOT_I2_MASK PIO_PC25
#define MOT_I2_PORT PIOC
#define MOT_I2_PCM ID_PIOC
#define MOT_PWM 5000

void motor_init(void);
void motor_dir(int dir);
void motor_set(int speed);


#endif // DRIVER_MOTOR_H
