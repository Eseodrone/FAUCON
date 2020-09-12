

#include "tof.h";


bool_e tof_init_OK;



void TOF_attribute_distances(void){
	p_datas_sensors_pooling->dist_0 = 0;
	p_datas_sensors_pooling->dist_1 = 0;
	p_datas_sensors_pooling->dist_2 = 0;
}

int16_t TOF_get_sensor_1_dist(void){
	return p_datas_sensors_pooling->dist_0;
}

int16_t TOF_get_sensor_2_dist(void){
	return p_datas_sensors_pooling->dist_1;
}

int16_t TOF_get_sensor_3_dist(void){
	return p_datas_sensors_pooling->dist_2;
}
