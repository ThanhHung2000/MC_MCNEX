/*
 * Common.h
 *
 *  Created on: Nov 10, 2025
 *      Author: MCNEX
 */
#include <stdint.h>

#ifndef INC_MY_LIB_COMMON_H_
#define INC_MY_LIB_COMMON_H_

uint32_t ReadBit(uint8_t* number, uint16_t startbit, uint8_t len);
void SetBit(uint8_t* number, uint16_t startbit, uint8_t len);
void ClearBit(uint8_t* number, uint16_t startbit, uint8_t len);

#endif /* INC_MY_LIB_COMMON_H_ */
