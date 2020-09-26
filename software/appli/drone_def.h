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
	float roll_target;
	float pitch_target;
	float yaw_target;
}target_values_t;

typedef struct{
	uint16_t m11;
	uint16_t m12;
	uint16_t m13;
	uint16_t m14;
	uint16_t m21;
	uint16_t m22;
	uint16_t m23;
	uint16_t m24;
}motor_cmd_t;

typedef struct{
	datas_sensors_pooling_t datas_sensors_pooling;
	PID_correction_t pid_correction;
	target_values_t target_values;
	motor_cmd_t motor_cmd;
}drone_data_t;



#endif /* APPLI_DRONE_DEF_H_ */
