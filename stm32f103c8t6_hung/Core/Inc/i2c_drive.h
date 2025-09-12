/*
 * i2c_drive.h
 *
 *  Created on: Feb 19, 2025
 *      Author: Hi
 */

#ifndef INC_I2C_DRIVE_H_
#define INC_I2C_DRIVE_H_
#include"main.h"



#define EEPROM_PAGE_SIZE 30
#define ADDD_SLAVE_24C32 0x50//68
#define ADDD_SLAVE_DS1302 0x68//68

#define DS1307_ADDRESS      (ADDD_SLAVE_DS1302)
#define DS1307_REG_SECOND     0x00
#define DS1307_REG_MINUTE     0x01
#define DS1307_REG_HOUR      0x02
#define DS1307_REG_DOW        0x03
#define DS1307_REG_DATE       0x04
#define DS1307_REG_MONTH      0x05
#define DS1307_REG_YEAR       0x06
#define DS1307_REG_CONTROL     0x07
#define DS1307_REG_UTC_HR    0x08
#define DS1307_REG_UTC_MIN    0x09
#define DS1307_REG_CENT        0x10
#define DS1307_TIMEOUT        1000

#define I2C_Ready 0
#define I2C_Busy  1
#define I2C_TIMEOUT_BUSY 50
typedef enum
{
  I2C_OK       = 0x00U,
  I2C_ERROR    = 0x01U,
  I2C_BUSY     = 0x02U,
  I2C_TIMEOUT  = 0x03U
} Status_I2C;
typedef struct {
	volatile bool mobus_control;
	volatile bool auto_control;
	volatile bool use_Automode;
	volatile bool ActiveState;
}MsgStateControlCoil;

typedef struct {

	volatile uint8_t sec;
    volatile uint8_t min;
    volatile uint8_t hour;
    volatile uint8_t dow;
    volatile uint8_t date;
    volatile uint8_t month;
    volatile uint16_t year;
    volatile uint8_t I2CBuff[8];
    volatile uint8_t update_time;
}DS1307_STRUCT;
extern DS1307_STRUCT    ds1307;
void DS1307_config();
void DS1307_settime(uint8_t sec,uint8_t min,uint8_t hour_24mode,uint8_t dayOfWeek,uint8_t date,uint8_t month, uint16_t year);
void DS1307_gettime();
void RTC_Get_Time(void);
uint8_t Master_Receive(uint8_t address,uint8_t regAddr,uint8_t* data);
void I2C_Init();

void reset_buffer(uint8_t * str,uint8_t size);
void settimer(void);
void gettime(void);
void Task_LED_UV(void);
void Realtime_UpdateWithDayOfWeek(uint8_t year, uint8_t month, uint8_t date, uint8_t day, uint8_t hour,uint8_t min, uint8_t sec);
void Mobus_write_singleCoil(uint8_t address, uint8_t value);
#endif /* INC_I2C_DRIVE_H_ */
