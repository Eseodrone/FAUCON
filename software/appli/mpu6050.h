#ifndef MPU6050_H
#define MPU6050_H

#include "MPU6050/stm32f4_mpu6050.h"

void MPU_init(void);
void MPU_test(void);
void MPU_int_demo(void);
void MPU_average_demo(void);
//void MPU_compute_angles(void);

#define NOISE_POSITIVE_REDUCTION_X	1
#define NOISE_NEGATIVE_REDUCTION_X	0.01
#define NOISE_POSITIVE_REDUCTION_Y  0.15
#define NOISE_NEGATIVE_REDUCTION_Y	0.15
#define NOISE_POSITIVE_REDUCTION_Z  0.15
#define NOISE_NEGATIVE_REDUCTION_Z	0.15
#define INT_TIME_X			0.001
#define INT_TIME			0.0012 //On ajoute le temps de traitement ?
#define MPU_RANGE_X			2750
#define MPU_RANGE_Y			2000
#define MPU_RANGE_Z			1600

#endif /*MPU6050_H*/
