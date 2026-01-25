/*
 * tim_base.h
 *
 *  Created on: Oct 27, 2025
 *      Author: MCNEX
 */
#include "stm32f4xx_hal.h"
#ifndef INC_TIM_BASE_H_
#define INC_TIM_BASE_H_

void Tim_Base_Init(void);
void Set_Time(uint16_t time);
//void MX_TIM7_Init(void);
void MX_TIM10_Init(void);
void Handle_Home(void);
void Choose_glass_group(uint8_t num);

#endif /* INC_TIM_BASE_H_ */
