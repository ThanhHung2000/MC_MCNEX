/*
 * can_drive.h
 *
 *  Created on: Feb 27, 2025
 *      Author: Chuc
 */

#ifndef _INC_CAN_DRIVE_H_
#define _INC_CAN_DRIVE_H_
#include "main.h"
#include "can_par.h"
#define TRUE 0x01U
#define FALSE 0x00U

#define CAN_RECEIVE_ALL_ID

#ifndef CAN_FIFO_GERNARATE
#define CAN_FIFO_GERNARATE
#endif

#define FIFOCANLENG             0xFCU //253

#define StopUpDate              0x00
#define StartUpDate             0x01
#define FishUpDate              0x02

#define DEFAULT_MODE 		 	0x00
#define STANDARD_MODE 		 	0x01
#define SWVERSION	  		 	0x02
#define EXTENDED_MODE 		 	0x03
#define EXTENDED_SESSION 	 	0x04
#define CONTROLDTC_OFF 		 	0x05
#define COMM_CONTROL_DISABLE 	0x06
#define PROGRAMMING_SESSION  	0x07
#define REQUEST_SEED		 	0x08
#define SEND_KEY			 	0x09
#define ROUTINE_BINARY_UPDATE	0x0A
#define REQUEST_DOWNLOAD_1		0x0B
#define REQUEST_DOWNLOAD_2		0x0C
#define REQUEST_TRANSFER_EXIT	0x0D
#define COMM_CONTROL_ENABLE		0x0E
#define ECU_RESET				0x0F
#define READ_PARTNUMBER	        0x10
#define CONTROL_FLOW            0x11
#define FIRT_FARME              0x12
#define CONSECUTIVE             0x13
#define FIRT_FARME_check        0x14
#define CONSECUTIVE_check       0x15
#define COMM_CONTROL_DISABLE2	0x16


//#define CAN_INFOR_STRUCT_INDEX rxMsgObj.channel

#define CanGetRxDataPtr(i)        RX_DataPtr[i]
#define CanGetLengData(i)         Data_Rx_Leght[i]
#define CanGetRxFun_HU(i)         CAN_Rx_Fun_Index[i]
#define CanGetRxPreCopy(i)        CanRxPreCoppyPtr[i]

#define CanGetRxPreCopy(i)        CanRxPreCoppyPtr[i]
//extern t_rxMsgObj rxMsgObj;

//uint8_t TpPreCopy(rxMsgObjPtr rx_channal);
typedef union
{
	uint8_t ID[4];
	uint32_t id;
}DataId;
typedef struct
{
	DataId DataIdCan;
	uint8_t LEN;
	uint8_t MSGTYPE;
	uint8_t DATA[8];

}MsgCanTranmitTypeDef;
typedef struct
{
	volatile MsgCanTranmitTypeDef data[FIFOCANLENG];//FIFOCANLENG=150
	volatile uint8_t head;
	volatile uint8_t tail;
	volatile uint8_t leng;

}FifoCanReceiveTypeDef;
void CanInit(void);
void CAN_TxMessage(void);
void CANTxMsgMessage(const uint8_t *pData);
#ifdef CAN_FIFO_GERNARATE
uint8_t GetDataCanReceive(MsgCanTranmitTypeDef *getDataCan);
#endif
void FillterConfig(void);
void FillterConfigAll(void);
uint8_t	CAN_HL_Receive(void);
void SEND_796(uint8_t mode);
void SEND_541(uint8_t IGN_Sw);
void SEND_7DF(uint8_t mode);
void Mode_Complete(MsgCanTranmitTypeDef * DataCan);
uint8_t TGT_Write_Flash (volatile uint8_t *buf, uint16_t index, uint8_t numbytes,uint8_t Page_Num);
#endif /* INC_CAN_DRIVE_H_ */
