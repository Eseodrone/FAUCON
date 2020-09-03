/*Ce fichier doit contenir la machine à états générale qui sera lancée dans le main*/
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
		state = MAIN_PROCESS;
		break;
	case MAIN_PROCESS :
		//Organiser l'appel des fonctions vennant des diffs modules
		break;
	case ERROR :
		//Prévoir un état de sécu avec une extinction de tous les moteurs ?
		//MC_put_all_motors_off();
		break;
	default :
		break;
	}
}
