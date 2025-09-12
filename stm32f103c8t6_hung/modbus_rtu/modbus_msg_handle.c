#include "main.h"

#include"drive_time.h"
#include "modbus_slaver_base.h"
#include "modbus_slaver_define.h"
#include"i2c_drive.h"
#include "string.h"


#define TIME_OUT_CONNECTED 60000 // thời gian chờ mất kết nối 20s

static int32_t lastTimeRecvMsgModbus;
static uint8_t isModbusConnected;
static void SlaverSerial_MessageReadInputHandle(Packet_Modbus_Slaver * msg);
static void SlaverSerial_MessageReadHoldingHandle(Packet_Modbus_Slaver * msg);
static void SlaveSerial_WriteSigleCoil(Packet_Modbus_Slaver * msg);
static void SlaverSerial_MessagePresetMutilHandle(Packet_Modbus_Slaver * msg);

static void MsgHandle_Active(void)
{
    isModbusConnected=1;
}

static void SlaverSerial_MessageReadHoldingHandle(Packet_Modbus_Slaver * msg)
{

}

static void SlaveSerial_WriteSigleCoil(Packet_Modbus_Slaver * msg)
{
	uint8_t cmd=(uint8_t)(msg->address);
    uint8_t * ptr= (MobusDataCoilPtr[0] + cmd);
    if(cmd<ADR_REG_TOTALCOIL)
    {
    	Mobus_write_singleCoil(cmd,ptr[0]);
    }
}
static void SlaverSerial_MessageReadInputHandle(Packet_Modbus_Slaver * msg)
{
    uint16_t cmd=msg->address;
    uint8_t * ptr=  (uint8_t *)(MobusDataPtr[INPUT_register] + msg->address);

    if(cmd==ADR_REG_INPUT_READ_GENERATOR)
    {
    	ptr[0]=ptr[0]; // avoid warning
    }
}

static void SlaverSerial_MessagePresetMutilHandle(Packet_Modbus_Slaver * msg)
{
    uint16_t cmd=msg->address;
    uint16_t * ptr=(MobusDataPtr[HOLDING_register] + msg->address );
    uint8_t year =(uint8_t)(ptr[0]>>8);
    uint8_t month =(uint8_t)(ptr[0]&0xffU);
    uint8_t date =(uint8_t)(ptr[1]>>8);
    uint8_t dow = (uint8_t)(ptr[1]&0xffU);
    uint8_t hour =(uint8_t)(ptr[2]>>8);
    uint8_t min = (uint8_t)(ptr[2]&0xffU);
    uint8_t giay =(uint8_t)(ptr[3]>>8);

    if(cmd==ADR_REG_HOLDING_UPDATE_REALTIME)
    {
        Realtime_UpdateWithDayOfWeek( year,month,date,dow,hour,min,giay);
    }

}

void SlaverSerial_MessageHandle(Packet_Modbus_Slaver * msg)
{
    MsgHandle_Active();
    // xử lí lệnh ghi xuống từ máy tính
    switch(msg->funtion)
    {
        case READ_HOLDING_REGISTERS://0x03 The master request one or more register from holding register
        {
            SlaverSerial_MessageReadHoldingHandle(msg);
        }
        break;
        case FORCE_SINGLE_COIL:    //0x05 write one coin
        {
        	SlaveSerial_WriteSigleCoil(msg);
        }
        break;
        case PRESET_SINGLE_REGISTER://0x06 The masters writes a value to one holding register.
        {

        }
        break;
        case READ_INPUT_REGISTERS:
        {
            SlaverSerial_MessageReadInputHandle(msg);
        }
        break;
        case PRESET_MULTIPLE_REGISTERS://0x10 : The master writes values multiple holding register at once.
        {
            SlaverSerial_MessagePresetMutilHandle(msg);

        }
        break;
    }
}
void SlaverSerial_Update(Packet_Modbus_Slaver * msg)
{
    switch(msg->funtion)
    {
        case READ_INPUT_REGISTERS:
        {

        }
        break;
        case READ_HOLDING_REGISTERS:
        {

        }
        break;
    }
}

uint8_t IsModbusConnect(void)
{
    return isModbusConnected;
}
void MsgHandle_Monitor(void)
{
    if(isModbusConnected)
    {
        if(millis()-lastTimeRecvMsgModbus>=TIME_OUT_CONNECTED)
        {
            isModbusConnected=0;
            lastTimeRecvMsgModbus=millis();
        }
    }
}

