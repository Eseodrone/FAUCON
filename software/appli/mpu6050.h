#ifndef MPU6050_H
#define MPU6050_H

#include "MPU6050/stm32f4_mpu6050.h"

//MACROS
#define INT_TIME			0.001
#define MPU_RANGE_X			1100
#define MPU_RANGE_Y			1100
#define MPU_RANGE_Z			1000

//PROTOTYPES
void MPU_init(void);
void MPU_test(void);
void MPU_average_demo(void);



#endif /*MPU6050_H*/
