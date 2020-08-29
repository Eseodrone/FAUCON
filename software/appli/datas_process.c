/*
 * datas_process.c

 *
 *  Created on: 23 juin 2020
 *      Author: Arnaud
 */


#include "datas_process.h"

/// INTERROGATION AVEC IT DE TIMER 2   ///
//IT 1ms, de plus basse priorit� que l'IT du systick !
void TIMER2_user_handler_it_1ms(void)
{
//compteur declar� dans tests_methods.h
	if(compt <= 5000){ compt += 1; }
	else{ compt = 0; }

//IDEE => Fonction d'interrogation des ToFs toutes les TIME_NO_POOLING_TOF ms, si en communication TOF_OK = 0, sinon, TOF_OK = 1
//compteur declar� dans datas_process.h
	static timeslot_e timeslot;

	if(compteur_no_pooling_tof <= TIME_NO_POOLING_TOF && TOF_OK ==1){
		compteur_no_pooling_tof++;
		TOF_OK = 1;
	}


	if(compteur_no_pooling_tof > TIME_NO_POOLING_TOF || TOF_OK == 0){
		TOF_OK = 0;
		compteur_no_pooling_tof = 0;

		timeslot = VL53L1X_process_it();

		if(timeslot == TIMESLOT_ASK_END){ //Si fin de com avec les ToFs
			TOF_OK = 1; //Alors TOF_OK = 1, on peut alors parler en I2C au gyro et lire les valeurs des tofs
		}
		else{
			TOF_OK = 0; //sinon communication pas finie =>
		}

	}

	//switch(timeslot)... //si l'on souhaite conditionner certaines choses aux timeslots des VL_53... pour r�partir la charge de calcul dans le temps.

	//notamment pour discuter avec le MPU6050 ! (car il utilise le m�me bus I2C ! donc il faut uniquement lui causer en IT !)
}


uint8_t data_process_main(){
	static state_e state;
	state = INIT_POSITION;
	static uint8_t DATAS_STRUCT_POSITION_OK = 0;

	switch (state) {
		case INIT_POSITION:
			//.................init()
			state = READ_AND_PROCESS_DATA;
			break;
		case READ_AND_PROCESS_DATA:
			if(TOF_OK){
				//maj nos datastrcut r�elles dans datas_sensors_pooling
				datas_gyro_maj(&mpu_datas_res);//com avec MPU
				datas_tof_maj();
				process_data();
			}
			//state = PROCESS_DATA;
			DATAS_STRUCT_POSITION_OK = 1;

			break;
		case STOP :
			break;


	}

	return DATAS_STRUCT_POSITION_OK;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void datas_tof_maj(){
	datas_sensors_pooling.dist_0 = VL53L1X_get_distance(0);
	datas_sensors_pooling.dist_1 = VL53L1X_get_distance(1);
	datas_sensors_pooling.dist_2 = VL53L1X_get_distance(2);
	datas_sensors_pooling.dist_3 = VL53L1X_get_distance(3);
	datas_sensors_pooling.dist_4 = VL53L1X_get_distance(4);
}

/*
 *@brief
 *
 */

void datas_gyro_maj(MPU6050_t* DataStruct){
	if(mpu_init_OK != FALSE){
		//Alimente gyro
		HAL_GPIO_WritePin( MPU6050_VCC_GPIO, MPU6050_VCC_PIN, SET);

		MPU6050_ReadGyroscope(DataStruct);

		//eteint gyro
		HAL_GPIO_WritePin( MPU6050_VCC_GPIO, MPU6050_VCC_PIN, RESET);
	}

//  maj la sruct de datas des mouvements
	datas_sensors_pooling.Gyroscope_X = DataStruct->Gyroscope_X;
	datas_sensors_pooling.Gyroscope_Y = DataStruct->Gyroscope_Y;
	datas_sensors_pooling.Gyroscope_Z = DataStruct->Gyroscope_Z;

}


void datas_acc_maj(MPU6050_t* DataStruct){
	if(mpu_init_OK != FALSE){
		//Alimente gyro
		HAL_GPIO_WritePin( MPU6050_VCC_GPIO, MPU6050_VCC_PIN, SET);

		MPU6050_ReadAccelerometer(DataStruct);

		//eteint gyro
		HAL_GPIO_WritePin( MPU6050_VCC_GPIO, MPU6050_VCC_PIN, RESET);
	}

	//  maj la sruct de datas des mouvements
	datas_sensors_pooling.Accelerometer_X = DataStruct->Accelerometer_X;
	datas_sensors_pooling.Accelerometer_Y = DataStruct->Accelerometer_Y;
	datas_sensors_pooling.Accelerometer_Z = DataStruct->Accelerometer_Z;

}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void process_data(){
	//gyor
	datas_drone_position.angle_X_ROLL += datas_sensors_pooling.Gyroscope_X;
	datas_drone_position.angle_Y_PITCH += datas_sensors_pooling.Gyroscope_Y;
	datas_drone_position.angle_Z_YAW += datas_sensors_pooling.Gyroscope_Z;

	datas_drone_position.angle_X_ROLL /= 16400; //convertir en �
	datas_drone_position.angle_Y_PITCH /= 16400; //convertir en �
	datas_drone_position.angle_Z_YAW /= 16400; //convertir en �

	//tofs
	//datas_drone_position.position_X = datas_sensors_pooling.CAT(dist_, TOF_ID_AXE_X);

	datas_drone_position.position_X = datas_sensors_pooling.dist_0 - datas_drone_position.X0 - MESURE_LARGEUR_DRONE_CM;
	datas_drone_position.position_Y = datas_sensors_pooling.dist_1 - datas_drone_position.Y0 - MESURE_LARGEUR_DRONE_CM;
	datas_drone_position.position_Z = datas_sensors_pooling.dist_2 - datas_drone_position.Z0 - MESURE_HAUEUR_DRONE_CM;


}

void init_datas(){
	datas_drone_position.X0 = 0;
	datas_drone_position.Y0 = 0;
	datas_drone_position.Z0 = 0;


//...................................


}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void send_datas_tof_bluetooth(){
	if(compt == 1000){
		//UART_puts(UART2_ID, "tof_1: ", 7);
		//envoiCaractere(datas_tof.dist_1);

		envoiCaractere('g');
	}
}









