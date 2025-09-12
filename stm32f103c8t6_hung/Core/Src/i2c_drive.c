/*
 * i2c_drive.c
 *
 *  Created on: Feb 19, 2025
 *      Author: Hi
 */
#include"i2c_drive.h"
#include"main.h"
#include "modbus_slaver_define.h"
#include "modbus_slaver_base.h"
#include "modbus_slaver_define.h"
#include"drive_time.h"
extern I2C_HandleTypeDef hi2c1;
#define TIME_RECOVER_HOUR    10800000U// 10800000ms tương đương delay 3h
DS1307_STRUCT data_rtc;
DS1307_STRUCT    ds1307;
uint8_t status_I2C=I2C_Ready;
void Task_FAN_QUARTER(void);
void itoacode(uint16_t value, uint8_t *buffer, int radix);
void reset_buffer(uint8_t * str,uint8_t size);
uint16_t MAX6675_ReadTemp(void);

uint8_t DS1302_DecodeBCD(uint8_t bin) {
    return (((bin & 0xf0) >> 4) * 10) + (bin & 0x0f);
}

static MsgStateControlCoil ControlCoil[ADR_REG_TOTALCOIL];
uint32_t lastModbusTime[ADR_REG_TOTALCOIL];
//Receive 2 byte in *data


uint8_t DS1307_DecodeBCD(uint8_t bin) {
    return (((bin & 0xf0) >> 4) * 10) + (bin & 0x0f);
}

uint8_t DS1307_EncodeBCD(uint8_t dec) {
    return (dec % 10 + ((dec / 10) << 4));
}
void I2C_WriteBuffer(I2C_HandleTypeDef hi, uint8_t DEV_Address, uint8_t sizebuf)
{
	while(HAL_I2C_Master_Transmit(&hi, (uint16_t)DEV_Address, (uint8_t*)&ds1307.I2CBuff,(uint16_t)sizebuf,(uint32_t)1000))
	{

	}
}
void I2C_ReadBuff(I2C_HandleTypeDef hi, uint8_t DEV_Address, uint8_t sizebuf)
{
	while(HAL_I2C_Master_Receive(&hi, (uint16_t)DEV_Address, (uint8_t*)&ds1307.I2CBuff,(uint16_t)sizebuf,(uint32_t)1000))
	{

	}
}

void DS1307_SetRegByte(uint8_t regAddr, uint8_t val) {
    uint8_t bytes[2] = { regAddr, val };
    HAL_I2C_Master_Transmit(&hi2c1, ADDD_SLAVE_DS1302 << 1, bytes, 2, DS1307_TIMEOUT);
}
uint8_t DS1307_GetRegByte(uint8_t regAddr) {
    uint8_t val;
    HAL_I2C_Master_Transmit(&hi2c1, ADDD_SLAVE_DS1302 << 1, &regAddr, 1, DS1307_TIMEOUT);
    //HAL_Delay(1);
    while(HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY);
    HAL_I2C_Master_Receive(&hi2c1, ADDD_SLAVE_DS1302 << 1, &val, 1, DS1307_TIMEOUT);
    HAL_Delay(1);
    return val;

}
void DS1307_SetClockHalt(uint8_t halt) {
    uint8_t ch = (halt ? 1 << 7 : 0);
    DS1307_SetRegByte(DS1307_REG_SECOND, ch | (DS1307_GetRegByte(DS1307_REG_SECOND) & 0x7f));
}
/**
 * @brief Sets UTC offset.
 * @note  UTC offset is not updated automatically.
 * @param hr UTC hour offset, -12 to 12.
 * @param min UTC minute offset, 0 to 59.
 */
void DS1307_SetTimeZone(int8_t hr, uint8_t min) {
    DS1307_SetRegByte(DS1307_REG_UTC_HR, hr);
    DS1307_SetRegByte(DS1307_REG_UTC_MIN, min);
}
void DS1307_config(){
    DS1307_SetClockHalt(0);
    DS1307_SetTimeZone(+8, 00);
}
void DS1307_settime(uint8_t sec,uint8_t min,uint8_t hour_24mode,uint8_t dayOfWeek,uint8_t date,uint8_t month, uint16_t year){
    DS1307_SetRegByte(DS1307_REG_SECOND, DS1307_EncodeBCD(sec ));
    DS1307_SetRegByte(DS1307_REG_MINUTE, DS1307_EncodeBCD(min));
    DS1307_SetRegByte(DS1307_REG_HOUR, DS1307_EncodeBCD(hour_24mode & 0x3f));//hour_24mode Hour in 24h format, 0 to 23.
    DS1307_SetRegByte(DS1307_REG_DOW, DS1307_EncodeBCD(dayOfWeek));//dayOfWeek Days since last Sunday, 0 to 6.
    DS1307_SetRegByte(DS1307_REG_DATE, DS1307_EncodeBCD(date));//date Day of month, 1 to 31.
    DS1307_SetRegByte(DS1307_REG_MONTH, DS1307_EncodeBCD(month));//month Month, 1 to 12.
    DS1307_SetRegByte(DS1307_REG_CENT, year / 100);
    DS1307_SetRegByte(DS1307_REG_YEAR, DS1307_EncodeBCD(year % 100));//2000 to 2099.

}
void RTC_Get_Time(void)
{
	ds1307.I2CBuff[0]=0x00U;
	I2C_WriteBuffer(hi2c1,(uint16_t)(ADDD_SLAVE_DS1302<<1),1);
	while(HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY);
	I2C_ReadBuff(hi2c1,(uint16_t)(ADDD_SLAVE_DS1302<<1),7);
	ds1307.sec=DS1307_DecodeBCD(ds1307.I2CBuff[0]);
}
void DS1307_gettime(){
    uint16_t cen;
    ds1307.sec=DS1307_DecodeBCD(DS1307_GetRegByte(DS1307_REG_SECOND) & 0x7f);

    ds1307.min=DS1307_DecodeBCD(DS1307_GetRegByte(DS1307_REG_MINUTE));
    ds1307.hour=DS1307_DecodeBCD(DS1307_GetRegByte(DS1307_REG_HOUR) & 0x3f);
    ds1307.dow=DS1307_DecodeBCD(DS1307_GetRegByte(DS1307_REG_DOW));
    ds1307.date=DS1307_DecodeBCD(DS1307_GetRegByte(DS1307_REG_DATE));
    ds1307.month=DS1307_DecodeBCD(DS1307_GetRegByte(DS1307_REG_MONTH));
    cen = DS1307_GetRegByte(DS1307_REG_CENT) * 100;
    ds1307.year=DS1307_DecodeBCD(DS1307_GetRegByte(DS1307_REG_YEAR)) + cen;


}

void I2C_Stop()
{
	I2C1->CR1 &=~(1<<0);//: : Peripheral disable
	while((I2C1->CR1&&I2C_CR1_PE)==I2C_CR1_PE);/*Peripheral enable*/
}
void I2C_Init()//I2C1  I2C1_REMAP=0 pb6:scl; pb7:sda
{
	RCC->APB1ENR |=(1<<21);//I2C 1 clock enabled
	//PB6; PB7
	RCC->APB2ENR |=(1<<3);//I/O port B clock enabled
	AFIO->MAPR &=!(1<<1);// No remap (SCL/PB6, SDA/PB7)
	GPIOB->CRL |=(3<<24)|(3<<28)|(3<<26)|(3<<30);//	Alternate function output Open-drain
												//
	//I2CCONFIG
	I2C_Stop();
	I2C1->CR2 |=(0x18<<0);//APB1 = 24MHz
	I2C1->CCR = 180;//// 100kHz = 36MHz / (2 * 120)
	I2C1->TRISE = 43; // 1000ns / (1/24MHz) + 1
	I2C1->CR1 |=I2C_CR1_PE;//: : Peripheral enable
}
void Master_Tranmit(uint8_t address,uint8_t regAddr,uint8_t data)
{
	I2C1->CR1 |= I2C_CR1_START; // Gửi tín hiệu START
	while (!(I2C1->SR1 & I2C_SR1_SB)); // Đợi START bit được set
	//sent address
	I2C1->DR = address<<1; // Gửi địa chỉ Slave
    while (!(I2C1->SR1 & I2C_SR1_ADDR)); // Đợi phản hồi từ Slave
    (void)I2C1->SR2; // Đọc SR2 để xóa cờ ADDR

    I2C1->DR = regAddr; // Gửi dia chi
    while (!(I2C1->SR1 & I2C_SR1_TXE)); // Đợi bộ đệm trống

    I2C1->DR = data; // Gửi dữ liệu
    while (!(I2C1->SR1 & I2C_SR1_TXE)); // Đợi bộ đệm trống
    I2C1->CR1 |= I2C_CR1_STOP; // Gửi tín hiệu STOP
}
uint8_t Master_Receive(uint8_t address,uint8_t regAddr,uint8_t * data)
{

	I2C1->CR1 |= I2C_CR1_START; // Gửi tín hiệu START
	while ((I2C1->SR1 & I2C_SR1_SB)!=I2C_SR1_SB); // Đợi START bit được set
	//sent address
	I2C1->DR = address<<1; // Gửi địa chỉ Slave
    //while (!(I2C1->SR1 & I2C_SR1_ADDR)); // Đợi phản hồi từ Slave
    (void)I2C1->SR2; // Đọc SR2 để xóa cờ ADDR

    I2C1->DR = regAddr; // Gửi dia chi
    //while (!(I2C1->SR1 & I2C_SR1_TXE)); // Đợi bộ đệm trống
	I2C1->CR1 |= I2C_CR1_START; // repeat START
	//while (!(I2C1->SR1 & I2C_SR1_SB)); // Đợi START bit được set
	HAL_Delay(1);
	//sent address
	I2C1->DR = ((address<<1)|1); // Gửi địa chỉ Slave read
    //while (!(I2C1->SR1 & I2C_SR1_ADDR)); // Đợi phản hồi từ Slave
	HAL_Delay(1);
    (void)I2C1->SR2; // Đọc SR2 để xóa cờ ADDR

    I2C1->CR1 &= ~I2C_CR1_ACK; // Không gửi ACK
    I2C1->CR1 |= I2C_CR1_STOP; // Gửi tín hiệu STOP
    //while (!(I2C1->SR1 & I2C_SR1_RXNE)); // Đợi dữ liệu
	HAL_Delay(1);
    *data = I2C1->DR; // Đọc dữ liệu từ thanh ghi DR
    return 0;
}


void reset_buffer(uint8_t * str,uint8_t size)
{
	for(int i=0;i<size;i++)
	{
		*(str+i)='\0';
	}
}
void settimer(void)
{
	uint8_t i=0x00U;
	uint8_t timer_set[7];
	timer_set[0]=DS1307_EncodeBCD(30);
	timer_set[1]=DS1307_EncodeBCD(59);
	timer_set[2]=DS1307_EncodeBCD(8);
	timer_set[3]=DS1307_EncodeBCD(4);
	timer_set[4]=DS1307_EncodeBCD(26);
	timer_set[5]=DS1307_EncodeBCD(2);
	timer_set[6]=DS1307_EncodeBCD(25);
	HAL_I2C_Mem_Write(&hi2c1, ADDD_SLAVE_DS1302 << 1,0x00,1, timer_set, sizeof(timer_set), DS1307_TIMEOUT);
	data_rtc.update_time=0x00U;
	for(i=0x00U; i<ADR_REG_TOTALCOIL ; i++)
	{
		ControlCoil[i].use_Automode=True;
		ControlCoil[i].ActiveState =False;
		ControlCoil[i].auto_control=False;
		ControlCoil[i].mobus_control=False;
		lastModbusTime[i]=0x00U;
	}
}
void gettime(void)
{
	uint8_t get_time[7];
	uint16_t datatime;
	if(data_rtc.update_time==0x01U)
	{
		get_time[0]=DS1307_EncodeBCD(data_rtc.sec);
		get_time[1]=DS1307_EncodeBCD(data_rtc.min);
		get_time[2]=DS1307_EncodeBCD(data_rtc.hour);
		get_time[3]=DS1307_EncodeBCD(data_rtc.dow);
		get_time[4]=DS1307_EncodeBCD(data_rtc.date);
		get_time[5]=DS1307_EncodeBCD(data_rtc.month);
		get_time[6]=DS1307_EncodeBCD(data_rtc.year);
		HAL_I2C_Mem_Write(&hi2c1, ADDD_SLAVE_DS1302 << 1,0x00,1, get_time, sizeof(get_time), DS1307_TIMEOUT);
		data_rtc.update_time=0x00U;

	}
	else
	{
		HAL_I2C_Mem_Read(&hi2c1, ADDD_SLAVE_DS1302 << 1,0x00,1, get_time, sizeof(get_time), DS1307_TIMEOUT);
		data_rtc.sec=DS1307_DecodeBCD(get_time[0]& 0x7f);
		data_rtc.min=DS1307_DecodeBCD(get_time[1]);
		data_rtc.hour=DS1307_DecodeBCD(get_time[2]& 0x3f);
		data_rtc.dow=DS1307_DecodeBCD(get_time[3]);
		data_rtc.date=DS1307_DecodeBCD(get_time[4]);
		data_rtc.month=DS1307_DecodeBCD(get_time[5]);
		data_rtc.year=DS1307_DecodeBCD(get_time[6])+2000;

	}
	// copy data real time data from data_rtc to holding register
	datatime=(data_rtc.year<<8)|(uint16_t)(data_rtc.month);
	*(MobusDataPtr[HOLDING_register] + ADR_REG_HOLDING_UPDATE_REALTIME ) =datatime;
	datatime=(uint16_t)(data_rtc.date<<8)|(uint16_t)(data_rtc.dow);
	*(MobusDataPtr[HOLDING_register] + ADR_REG_HOLDING_UPDATE_REALTIME +1) =datatime;
	datatime=(uint16_t)(data_rtc.hour<<8)|(uint16_t)(data_rtc.min);
	*(MobusDataPtr[HOLDING_register] + ADR_REG_HOLDING_UPDATE_REALTIME +2) =datatime;
	datatime=(uint16_t)(data_rtc.sec<<8)|(uint16_t)(0);
	*(MobusDataPtr[HOLDING_register] + ADR_REG_HOLDING_UPDATE_REALTIME +3) =datatime;
}
void Task_LED_UV(void)
{
	uint32_t time_now=millis();
	uint8_t i=0x00U;
	for( i=0;i<ADR_REG_TOTALCOIL;i++)
	{
		if( time_now>=(TIME_RECOVER_HOUR+lastModbusTime[i]))
		{
			ControlCoil[i].use_Automode=True;
		}
	}
	if(data_rtc.hour>=9U && data_rtc.hour<11U)//LED UV1 enable
	{

		ControlCoil[ADR_REG_COIL_0].auto_control=True;
		ControlCoil[ADR_REG_COIL_1].auto_control=False;
		ControlCoil[ADR_REG_COIL_2].auto_control=False;
	}
	else if(data_rtc.hour>=11U &&  data_rtc.hour<13U)
	{
		ControlCoil[ADR_REG_COIL_0].auto_control=False;
		ControlCoil[ADR_REG_COIL_1].auto_control=True;
		ControlCoil[ADR_REG_COIL_2].auto_control=False;
	}
	else if(data_rtc.hour>=13U && data_rtc.hour<15U)
	{
		ControlCoil[ADR_REG_COIL_0].auto_control=False;
		ControlCoil[ADR_REG_COIL_1].auto_control=False;
		ControlCoil[ADR_REG_COIL_2].auto_control=True;
	}
	else
	{
		ControlCoil[ADR_REG_COIL_0].auto_control=False;
		ControlCoil[ADR_REG_COIL_1].auto_control=False;
		ControlCoil[ADR_REG_COIL_2].auto_control=False;
	}
	Task_FAN_QUARTER();

}
void Task_FAN_QUARTER(void)
{
	uint8_t i=0x00U;
	if(data_rtc.hour>=3U && data_rtc.hour<=6U)
	{
		ControlCoil[ADR_REG_COIL_3].auto_control=True;
	}
	else if(data_rtc.hour>18U && data_rtc.hour<=20U)
	{
		ControlCoil[ADR_REG_COIL_3].auto_control=True;
	}
	else if(data_rtc.hour>= 11U && data_rtc.hour<13U)
	{
		ControlCoil[ADR_REG_COIL_3].auto_control=True;
	}
	else
	{
		ControlCoil[ADR_REG_COIL_3].auto_control=False;
	}
	for(i=0x00U;i<ADR_REG_TOTALCOIL;i++)
	{
		ControlCoil[i].ActiveState= ControlCoil[i].use_Automode?ControlCoil[i].auto_control:ControlCoil[i].mobus_control;

		*(MobusDataCoilPtr[0] +i)=ControlCoil[i].ActiveState;

		HAL_GPIO_WritePin(GPIOA, 1<<(i+3),ControlCoil[i].ActiveState );
	}

}
//11 10 00 64 00 04 08 19 05 07 04 0b 00 30 00 a5 0c: năm, tháng, ngày, thứ, giờ phút, giây , null.
void Realtime_UpdateWithDayOfWeek(uint8_t year, uint8_t month, uint8_t date, uint8_t dow, uint8_t hour,uint8_t min, uint8_t sec)
{
	if((data_rtc.hour != hour) || (data_rtc.min !=min))
	{
		data_rtc.update_time=0x01U;
		data_rtc.year  =(uint16_t)year;
		data_rtc.month =month;
		data_rtc.date =date;
		data_rtc.dow =dow;
		data_rtc.hour=hour;
		data_rtc.min =min;
		data_rtc.sec =sec;
	}
}

void Mobus_write_singleCoil(uint8_t address, uint8_t value)//11  05 00 02 ff 00 2f 6a :write coil
{
	ControlCoil[address].use_Automode	= False;
	ControlCoil[address].mobus_control = (value>0)?True:False;
	lastModbusTime[address] = millis();

}
/*  SCK_GPIO_Port : pa11
 *  SO_GPIO_Port   :pa10
 *
 *
 */
uint16_t MAX6675_ReadTemp(void)
{
    uint16_t data = 0;

    for (int i = 15; i >= 0; i--)
    {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET);;
        HAL_Delay(1);  // delay 1 ms
        data <<= 1;
        if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10)) data |= 0x01;

        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET);
        HAL_Delay(1);  // delay 1 ms
    }

    if (data & 0x04) {
        // lỗi: chưa kết nối cảm biến
        return 0xFFFF;
    }

    return ((data >> 3) & 0x0FFF);  // mỗi đơn vị = 0.25°C
}
