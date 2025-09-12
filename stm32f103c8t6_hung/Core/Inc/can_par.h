/*
 * can_par.h
 *
 *  Created on: Mar 1, 2025
 *      Author: Hi
 */

#ifndef CAN_PAR_H_
#define CAN_PAR_H_
#include"main.h"
#include"can_drive.h"
#include <tpmc.h>


//#define IDE_NUMBER_MAX 0x32U //50
#define CAN_NUBER_RX_OBJECT  15
#define CAN_NUMBER_TX_OBJECT 0x02U
#define IDE_NUMBER_MAX 70U
typedef struct DataLed_8_Tag
{
  uint8_t LED1 : 1;
  uint8_t LED2 : 1;
  uint8_t LED3 : 1;
  uint8_t LED4 : 1;
  uint8_t LED5 : 1;
  uint8_t LED6 : 1;
  uint8_t LED7 : 1;
  uint8_t LED8 : 1;
} DataLed_8;
typedef union DataLed
{
	uint8_t _c[1];
  DataLed_8 LED8;
} _c_cLED9;

typedef volatile struct
{
	volatile uint8_t * Data;
	uint8_t channel;
}t_rxMsgObj;

typedef t_rxMsgObj    * rxMsgObjPtr;
typedef uint8_t CANObject	;

typedef void (* CAN_Rx_Fun_Ptr)(CANObject);

typedef uint8_t (* AppPreCopy_Ptr)(rxMsgObjPtr rx_channal);


typedef unsigned char * RxDataPtr;

extern void CAN_RX_IndicationFct(CANObject object);
extern void CanRxCGW1IndicationFct(CANObject rcvObject);


extern RxDataPtr RX_DataPtr[CAN_NUBER_RX_OBJECT];
extern const uint8_t Data_Rx_Leght[CAN_NUBER_RX_OBJECT];

extern const CAN_Rx_Fun_Ptr CAN_Rx_Fun_Index[CAN_NUBER_RX_OBJECT];
extern const uint32_t CanRxId0[CAN_NUBER_RX_OBJECT];
extern const AppPreCopy_Ptr CanRxPreCoppyPtr[CAN_NUBER_RX_OBJECT];
extern const uint8_t g_descSidMap[IDE_NUMBER_MAX];
extern  _c_cLED9 LED_UV;
uint8_t TpPreCopy(rxMsgObjPtr rx_channal);
#endif /* CAN_PAR_H_ */
