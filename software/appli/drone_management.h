/*
 * deplacement_drone.h
 *
 *  Created on: 30 juin 2020
 *      Author: Arnaud
 */

#ifndef APPLI_DRONE_MANAGEMENT_H_
#define APPLI_DRONE_MANAGEMENT_H_

#include "datas_process.h"
#define MINOR_ANGLE		5
#define INF 55
#define SUP 65
#define IMPULSION_TIME	1500 //1,5s

typedef enum
{
	FIXATION_INIT,
	FIXING,
	FIXED,
}fixes_e;

typedef enum
{
	NEED_FIXATION = 0,
	FIXATION_IN_PROGRESS,
	FIXATION_OK,
	FIXATION_TIMEOUT, //Au delà de 3 sec on recalcule ?
}fixing_modes_e;

typedef enum{
	TRANS_Z_HAUT = 0,
	TRANS_Z_BAS
}trans_z_e;

void DM_init(void);
void DM_mode_management(void);
int8_t DM_stabilise_drone(void);
void DM_correct_roll_angle(void);
void DM_correct_pitch_angle(void);
void DM_correct_yaw_angle(void);
void DM_correct_altitude(void);
void DM_compute_position(void);

#endif /* APPLI_DRONE_MANAGEMENT_H_ */
