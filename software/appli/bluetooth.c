/*
 * bluetooth.c


 *
 *  Created on: 22 mars 2020
 *      Author: Martin ARONDEL
 */


#include "bluetooth.h"
#include "drone_def.h"
#include "datas_process.h"



void BLUETOOTH_init(void){
	UART_init(UART2_ID, 38400);
}

//envoie un caractï¿½re
void BLUETOOTH_envoi_caractere(uint8_t c){
	c=UART_get_next_byte(UART2_ID);
	UART_putc(UART2_ID,c);
}


//reception d'un caractï¿re
char BLUETOOTH_reception_caractere(void){
	if(UART_data_ready(UART2_ID)){
		char c;
		c = UART_getc(UART2_ID);
		return c;
	}
	else{
		return 0;
	}
}

//reception d'une Trame de 4 informations
uint8_t *BLUETOOTH_reception_trame(void){
	static uint8_t trame[1];
	if(UART_data_ready(UART2_ID)){
		UART_gets(UART2_ID, trame, 1);
		return trame;
	}
	else{
		return 0;
	}
}

void BLUETOOTH_envoi_trame7(uint8_t info1, uint8_t info2, uint8_t info3, uint8_t info4, uint8_t info5, uint8_t info6, uint8_t info7){
	static uint8_t trame[7];
	trame[0] = info1;
	trame[1] = info2;
	trame[2] = info3;
	trame[3] = info4;
	trame[4] = info5;
	trame[5] = info6;
	trame[6] = info7;
	UART_puts(UART2_ID, trame, 7);
}

void BLUETOOTH_envoi_trame3(uint8_t info1, uint8_t info2, uint8_t info3){
	static uint8_t trame[3];
	trame[0] = info1;
	trame[1] = info2;
	trame[2] = info3;
	UART_puts(UART2_ID, trame, 3);
}

void BLUETOOTH_envoi_trame5(uint8_t info1, uint8_t info2, uint8_t info3, uint8_t info4, uint8_t info5){
	static uint8_t trame[5];
	trame[0] = info1;
	trame[1] = info2;
	trame[2] = info3;
	trame[3] = info4;
	trame[4] = info5;
	UART_puts(UART2_ID, trame, 5);
}

void BLUETOOTH_envoi_trame2(uint8_t info1, uint8_t info2){
	static uint8_t trame[2];
	trame[0] = info1;
	trame[1] = info2;
	UART_puts(UART2_ID, trame, 2);
}


void main_bluetooth (void){
	/* reception du caractere start et stop : analyse de la trame */

	char c = BLUETOOTH_reception_caractere();
	if(c=='s'){
		data_process_start();
		//mettre fonction pour allumer le drone
	}
	if(c=='u'){
		data_process_stop();
		//mettre foncion pour éteindre le drone
	}




	/* envoie trames avec toutes les infos */

	/*
	drone_data_t info_envoie;

	info_envoie.datas_sensors_pooling.roll_angle;
	info_envoie.datas_sensors_pooling.pitch_angle;
	info_envoie.datas_sensors_pooling.yaw_angle;

	info_envoie.datas_sensors_pooling.dist_0 = 4000;
	info_envoie.datas_sensors_pooling.dist_1 = 2000;
	info_envoie.datas_sensors_pooling.dist_2 = 1000;
	info_envoie.datas_sensors_pooling.dist_3 = 500;
	info_envoie.datas_sensors_pooling.dist_4 = 10;


	float info1 = info_envoie.datas_sensors_pooling.roll_angle;
	float info2 = info_envoie.datas_sensors_pooling.pitch_angle;
	float info3 = info_envoie.datas_sensors_pooling.yaw_angle;

	uint16_t info4 = info_envoie.datas_sensors_pooling.dist_0;
	uint16_t info5 = info_envoie.datas_sensors_pooling.dist_1;
	uint16_t info6 = info_envoie.datas_sensors_pooling.dist_2;
	uint16_t info7 = info_envoie.datas_sensors_pooling.dist_3;
	uint16_t info8 = info_envoie.datas_sensors_pooling.dist_4;

	//convert en 0 à 250

	info1 = info1/1,44;
	info2 = info2/1,44;
	info3 = info3/1,44;

	info4 = info4/16;
	info5 = info5/16;
	info6 = info6/16;
	info7 = info7/16;
	info8 = info8/16;

// foncrion envoie trames

	BLUETOOTH_envoi_trame3(info1,info2,info3);
	BLUETOOTH_envoi_trame5(info4,info5,info6,info7,info8); */
}



