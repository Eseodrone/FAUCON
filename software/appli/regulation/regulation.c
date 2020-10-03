/*
 * regulation.c
 *
 *  Created on: 24 sept. 2020
 *      Author: Theo
 */

#include "regulation.h"
#include "drone_def.h"


//Pointeurs sur les structure utilisées par la régulation
static datas_sensors_pooling_t * datas_sensors_pooling;
static target_values_t * target_values;
static PID_correction_t * PID_correction;
//Paramètres pour les pid
#define PID_ANGLE_FREQUENCY 250
#define PID_ANGLE_MAX_OUTPUT 200	//En admettant que mon signal moteur vari de 0 à 1000
#define PID_DIST_FREQUENCY 250
#define PID_DIST_MAX_OUTPUT 200
//Paramètre pour la correction des angles
#define ANGLE_CORRECTION_AMOUNT 42

//pid par défaut									P		I	  D			Frequence			Output Max
static float PID0_Settings_Roll[PID_NB_SETTINGS] = {2.0f, 0.0f, 0.0f, PID_ANGLE_FREQUENCY, PID_ANGLE_MAX_OUTPUT};
static float PID0_Settings_Pitch[PID_NB_SETTINGS] = {2.0f, 0.0f, 0.0f, PID_ANGLE_FREQUENCY, PID_ANGLE_MAX_OUTPUT};
static float PID0_Settings_Yaw[PID_NB_SETTINGS] = {2.0f, 0.0f, 0.0f, PID_ANGLE_FREQUENCY, PID_ANGLE_MAX_OUTPUT};
static float PID0_Settings_Z[PID_NB_SETTINGS] = {0.2f, 0.0f, 0.0f, PID_DIST_FREQUENCY, PID_DIST_MAX_OUTPUT};

static float PID1_Settings_Roll[PID_NB_SETTINGS] = {4.0f, 0.0f, 0.0f, PID_ANGLE_FREQUENCY, PID_ANGLE_MAX_OUTPUT};
static float PID1_Settings_Pitch[PID_NB_SETTINGS] = {4.0f, 0.0f, 0.0f, PID_ANGLE_FREQUENCY, PID_ANGLE_MAX_OUTPUT};
static float PID1_Settings_Yaw[PID_NB_SETTINGS] = {4.0f, 0.0f, 0.0f, PID_ANGLE_FREQUENCY, PID_ANGLE_MAX_OUTPUT};
static float PID1_Settings_Z[PID_NB_SETTINGS] = {0.1f, 0.0f, 0.0f, PID_DIST_FREQUENCY, PID_DIST_MAX_OUTPUT};



//Tableaux pids
PID_t pids[PID_COUNT];


//Init du module, on repère où sont les structures que l'on va utiliser par la suite
void REGULATION_init(datas_sensors_pooling_t * datas_sensors_pooling_,target_values_t * target_values_,  PID_correction_t * PID_correction_, uint8_t preset){
	//On mémorise nos structures de données que l on va utiliser
	datas_sensors_pooling = datas_sensors_pooling_;
	target_values = target_values_;
	//printf("Init Angle target: %d\n",(int) target_values->pitch_target);
	PID_correction = PID_correction_;

	//Configuration de nos pid
	REGULATION_config_pids(preset);

}

void REGULATION_process_angle(void){
	PID_correction->roll_pid = PID_compute(&pids[PID_ANGLE_ROLL], target_values->roll_target, datas_sensors_pooling->roll_angle);
	PID_correction->pitch_pid = PID_compute(&pids[PID_ANGLE_PITCH], target_values->pitch_target, datas_sensors_pooling->pitch_angle);
	PID_correction->yaw_pid = PID_compute(&pids[PID_ANGLE_YAW], target_values->yaw_target, datas_sensors_pooling->yaw_angle);
}

void REGULATION_process_dist(void){
	 //printf("dist z mes : %d\n",datas_sensors_pooling->dist_low_Z);
	 //printf("dist z tar : %d\n",target_values->z_target);

	 PID_correction->Z_pid  = PID_compute(&pids[PID_DIST_Z], target_values->z_target, datas_sensors_pooling->dist_low_Z);
}

float REGULATION_update_angle(void){
	static float angle_value [ANGLE_CORRECTION_AMOUNT] = {37.5952, 35.9033, 34.1455, 32.2778, 30.3003, 28.2568, 26.2134, 24.0601, 21.9946, 19.9731, 17.9297, \
			15.7983 , 13.5352 , 11.1401 , 8.6353 , 6.0425 , 3.3618 , 0.6592 , -1.9995 , -4.79 , -7.4487 , -10.0635 , -12.5903 , -15.0732 , -16.9849 , -19.9292, \
			-22.3022 , -24.6313 , -26.9604 , -29.2236 , -31.355 , -33.3545 , -35.2222 , -36.936 , -38.562 , -40.1221 , -41.6821 , -43.2861 , -44.9121 , -46.5381, \
			-48.0542 , -49.1089 };
	static uint8_t time_round = 0;
	time_round++;
	if (time_round==ANGLE_CORRECTION_AMOUNT){
		time_round=0;
	}
	return angle_value[time_round];
}

//preset doit être un chiffre entre 0 et ?
void REGULATION_config_pids(uint8_t preset){
	if(preset == 0){
		PID_init(&pids[PID_ANGLE_ROLL], PID0_Settings_Roll);
		PID_init(&pids[PID_ANGLE_PITCH], PID0_Settings_Pitch);
		PID_init(&pids[PID_ANGLE_YAW], PID0_Settings_Yaw);
		PID_init(&pids[PID_DIST_Z], PID0_Settings_Z);
	}else if (preset == 1){
		PID_init(&pids[PID_ANGLE_ROLL], PID1_Settings_Roll);
		PID_init(&pids[PID_ANGLE_PITCH], PID1_Settings_Pitch);
		PID_init(&pids[PID_ANGLE_YAW], PID1_Settings_Yaw);
		PID_init(&pids[PID_DIST_Z], PID1_Settings_Z);
	}else{
		PID_init(&pids[PID_ANGLE_ROLL], PID0_Settings_Roll);
		PID_init(&pids[PID_ANGLE_PITCH], PID0_Settings_Pitch);
		PID_init(&pids[PID_ANGLE_YAW], PID0_Settings_Yaw);
		PID_init(&pids[PID_DIST_Z], PID0_Settings_Z);
	}
}


