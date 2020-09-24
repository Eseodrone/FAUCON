
#include "mpu6050.h"
#include "datas_process.h" // A MODIF ULTERIEUREMENT
#include <stdbool.h>
#include "main.h"
#include "stm32f4xx_hal.h"


//* boolean de l'init du mpu
bool_e mpu_init_OK;

static drone_data_t* drone_data;


static uint8_t AVERAGE_X;
static uint8_t AVERAGE_Y;
static uint8_t AVERAGE_Z;

bool_e MPU_init(drone_data_t* drone){
	drone_data = drone;
	//on init le mpu, structure et boolean de .h
	if(MPU6050_Init(&mpu_datas_res, MPU6050_VCC_GPIO, MPU6050_VCC_PIN, MPU6050_Device_0, MPU6050_Accelerometer_8G, MPU6050_Gyroscope_2000s) == MPU6050_Result_Ok)
		mpu_init_OK = TRUE; //A revoir du coup
	AVERAGE_X = 725;
	AVERAGE_Y = -5;
	AVERAGE_Z = 12;

	return mpu_init_OK;
}

void MPU_angle_computer(void)
{
	//Le MPU doit être préalablement initialisé
	MPU6050_ReadAll(p_mpu_datas_res);
	//TODO voir avec flo
	/*
	drone_data->datas_sensors_pooling.Gyroscope_X += (p_mpu_datas_res->Gyroscope_X-AVERAGE_X)*INT_TIME;
	drone_data->datas_sensors_pooling.Gyroscope_Y += (p_mpu_datas_res->Gyroscope_Y+AVERAGE_Y)*INT_TIME; //average negatif
	drone_data->datas_sensors_pooling.Gyroscope_Z += (p_mpu_datas_res->Gyroscope_Z-AVERAGE_Z)*INT_TIME;

	drone_data->datas_sensors_pooling.Gyroscope_X = (p_datas_sensors_pooling->Gyroscope_X)/MPU_RANGE_X*360;
	drone_data->datas_sensors_pooling.Gyroscope_Y = (p_datas_sensors_pooling->Gyroscope_Y)/MPU_RANGE_Y*360;
	drone_data->datas_sensors_pooling.Gyroscope_Z = (p_datas_sensors_pooling->Gyroscope_Z)/MPU_RANGE_Z*360;
	*/
}



