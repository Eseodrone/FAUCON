/*Ce fichier doit contenir la machine � �tats g�n�rale qui sera lanc�e dans le main*/
#include "main_program_file.h"

typedef enum
{
	SENSORS_INIT,
	MAIN_PROCESS,
	ERROR,
}mode_e;

void MPF_state_machine(void)
{
	static mode_e state = SENSORS_INIT;
	switch(state)
	{
	case SENSORS_INIT :
		//Appeler ici toutes les fonctions d'init des capteurs
		//MPU,TOFs,HC-05...
		//MPU_init();
		//VL53L1X_init();
		//BLUETOOTH_init();
		state = MAIN_PROCESS;
		break;
	case MAIN_PROCESS :
		//Organiser l'appel des fonctions vennant des diffs modules
		//MPU_angle_computer();
		//if(DM_stabilise_drone())
		//{
			//TOF_attribute_distances();
			//DM_compute_position();
			//Tente translation ?
		//}
		break;
	case ERROR :
		//Pr�voir un �tat de s�cu avec une extinction de tous les moteurs ?
		//MC_put_all_motors_off();
		break;
	default :
		break;
	}
}
