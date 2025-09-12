/*
 * tpmc.h
 *
 *  Created on: Mar 6, 2025
 *      Author: Chuc
 */

#ifndef TPMC_H_
#define TPMC_H_
#include "main.h"
#include "tp_cfg.h"
# define kTpFCClearToSend           0x00
typedef struct
{
	uint8_t * DataCanBufferPtr;
	uint8_t * DataApplBufferPtr;

}tTpBufferPtr;
typedef struct
{
  tTpBufferPtr DataBufferPtr;
///* Description : List of pointer to receive buffers from the application      */
///******************************************************************************/
  uint16_t DataIndex;
///* Description : Index to transmit data - used instead of DataSegments        */
///* Val. range / Coding: 0..4095                                               */
///******************************************************************************/
  uint16_t DataLength;
///* Description : Length of data to be transmitted. The size of this variable  */
///*               limits the maximum size of messages.                         */
///* Val. range / Coding: 0..4095                                               */
///******************************************************************************/
//#if (TP_USE_MULTIPLE_ECU_NR == kTpOn)
//  canuint8 EcuNumber;
///* Description :                                                              */
///* Val. range / Coding: 0..0xff                                               */
///******************************************************************************/
//#endif
//#if defined(TP_ENABLE_MF_RECEPTION)
//# if (TP_USE_EXTENDED_API_BS == kTpOn)
//  canuint8 BlockSize;
///* Description :                                                              */
///* Val. range / Coding: 0..0xff                                               */
///******************************************************************************/
//# endif
//# if (TP_USE_EXTENDED_API_STMIN == kTpOn)
//  canuint8 STMin;
///* Description :                                                              */
///* Val. range / Coding: 0..0xff                                               */
///******************************************************************************/
//# endif
//#endif
//#if defined (TP_ENABLE_SINGLE_CHANNEL_TP)
//#else
uint8_t  Connection;
///* Description :   Indirection Table to store the ConnectionNr                */
///* Val. range / Coding: 0..255                                                */
///******************************************************************************/
//#endif
//#if defined(TP_ENABLE_MF_RECEPTION)
//# if defined (TP_ENABLE_FC_WAIT)
//#  if defined (TP_ENABLE_NORMAL_ADDRESSING)
//  canuint8 FFDataBuffer[6];
//#  else
//  canuint8 FFDataBuffer[5];
//#  endif
///* Description : Non ISO feature: acknowledged connections                    */
///* with FC after last CF                                                      */
///* Val. range / Coding: 0..1                                                  */
///******************************************************************************/
//# endif
//#endif
uint8_t sequencenumber:4; /*lint !e46*/
///* Description : SequenceNumber SN                                            */
///* Val. range / Coding: 0..0xf                                                */
///******************************************************************************/
//
//# if (kTpNumberOfCanChannels > 1)
//  canbittype CanChannel:4;    /*lint !e46*/
///* Description : For a MultiCAN System, the physical CanChannel               */
///*               have to be stored in RAM for being changed by the            */
///*               application. To support this, the additional functions       */
///*               TpTxSetCanChannel and TpRxGetCanChannel are provided.        */
///* Val. range / Coding: 0..7                                                  */
///******************************************************************************/
//# else
  uint8_t stuffbits1:4;
//# endif
//
//#if (defined(TP_ENABLE_FC_WAIT) || defined(TP_ENABLE_FC_SUPPRESS) || defined(TP_ENABLE_FC_OVERFLOW))
  uint8_t ApplGetBufferStatus:2;        /*lint !e46*/
///* Description : Status flag to proceed SF/FF reception  */
///*                                                       */
///* Val. range / Coding: 0..3                                                  */
///******************************************************************************/
//#endif
//#if defined(TP_ENABLE_MF_RECEPTION)
//# if (TP_USE_RX_CHANNEL_WITHOUT_FC == kTpOn)
//  canbittype withoutFC:1;       /*lint !e46*/
///* Description : Channel without using FlowControls                           */
///*               (Static:GenTool provides no send message for this tpChannel) */
///* Val. range / Coding: 0..1                                                  */
///******************************************************************************/
//# endif
//#endif
//#if defined (TP_ENABLE_DYN_AWAIT_CORRECT_SN)
//  canbittype SNwait:1;       /*lint !e46*/
///* Description : Wait for correct SN instead of reinitialization              */
///* Val. range / Coding: 0 = strict SN check, 1 = wait for correct SN          */
///******************************************************************************/
//#endif
//#if (TP_USE_APPL_PRECOPY == kTpOn) && (TP_USE_FAST_PRECOPY == kTpOff) && defined (TP_DISABLE_CHECKTA_COMPATIBILITY)
//  canbittype ta_type:1;
//#endif

} tTpRxInfoStruct;





#endif /* TPMC_H_ */
