/*
 * drive_gpio.h
 *
 *  Created on: Mar 2, 2025
 *      Author: Hi
 */

#ifndef DRIVE_GPIO_H_
#define DRIVE_GPIO_H_
#include"main.h"
#include <can_par.h>
#include "mgr_com.h"

void ISR_Led_HU(CANObject object);
void LED_RUN (volatile tMsg_CAN_Rx_Data_s *Rx_Data1,volatile tMsg_CAN_Rx_Data_s *Rx_Data2);
#endif /* DRIVE_GPIO_H_ */
