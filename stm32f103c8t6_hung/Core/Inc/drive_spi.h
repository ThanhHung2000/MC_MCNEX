/*
 * drive_spi.h
 *
 *  Created on: May 5, 2025
 *      Author: Chuc
 */

#ifndef INC_DRIVE_SPI_H_
#define INC_DRIVE_SPI_H_
#include "main.h"
#define    W5500_CS_LOW()   HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, 0);
#define    W5500_CS_HIGH()  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, 1);                      // Nhả CS


void ETH_CSN_Write(uint8_t data);
uint8_t SPI_ETH_SpiIsBusBusy();
void SPI_ETH_SpiUartWriteTxData(uint8_t data);
uint8_t SPI_ETH_SpiUartGetRxBufferSize();
uint8_t SPI_ETH_SpiUartReadRxData();
uint8_t SPI_ETH_SpiUartGetTxBufferSize();
void SPI_ETH_SpiUartClearRxBuffer();
void SPI_ETH_SpiUartClearTxBuffer();
uint8_t w5500_read_version(void);
void initw5500(void);
void doc1byte(uint32_t address,uint8_t *data);
#endif /* INC_DRIVE_SPI_H_ */
