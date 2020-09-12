/*
 * datas_process.h

 *
 *  Created on: 23 juin 2020
 *      Author: Arnaud
 */

#ifndef APPLI_DATAS_PROCESS_H_
#define APPLI_DATAS_PROCESS_H_

#include "save_old/tests_methods.h"
#include "mpu6050.h"
#include "tof.h"

#define CAT(A, B) A ## B


#define DIST_FORWARD 1
#define DIST_HEIGHT 1
#define DIST_FACE 1

#define TOF_ID_AXE_X 0
#define TOF_ID_AXE_Y 1
#define TOF_ID_AXE_Z 2

#define TIME_NO_POOLING_TOF 150
#define MESURE_HAUEUR_DRONE_CM 10
#define MESURE_LARGEUR_DRONE_CM 30



typedef enum state_e{
	INIT_POSITION,
	READ_AND_PROCESS_DATA,
	STOP,

}state_e;

//Structure de mpu modifi�e automatiquement par fonctions
MPU6050_t mpu_datas_res;

extern uint32_t compteur_no_pooling_tof;
extern bool_e TOF_OK;



typedef struct{
	int16_t Accelerometer_X; /*!< Accelerometer value X axis */
	int16_t Accelerometer_Y; /*!< Accelerometer value Y axis */
	int16_t Accelerometer_Z; /*!< Accelerometer value Z axis */
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
	uint16_t X0;
	uint16_t Y0;
	uint16_t Z0;

	uint16_t position_X;
	uint16_t position_Y;
	uint16_t position_Z;

	uint16_t angle_X_ROLL;
	uint16_t angle_Y_PITCH;
	uint16_t angle_Z_YAW;
}datas_drone_position_t;


extern datas_sensors_pooling_t* p_datas_sensors_pooling;
extern datas_drone_position_t* p_datas_drone_position;


void TIMER2_user_handler_it_1ms(void);
void datas_tof_maj();
void datas_gyro_maj();
void datas_acc_maj();
void send_datas_tof_bluetooth();
uint8_t data_process_main();
void process_data();




#endif /* APPLI_DATAS_PROCESS_H_ */
