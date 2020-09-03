
#include "mpu6050.h"
#include "datas_process.h" // A MODIF ULTERIEUREMENT
#include <stdbool.h>
#include "main.h"
#include "stm32f4xx_hal.h"
#include <math.h>

//static float dt;
//unsigned long t_now, t_last;

void MPU_init(void){
	//on init le mpu, structure et boolean de .h
	if(MPU6050_Init(&mpu_datas_res, MPU6050_VCC_GPIO, MPU6050_VCC_PIN, MPU6050_Device_0, MPU6050_Accelerometer_8G, MPU6050_Gyroscope_2000s) == MPU6050_Result_Ok)
		mpu_init_OK = TRUE;
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
	static int16_t current_value = 0;
	static bool PRINTF = FALSE;
	int angle_x = 0;
	int angle_y = 0;
	int angle_z = 0;
	int8_t nb_values = 0;
	int32_t average_x = 0;
	int32_t average_y = 0;
	int32_t average_z = 0;
	/* Initialize MPU6050 sensor */
	if (MPU6050_Init(&MPU6050_Data, MPU6050_VCC_GPIO, MPU6050_VCC_PIN, MPU6050_Device_0, MPU6050_Accelerometer_8G, MPU6050_Gyroscope_2000s) != MPU6050_Result_Ok)
	{
		/* Display error to user */
		debug_printf("MPU6050 Error\n");

		/* Infinite loop */
			while (1);
	}
	//Acquisition de 1000 valeurs afin d'atténuer le bruit du capteur
	nb_values = 1000;
	while(nb_values)
	{
		average_x += MPU6050_Data.Gyroscope_X;
		average_y += MPU6050_Data.Gyroscope_Y;
		average_z += MPU6050_Data.Gyroscope_Z;
		nb_values --;
	}
	average_x /= 1000;
	average_y /= 1000;
	average_z /= 1000;
	while (1) {
		/* Read all data from sensor */
		MPU6050_ReadAll(&MPU6050_Data);
//		if((MPU6050_Data.Gyroscope_X*0.001>0.1) || (MPU6050_Data.Gyroscope_X*0.001<-0.1))
//		{
		if(((angle_x += (MPU6050_Data.Gyroscope_X-average_x)*0.001)>360) || ((angle_x += (MPU6050_Data.Gyroscope_X-average_x)*0.001)<-360))
		{
			angle_x = 0;
			current_value = angle_x;
		}
		if(((MPU6050_Data.Gyroscope_X-average_x)*0.001)!= current_value)
		{
			angle_x += (MPU6050_Data.Gyroscope_X-average_x)*0.001;
			PRINTF = TRUE;
		}

//		}
		if(PRINTF)
		{
			printf("X : %d\n",angle_x);
			PRINTF = FALSE;
		}
		/* Little delay */
		HAL_Delay(0.1);
	}
}

void MPU_int_demo(void)
{
	MPU6050_t MPU6050_Data;
	//t_now = 0; t_last = 0; dt = 0;
	int16_t angle_x = 0;
	int16_t angle_y = 0;
	int16_t angle_z = 0;
	/* Initialize MPU6050 sensor */
	if (MPU6050_Init(&MPU6050_Data, MPU6050_VCC_GPIO, MPU6050_VCC_PIN, MPU6050_Device_0, MPU6050_Accelerometer_8G, MPU6050_Gyroscope_2000s) != MPU6050_Result_Ok)
	{
		/* Display error to user */
		debug_printf("MPU6050 Error\n");

		/* Infinite loop */
			while (1);
	}
	while (1) {
		/* Read all data from sensor */
		//t_last = HAL_GetTick();
		MPU6050_ReadAll(&MPU6050_Data);
		//t_now = HAL_GetTick();
		//dt = ((t_now - t_last)/1000); //ms-->s
		//X
		if((MPU6050_Data.Gyroscope_X*INT_TIME_X>NOISE_POSITIVE_REDUCTION_X) || (MPU6050_Data.Gyroscope_X*INT_TIME_X<-NOISE_NEGATIVE_REDUCTION_X))
		{
//			if(((angle_x += (MPU6050_Data.Gyroscope_X)*INT_TIME)>MPU_RANGE) || ((angle_x += (MPU6050_Data.Gyroscope_X)*INT_TIME)<-MPU_RANGE))
//			{
//				angle_x = 0;
//			}
			if(MPU6050_Data.Gyroscope_X*INT_TIME_X<0) //Amplification de la valeur négative "moins sensible"
			{
				angle_x += ((MPU6050_Data.Gyroscope_X)*(INT_TIME_X)-1.5);
			}
			else
			{
				angle_x += (MPU6050_Data.Gyroscope_X)*(INT_TIME_X); //Valeur positive
			}

			//angle_x = (angle_x*360)/2000;
		}
		//Y
		if((MPU6050_Data.Gyroscope_Y*INT_TIME>NOISE_POSITIVE_REDUCTION_Y) || (MPU6050_Data.Gyroscope_Y*INT_TIME<-NOISE_NEGATIVE_REDUCTION_Y))
		{
//			if(((angle_y += (MPU6050_Data.Gyroscope_Y)*INT_TIME)>360) || ((angle_y += (MPU6050_Data.Gyroscope_Y)*INT_TIME)<-360))
//			{
//				angle_y = 0;
//			}
			angle_y += ((MPU6050_Data.Gyroscope_Y)*INT_TIME);
		}
//		//Z
		if((MPU6050_Data.Gyroscope_Z*INT_TIME>NOISE_POSITIVE_REDUCTION_Z) || (MPU6050_Data.Gyroscope_Z*INT_TIME<-NOISE_NEGATIVE_REDUCTION_Z))
		{
//			if(((angle_z += (MPU6050_Data.Gyroscope_Z)*INT_TIME)>360) || ((angle_z += (MPU6050_Data.Gyroscope_Z)*INT_TIME)<-360))
//			{
//				angle_z = 0;
//			}
			if(MPU6050_Data.Gyroscope_Z*INT_TIME<0)
			{
				angle_z += ((MPU6050_Data.Gyroscope_Z)*INT_TIME);
			}
			else
			{
				angle_z += ((MPU6050_Data.Gyroscope_Z*0.5)*INT_TIME); //Atténuation côté positif
			}
		}
//		printf("X : %d\t Y : %d\t Z : %d\n",angle_x,angle_y,angle_z);
		printf("X : %d\n",(angle_x*360)/MPU_RANGE_X);
		//printf("X : %d \n",angle_x);
		//printf("Y : %d \n",(angle_y*360)/MPU_RANGE_Y);
		//printf("Y : %d \n",angle_y);
		//printf("Z : %d \n",(angle_z*360)/MPU_RANGE_Z);
		//printf("Z : %d \n",angle_z);

		/* Little delay */
		HAL_Delay(0.1);
	}
}

/****************************************************************************************************************************************************************/
/*
float   temperature=0;
MPU6050_t MPU6050_Sensor;
char     data[120];


// rotation angle of the sensor
unsigned long last_read_time;
float         last_x_angle;  // These are the filtered angles
float         last_y_angle;
float         last_z_angle;
float         last_gyro_x_angle;  // Store the gyro angles to compare drift
float         last_gyro_y_angle;
float         last_gyro_z_angle;

void set_last_read_angle_data(unsigned long time, float x, float y, float z, float x_gyro, float y_gyro, float z_gyro) {
  last_read_time = time;
  last_x_angle = x;
  last_y_angle = y;
  last_z_angle = z;
  last_gyro_x_angle = x_gyro;
  last_gyro_y_angle = y_gyro;
  last_gyro_z_angle = z_gyro;
}



float    base_x_gyro = 0;
float    base_y_gyro = 0;
float    base_z_gyro = 0;
float    base_x_accel = 0;
float    base_y_accel = 0;
float    base_z_accel = 0;

float gyro_angle_z=0;

// This global variable tells us how to scale gyroscope data
float    GYRO_FACTOR=0;

//unsigned long t_now=0,t_last=0;
//float dt=0; */

///* Private variables ---------------------------------------------------------*/
//I2C_HandleTypeDef hi2c1;
//
//UART_HandleTypeDef huart2;
//
///* Private function prototypes -----------------------------------------------*/
//void SystemClock_Config(void);
//static void MX_GPIO_Init(void);
//static void MX_USART2_UART_Init(void);
//static void MX_I2C1_Init(void);
//
///* Private function prototypes -----------------------------------------------*/
//
////void get_mpudata_average() {
////
////  int read_count = 10;
////  // Discard the first reading (don't know if this is needed or
////  // not, however, it won't hurt.)
////  // Read and average the raw values
////  for (int i = 0; i < read_count; i++) {
////    TM_MPU6050_ReadAll(&MPU6050_Data0);
////    base_x_gyro += MPU6050_Data0.Gyroscope_X;
////    base_y_gyro += MPU6050_Data0.Gyroscope_Y;
////    base_z_gyro += MPU6050_Data0.Gyroscope_Z;
////    base_x_accel +=  MPU6050_Data0.Accelerometer_X;
////    base_y_accel +=  MPU6050_Data0.Accelerometer_Y;
////    base_y_accel +=  MPU6050_Data0.Accelerometer_Z;
////  }
////
////    base_x_gyro /= read_count;
////    base_y_gyro /= read_count;
////    base_z_gyro /= read_count;
////    base_x_accel /= read_count;
////    base_y_accel /= read_count;
////    base_z_accel /= read_count;
////}
//
//
///**
//  * @brief  The application entry point.
//  *
//  * @retval None
//  */
//void MPU_compute_angles(void)
//{
//	inline unsigned long get_last_time() {return last_read_time;}
//	inline float get_last_x_angle() {return last_x_angle;}
//	inline float get_last_y_angle() {return last_y_angle;}
//	inline float get_last_z_angle() {return last_z_angle;}
//	inline float get_last_gyro_x_angle() {return last_gyro_x_angle;}
//	inline float get_last_gyro_y_angle() {return last_gyro_y_angle;}
//	inline float get_last_gyro_z_angle() {return last_gyro_z_angle;}
//
// /* Initialize MPU6050*/
//  MPU_init();
//  GYRO_FACTOR = 131.0;
//  const float RADIANS_TO_DEGREES = 57.2958; //180/3.14159
//  set_last_read_angle_data(0, 0, 0, 0, 0, 0, 0);
//
//  HAL_Delay(5000);
//
//  while (1)
//  {
//	  HAL_Delay(200);
//
//	  t_last = HAL_GetTick();
//		/* Read all data from sensor 1 */
//	  MPU6050_ReadAll(&MPU6050_Sensor);
//	  t_now = HAL_GetTick();
//
//		// Remove offsets and scale gyro data
////        float gyro_x = (MPU6050_Data0.Gyroscope_X - base_x_gyro)/GYRO_FACTOR;
////        float gyro_y = (MPU6050_Data0.Gyroscope_Y - base_y_gyro)/GYRO_FACTOR;
////        float gyro_z = (MPU6050_Data0.Gyroscope_Z - base_z_gyro)/GYRO_FACTOR;
//		float gyro_x = (MPU6050_Sensor.Gyroscope_X)/GYRO_FACTOR;
//        float gyro_y = (MPU6050_Sensor.Gyroscope_Y)/GYRO_FACTOR;
//        float gyro_z = (MPU6050_Sensor.Gyroscope_Z)/GYRO_FACTOR;
//
////        float accel_x = MPU6050_Sensor.Accelerometer_X ; // - base_x_accel;
////        float accel_y = MPU6050_Sensor.Accelerometer_Y ; // - base_y_accel;
////        float accel_z = MPU6050_Sensor.Accelerometer_Z ; // - base_z_accel;
////
////        float accel_angle_y = atan(-1*accel_x/sqrt(pow(accel_y,2) + pow(accel_z,2)))*RADIANS_TO_DEGREES;
////        float accel_angle_x = atan(accel_y/sqrt(pow(accel_x,2) + pow(accel_z,2)))*RADIANS_TO_DEGREES;
////        float accel_angle_z = 0;  //Accelerometer doesn't give z-angle
//
//        // Compute the (filtered) gyro angles
//        dt =(t_now - get_last_time())/1000.0;
//
//        float gyro_angle_x = gyro_x*dt + get_last_x_angle();
//        float gyro_angle_y = gyro_y*dt + get_last_y_angle();
//
//				int gz_threshold = 2; // gyro z raw data fluctuation threshold value when gyro doesn't move. It is up to your mpu6050. It is just a personal approach.
//				if(gyro_z < gz_threshold && gyro_z > -gz_threshold) // When gyro stands ignore the gyro z small fluctuations to prevent z angle irregular increments
//					 gyro_z = 0;
//
//           gyro_angle_z = gyro_z*dt + get_last_z_angle();
//
//        // Compute the drifting gyro angles
//        float unfiltered_gyro_angle_x = gyro_x*dt + get_last_gyro_x_angle();
//        float unfiltered_gyro_angle_y = gyro_y*dt + get_last_gyro_y_angle();
//        float unfiltered_gyro_angle_z = gyro_z*dt + get_last_gyro_z_angle();
//
//        // Apply the complementary filter to figure out the change in angle - choice of alpha is
//        /* estimated now.  Alpha depends on the sampling rate... */
//        const float alpha = 0.96;
//        float angle_x = alpha*gyro_angle_x; //+ (1.0 - alpha)*accel_angle_x;
//        float angle_y = alpha*gyro_angle_y; //+ (1.0 - alpha)*accel_angle_y;
//        float angle_z = gyro_angle_z;  //Accelerometer doesn't give z-angle
//
//		    /* Update the saved data with the latest values */
//        set_last_read_angle_data(t_now, angle_x, angle_y, angle_z, unfiltered_gyro_angle_x, unfiltered_gyro_angle_y, unfiltered_gyro_angle_z);
//
//
//			 	/* Send the angle values to serial buffer	*/
//        	debug_printf("Angle values\n- X:%3.4f\n- Y:%3.4f\n- Z:%3.4f\nTemperature\n- %3.4f\n\n\n",
//                    	angle_x ,
//						angle_y ,
//						angle_z,
//						MPU6050_Sensor.Temperature);
//  }
//}
//
///**
//  * @brief System Clock Configuration
//  * @retval None
//  */
//void SystemClock_Config(void)
//{
//
//  RCC_OscInitTypeDef RCC_OscInitStruct;
//  RCC_ClkInitTypeDef RCC_ClkInitStruct;
//
//    /**Configure the main internal regulator output voltage
//    */
//  __HAL_RCC_PWR_CLK_ENABLE();
//
//  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
//
//    /**Initializes the CPU, AHB and APB busses clocks
//    */
//  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
//  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
//  RCC_OscInitStruct.HSICalibrationValue = 16;
//  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
//  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
//  RCC_OscInitStruct.PLL.PLLM = 8;
//  RCC_OscInitStruct.PLL.PLLN = 168;
//  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
//  RCC_OscInitStruct.PLL.PLLQ = 4;
//  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
//  {
//    _Error_Handler(__FILE__, __LINE__);
//  }
//
//    /**Initializes the CPU, AHB and APB busses clocks
//    */
//  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
//                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
//  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
//  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
//  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
//  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
//
//  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
//  {
//    _Error_Handler(__FILE__, __LINE__);
//  }
//
//    /**Configure the Systick interrupt time
//    */
//  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
//
//    /**Configure the Systick
//    */
//  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
//
//  /* SysTick_IRQn interrupt configuration */
//  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
//}
//
///* I2C1 init function */
//static void MX_I2C1_Init(void)
//{
//
//  hi2c1.Instance = I2C1;
//  hi2c1.Init.ClockSpeed = 400000;
//  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_16_9;
//  hi2c1.Init.OwnAddress1 = 0;
//  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
//  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
//  hi2c1.Init.OwnAddress2 = 0;
//  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
//  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
//  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
//  {
//    _Error_Handler(__FILE__, __LINE__);
//  }
//
//}
//
///* USART2 init function */
//static void MX_USART2_UART_Init(void)
//{
//
//  huart2.Instance = USART2;
//  huart2.Init.BaudRate = 115200;
//  huart2.Init.WordLength = UART_WORDLENGTH_8B;
//  huart2.Init.StopBits = UART_STOPBITS_1;
//  huart2.Init.Parity = UART_PARITY_NONE;
//  huart2.Init.Mode = UART_MODE_TX;
//  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
//  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
//  if (HAL_UART_Init(&huart2) != HAL_OK)
//  {
//    _Error_Handler(__FILE__, __LINE__);
//  }
//
//}
//
///** Pinout Configuration
//*/
//static void MX_GPIO_Init(void)
//{
//
//  /* GPIO Ports Clock Enable */
//  __HAL_RCC_GPIOA_CLK_ENABLE();
//  __HAL_RCC_GPIOB_CLK_ENABLE();
//
//}
//
//
///**
//  * @brief  This function is executed in case of error occurrence.
//  * @param  file: The file name as string.
//  * @param  line: The line in file as a number.
//  * @retval None
//  */
//void _Error_Handler(char *file, int line)
//{
//  /* USER CODE BEGIN Error_Handler_Debug */
//  /* User can add his own implementation to report the HAL error return state */
//  while(1)
//  {
//  }
//  /* USER CODE END Error_Handler_Debug */
//}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
