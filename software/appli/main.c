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
#include "systick.h"
#include "bluetooth.h"
#include "save_old/tests_methods.h"
#include "datas_process.h"
#include "mpu6050.h"

int main(void){
	HAL_Init();
	SYS_init();			//initialisation du systeme (horloge...)

	GPIO_Configure();
	//UART DU BLUETOOTH
	BLUETOOTH_init();
	//UNI
	//SYS_set_std_usart(UART2_ID, UART2_ID, UART2_ID);


	BSP_GPIO_PinCfg(LEDS_GPIO, LED_GREEN_PIN | LED_ORANGE_PIN | LED_RED_PIN | LED_BLUE_PIN, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FAST, 0);
	BSP_GPIO_PinCfg(GPIOA, GPIO_PIN_7, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FAST, 0);

	Systick_init();


	//INIT DES CAPTEURS (Il faudra en faire une générale ou gérer ça par classe/module)
	//MC_init_pwm_tim1_tim3();
	VL53L1X_init();
	//MPU_init();

	TIMER2_run_1ms();

	while (1)
	{
		test_tof_1();


	}
}












