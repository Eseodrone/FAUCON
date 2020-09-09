//
#include "stm32f4xx_hal.h"

typedef struct
{
	float sensor_1;
	float sensor_2;
	float sensor_3;
}tof_distances_t;

static tof_distances_t tof_distances;

void TOF_attribute_distances(void)
{
	tof_distances.sensor_1 = 0;//A remplacer Par VLX_get_distance(...)
	tof_distances.sensor_2 = 0;
	tof_distances.sensor_3 = 0;
}

int16_t TOF_get_sensor_1_dist(void)
{
	return (int)tof_distances.sensor_1;
}

int16_t TOF_get_sensor_2_dist(void)
{
	return (int)tof_distances.sensor_2;
}

int16_t TOF_get_sensor_3_dist(void)
{
	return (int)tof_distances.sensor_3;
}
