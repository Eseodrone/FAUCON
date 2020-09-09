
#include "mpu6050.h"
#include "datas_process.h" // A MODIF ULTERIEUREMENT
#include <stdbool.h>
#include "main.h"
#include "stm32f4xx_hal.h"
#include <math.h>

void MPU_init(void){
	//on init le mpu, structure et boolean de .h
	if(MPU6050_Init(&mpu_datas_res, MPU6050_VCC_GPIO, MPU6050_VCC_PIN, MPU6050_Device_0, MPU6050_Accelerometer_8G, MPU6050_Gyroscope_2000s) == MPU6050_Result_Ok)
		mpu_init_OK = TRUE; //A revoir du coup
}

void MPU_test(void)
{
	MPU6050_t MPU6050_Data;
	//int32_t gyro_x = 0;
	//int32_t gyro_y = 0;
	//int32_t gyro_z = 0;
	/* Initialize MPU6050 sensor */
		if (MPU6050_Init(&MPU6050_Data, MPU6050_VCC_GPIO, MPU6050_VCC_PIN, MPU6050_Device_0, MPU6050_Accelerometer_8G, MPU6050_Gyroscope_2000s) != MPU6050_Result_Ok)
		{
			/* Infinite loop */
			while (1);
		}
		while(1)
		{
			/* Read all data from sensor */
			MPU6050_ReadAll(&MPU6050_Data);
//			gyro_x = gyro_x + (MPU6050_Data.Gyroscope_X*0.01/131);
			//gyro_y += MPU6050_Data.Gyroscope_Y;
			//gyro_z += MPU6050_Data.Gyroscope_Z;
			debug_printf("AX%4d\tAY%4d\tAZ%4d\nGX%4d\tGY%4d\tGZ%4d\n",
										MPU6050_Data.Accelerometer_X,
										MPU6050_Data.Accelerometer_Y,
										MPU6050_Data.Accelerometer_Z,
										MPU6050_Data.Gyroscope_X,
										MPU6050_Data.Gyroscope_Y,
										MPU6050_Data.Gyroscope_Z);

			/* Little delay */
			HAL_Delay(500);
		}

}

void MPU_average_demo(void)
{
	MPU6050_t MPU6050_Data;
	float angle_x = 0.0;
	float angle_y = 0.0;
	float angle_z = 0.0;
	int8_t nb_values = 0;
	float average_x = 0.0;
	float average_y = 0.0;
	float average_z = 0.0;
	/* Initialize MPU6050 sensor */
	if (MPU6050_Init(&MPU6050_Data, MPU6050_VCC_GPIO, MPU6050_VCC_PIN, MPU6050_Device_0, MPU6050_Accelerometer_8G, MPU6050_Gyroscope_2000s) != MPU6050_Result_Ok)
	{
		/* Display error to user */
		debug_printf("MPU6050 Error\n");

		/* Infinite loop */
			while (1);
	}
	//Acquisition de 100 valeurs afin d'att�nuer le bruit du capteur (ça marche pas en vrai pb de cast)
//	nb_values = 100;
//	while(nb_values)
//	{
//		average_x += MPU6050_Data.Gyroscope_X;
//		average_y += MPU6050_Data.Gyroscope_Y;
//		average_z += MPU6050_Data.Gyroscope_Z;
//		nb_values --;
//	}
//	printf("X : %d\n",(int)average_x);
	average_x = 725; //Les moyennes sont à adapter au tatonnement selon le mpu que nous utiliserons, elles sont spécifiques
	average_y = -5;	 //à chaque capteur et rendent les mesures plus précises.
	average_z = 12;	 //
	printf("Averages : %d\t %d\t %d\t\n",(int)average_x,(int)average_y,(int)average_z);
	HAL_Delay(2000);
	while (1) {
		/* Read all data from sensor */
		MPU6050_ReadAll(&MPU6050_Data);
		angle_x += (MPU6050_Data.Gyroscope_X-average_x)*INT_TIME;
		angle_y += (MPU6050_Data.Gyroscope_Y+average_y)*INT_TIME; //Average n�gatif
		angle_z += (MPU6050_Data.Gyroscope_Z-average_z)*INT_TIME;
		printf("X : %d\t\t",((int)angle_x*360)/MPU_RANGE_X);
		printf("Y : %d\t\t",((int)angle_y*360)/MPU_RANGE_Y);
		printf("Z : %d\n",((int)angle_z*360)/MPU_RANGE_Z);
//		printf("GZ : %d\n",MPU6050_Data.Gyroscope_Z);

		/* Little delay */
		HAL_Delay(0.1); //Ce temps correspond à celui de l'intégration de la vitesse angulaire
	}
}
