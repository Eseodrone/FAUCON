#ifndef MOTORS_CONTROL_H_
#define MOTORS_CONTROL_H_

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include "stm32f4xx_hal.h"
#include "macro_types.h"
#include "stm32f4_uart.h"
#include "stm32f4_sys.h"
#include "stm32f4_spi.h"
#include "stm32f4_gpio.h"
#include "stm32f4_timer.h"

///*** PORTS INIT ***///
#define PWM_MOT_11 GPIOC, GPIO_PIN_6
#define PWM_MOT_12 GPIOC, GPIO_PIN_7
#define PWM_MOT_13 GPIOC, GPIO_PIN_8
#define PWM_MOT_14 GPIOC, GPIO_PIN_9

#define PWM_MOT_21 GPIOE, GPIO_PIN_9
#define PWM_MOT_22 GPIOE, GPIO_PIN_11
#define PWM_MOT_23 GPIOE, GPIO_PIN_13
#define PWM_MOT_24 GPIOE, GPIO_PIN_14

//PWM
//MACROS DE Mr POIRAUD
#define TIM1_8_9_10_11_CLK    			168000000	//Fréquence des évènements d'horloge pour les timers 1, 8, 9, 10, 11
#define TIM2_3_4_5_6_7_12_13_14_CLK    	84000000	//Fréquence des évènements d'horloge pour les timers 2, 3, 4, 5, 6, 7, 12, 13 et 14

//L'horloge du timer 3 est a 84MHz
//Si l'on veut une PWM a 20kHz (inaudible) et 100 pas de rapports cycliques possibles, il faut prediviser par 42 :
//168MHz/84 = 2MHz -> 500ns par pas... * 100 pas = 20kHz de frequence PWM
#define PWM_FREQ_TIM	500 	//Fréquence du signal PWM, en Hz
#define	PWM_PERIOD_TIM	100		//Nombre jusqu'auquel le timer va compter durant une période PWM

#define	PWM_PRESC_TIM_3	((TIM2_3_4_5_6_7_12_13_14_CLK / PWM_FREQ_TIM) / PWM_PERIOD_TIM)	//Prédiviseur : nombre d'évènements qui provoquent +1 sur le décompte du timer
#define	PWM_PRESC_TIM_1	((TIM1_8_9_10_11_CLK / PWM_FREQ_TIM) / PWM_PERIOD_TIM)	//Prédiviseur : nombre d'évènements qui provoquent +1 sur le décompte du timer

//PROTOTYPES DE FONCTION
void MC_init_pwm_tim1_tim3(void);
void MC_pwm_timer_set_duty(TIM_HandleTypeDef tim_handle, int channel, uint16_t duty);
void MC_M1_1(uint16_t TIME);
void MC_M1_2(uint16_t TIME);
void MC_M1_3(uint16_t TIME);
void MC_M1_4(uint16_t TIME);
void MC_M2_1(uint16_t TIME);
void MC_M2_2(uint16_t TIME);
void MC_M2_3(uint16_t TIME);
void MC_M2_4(uint16_t TIME);
void MC_put_all_motors_off(void);

#endif /*MOTORS_CONTROL_H*/

/*
 * Explications :
 * - Le timer compte des évènements...
 * - Nous disposons d'une horloge à 84MHz...
 * - Tout les X évènements (les X périodes d'horloge), le timer compte +1.... Ce X est le prédiviseur. (PWM_PRESC)
 * - Le timer va compter jusqu'à "PWM_PERIOD"... Puis revenir à 0. Et ainsi de suite.
 * - La durée qu'il met à compter jusqu'à cette PWM_PERIOD nous donne la période du signal PWM... L'inverse est la fréquence !
 * 				Période du signal PWM 	= (PWM_PERIOD	*	PWM_PRESC)/84MHz
 * 	Exemple :				50µs	 	= (100			*	42)/84MHz
 * 	 			Fréquence du signal PWM = 84MHz/(PWM_PERIOD	*	PWM_PRESC)
 * 	Exemple :				50µs	 	= 84MHz/(100		*	42)
 */

//==========================
/*
 PWM_MOT_11 GPIOC, GPIO_PIN_6 => TIM3_CH1
 PWM_MOT_12 GPIOC, GPIO_PIN_7 => TIM3_CH2
 PWM_MOT_13 GPIOC, GPIO_PIN_8 => TIM3_CH3
 PWM_MOT_14 GPIOC, GPIO_PIN_9 => TIM3_CH4

 PWM_MOT_21 GPIOE, GPIO_PIN_9 => TIM1_CH1
 PWM_MOT_22 GPIOE, GPIO_PIN_11 => TIM1_CH2
 PWM_MOT_23 GPIOE, GPIO_PIN_13 => TIM1_CH3
 PWM_MOT_24 GPIOE, GPIO_PIN_14 => TIM1_CH4
*/
