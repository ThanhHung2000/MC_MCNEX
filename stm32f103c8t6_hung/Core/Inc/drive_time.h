/*
 * drive_time.h
 *
 *  Created on: Mar 2, 2025
 *      Author: Hi
 */

#ifndef DRIVE_TIME_H_
#define DRIVE_TIME_H_
#include"main.h"
#include "drive_spi.h"

extern SPI_HandleTypeDef hspi2;
#define CY_SCB_SPI_ETH_H

#define TID_TIMER_1ms        0
#define TID_COUNT_10ms       1
#define TID_TIMER_1000ms      2
#define TID_TIMER_UPDT       3
#define TOTAL_TIMER_DELAY    4

#define TIMER_UPDATE 0
#define TOTAL_TIMER 3
#define HI8(offset)          (uint8_t)(offset>>8)
#define LO8(offset)          (uint8_t)(offset)
#define CYSWAP_ENDIAN32(x)  \
        ((uint32_t)((((x) >> 24) & 0x000000FFu) | (((x) & 0x00FF0000u) >> 8) | (((x) & 0x0000FF00u) << 8) | ((x) << 24)))

/* Swap the byte ordering of 16 bit value */
#define CYSWAP_ENDIAN16(x)      ((uint16_t)(((x) << 8) | (((x) >> 8) & 0x00FFu)))

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
    /* ARM naturally returns 32 bit value. */
typedef uint32_t cystatus;

#define CyDelay(ms) HAL_Delay(ms)
typedef enum
{
	False =0x00U,
	True =0x01U
}bool;




#define CYRET_SUCCESS           (0x00u)           /* Successful */
#define CYRET_BAD_PARAM         (0x01u)           /* One or more invalid parameters */
#define CYRET_INVALID_OBJECT    (0x02u)           /* Invalid object specified */
#define CYRET_MEMORY            (0x03u)           /* Memory related failure */
#define CYRET_LOCKED            (0x04u)           /* Resource lock failure */
#define CYRET_EMPTY             (0x05u)           /* No more objects available */
#define CYRET_BAD_DATA          (0x06u)           /* Bad data received (CRC or other error check) */
#define CYRET_STARTED           (0x07u)           /* Operation started, but not necessarily completed yet */
#define CYRET_FINISHED          (0x08u)           /* Operation completed */
#define CYRET_CANCELED          (0x09u)           /* Operation canceled */
#define CYRET_TIMEOUT           (0x10u)           /* Operation timed out */
#define CYRET_INVALID_STATE     (0x11u)           /* Operation not setup or is in an improper state */
#define CYRET_UNKNOWN           ((cystatus) 0xFFFFFFFFu)    /* Unknown failure */



typedef struct
{
	uint32_t Time_Cur;
	uint8_t active;
	uint32_t Time_Delay;
	uint32_t End_Time;

}Msg_Timer_Delay;

void Delay_SetTimer(uint8_t id,uint32_t timer);
void reset_timer_one_channel(uint8_t id);
uint8_t Delay_GetTimer(uint8_t id);
void reset_timer(void);
uint8_t check_timedelay(uint8_t id, uint32_t timedelay);
void reset_delaytimer(uint8_t id);
extern Msg_Timer_Delay TID_Timer[TOTAL_TIMER_DELAY];
extern volatile uint8_t update_status;
extern uint32_t millis(void);
extern uint8_t IsTickChange(void);//100ms
extern uint8_t IsSecondChange();
void Timer2_Init(void);
void Timer3_Init(void);
#endif /* DRIVE_TIME_H_ */
