#include "main.h"
#include "modbus_msg_handle.h"
#include "modbus_slaver_base.h"
#include<uart_drive.h>

#define WRITE_BUS_RS485 1
#define READ_BUS_RS485 0

#define USE_SCB_RS485

#define SLAVER_ADDRESS              17U

uint8 modbusCompHavedata = 0U;



#define BUFFER_SIZE 100
unsigned char buf_send_m_comp[BUFFER_SIZE];


Packet_Modbus_Slaver packet_comp; // gói dữ liệu comport

void PacketComp_Init(void)
{
    packet_comp.id_slaver=SLAVER_ADDRESS;
    packet_comp.buf_recv=(uint8_t *)rx_data;
    packet_comp.buf_resp=buf_send_m_comp;
    packet_comp.size_buf_recv=BUFFER_SIZE;
    packet_comp.size_buf_resp=BUFFER_SIZE;
    packet_comp.OnMessageRecvHandle=SlaverSerial_MessageHandle;
    packet_comp.systemInforRefresh=SlaverSerial_Update;
}

void ModbusSlaverComp_Init(void)
{
    PacketComp_Init();
}


void ModbusSlaverComp_SendData(uint8_t * frame,uint8_t len)
{

	//HAL_UART_Transmit(&huart3,(const uint8_t*)frame,len, 100);

	HAL_USART_TxMsg((const uint8_t *)frame,len, 100);

}
void ModbusSlaverComp_Run(void) // 10ms
{
    if(modbusCompHavedata==0x01U)// có dữ liệu gửi đến
    {
        if(ModbusSlaver_DecodeMsg(&packet_comp)==SUCCESSFUL)
        {
            Slaver_Do(&packet_comp);// thực thi lệnh
            Slaver_PrepareRespond(&packet_comp);// chuẩn bị dữ liệu phản hồi
            ModbusSlaverComp_SendData(packet_comp.buf_resp,packet_comp.len_buf_resp);
        }
        modbusCompHavedata=0;//clear flag
    }
}
