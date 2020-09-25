/*
 * regulation.h
 *
 *  Created on: 24 sept. 2020
 *      Author: Theo
 */

#ifndef APPLI_REGULATION_REGULATION_H_
#define APPLI_REGULATION_REGULATION_H_

#include "../../lib/btm/Pid.h"

// COMMENT CA MARCHE
//On modifi les coefs des pids dans le regulation.c il y a les tableaux en haut
//Regulation_init une fois dans le main en passant en param�tre nos structure de donn�es
//Pi on appelle regulation_procces � 250 Hz avant l'envoit aux moteurs
//Et voil� :)

//Nom des pids
typedef enum{
	PID_ANGLE_ROLL,
	PID_ANGLE_PITCH,
	PID_ANGLE_YAW,

	PID_COUNT
}PID_names_t;

//Structure temporaires pour les angles
typedef struct{
	float x;
	float y;
	float z;
}angles_t;

//Structure temporaires pour les consignes
typedef struct{
	float roll_target;
	float pitch_target;
	float yaw_target;
}consignes_t;

//Structure temporaires pour les sorties de pid
typedef struct{
	float pid_roll ;
	float pid_pitch ;
	float pid_yaw ;
}pids_outputs_t;


//Init du module, on rep�re o� sont les structures que l'on va utiliser par la suite
void REGULATION_init(angles_t * angles,consignes_t * consignes,  pids_outputs_t * pid_outputs);

//Process des pids et mise � jour structures pid_outputs_t
void REGULATION_process_angle(void);



#endif /* APPLI_REGULATION_REGULATION_H_ */
