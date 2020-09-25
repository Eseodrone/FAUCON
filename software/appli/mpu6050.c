
#include "mpu6050.h"
#include "datas_process.h" // A MODIF ULTERIEUREMENT
#include <stdbool.h>
#include "main.h"
#include "stm32f4xx_hal.h"


//* boolean de l'init du mpu
bool_e mpu_init_OK;

static drone_data_t* drone_data;
static Gyro_datas_t angular_speed;


static float AVERAGE_X;
static float AVERAGE_Y;
static float AVERAGE_Z;

bool_e MPU_init(drone_data_t* drone){
	drone_data = drone;  //Pourquoi ça ?
	//on init le mpu, structure et boolean de .h
	if(MPU6050_Init(&mpu_datas_res, MPU6050_VCC_GPIO, MPU6050_VCC_PIN, MPU6050_Device_0, MPU6050_Accelerometer_8G, MPU6050_Gyroscope_500s) == MPU6050_Result_Ok)
		mpu_init_OK = TRUE; //A revoir du coup
	AVERAGE_X = 725;
	AVERAGE_Y = -5;
	AVERAGE_Z = 12;
	return mpu_init_OK;
}
/*
void MPU_angle_computer(void)
{
	//Le MPU doit être préalablement initialisé
	MPU6050_ReadGyroscope(p_mpu_datas_res);
	drone_data->datas_sensors_pooling.roll_angle += ((p_mpu_datas_res->Gyroscope_X-AVERAGE_X)*INT_TIME);
	drone_data->datas_sensors_pooling.pitch_angle += ((p_mpu_datas_res->Gyroscope_Y+AVERAGE_Y)*INT_TIME); //Average negatif
	drone_data->datas_sensors_pooling.yaw_angle += ((p_mpu_datas_res->Gyroscope_Z-AVERAGE_Z)*INT_TIME);

	printf("Roll angle : %d\n",(int)drone_data->datas_sensors_pooling.roll_angle/MPU_RANGE_X*360);
	//printf("Roll angle : %d",(int)drone_data->datas_sensors_pooling.pitch_angle/MPU_RANGE_Y*360);
	//printf("Roll angle : %d",(int)drone_data->datas_sensors_pooling.yaw_angle/MPU_RANGE_Z*360);
}*/


void MPU_angle_computer(void)
{
	//Le MPU doit être préalablement initialisé
	MPU6050_ReadGyroscope(p_mpu_datas_res);
	//Conversion en vitesse angulaire
	angular_speed.Gyro_X = (((float)p_mpu_datas_res->Gyroscope_X-AVERAGE_X)*MPU6050_GYRO_SENS_500*RAD_TO_DEG);
	angular_speed.Gyro_Y = (((float)p_mpu_datas_res->Gyroscope_Y+AVERAGE_Y)*MPU6050_GYRO_SENS_500*RAD_TO_DEG); //Average negatif
	angular_speed.Gyro_Z = (((float)p_mpu_datas_res->Gyroscope_Z-AVERAGE_Z)*MPU6050_GYRO_SENS_500*RAD_TO_DEG);
	//Intégration de la vitesse angulaire par rapport au TIM2 (Fréquence d'appel de l'IT) --> /f = *T car f=1/T
	drone_data->datas_sensors_pooling.roll_angle += (float)(angular_speed.Gyro_X/FREQUENCY);
	drone_data->datas_sensors_pooling.pitch_angle += (float)(angular_speed.Gyro_Y/FREQUENCY);
	drone_data->datas_sensors_pooling.yaw_angle += (float)(angular_speed.Gyro_Z/FREQUENCY);
}
