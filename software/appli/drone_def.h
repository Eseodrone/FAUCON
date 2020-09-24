/*
 * drone_def.h
 *
 *  Created on: 24 sept. 2020
 *      Author: pierr
 */

#ifndef APPLI_DRONE_DEF_H_
#define APPLI_DRONE_DEF_H_

typedef struct{
	int16_t Gyroscope_X;     /*!< Gyroscope value X axis */
	int16_t Gyroscope_Y;     /*!< Gyroscope value Y axis */
	int16_t Gyroscope_Z;     /*!< Gyroscope value Z axis */

	uint16_t dist_0;
	uint16_t dist_1;
	uint16_t dist_2;
	uint16_t dist_3;
	uint16_t dist_4;
}datas_sensors_pooling_t;

typedef struct{
	float roll_pid;
	float pitch_pid;
	float yaw_pid;
}PID_correction_t;

typedef struct{
	datas_sensors_pooling_t datas_sensors_pooling;
	PID_correction_t pid_correction;
}drone_data_t;



#endif /* APPLI_DRONE_DEF_H_ */
