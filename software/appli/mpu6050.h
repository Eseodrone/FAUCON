#ifndef MPU6050_H
#define MPU6050_H

#include "MPU6050/stm32f4_mpu6050.h"
#include "drone_def.h"

//MACROS
#define INT_TIME			0.001
#define MPU_RANGE_X			1100
#define MPU_RANGE_Y			1100
#define MPU_RANGE_Z			1000


//
extern bool_e mpu_init_OK;

//PROTOTYPES
bool_e MPU_init(drone_data_t* drone);
void MPU_test(void);
void MPU_average_demo(void);
void MPU_angle_computer(void);




#endif /*MPU6050_H*/



