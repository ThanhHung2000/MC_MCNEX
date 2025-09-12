/*
 * spi.c
 *
 *  Created on: Aug 6, 2025
 *      Author: Admin
 */




#include "spi.h"
// Khai báo chân CS và RST
#define W5500_CS_PIN       GPIO_PIN_8
#define W5500_CS_GPIO_PORT GPIOA
#define W5500_RST_PIN      GPIO_PIN_12
#define W5500_RST_GPIO_PORT GPIOA

extern SPI_HandleTypeDef hspi2;

// Địa chỉ thanh ghi phiên bản của W5500



// Hàm đọc 1 byte từ thanh ghi của W5500
uint8_t W5500_ReadRegister(uint32_t addr) {
    uint8_t rx_data, tx_data[3];

    HAL_GPIO_WritePin(W5500_CS_GPIO_PORT, W5500_CS_PIN, GPIO_PIN_RESET); // Kéo CS xuống LOW
    tx_data[0] = (addr & 0x00FF0000) >> 16;
    tx_data[1] = (addr & 0x0000FF00) >> 8;
    tx_data[2] = (addr & 0x000000FF) >> 0;

                          // Dữ liệu rỗng để nhận về

    HAL_SPI_Transmit(&hspi2, tx_data, 3, 100);
    HAL_SPI_Receive(&hspi2, &rx_data, 1, 100);

    HAL_GPIO_WritePin(W5500_CS_GPIO_PORT, W5500_CS_PIN, GPIO_PIN_SET); // Kéo CS lên HIGH

    return rx_data;
}
