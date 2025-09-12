/*
 * can_drive.c
 *
 *  Created on: Feb 27, 2025
 *      Author: Chuc
 */
#include "main.h"
#include <can_drive.h>
#include "stm32f1xx_hal_can.h"
#include <can_par.h>
#include "mgr_com.h"
#define  NUMBERLENGID  0x5DCU // from 0x260->0x7df :1407 byte
#define  IDNUMBERfirst CanRxId0[0]
CAN_TxHeaderTypeDef TxHeader;

t_rxMsgObj rxMsgObj;
uint8_t KeyData[4] = {0x00,};
volatile CAN_RxHeaderTypeDef  rxHeader;
volatile uint8_t datarx[8];
volatile uint32_t cout_CAN_ir=0x0000U;
volatile uint8_t Transfer_Mode;

volatile uint8_t CAN_Rx_Complete_Flag = 0x00U;

uint32_t TxMailbox;
unsigned char DataCanTx[8]		= {0x00U,};
extern CAN_HandleTypeDef hcan;
extern uint8_t download_type;

uint8_t object_message[NUMBERLENGID]={0xFFU,};

CAN_FilterTypeDef canfilterconfig;
CAN_TxHeaderTypeDef txHeader;

MsgCanTranmitTypeDef *CanReceiveUart;
MsgCanTranmitTypeDef dataCanReceivefromUART;
MsgCanTranmitTypeDef dataCanReceivefromCAN;
FifoCanReceiveTypeDef dataFifoReceivefromCAN;

void CanInit(void){

	dataFifoReceivefromCAN.head=0x00U;
	dataFifoReceivefromCAN.tail=0x00U;
	dataFifoReceivefromCAN.leng=0x00U;
	HAL_CAN_Start(&hcan);
	for(uint16_t i=0;i<NUMBERLENGID;i++)
	{
		object_message[i]=0xFFU;
	}
    uint32_t num_filters = sizeof(CanRxId0) / sizeof(CanRxId0[0]);

	for (uint32_t i = 0; i < num_filters; i++)
	{
		object_message[CanRxId0[i]-CanRxId0[0]]=i;
	}
}
void FillterConfig(void){
    // Các ID cần lọc
    uint32_t num_filters = sizeof(CanRxId0) / sizeof(CanRxId0[0]);
	// Cấu hình bộ lọc cho từng ID
	for (uint32_t i = 0; i < num_filters; i++) {
		canfilterconfig.FilterBank = i;  // Mỗi bộ lọc có FilterBank riêng
		canfilterconfig.FilterMode = CAN_FILTERMODE_IDMASK;  // Sử dụng chế độ ID Mask
		canfilterconfig.SlaveStartFilterBank = 0;

		canfilterconfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;  // Gán bộ lọc vào FIFO0
		// Dịch ID sang trái 5 bit để tương thích với bộ lọc 32 bit
		canfilterconfig.FilterIdHigh = (CanRxId0[i] << 5);
		canfilterconfig.FilterIdLow = 0x0000U;

		// Mặt nạ lọc là 0x7FF (chỉ nhận ID chính xác)
		canfilterconfig.FilterMaskIdHigh = (0x7FF << 5);
		canfilterconfig.FilterMaskIdLow = 0x0000U;

		canfilterconfig.FilterScale = CAN_FILTERSCALE_32BIT;  // Sử dụng bộ lọc 32 bit
		canfilterconfig.FilterActivation = ENABLE;  // Kích hoạt bộ lọc

		// Cấu hình bộ lọc cho CAN
		HAL_CAN_ConfigFilter(&hcan, &canfilterconfig);
	}
}
void FillterConfigAll(void)
{
	// Cấu hình bộ lọc CAN để nhận tất cả ID
	canfilterconfig.FilterBank = 0;  // Sử dụng FilterBank đầu tiên
	canfilterconfig.FilterMode = CAN_FILTERMODE_IDMASK;  // Chế độ ID Mask
	canfilterconfig.FilterScale = CAN_FILTERSCALE_32BIT;  // Dùng bộ lọc 32-bit
	canfilterconfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;  // Nhận dữ liệu vào FIFO0

	// Nhận tất cả ID -> Đặt ID lọc về 0x0000 và mặt nạ lọc về 0x0000
	canfilterconfig.FilterIdHigh = 0x0000;
	canfilterconfig.FilterIdLow = 0x0000;
	canfilterconfig.FilterMaskIdHigh = 0x0000;  // Mặt nạ 0x0000 -> nhận tất cả ID
	canfilterconfig.FilterMaskIdLow = 0x0000;

	canfilterconfig.FilterActivation = ENABLE;  // Kích hoạt bộ lọc

	// Cấu hình bộ lọc vào CAN
	HAL_CAN_ConfigFilter(&hcan, &canfilterconfig);

}
void CAN_TxMessage(void)
{
	TxHeader.IDE = CAN_ID_STD;
	TxHeader.StdId = 0x2DF;
	TxHeader.RTR = CAN_RTR_DATA;
	TxHeader.DLC = 8;
	TxHeader.TransmitGlobalTime = DISABLE;
	HAL_CAN_AddTxMessage(&hcan, &TxHeader, DataCanTx, &TxMailbox);
}
void CANTxMsgMessage(const uint8_t *pData)
{

	TxHeader.IDE = (uint32_t)pData[5];
	if(TxHeader.IDE==0x04U)
	{
		TxHeader.ExtId  = (uint32_t)((pData[0]<<24))|(pData[1]<<16)|(pData[2]<<8)|pData[3];
		TxHeader.RTR = CAN_RTR_DATA;
		TxHeader.DLC = pData[4];
		TxHeader.TransmitGlobalTime = DISABLE;
		HAL_CAN_AddTxMessage(&hcan, &TxHeader, pData+6, &TxMailbox);
	}
	else if(TxHeader.IDE==0x00U)
	{
		TxHeader.StdId = (uint32_t)(pData[2]<<8)|pData[3];
		TxHeader.RTR = CAN_RTR_DATA;
		TxHeader.DLC = pData[4];
		TxHeader.TransmitGlobalTime = DISABLE;
		HAL_CAN_AddTxMessage(&hcan, &TxHeader, pData+6, &TxMailbox);
	}


}
void USB_LP_CAN1_RX0_IRQHandler(void)
{

  /* USER CODE BEGIN USB_LP_CAN1_RX0_IRQn 0 */
  /* USER CODE END USB_LP_CAN1_RX0_IRQn 0 */
	HAL_CAN_GetRxMessage(&hcan, CAN_RX_FIFO0,&rxHeader,(uint8_t *)datarx);
    #ifndef CAN_RECEIVE_ALL_ID
	{
		{
			rxMsgObj.channel=object_message[rxHeader.StdId-IDNUMBERfirst];//IDNUMBERfirst
			rxMsgObj.Data=datarx;

				if(CAN_HL_Receive()==TRUE)
				{
					// routine tao flag ngan bang cach goi ham xu ly bang con tro ham.
					if(CanGetRxFun_HU(rxMsgObj.channel)!=NULL)
					{
						CanGetRxFun_HU(rxMsgObj.channel)(rxMsgObj.channel);
					}

				}

		}
	}
	#else
	{
//		dataCanReceivefromCAN.ID[0]=(uint8_t)(rxHeader.StdId>>24);
//		dataCanReceivefromCAN.ID[1]=(uint8_t)(rxHeader.StdId>>16);
//		dataCanReceivefromCAN.ID[2]=(uint8_t)(rxHeader.StdId>>8);
//		dataCanReceivefromCAN.ID[0]=(uint8_t)(rxHeader.StdId);
		dataCanReceivefromCAN.DataIdCan.id=rxHeader.StdId;

		dataCanReceivefromCAN.LEN=(uint8_t)(rxHeader.DLC);
		dataCanReceivefromCAN.MSGTYPE=(uint8_t)(rxHeader.IDE);

		for(uint8_t i=0;i<rxHeader.DLC;i++)
		{
			dataCanReceivefromCAN.DATA[i]=datarx[i];
		}
		#ifdef CAN_FIFO_GERNARATE
			if(++dataFifoReceivefromCAN.leng<=FIFOCANLENG)
			{
				dataFifoReceivefromCAN.data[dataFifoReceivefromCAN.head]=dataCanReceivefromCAN;//coppy data
				dataFifoReceivefromCAN.head =(dataFifoReceivefromCAN.head+1)%FIFOCANLENG;
			}
			cout_CAN_ir++;
        #endif

	}
#endif

  /* USER CODE END USB_LP_CAN1_RX0_IRQn 1 */
}
#ifdef CAN_FIFO_GERNARATE
uint8_t GetDataCanReceive(MsgCanTranmitTypeDef *getDataCan)
{
	if(dataFifoReceivefromCAN.leng==0x00U)
	{
		return 0x00U;
	}
	else
	{
		*getDataCan=dataFifoReceivefromCAN.data[dataFifoReceivefromCAN.tail];
		dataFifoReceivefromCAN.tail=(dataFifoReceivefromCAN.tail+1)%FIFOCANLENG;
		if(dataFifoReceivefromCAN.leng>0x00U)
		{
			dataFifoReceivefromCAN.leng--;
		}

	}
	return 0x01U;
}
#endif
uint8_t	CAN_HL_Receive(void)
{
	//copy data tu RAM receive buffer to RAM Buffer
	volatile uint8_t * Rxdataptr;
	volatile uint8_t * Txdadaptr;
	uint8_t coppycount;
	uint8_t ret=FALSE;
	if(CanGetRxPreCopy(rxMsgObj.channel) !=NULL)//precopy 0x16 // dung con tro ham co truyen du lieu CanRxInforStruct
    {
		CanGetRxPreCopy(rxMsgObj.channel)(&rxMsgObj);
		//RETURN 0
	}
	if(CanGetRxDataPtr(rxMsgObj.channel)!=NULL)
	{
		Rxdataptr= CanGetRxDataPtr(rxMsgObj.channel);
		Txdadaptr=rxMsgObj.Data;
		for(coppycount=0;coppycount<CanGetLengData(rxMsgObj.channel);coppycount++)
		{
			*Rxdataptr=*Txdadaptr;
			Rxdataptr++;
			Txdadaptr++;
		}
		ret=TRUE;
	}
	return ret;
}
void SEND_7DF(uint8_t mode)
{
       // All CAN register are on page 0x0C
//	CAN_Rx_Complete_Flag = 0x00U;
	switch(mode)
	{
		case STANDARD_MODE:
			DataCanTx[0] = 0x02;          // on message object used
			DataCanTx[1] = 0x10;              // Initialize data registers based
			DataCanTx[2] = 0x81;
			DataCanTx[3] = 0x55;
			DataCanTx[4] = 0x55;
			DataCanTx[5] = 0x55;
			DataCanTx[6] = 0x55;
			DataCanTx[7] = 0x55;

			TxHeader.IDE = CAN_ID_STD;
			TxHeader.StdId = 0X7DF;
			TxHeader.RTR = CAN_RTR_DATA;
			TxHeader.DLC = 8;
			TxHeader.TransmitGlobalTime = DISABLE;
			HAL_CAN_AddTxMessage(&hcan, &TxHeader, DataCanTx, &TxMailbox);
		break;
		case EXTENDED_MODE:

			DataCanTx[0] = 0x02;          // on message object used
			DataCanTx[1] = 0x10;              // Initialize data registers based
			DataCanTx[2] = 0x90;
			DataCanTx[3] = 0x55;
			DataCanTx[4] = 0x55;
			DataCanTx[5] = 0x55;
			DataCanTx[6] = 0x55;
			DataCanTx[7] = 0x55;

			TxHeader.IDE = CAN_ID_STD;
			TxHeader.StdId = 0X7DF;
			TxHeader.RTR = CAN_RTR_DATA;
			TxHeader.DLC = 8;
			TxHeader.TransmitGlobalTime = DISABLE;
			HAL_CAN_AddTxMessage(&hcan, &TxHeader, DataCanTx, &TxMailbox);
		break;
		case CONTROLDTC_OFF:
			DataCanTx[0] = 0x02;          // on message object used
			DataCanTx[1] = 0x85;              // Initialize data registers based
			DataCanTx[2] = 0x82;
			DataCanTx[3] = 0x55;
			DataCanTx[4] = 0x55;
			DataCanTx[5] = 0x55;
			DataCanTx[6] = 0x55;
			DataCanTx[7] = 0x55;

			TxHeader.IDE = CAN_ID_STD;
			TxHeader.StdId = 0X7DF;
			TxHeader.RTR = CAN_RTR_DATA;
			TxHeader.DLC = 8;
			TxHeader.TransmitGlobalTime = DISABLE;
			HAL_CAN_AddTxMessage(&hcan, &TxHeader, DataCanTx, &TxMailbox);
		break;
		case COMM_CONTROL_DISABLE:
			DataCanTx[0] = 0x02;          // on message object used
			DataCanTx[1] = 0x28;              // Initialize data registers based
			DataCanTx[2] = 0x02;
			DataCanTx[3] = 0x55;
			DataCanTx[4] = 0x55;
			DataCanTx[5] = 0x55;
			DataCanTx[6] = 0x55;
			DataCanTx[7] = 0x55;

			TxHeader.IDE = CAN_ID_STD;
			TxHeader.StdId = 0X7DF;
			TxHeader.RTR = CAN_RTR_DATA;
			TxHeader.DLC = 8;
			TxHeader.TransmitGlobalTime = DISABLE;
			HAL_CAN_AddTxMessage(&hcan, &TxHeader, DataCanTx, &TxMailbox);
		break;
		case COMM_CONTROL_ENABLE:
			DataCanTx[0] = 0x02;          // on message object used
			DataCanTx[1] = 0x29;              // Initialize data registers based
			DataCanTx[2] = 0x02;
			DataCanTx[3] = 0x55;
			DataCanTx[4] = 0x55;
			DataCanTx[5] = 0x55;
			DataCanTx[6] = 0x55;
			DataCanTx[7] = 0x55;

			TxHeader.IDE = CAN_ID_STD;
			TxHeader.StdId = 0X7DF;
			TxHeader.RTR = CAN_RTR_DATA;
			TxHeader.DLC = 8;
			TxHeader.TransmitGlobalTime = DISABLE;
			HAL_CAN_AddTxMessage(&hcan, &TxHeader, DataCanTx, &TxMailbox);
			break;
		case COMM_CONTROL_DISABLE2:
			DataCanTx[0] = 0x02;          // on message object used
			DataCanTx[1] = 0x3E;              // Initialize data registers based
			DataCanTx[2] = 0x80;
			DataCanTx[3] = 0x55;
			DataCanTx[4] = 0x55;
			DataCanTx[5] = 0x55;
			DataCanTx[6] = 0x55;
			DataCanTx[7] = 0x55;

			TxHeader.IDE = CAN_ID_STD;
			TxHeader.StdId = 0X7DF;
			TxHeader.RTR = CAN_RTR_DATA;
			TxHeader.DLC = 8;
			TxHeader.TransmitGlobalTime = DISABLE;
			HAL_CAN_AddTxMessage(&hcan, &TxHeader, DataCanTx, &TxMailbox);
		break;
		default:
		break;
	}
}
void SEND_796(uint8_t mode)
{
//	CAN_Rx_Complete_Flag = 0x00U;
	switch(mode)
	{
		case SWVERSION:
			DataCanTx[0] = 0x03;          // on message object used
			DataCanTx[1] = 0x22;              // Initialize data registers based
			DataCanTx[2] = 0xF1;
			DataCanTx[3] = 0x95;
			DataCanTx[4] = 0x55;
			DataCanTx[5] = 0x55;
			DataCanTx[6] = 0x55;
			DataCanTx[7] = 0x55;

			TxHeader.IDE = CAN_ID_STD;
			TxHeader.StdId = 0x796;
			TxHeader.RTR = CAN_RTR_DATA;
			TxHeader.DLC = 8;
			TxHeader.TransmitGlobalTime = DISABLE;
			HAL_CAN_AddTxMessage(&hcan, &TxHeader, DataCanTx, &TxMailbox);
		break;
		case EXTENDED_SESSION:
			DataCanTx[0] = 0x02;          // on message object used
			DataCanTx[1] = 0x10;              // Initialize data registers based
			DataCanTx[2] = 0x03;
			DataCanTx[3] = 0x55;
			DataCanTx[4] = 0x55;
			DataCanTx[5] = 0x55;
			DataCanTx[6] = 0x55;
			DataCanTx[7] = 0x55;

			TxHeader.IDE = CAN_ID_STD;
			TxHeader.StdId = 0x796;
			TxHeader.RTR = CAN_RTR_DATA;
			TxHeader.DLC = 8;
			TxHeader.TransmitGlobalTime = DISABLE;
			HAL_CAN_AddTxMessage(&hcan, &TxHeader, DataCanTx, &TxMailbox);
		break;
		case PROGRAMMING_SESSION:
			DataCanTx[0] = 0x02;          // on message object used
			DataCanTx[1] = 0x10;              // Initialize data registers based
			DataCanTx[2] = 0x02;
			DataCanTx[3] = 0x00;
			DataCanTx[4] = 0x00;
			DataCanTx[5] = 0x00;
			DataCanTx[6] = 0x00;
			DataCanTx[7] = 0x00;
			TxHeader.IDE = CAN_ID_STD;
			TxHeader.StdId = 0x796;
			TxHeader.RTR = CAN_RTR_DATA;
			TxHeader.DLC = 8;
			TxHeader.TransmitGlobalTime = DISABLE;
			HAL_CAN_AddTxMessage(&hcan, &TxHeader, DataCanTx, &TxMailbox);

		break;
		case REQUEST_SEED:
			DataCanTx[0] = 0x02;          // on message object used
			DataCanTx[1] = 0x27;              // Initialize data registers based
			DataCanTx[2] = 0x01;
			DataCanTx[3] = 0x55;
			DataCanTx[4] = 0x55;
			DataCanTx[5] = 0x55;
			DataCanTx[6] = 0x55;
			DataCanTx[7] = 0x55;

			TxHeader.IDE = CAN_ID_STD;
			TxHeader.StdId = 0x796;
			TxHeader.RTR = CAN_RTR_DATA;
			TxHeader.DLC = 8;
			TxHeader.TransmitGlobalTime = DISABLE;
			HAL_CAN_AddTxMessage(&hcan, &TxHeader, DataCanTx, &TxMailbox);
		break;
		case SEND_KEY:
			DataCanTx[0] = 0x06;          // on message object used
			DataCanTx[1] = 0x27;              // Initialize data registers based
			DataCanTx[2] = 0x02;
			DataCanTx[3] = KeyData[0];
			DataCanTx[4] = KeyData[1];
			DataCanTx[5] = KeyData[2];
			DataCanTx[6] = KeyData[3];
			DataCanTx[7] = 0x55;

			TxHeader.IDE = CAN_ID_STD;
			TxHeader.StdId = 0x796;
			TxHeader.RTR = CAN_RTR_DATA;
			TxHeader.DLC = 8;
			TxHeader.TransmitGlobalTime = DISABLE;
			HAL_CAN_AddTxMessage(&hcan, &TxHeader, DataCanTx, &TxMailbox);

		break;

		case ROUTINE_BINARY_UPDATE:
			DataCanTx[0] = 0x05;          // on message object used
			DataCanTx[1] = 0x31;              // Initialize data registers based
			DataCanTx[2] = 0x01;
			DataCanTx[3] = 0xFF;
			DataCanTx[4] = 0x00;
			DataCanTx[5] = download_type;
			DataCanTx[6] = 0x55;
			DataCanTx[7] = 0x55;

			TxHeader.IDE = CAN_ID_STD;
			TxHeader.StdId = 0x796;
			TxHeader.RTR = CAN_RTR_DATA;
			TxHeader.DLC = 8;
			TxHeader.TransmitGlobalTime = DISABLE;
			HAL_CAN_AddTxMessage(&hcan, &TxHeader, DataCanTx, &TxMailbox);
		break;
		case REQUEST_DOWNLOAD_1:
			DataCanTx[0] = 0x10;          // on message object used
			DataCanTx[1] = 0x09;              // Initialize data registers based
			DataCanTx[2] = 0x34;
			DataCanTx[3] = 0x00U;
			DataCanTx[4] = 0x33;
			DataCanTx[5] = 0x00U;//download_type;
			DataCanTx[6] = 0x40;
			DataCanTx[7] = 0x00U;

			TxHeader.IDE = CAN_ID_STD;
			TxHeader.StdId = 0x796;
			TxHeader.RTR = CAN_RTR_DATA;
			TxHeader.DLC = 8;
			TxHeader.TransmitGlobalTime = DISABLE;
			HAL_CAN_AddTxMessage(&hcan, &TxHeader, DataCanTx, &TxMailbox);
		break;
		case REQUEST_DOWNLOAD_2:
			DataCanTx[0] = 0x21;          // on message object used
			DataCanTx[1] = 0x55;//TGT_Info[14];              // Initialize data registers based
			DataCanTx[2] = 0x55;//TGT_Info[13];
			DataCanTx[3] = 0x55;//TGT_Info[12];
			DataCanTx[4] = 0x55;
			DataCanTx[5] = 0x55;
			DataCanTx[6] = 0x55;
			DataCanTx[7] = 0x55;

			TxHeader.IDE = CAN_ID_STD;
			TxHeader.StdId = 0x796;
			TxHeader.RTR = CAN_RTR_DATA;
			TxHeader.DLC = 8;
			TxHeader.TransmitGlobalTime = DISABLE;
			HAL_CAN_AddTxMessage(&hcan, &TxHeader, DataCanTx, &TxMailbox);

		break;
		case REQUEST_TRANSFER_EXIT:
			DataCanTx[0] = 0x01;          // on message object used
			DataCanTx[1] = 0x37;              // Initialize data registers based
			DataCanTx[2] = 0x55;
			DataCanTx[3] = 0x55;
			DataCanTx[4] = 0x55;
			DataCanTx[5] = 0x55;
			DataCanTx[6] = 0x55;
			DataCanTx[7] = 0x55;

			TxHeader.IDE = CAN_ID_STD;
			TxHeader.StdId = 0x796;
			TxHeader.RTR = CAN_RTR_DATA;
			TxHeader.DLC = 8;
			TxHeader.TransmitGlobalTime = DISABLE;
			HAL_CAN_AddTxMessage(&hcan, &TxHeader, DataCanTx, &TxMailbox);

		break;
		case ECU_RESET:
			DataCanTx[0] = 0x02;          // on message object used
			DataCanTx[1] = 0x11;              // Initialize data registers based
			DataCanTx[2] = 0x01;
			DataCanTx[3] = 0x55;
			DataCanTx[4] = 0x55;
			DataCanTx[5] = 0x55;
			DataCanTx[6] = 0x55;
			DataCanTx[7] = 0x55;

			TxHeader.IDE = CAN_ID_STD;
			TxHeader.StdId = 0x796;
			TxHeader.RTR = CAN_RTR_DATA;
			TxHeader.DLC = 8;
			TxHeader.TransmitGlobalTime = DISABLE;
			HAL_CAN_AddTxMessage(&hcan, &TxHeader, DataCanTx, &TxMailbox);
		break;
		case READ_PARTNUMBER:
			DataCanTx[0] = 0x03;          // on message object used
			DataCanTx[1] = 0x22;              // Initialize data registers based
			DataCanTx[2] = 0xF1;
			DataCanTx[3] = 0x87;
			DataCanTx[4] = 0x55;
			DataCanTx[5] = 0x55;
			DataCanTx[6] = 0x55;
			DataCanTx[7] = 0x55;

			TxHeader.IDE = CAN_ID_STD;
			TxHeader.StdId = 0x796;
			TxHeader.RTR = CAN_RTR_DATA;
			TxHeader.DLC = 8;
			TxHeader.TransmitGlobalTime = DISABLE;
			HAL_CAN_AddTxMessage(&hcan, &TxHeader, DataCanTx, &TxMailbox);
		break;
		case CONTROL_FLOW:
			DataCanTx[0] = 0x30;          // on message object used
			DataCanTx[1] = 0x08;              // Initialize data registers based
			DataCanTx[2] = 0x01;
			DataCanTx[3] = 0x55;
			DataCanTx[4] = 0x55;
			DataCanTx[5] = 0x55;
			DataCanTx[6] = 0x55;
			DataCanTx[7] = 0x55;

			TxHeader.IDE = CAN_ID_STD;
			TxHeader.StdId = 0x796;
			TxHeader.RTR = CAN_RTR_DATA;
			TxHeader.DLC = 8;
			TxHeader.TransmitGlobalTime = DISABLE;
			HAL_CAN_AddTxMessage(&hcan, &TxHeader, DataCanTx, &TxMailbox);
		break;
		case FIRT_FARME:
			DataCanTx[0] = 0x10;          // on message object used
			DataCanTx[1] = 0x0C;              // Initialize data registers based
			DataCanTx[2] = 0x3D;
			DataCanTx[3] = 0x4D;
			DataCanTx[4] = 0x43;
			DataCanTx[5] = 0x4E;
			DataCanTx[6] = 0x45;
			DataCanTx[7] = 0x58;

			TxHeader.IDE = CAN_ID_STD;
			TxHeader.StdId = 0x796;
			TxHeader.RTR = CAN_RTR_DATA;
			TxHeader.DLC = 8;
			TxHeader.TransmitGlobalTime = DISABLE;
			HAL_CAN_AddTxMessage(&hcan, &TxHeader, DataCanTx, &TxMailbox);
		break;
			case CONSECUTIVE:
			DataCanTx[0] = 0x21;          // on message object used
			DataCanTx[1] = 0x57;              // Initialize data registers based
			DataCanTx[2] = 0x52;
			DataCanTx[3] = 0x49;
			DataCanTx[4] = 0x54;
			DataCanTx[5] = 0x45;
			DataCanTx[6] = 0x32;
			DataCanTx[7] = 0x30;

			TxHeader.IDE = CAN_ID_STD;
			TxHeader.StdId = 0x796;
			TxHeader.RTR = CAN_RTR_DATA;
			TxHeader.DLC = 8;
			TxHeader.TransmitGlobalTime = DISABLE;
			HAL_CAN_AddTxMessage(&hcan, &TxHeader, DataCanTx, &TxMailbox);
		break;
		case FIRT_FARME_check:// bo FF->SF
			DataCanTx[0] = 0x07;          // on message object used
			DataCanTx[1] = 0x23;              // Initialize data registers based
			DataCanTx[2] = 0x23;
//			DataCanTx[3] = address_check[0];
//			DataCanTx[4] = address_check[1];
//			DataCanTx[5] = address_check[2];
			DataCanTx[6] = 0x00;
			DataCanTx[7] = 0xfd;

			TxHeader.IDE = CAN_ID_STD;
			TxHeader.StdId = 0x796;
			TxHeader.RTR = CAN_RTR_DATA;
			TxHeader.DLC = 8;
			TxHeader.TransmitGlobalTime = DISABLE;
			HAL_CAN_AddTxMessage(&hcan, &TxHeader, DataCanTx, &TxMailbox);
		break;
		case CONSECUTIVE_check:
			DataCanTx[0] = 0x21;          // on message object used
			DataCanTx[1] = 0x00;              // Initialize data registers based
			DataCanTx[2] = 0x10;
			DataCanTx[3] = 0x55;
			DataCanTx[4] = 0x55;
			DataCanTx[5] = 0x55;
			DataCanTx[6] = 0x55;
			DataCanTx[7] = 0x55;

			TxHeader.IDE = CAN_ID_STD;
			TxHeader.StdId = 0x796;
			TxHeader.RTR = CAN_RTR_DATA;
			TxHeader.DLC = 8;
			TxHeader.TransmitGlobalTime = DISABLE;
			HAL_CAN_AddTxMessage(&hcan, &TxHeader, DataCanTx, &TxMailbox);
		break;
		default:
		break;
	}

}
void SEND_541(uint8_t IGN_Sw)
{
	DataCanTx[0] = IGN_Sw;          // on message object used
	DataCanTx[1] = 0x00U;              // Initialize data registers based
	DataCanTx[2] = 0x00U;
	DataCanTx[3] = 0x00U;
	DataCanTx[4] = 0x00U;
	DataCanTx[5] = 0x00U;
	DataCanTx[6] = 0x00U;
	DataCanTx[7] = 0x00U;

	TxHeader.IDE = CAN_ID_STD;
	TxHeader.StdId = 0x541;
	TxHeader.RTR = CAN_RTR_DATA;
	TxHeader.DLC = 8;
	TxHeader.TransmitGlobalTime = DISABLE;
	HAL_CAN_AddTxMessage(&hcan, &TxHeader, DataCanTx, &TxMailbox);
}
uint8_t TGT_Write_Flash (volatile uint8_t *buf, uint16_t index, uint8_t numbytes,uint8_t Page_Num)
{
	static uint8_t cnt = 0;
	static uint8_t page = 0xFF;
	uint8_t last_byte = 0x00U;
	if ( page != Page_Num )
	{
		page = Page_Num;
		DataCanTx[0] = 0x10;
		DataCanTx[1] = 0xff;
		DataCanTx[2] = 0x36;
		DataCanTx[3] = Page_Num+1;
		DataCanTx[4] = *(buf+0);
		DataCanTx[5] = *(buf+1);
		DataCanTx[6] = *(buf+2);
		DataCanTx[7] = *(buf+3);
		cnt = 0;
	}
	else
	{
		if ( cnt == 16 )
		{
			DataCanTx[0] = 0x20;
			DataCanTx[1] = *(buf+index +0);
			DataCanTx[2] = *(buf+index +1);
			DataCanTx[3] = *(buf+index +2);
			DataCanTx[4] = *(buf+index +3);
			DataCanTx[5] = *(buf+index +4);
			DataCanTx[6] = *(buf+index +5);
			DataCanTx[7] = *(buf+index +6);
			cnt = 0;

		}
		else
		{
			DataCanTx[0] = cnt+0x20;//cnt+0x20
			DataCanTx[1] = *(buf+index +0);
			DataCanTx[2] = *(buf+index +1);
			DataCanTx[3] = *(buf+index +2);
			DataCanTx[4] = *(buf+index +3);
			DataCanTx[5] = *(buf+index +4);
			DataCanTx[6] = *(buf+index +5);
			DataCanTx[7] = *(buf+index +6);
		}

		if ( numbytes == 4 )
		{
			DataCanTx[0] = cnt+0x20;
			DataCanTx[1] = *(buf+index +0);
			DataCanTx[2] = *(buf+index +1);
			DataCanTx[3] = *(buf+index +2);
			DataCanTx[4] = *(buf+index +3);
			DataCanTx[5] = 0x55U;
			DataCanTx[6] = 0x55U;
			DataCanTx[7] = 0x55U;
		}

	}
	last_byte++;
	cnt++;
	// Other bytes of the message are "don't care".
	CAN_Rx_Complete_Flag = 0x00U;
	TxHeader.IDE = CAN_ID_STD;
	TxHeader.StdId = 0x796U;
	TxHeader.RTR = CAN_RTR_DATA;
	TxHeader.DLC = 8;
	TxHeader.TransmitGlobalTime = DISABLE;
	HAL_CAN_AddTxMessage(&hcan, &TxHeader, DataCanTx, &TxMailbox);
	return DataCanTx[0];
}

void Mode_Complete(MsgCanTranmitTypeDef * DataCan)// 79E là id phản hồi.
{

	switch(Transfer_Mode)
	{
		case SWVERSION:
			if ( DataCan->DATA[1] == 0x62U && DataCan->DATA[2] == 0xF1U )
			{
				CAN_Rx_Complete_Flag = 0x01U;       // Indicate Rx Complete
			}
		break;
		case EXTENDED_SESSION:
			if ( DataCan->DATA[1] == 0x50 && DataCan->DATA[2] == 0x03 )
			{
				CAN_Rx_Complete_Flag = 1;       // Indicate Rx Complete
			}
		break;
		case PROGRAMMING_SESSION:
			if ( DataCan->DATA[1] == 0x50 && DataCan->DATA[2] == 0x02 )
			{
				CAN_Rx_Complete_Flag = 1;       // Indicate Rx Complete
			}
		break;
		case CONTROLDTC_OFF:
			if ( DataCan->DATA[1] == 0xC5 && DataCan->DATA[2] == 0x02 )
			{
				CAN_Rx_Complete_Flag = 1;       // Indicate Rx Complete
			}
		break;
		case REQUEST_SEED:
			if ( DataCan->DATA[1] == 0x67 && DataCan->DATA[2] == 0x01 )
			{
				KeyData[0] = DataCan->DATA[3]+0x10;       // Indicate Rx Complete
				KeyData[1] = DataCan->DATA[4]+0x10;       // Indicate Rx Complete
				KeyData[2] = DataCan->DATA[5]+0x10;       // Indicate Rx Complete
				KeyData[3] = DataCan->DATA[6]+0x10;       // Indicate Rx Complete
				CAN_Rx_Complete_Flag = 1;       // Indicate Rx Complete
			}
		break;
		case REQUEST_DOWNLOAD_1:
		case REQUEST_DOWNLOAD_2:
			//if ( CAN_Rx_Buf[0] == 0x76 || CAN_Rx_Buf[1] == 0x08 )
			//{
				CAN_Rx_Complete_Flag = 1;       // Indicate Rx Complete
			//}
		break;
		case SEND_KEY:
			if ( DataCan->DATA[1] == 0x67 && DataCan->DATA[2] == 0x02 )
			{
				CAN_Rx_Complete_Flag = 1;       // Indicate Rx Complete
			}
		break;
		case ROUTINE_BINARY_UPDATE:
			if ( DataCan->DATA[1] == 0x71 && DataCan->DATA[2] == 0x01 )
			{
				CAN_Rx_Complete_Flag = 1;       // Indicate Rx Complete
			}
		break;

		default:
			CAN_Rx_Complete_Flag = 0;
		break;
	}
}
