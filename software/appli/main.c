 /**
  *
  * \file main.c
  */

/* Includes ------------------------------------------------------------------*/

#include <vl53l1x/vl53l1x.h>
#include "stm32f4xx_hal.h"
#include "macro_types.h"
#include "main.h"
#include "stm32f4_uart.h"
#include "stm32f4_sys.h"
#include "stm32f4_gpio.h"
#include "stm32f4_timer.h"
#include "MPU6050/stm32f4_mpu6050.h"
#include "systick.h"


int main(void)
{
	HAL_Init();
	SYS_init();			//initialisation du systeme (horloge...)

	GPIO_Configure();
	UART_init();

	BSP_GPIO_PinCfg(LEDS_GPIO, LED_GREEN_PIN | LED_ORANGE_PIN | LED_RED_PIN | LED_BLUE_PIN, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FAST, 0);

	Systick_init();

	VL53L1X_init();

	MPU6050_t mpu6050_datastruct;
	bool_e mpu6050_initialized = FALSE;
	if(MPU6050_Init(&mpu6050_datastruct, MPU6050_VCC_GPIO, MPU6050_VCC_PIN, MPU6050_Device_0, MPU6050_Accelerometer_8G, MPU6050_Gyroscope_2000s) == MPU6050_Result_Ok)
		mpu6050_initialized = TRUE;


	TIMER2_run_1ms();

	while (1)
	{
		VL53L1X_process_main();
	}
}


//IT 1ms, de plus basse priorit� que l'IT du systick !
void TIMER2_user_handler_it_1ms(void)
{
	timeslot_e timeslot;
	timeslot = VL53L1X_process_it();

	//switch(timeslot)... //si l'on souhaite conditionner certaines choses aux timeslots des VL_53... pour r�partir la charge de calcul dans le temps.

	//notamment pour discuter avec le MPU6050 ! (car il utilise le m�me bus I2C ! donc il faut uniquement lui causer en IT !)
}








