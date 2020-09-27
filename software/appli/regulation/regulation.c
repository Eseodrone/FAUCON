/*
 * regulation.c
 *
 *  Created on: 24 sept. 2020
 *      Author: Theo
 */

#include "regulation.h"
#include "drone_def.h"


//Pointeurs sur les structure utilis�es par la r�gulation
static datas_sensors_pooling_t * datas_sensors_pooling;
static target_values_t * target_values;
static PID_correction_t * PID_correction;
//Param�tres pour les pid
#define PID_ANGLE_FREQUENCY 250
#define PID_ANGLE_MAX_OUTPUT 200	//En admettant que mon signal moteur vari de 0 � 1000
//											P		I	  D			Frequence			Output Max
static float PID_Settings_Roll[PID_NB_SETTINGS] = {4.0f, 0.0f, 0.0f, PID_ANGLE_FREQUENCY, PID_ANGLE_MAX_OUTPUT};
static float PID_Settings_Pitch[PID_NB_SETTINGS] = {4.0f, 0.0f, 0.0f, PID_ANGLE_FREQUENCY, PID_ANGLE_MAX_OUTPUT};
static float PID_Settings_Yaw[PID_NB_SETTINGS] = {4.0f, 0.0f, 0.0f, PID_ANGLE_FREQUENCY, PID_ANGLE_MAX_OUTPUT};


//Tableaux pids
PID_t pids[PID_COUNT] ;


//Init du module, on rep�re o� sont les structures que l'on va utiliser par la suite
void REGULATION_init(datas_sensors_pooling_t * datas_sensors_pooling_,target_values_t * target_values_,  PID_correction_t * PID_correction_){
	//On m�morise nos structures de donn�es que l on va utiliser
	datas_sensors_pooling = datas_sensors_pooling_;
	target_values = target_values_;
	printf("Init Angle target: %d\n",(int) target_values->pitch_target);
	PID_correction = PID_correction_;

	//Configuration de nos pid
	PID_init(&pids[PID_ANGLE_ROLL], PID_Settings_Roll);
	PID_init(&pids[PID_ANGLE_PITCH], PID_Settings_Pitch);
	PID_init(&pids[PID_ANGLE_YAW], PID_Settings_Yaw);
}

//Process des pids et mise � jour structures pid_outputs_t
void REGULATION_process_angle(void){
	//On admet que le roll corespond au x le y au pitch et z au yaw
	//PID_correction->roll_pid = PID_compute(&pids[PID_ANGLE_ROLL], target_values->roll_target, datas_sensors_pooling->roll_angle);
	//printf("Angle : %d\n",(int)  datas_sensors_pooling->pitch_angle);
	PID_correction->pitch_pid = PID_compute(&pids[PID_ANGLE_PITCH], target_values->pitch_target, datas_sensors_pooling->pitch_angle);
	//PID_correction->yaw_pid = PID_compute(&pids[PID_ANGLE_YAW], target_values->yaw_target, datas_sensors_pooling->yaw_angle);
}


