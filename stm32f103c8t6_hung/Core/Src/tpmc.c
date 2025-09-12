/*
 * tpmc.c
 *
 *  Created on: Mar 6, 2025
 *      Author: Chuc
 */

#include "tpmc.h"
#include "can_par.h"
#define DL_OFFSET                      1
#define FF_OFFSET                      2
#define SF_OFFSET                      1
#define CF_OFFSET                      1
#define STMIN_OFFSET                   2
#define BS_OFFSET                      1
#define kTpRxChannelCount              0x01U
#define kL4_SingleFrame                0x00
#define kL4_FirstFrame                 0x10
#define kL4_ConsecutiveFrame           0x20
#define kL4_FlowControl                0x30
#define   FRAME_DATA_PTR (rx_channal->Data)
/*******************************************************************************
* Transport protocol states
*******************************************************************************/
typedef enum {
  kRxState_Idle = 0,
  kRxState_ApplInformed,
  kRxState_CanFrameReceived,
  kRxState_WaitCF,
  kRxState_WaitFC,
  kRxState_WaitForFCConfIsr,
  kRxState_Error
} tTpRxStateEngine;
typedef struct
{
  uint8_t Timer;
  uint8_t BSCounter;
  tTpRxStateEngine engine;
  uint8_t queued:1;
  uint8_t retransmit:1;
} tTpRxState;

static  tTpRxState  tpRxState[kTpRxChannelCount]={0,};

tTpRxInfoStruct tpRxInfoStruct[kTpRxChannelCount];


uint8_t TpPreCopy(rxMsgObjPtr rx_channal)
{
	uint8_t count;
	// check tpRxState[].engine ==Idle ->ok =>kRxState_CanFrameReceived
	if(tpRxState[0].engine==kRxState_Idle)
	{
		switch(*(uint8_t *)(FRAME_DATA_PTR)&0xF0U)
		{
			case kL4_SingleFrame:
			{

			 // check lengt >0 và <=7 thì ok
				tpRxInfoStruct[0].DataLength=*(uint8_t *)(FRAME_DATA_PTR);
				if(tpRxInfoStruct[0].DataLength<=0x07U && tpRxInfoStruct[0].DataLength!=0x00U)
				{
					tpRxInfoStruct[0].ApplGetBufferStatus= kTpFCClearToSend;
					tpRxInfoStruct[0].DataBufferPtr.DataCanBufferPtr = (uint8_t*)(FRAME_DATA_PTR+SF_OFFSET);

					tpRxInfoStruct[0].DataBufferPtr.DataApplBufferPtr = __ApplTpRxGetBuffer(1,tpRxInfoStruct[0].DataLength);
					for( count=0x00U;count<=(tpRxInfoStruct[0].DataLength-1);count++)
					{
						*(tpRxInfoStruct[0].DataBufferPtr.DataApplBufferPtr + count)=*((uint8_t*)(SF_OFFSET+FRAME_DATA_PTR + count));
					}
					//tpRxState[0].engine =kRxState_ApplInformed;
				}
			}
			break;
			case kL4_FirstFrame:
			{

			}
			break;
			case kL4_ConsecutiveFrame:
			{

			}
			break;
			case kL4_FlowControl:
			{

			}
			break;
		}
	}
	return 0;
}
