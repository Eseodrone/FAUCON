/*
 * datas_process.c

 *
 *  Created on: 23 juin 2020
 *      Author: Arnaud
 */


#include "datas_process.h"
#include "stm32f4_timer.h"

uint32_t compteur_TIM_5 = 0;

drone_data_t* drone_data;

bool_e TOF_OK = 0;
uint32_t compteur_no_pooling_tof = 0;
uint32_t compteur_no_pooling_mpu = 0;



/// INTERROGATION AVEC IT DE TIMER 2   ///
//IT 1ms, de plus basse priorit� que l'IT du systick !
void TIMER5_user_handler_it_1ms(void)
{
	static timeslot_e timeslot;

//compteur declar� dans tests_methods.h
	if(compt <= 5000){ compt += 1; }
	else{ compt = 0; }
	//Fonction d'interrogation des ToFs toutes les TIME_NO_POOLING_TOF ms, si en communication TOF_OK = 0, sinon, TOF_OK = 1

	if(compteur_no_pooling_tof <= TIME_MS_POOLING_TOF && TOF_OK ==1){
		compteur_no_pooling_tof++;
		TOF_OK = 1;
	}


	if(compteur_no_pooling_tof > TIME_MS_POOLING_TOF || TOF_OK == 0){
		TOF_OK = 0;
		compteur_no_pooling_tof = 0;

		timeslot = VL53L1X_process_it();

		if(timeslot == TIMESLOT_ASK_END){ //Si fin de com avec les ToFs
			TOF_OK = 1; //Alors TOF_OK = 1, on peut alors parler en I2C au gyro et lire les valeurs des tofs
			datas_tof_maj();
		}
		else{
			TOF_OK = 0; //sinon communication pas finie =>
		}

	}

	if(compteur_no_pooling_mpu <= TIME_MS_POOLING_MPU){
		compteur_no_pooling_mpu++;
	}
	else{
		MPU_angle_computer();
		compteur_no_pooling_mpu = 0;
	}

	REGULATION_process_angle();
	MC_PID_correction();
	//MC_update_motors();
}



void data_process_init(drone_data_t * drone){
	drone_data = drone;
	MPU_init(drone_data);
	VL53L1X_init();
	//REGULATION_init(&(drone->datas_sensors_pooling),&(drone->target_values),&(drone->target_values));
	//TIMER2_run_1ms();
	TIMER5_run_1ms();

}




void datas_tof_maj(){
	drone_data->datas_sensors_pooling.dist_0 = VL53L1X_get_distance(0);
	drone_data->datas_sensors_pooling.dist_1 = VL53L1X_get_distance(1);
	drone_data->datas_sensors_pooling.dist_2 = VL53L1X_get_distance(2);
	drone_data->datas_sensors_pooling.dist_3 = VL53L1X_get_distance(3);
	drone_data->datas_sensors_pooling.dist_4 = VL53L1X_get_distance(4);
}



uint8_t uint16_to_uint8(uint16_t data16){
	return data16 >> 8;
}






