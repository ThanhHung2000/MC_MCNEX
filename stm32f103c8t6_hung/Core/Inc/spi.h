/*
 * spi.h
 *
 *  Created on: Aug 6, 2025
 *      Author: Admin
 */

#ifndef INC_SPI_H_
#define INC_SPI_H_

#include "main.h"
#include "stm32f1xx_hal.h"

uint8_t W5500_ReadRegister(uint32_t addr);
#define VERSIONR_ADDR 0x0039


#endif /* INC_SPI_H_ */
