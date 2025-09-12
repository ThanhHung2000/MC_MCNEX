/*
 * drive_spi.c
 *
 *  Created on: May 3, 2025
 *      Author: R&D
 */
#include "drive_spi.h"
#include "w5500.h"



uint8_t SPI_ETH_SpiIsBusBusy()
{
	//return __HAL_SPI_GET_FLAG(&hspi2, SPI_FLAG_BSY);
	return 0;
}
void ETH_CSN_Write(uint8_t data)
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, data);
}

void SPI_ETH_SpiUartWriteTxData(uint8_t data)
{
    //HAL_SPI_Transmit(&hspi2, &data, 1, 100);
}
uint8_t SPI_ETH_SpiUartGetRxBufferSize()
{
	uint8_t rx_buf=0x00U;
    //HAL_SPI_Receive(&hspi2, &rx_buf, 1, 100);
	return rx_buf;
}
uint8_t SPI_ETH_SpiUartReadRxData()
{
	uint8_t a=0x00U;
	return a;
}
uint8_t SPI_ETH_SpiUartGetTxBufferSize()
{
	return 0;
}
void SPI_ETH_SpiUartClearRxBuffer()
{

}
void SPI_ETH_SpiUartClearTxBuffer()
{

}

uint8_t w5500_read_version(void)
{
    uint8_t version = 0;
    uint32_t add=0x00U;
    add |= (0x12U<<8);
    version=WIZCHIP_READ(add);
    return version;
}
#define MAX_DHCP_RETRY   5
#define SOCKET_DHCP 0  // Socket 0 dùng cho DHCP
#include "dhcp.h"
uint8_t DHCP_buffer[102];//1024

volatile uint8_t dhcp_ip_assigned = 0;

void dhcp_assign_callback(void)
{
    dhcp_ip_assigned = 1;
    uint8_t ip[4], gw[4], sn[4];
    getIPfromDHCP(ip);
    getGWfromDHCP(gw);
    getSNfromDHCP(sn);

    setSIPR(ip);
    setGAR(gw);
    setSUBR(sn);

   // printf("DHCP IP: %d.%d.%d.%d\r\n", ip[0], ip[1], ip[2], ip[3]);
}
void dhcp_conflict_callback(void)
{
//    printf("DHCP IP Conflict!!\r\n");
}
void set_dhcp_ip(void)
{
    DHCP_init(SOCKET_DHCP, DHCP_buffer);  // SOCKET_DHCP là socket dùng cho DHCP
    reg_dhcp_cbfunc(dhcp_assign_callback, dhcp_assign_callback, dhcp_conflict_callback);

    int retry = 0;
    while (dhcp_ip_assigned == 0 && retry < MAX_DHCP_RETRY)
    {
        if (DHCP_run() == DHCP_IP_LEASED)
        {
            break;
        }
        HAL_Delay(1000);
        retry++;
    }

    if (retry >= MAX_DHCP_RETRY)
    {
        printf("DHCP Failed. Using static IP fallback.\r\n");
        // gọi lại hàm cấu hình IP tĩnh nếu muốn
    }
}
void initw5500(void)
{
	uint8_t ip[4]   = {192, 168, 165, 67};
	uint8_t sn[4]   = {255, 255, 255, 0};
	uint8_t gw[4]   = {192, 168, 165, 254};
	uint8_t mac[6]  = {0x00, 0x08, 0xDC, 0x00, 0x00, 0x01};
	uint8_t tx_size[8] = {2,2,2,2,2,2,2,2};  // 2KB cho mỗi socket truyền
	uint8_t rx_size[8] = {2,2,2,2,2,2,2,2};  // 2KB cho mỗi socket nhận
	wizchip_init(tx_size, rx_size);  // tx_size/rx_size: {2,2,2,2,2,2,2,
	setSHAR(mac);   // Set MAC address
	setGAR(gw);     // Set Gateway address
	setSUBR(sn);    // Set Subnet mask
	setSIPR(ip);    // Set IP address

	//set_dhcp_ip();

}
void doc1byte(uint32_t address,uint8_t *data)
{
	WIZCHIP_READ_BUF(address,data,2);
}

