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
#include "motors_control.h"
#include "drone_def.h"

int main(void){
	HAL_Init();
	SYS_init();			//initialisation du systeme (horloge...)
	GPIO_Configure();

	//* UART DU BLUETOOTH
	BLUETOOTH_init();

	//* UART DU PRINTF ==> Activation SEULEMENT si on utilise pas le bluetooth
	SYS_set_std_usart(UART2_ID, UART2_ID, UART2_ID);

	//* LEDs F4
	BSP_GPIO_PinCfg(LEDS_GPIO, LED_GREEN_PIN | LED_ORANGE_PIN | LED_RED_PIN | LED_BLUE_PIN, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FAST, 0);
	BSP_GPIO_PinCfg(GPIOA, GPIO_PIN_7, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FAST, 0);



	Systick_init();

	drone_data_t drone;

	//* init de la lecture des capteurs, utilise TIMER2
	//data_process_init(&drone);
	MPU_init(&drone);
	//TESTS MOTEUR
	//MC_init_pwm_tim1_tim3();
	//MC_esc_calibration();
	//test_moteur_PC6(60);
	//MC_test_motor_one_by_one();
	//MC_test_progressive_pwm();
	//MC_test_all_motors();
	//HAL_Delay(4000);
	//MC_put_all_motors_off();
	while (1){
		HAL_Delay(4);
		MPU_angle_computer();
//		printf("Gyro x : %d\n",p_mpu_datas_res->Gyroscope_X);
	}
}












