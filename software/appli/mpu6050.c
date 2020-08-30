//Ici prochainement, le code de votre MPU.
#include "mpu6050.h"
#include "datas_process.h" // A MODIF ULTERIEUREMENT

void MPU_init(void){
	//on init le mpu, structure et boolean de .h
	if(MPU6050_Init(&mpu_datas_res, MPU6050_VCC_GPIO, MPU6050_VCC_PIN, MPU6050_Device_0, MPU6050_Accelerometer_8G, MPU6050_Gyroscope_2000s) == MPU6050_Result_Ok)
		mpu_init_OK = TRUE;
}
