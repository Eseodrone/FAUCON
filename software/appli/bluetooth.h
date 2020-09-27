/*
 * bluetooth.h
 *
 *  Created on: 22 mars 2020
 *      Author: martin
 */

#ifndef APPLI_BLUETOOTH_H_
#define APPLI_BLUETOOTH_H_

#include "stm32f4xx_hal.h"
#include "macro_types.h"
#include "stm32f4_uart.h"
#include "stm32f4_sys.h"


void BLUETOOTH_init(void);
void BLUETOOTH_envoi_trame(uint8_t info1, uint8_t info2, uint8_t info3, uint8_t info4);
void BLUETOOTH_envoi_caractere(uint8_t c);
char BLUETOOTH_reception_caractere(void);
void BLUETOOTH_reception_trame(void);

#endif /* APPLI_BLUETOOTH_H_ */
