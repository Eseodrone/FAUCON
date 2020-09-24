/*
 * regulation.c
 *
 *  Created on: 24 sept. 2020
 *      Author: Theo
 */

#include "regulation.h"


//Pointeurs sur les structure utilisées par la régulation
static angles_t * angles;
static consignes_t * consignes;
static pids_outputs_t * pid_outputs;
//Paramètres pour les pid
#define PID_ANGLE_FREQUENCY 250
#define PID_ANGLE_MAX_OUTPUT 200	//En admettant que mon signal moteur vari de 0 à 1000
//											P		I	  D			Frequence			Output Max
static float PID_Settings_Roll[PID_NB_SETTINGS] = {4.0f, 0.0f, 0.0f, PID_ANGLE_FREQUENCY, PID_ANGLE_MAX_OUTPUT};
static float PID_Settings_Pitch[PID_NB_SETTINGS] = {4.0f, 0.0f, 0.0f, PID_ANGLE_FREQUENCY, PID_ANGLE_MAX_OUTPUT};
static float PID_Settings_Yaw[PID_NB_SETTINGS] = {4.0f, 0.0f, 0.0f, PID_ANGLE_FREQUENCY, PID_ANGLE_MAX_OUTPUT};


//Tableaux pids
PID_t pids[PID_COUNT] ;


//Init du module, on repère où sont les structures que l'on va utiliser par la suite
void REGULATION_init(angles_t * angles_,consignes_t * consignes_,  pids_outputs_t * pid_outputs_){
	//On mémorise nos structures de données que l on va utiliser
	angles = angles_ ;
	consignes = consignes_ ;
	pid_outputs = pid_outputs_ ;

	//Configuration de nos pid
	PID_init(&pids[PID_ANGLE_ROLL], PID_Settings_Roll);
	PID_init(&pids[PID_ANGLE_PITCH], PID_Settings_Pitch);
	PID_init(&pids[PID_ANGLE_YAW], PID_Settings_Yaw);
}

//Process des pids et mise à jour structures pid_outputs_t
void REGULATION_process_angle(void){
	//On admet que le roll corespond au x le y au pitch et z au yaw
	pid_outputs->pid_roll = PID_compute(&pids[PID_ANGLE_ROLL], consignes->roll_target, angles->x);
	pid_outputs->pid_pitch = PID_compute(&pids[PID_ANGLE_PITCH], consignes->pitch_target, angles->y);
	pid_outputs->pid_yaw = PID_compute(&pids[PID_ANGLE_YAW], consignes->yaw_target, angles->z);
}



