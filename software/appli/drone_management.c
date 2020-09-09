/*
 * deplacement_drone.c
 *
 *  Created on: 30 juin 2020
 *      Author: Arnaud
 */

//==================================================================================================
/*
Pour les d�placements du drone, on distingue :
- Translations sur axe Z
- Translations sur axe X et Y
- Rotation sur les axes X et Y
- Rotation sur l'axe Z

====>>>>>> ATTENTION A L'INERTIE !!!!! <<<<<<<=====
==>> TRANSLATIONS : il faut absolument v�rifier que le drone ait une position angulaire nulle avant d'effectuer une translation
==>> Prendre en compte

Translation sur Z:
Moteurs de face haute ou basse.

Translation sur X et Y:
Selon l'axe X ou Y ; rotation de 90�, mise en marche de tous les moteurs de la face basse ou haute, dur�e/puissance selon la distance � parcourir, puis rotation de -90�.


Rotation sur X et Y:
Utilisation de tous les moteurs pour effectuer une rotation ; Algo de calcul � impl�menter (check controlleur de vol de drone de commerce),

Rotation sur Z:
Utilisation de tous les moteurs de chaque face.

---------------------------------------

Rappels des moterus :
 PWM_MOT_11 GPIOC, GPIO_PIN_6 => TIM3_CH1
 PWM_MOT_12 GPIOC, GPIO_PIN_7 => TIM3_CH2
 PWM_MOT_13 GPIOC, GPIO_PIN_8 => TIM3_CH3
 PWM_MOT_14 GPIOC, GPIO_PIN_9 => TIM3_CH4

 PWM_MOT_21 GPIOE, GPIO_PIN_9 => TIM1_CH1
 PWM_MOT_22 GPIOE, GPIO_PIN_11 => TIM1_CH2
 PWM_MOT_23 GPIOE, GPIO_PIN_13 => TIM1_CH3
 PWM_MOT_24 GPIOE, GPIO_PIN_14 => TIM1_CH4

*/
//==================================================================================================


#include <drone_management.h>
#include "motors_control.h"
#include "stdbool.h"

typedef struct
{
	float x_pos;
	float y_pos;
	float z_pos;
}drone_position_t;

static float roll;
static float pitch;
static float yaw;
static drone_position_t drone_position;
static uint8_t ROLL_FIXATION;
static uint8_t PITCH_FIXATION;
static uint8_t YAW_FIXATION;
static bool STABILISATION_OK;
static void DM_check_angles(void);





//FONCTIONS DE STABILISATION
void DM_init(void)
{
	roll = 0;
	pitch = 0;
	yaw = 0;
	ROLL_FIXATION = FALSE;
	PITCH_FIXATION = FALSE;
	YAW_FIXATION = FALSE;
	STABILISATION_OK = FALSE;
	//Init des moteurs
}

void DM_check_angles(void)
{
	roll = MPU_get_roll();
	pitch = MPU_get_pitch();
	yaw = MPU_get_yaw();
	//ROLL
	if (roll>=MINOR_ANGLE && roll<=360-MINOR_ANGLE)
	{
		ROLL_FIXATION = NEED_FIXATION;
	}

	else
	{
		ROLL_FIXATION = FIXATION_OK;
	}
	//PITCH
	if (pitch>=MINOR_ANGLE && pitch<=360-MINOR_ANGLE)
	{
		PITCH_FIXATION = NEED_FIXATION;
	}

	else
	{
		PITCH_FIXATION = FIXATION_OK;
	}
	//YAW
	if (yaw>=MINOR_ANGLE && yaw<=360-MINOR_ANGLE)
	{
		YAW_FIXATION = NEED_FIXATION;
	}

	else
	{
		YAW_FIXATION = FIXATION_OK;
	}
}

void DM_mode_management(void)
{
	static fixes_e mode = FIXATION_INIT;
	switch(mode)
	{
	case FIXATION_INIT :
		DD_init();
		mode=FIXING;
		break;
	case FIXING :
		DD_check_angles();
		if(PITCH_FIXATION == NEED_FIXATION)
		{
			PITCH_FIXATION = FIXATION_IN_PROGRESS;
			if(pitch > MINOR_ANGLE && pitch < 180)
			{
				//Combinaison moteurs
				//Voir google sheet drive
				//M1_1 : Inf
				//M1_2 : Inf
				//M1_3 : Sup
				//M1_4 : Sup
				//M2_1 : Sup
				//M2_2 : Sup
				//M2_3 : Inf
				//M2_4 : Inf
				//Rotation en avant

				M1_1(INF); //55
				M1_2(INF);
				M1_3(SUP);
				M1_4(SUP);
				M2_1(SUP);
				M2_2(SUP);
				M2_3(INF);
				M2_4(INF);

			}
			else if(pitch < 360-MINOR_ANGLE)
			{
				//Combinaison moteurs
				//Rotation en arri�re

				M1_1(SUP);
				M1_2(SUP);
				M1_3(INF);
				M1_4(INF);
				M2_1(INF);
				M2_2(INF);
				M2_3(SUP);
				M2_4(SUP);
			}
			break;
		}
		if(ROLL_FIXATION == NEED_FIXATION && PITCH_FIXATION == FIXATION_OK)
		{
			ROLL_FIXATION = FIXATION_IN_PROGRESS;
			if(roll > MINOR_ANGLE && pitch < 180)
			{
				//Combinaison moteurs
				//Rotation � gauche
				M1_1(SUP);
				M1_2(INF);
				M1_3(SUP);
				M1_4(INF);
				M2_1(INF);
				M2_2(SUP);
				M2_3(INF);
				M2_4(SUP);
			}
			else if(roll < 360-MINOR_ANGLE)
			{
				//Combinaison moteurs
				//Rotation � droite
				M1_1(INF);
				M1_2(SUP);
				M1_3(INF);
				M1_4(SUP);
				M2_1(SUP);
				M2_2(INF);
				M2_3(SUP);
				M2_4(INF);
			}
			break;
		}
		if(YAW_FIXATION == NEED_FIXATION && PITCH_FIXATION == FIXATION_OK && ROLL_FIXATION == FIXATION_OK)
		{
			YAW_FIXATION = FIXATION_IN_PROGRESS;
			if(yaw > MINOR_ANGLE && pitch < 180)
			{
				//Combinaison moteurs
				//Rotation � gauche
				M1_1(INF);
				M1_2(SUP);
				M1_3(SUP);
				M1_4(INF);
				M2_1(INF);
				M2_2(SUP);
				M2_3(SUP);
				M2_4(INF);

			}
			else if(yaw < 360-MINOR_ANGLE)
			{
				//Combinaison moteurs
				//Rotation � droite
				M1_1(SUP);
				M1_2(INF);
				M1_3(INF);
				M1_4(SUP);
				M2_1(SUP);
				M2_2(INF);
				M2_3(INF);
				M2_4(SUP);
			}
			break;
		}
		mode = FIXED;
		break;
	case FIXED :
		STABILISATION_OK = TRUE;
		break;
	default :
		printf("Error stabilising the drone");
		break;
	}
}

int8_t DM_stabilise_drone(void)
{
	//ROLL
	if(roll > 10 || roll < -10)
	{
		DM_correct_roll_angle();
	}
	else
	{
		ROLL_FIXATION = TRUE;
	}
	//PITCH
	if(pitch > 10 || pitch < -10)
	{
		DM_correct_pitch_angle();
	}
	else
	{
		PITCH_FIXATION = TRUE;
	}
	//YAW
	if(yaw > 10 || yaw < -10)
	{
		DM_correct_yaw_angle();
	}
	else
	{
		YAW_FIXATION = TRUE;
	}
	if(ROLL_FIXATION && PITCH_FIXATION && YAW_FIXATION)
		return 1;
	else
		return 0;
}


void DM_correct_roll_angle(void)
{
	if(roll > 10)
	{
		//Combi moteurs ON
		HAL_Delay(IMPULSION_TIME);
		//Combi moteurs OFF
	}
	else if(roll < -10)
	{
		HAL_Delay(IMPULSION_TIME);
	}
}

void DM_correct_pitch_angle(void)
{
	if(pitch > 10)
	{
		//Combi moteurs ON
		HAL_Delay(IMPULSION_TIME);
		//Combi moteurs OFF
	}
	else if(pitch < -10)
	{
		HAL_Delay(IMPULSION_TIME);
	}
}

void DM_correct_yaw_angle(void)
{
	if(yaw > 10)
	{
		//Combi moteurs ON
		HAL_Delay(IMPULSION_TIME);
		//Combi moteurs OFF
	}
	else if(yaw < -10)
	{
		HAL_Delay(IMPULSION_TIME);
	}
}

void DM_correct_altitude(void)
{
	//if(drone_position.z_pos < )
	//Moteurs sup même vitesse
	MC_f1_m1_PC6(PWM_MAX_MOTOR_ON);
	MC_f1_m2_PC7(PWM_MAX_MOTOR_ON);
	MC_f1_m3_PC8(PWM_MAX_MOTOR_ON);
	MC_f1_m4_PC9(PWM_MAX_MOTOR_ON);
	HAL_Delay(IMPULSION_TIME);
	MC_f1_m1_PC6(PWM_MIN_MOTOR_OFF);
	MC_f1_m2_PC7(PWM_MIN_MOTOR_OFF);
	MC_f1_m3_PC8(PWM_MIN_MOTOR_OFF);
	MC_f1_m4_PC9(PWM_MIN_MOTOR_OFF);
	//else if(drone_position.z_pos > )
	//Moteurs inf même vitesse
	MC_f2_m1_PE9(PWM_MAX_MOTOR_ON);
	MC_f2_m2_PE11(PWM_MAX_MOTOR_ON);
	MC_f2_m3_PE13(PWM_MAX_MOTOR_ON);
	MC_f2_m4_PE14(PWM_MAX_MOTOR_ON);
	HAL_Delay(IMPULSION_TIME);
	MC_f2_m1_PE9(PWM_MIN_MOTOR_OFF);
	MC_f2_m2_PE11(PWM_MIN_MOTOR_OFF);
	MC_f2_m3_PE13(PWM_MIN_MOTOR_OFF);
	MC_f2_m4_PE14(PWM_MIN_MOTOR_OFF);
}

void DM_compute_position(void)
{
	drone_position.x_pos = TOF_get_sensor_3_dist(); //A voir quel capteur mesure quel axe
	drone_position.y_pos = TOF_get_sensor_2_dist();
	drone_position.z_pos = TOF_get_sensor_1_dist();

}

// FONCTIONS DE TRANSLATION
void trans_simple_Z(trans_z_e direction, uint8_t time){
	if(direction == TRANS_Z_BAS){
		//moteurs haut pr aller vers bas
	}
	else if (direction == TRANS_Z_HAUT) {
		//moteurs bas pr aller vers haut

	}

}

