#ifndef TOF_H
#define TOF_H

#include "stm32f4xx_hal.h"
#include "datas_process.h"


extern bool_e tof_init_OK;


void TOF_attribute_distances(void);
int16_t TOF_get_sensor_1_dist(void);
int16_t TOF_get_sensor_2_dist(void);
int16_t TOF_get_sensor_3_dist(void);






#endif /*TOF_H*/
