#ifndef MPU6050_H
#define MPU6050_H

#include "MPU6050/stm32f4_mpu6050.h"
#include "drone_def.h"

//MACROS
#define RAD_TO_DEG		((float)57.3)
#define FREQUENCY 		((float)250)


extern bool_e mpu_init_OK;

//PROTOTYPES
bool_e MPU_init(drone_data_t* drone);
void MPU_test(void);
void MPU_average_demo(void);
void MPU_angle_computer(void);

typedef struct{
	float Gyro_X;
	float Gyro_Y;
	float Gyro_Z;
}Gyro_datas_t; //Stocke les vitesses angulaires



#endif /*MPU6050_H*/



