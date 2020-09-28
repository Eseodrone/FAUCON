/*
 * bluetooth.c


 *
 *  Created on: 22 mars 2020
 *      Author: Martin ARONDEL
 */


#include "bluetooth.h"
#include "drone_def.h"
#include "datas_process.h"


static drone_data_t * drone_data;


void BLUETOOTH_init(drone_data_t * drone_data_){
	UART_init(UART2_ID, 38400);
	drone_data = drone_data_;
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



//48.8 => 49 | 48.3 => 48
#define FLOAT_TO_UINT8(x) ((x)>=0?(int)((x)+0.5):(int)((x)-0.5))

#define TOF_DATAS			5
#define MPU_DATAS			0
#define SIZE_BYTE_BUFFER_TRAME	(TOF_DATAS*2)+(MPU_DATAS*2)



void main_bluetooth (){
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




	uint16_t backward_X = drone_data->datas_sensors_pooling.dist_backward_X;
	uint16_t side_Y = drone_data->datas_sensors_pooling.dist_side_Y;
	uint16_t forward_X = drone_data->datas_sensors_pooling.dist_forward_X;
	uint16_t low_Z = drone_data->datas_sensors_pooling.dist_low_Z;
	uint16_t high_Z = drone_data->datas_sensors_pooling.dist_high_Z;


	float roll_angle = drone_data->datas_sensors_pooling.roll_angle;
	float pitch_angle = drone_data->datas_sensors_pooling.pitch_angle;
	float yaw_angle = drone_data->datas_sensors_pooling.yaw_angle;

//	roll_angle = (int16_t) FLOAT_TO_UINT8(roll_angle);
//	pitch_angle	= (int16_t) FLOAT_TO_UINT8(pitch_angle);
//	yaw_angle	= (int16_t) FLOAT_TO_UINT8(yaw_angle);


	roll_angle = (int16_t) (roll_angle);
	pitch_angle	= (int16_t) (pitch_angle);
	yaw_angle	= (int16_t) (yaw_angle);

	//uint8_t data[2] = {data16, (data16 >> 8)}; // {lower byte, upper byte)

	uint8_t buffer_uint8_to_send[SIZE_BYTE_BUFFER_TRAME];
	uint16_t size_ocets = SIZE_BYTE_BUFFER_TRAME;

	buffer_uint8_to_send[0] = (backward_X);
	buffer_uint8_to_send[1] = (backward_X >> 8);

	buffer_uint8_to_send[2] = (side_Y);
	buffer_uint8_to_send[3] = (side_Y >> 8);

	buffer_uint8_to_send[4] = (forward_X);
	buffer_uint8_to_send[5] = (forward_X >> 8);

	buffer_uint8_to_send[6] = (low_Z);
	buffer_uint8_to_send[7] = (low_Z >> 8);

	buffer_uint8_to_send[8] = (high_Z);
	buffer_uint8_to_send[9] = (high_Z >> 8);
//_______________________________________________

//	buffer_uint8_to_send[10] = (roll_angle);
//	buffer_uint8_to_send[11] = (roll_angle >> 8);
//
//	buffer_uint8_to_send[10] = (pitch_angle);
//	buffer_uint8_to_send[11] = (pitch_angle >> 8);
//
//	buffer_uint8_to_send[10] = (yaw_angle);
//	buffer_uint8_to_send[11] = (yaw_angle >> 8);


	DIALOG_send_packet(size_ocets, buffer_uint8_to_send);




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
*/
}



