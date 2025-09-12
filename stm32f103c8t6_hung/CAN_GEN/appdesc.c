/*
 * appdesc.c
 *
 *  Created on: Apr 26, 2025
 *      Author: R&D
 */
#include"appdesc.h"
#include "desc.h"
/*  ********************************************************************************
 * Function name:ApplDescOnTransitionSession
 * Description:Notification function for state change of the given state group, defined by
 * CANdelaStudio.
 * Returns:  nothing
 * Parameter(s):
 *   - newState:
 *       - The state which will be set.
 *       - Access type: read
 *   - formerState:
 *       - The current state of this state group.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" may not be called.
 *   - The function "DescSetNegResponse" may not be called.
 ********************************************************************************  */
void  ApplDescOnTransitionSession(DescStateGroup newState, DescStateGroup formerState)
{
  /* <<TBD>> Remove this comment once you have completely implemented this function!!! */
   /* This is only a notification function. Using the "formerState" and the "newState"
   * parameter you can distinguish the different transitions for this state group.
   */
  /* Avoids warnings */
//  DESC_IGNORE_UNREF_PARAM(newState);
//  DESC_IGNORE_UNREF_PARAM(formerState);
}

/*  ********************************************************************************
 * Function name:ApplDescOnTransitionSecurityAccess
 * Description:Notification function for state change of the given state group, defined by
 * CANdelaStudio.
 * Returns:  nothing
 * Parameter(s):
 *   - newState:
 *       - The state which will be set.
 *       - Access type: read
 *   - formerState:
 *       - The current state of this state group.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" may not be called.
 *   - The function "DescSetNegResponse" may not be called.
 ********************************************************************************  */
void  ApplDescOnTransitionSecurityAccess(DescStateGroup newState, DescStateGroup formerState)
{
  /* <<TBD>> Remove this comment once you have completely implemented this function!!! */
   /* This is only a notification function. Using the "formerState" and the "newState"
   * parameter you can distinguish the different transitions for this state group.
   */
  /* Avoids warnings */
//  DESC_IGNORE_UNREF_PARAM(newState);
//  DESC_IGNORE_UNREF_PARAM(formerState);
;
}
/*  ********************************************************************************
 * Function name:ApplDescEcuResetHard (Service request header:$11 $1 )
 * Description: not available
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or later
 * but before calling "DescProcessingDone".
 ********************************************************************************  */
void  ApplDescEcuResetHard(DescMsgContext* pMsgContext)
{
  /* <<TBD>> Remove this comment once you have completely implemented this function!!! */
  /* Contains no request data */
  /* Contains no response data */
  /* User service processing finished. */

  // cjlee_211012_EngCondition
//  if (Diag_Ecu_Reset() == 0x01)
//  {
//  }
//  else
//  {
//    DescSetNegResponse(kDescNrcRequestOutOfRange);
//  }
//  DescProcessingDone();
}
/*  ********************************************************************************
 * Function name:ApplDescEcuResetSoft (Service request header:$11 $3 )
 * Description: not available
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or later
 * but before calling "DescProcessingDone".
 ********************************************************************************  */
void  ApplDescEcuResetSoft(DescMsgContext* pMsgContext)
{
  /* <<TBD>> Remove this comment once you have completely implemented this function!!! */
  /* Contains no request data */
  /* Contains no response data */
  /* User service processing finished. */

  // cjlee_211012_EngCondition
//  if (Diag_Ecu_Reset() == 0x01)
//  {
//  }
//  else
//  {
//    DescSetNegResponse(kDescNrcRequestOutOfRange);
//  }
//  DescProcessingDone();
}
/*  ********************************************************************************
 * Function name:ApplDescClearDiagInfo (Service request header:$14 )
 * Description: not available
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or later
 * but before calling "DescProcessingDone".
 ********************************************************************************  */
void  ApplDescClearDiagInfo(DescMsgContext* pMsgContext)
{
  /* <<TBD>> Remove this comment once you have completely implemented this function!!! */
  /* Dummy example how to access the request data. */
  /* Assumming the FIRST DATA byte contains important data which has to be less than a constant value. */
//  if( (pMsgContext->reqData[0] == 0x80) || (pMsgContext->reqData[0] == 0xFF) )
//  {
//  	Diag_Clear_DTC();
//    /* Received data is in range process further. */
//    /* Contains no response data */
//  }
//  else
//  {
//    /* Request contains invalid data - send negative response! */
//    DescSetNegResponse(kDescNrcRequestOutOfRange);
//  }
//  /* User service processing finished. */
//  DescProcessingDone();
}
/*  ********************************************************************************
 * Function name:ApplDescReadDtcRNODTCBSM (Service request header:$19 $1 )
 * Description: not available
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or later
 * but before calling "DescProcessingDone".
 ********************************************************************************  */
void  ApplDescReadDtcRNODTCBSM(DescMsgContext* pMsgContext)
{
  /* <<TBD>> Remove this comment once you have completely implemented this function!!! */
  /* Dummy example how to access the request data. */
  /* Assumming the FIRST DATA byte contains important data which has to be less than a constant value. */
//  if(pMsgContext->reqData[0] < 0xFF)
//  {
//    /* Received data is in range process further. */
//    /* Dummy example of how to write the response data. */
//		pMsgContext->resData[0] = 0x08;
//		pMsgContext->resData[1] = 0x00;
//		pMsgContext->resData[2] = 0x00;
//		pMsgContext->resData[3] = Diag_Read_Dtc_Number();
//    /* Always set the correct length of the response data. */
//    pMsgContext->resDataLen = 4;
//  }
//  else
//  {
//    /* Request contains invalid data - send negative response! */
//    DescSetNegResponse(kDescNrcRequestOutOfRange);
//  }
//  /* User service processing finished. */
//  DescProcessingDone();
}
/*  ********************************************************************************
 * Function name:ApplDescReadDtcRDTCBSM (Service request header:$19 $2 )
 * Description: not available
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or later
 * but before calling "DescProcessingDone".
 ********************************************************************************  */
void  ApplDescReadDtcRDTCBSM(DescMsgContext* pMsgContext)
{
//	//U8 ID;
//	SEG_XDATA U8 ret;
//  /* <<TBD>> Remove this comment once you have completely implemented this function!!! */
//  /* Dummy example how to access the request data. */
//  /* Assumming the FIRST DATA byte contains important data which has to be less than a constant value. */
//  if(pMsgContext->reqData[0] < 0xFF)
//  {
//    /* Received data is in range process further. */
//    /* Dummy example of how to write the response data. */
//	pMsgContext->resData[pMsgContext->resDataLen++] = 0x08;
//
//	ret = Diag_Read_Dtc(0x01);
//	if ( ret != 0x00 )
//	{
//
//		pMsgContext->resData[pMsgContext->resDataLen++] = 0x96;
//    	pMsgContext->resData[pMsgContext->resDataLen++] = 0x21; // 0x1E -> 0x21
//    	pMsgContext->resData[pMsgContext->resDataLen++] = 0x00;
//    	pMsgContext->resData[pMsgContext->resDataLen++] = ret;
//	}
//	ret = Diag_Read_Dtc(0x02);
//	if ( ret != 0x00 )
//	{
//		pMsgContext->resData[pMsgContext->resDataLen++] = 0x96;
//		pMsgContext->resData[pMsgContext->resDataLen++] = 0x1F;
//    	pMsgContext->resData[pMsgContext->resDataLen++] = 0x00;
//		pMsgContext->resData[pMsgContext->resDataLen++] = ret;
//	}
//	ret = Diag_Read_Dtc(0x03);
//	if ( ret != 0x00 )
//	{
//		pMsgContext->resData[pMsgContext->resDataLen++] = 0x96;
//		pMsgContext->resData[pMsgContext->resDataLen++] = 0xA7;
//    	pMsgContext->resData[pMsgContext->resDataLen++] = 0x00;
//		pMsgContext->resData[pMsgContext->resDataLen++] = ret;
//	}
//
//    /* Always set the correct length of the response data. */
//    //pMsgContext->resDataLen = 1;
//  }
//  else
//  {
//    /* Request contains invalid data - send negative response! */
//    DescSetNegResponse(kDescNrcRequestOutOfRange);
//  }
//  /* User service processing finished. */
//  DescProcessingDone();
}
/*  ********************************************************************************
 * Function name:ApplDescReadMemoryByAddress (Service request header:$23 )
 * Description: not available
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request byte after the Sid.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response after the Sid.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the service request length, excluding Sid byte only.
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the total service response length, excluding the Sid only.
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or later
 * but before calling "DescProcessingDone".
 ********************************************************************************  */
void  ApplDescReadMemoryByAddress(DescMsgContext* pMsgContext)
{
  /* <<TBD>> Remove this comment once you have completely implemented this function!!! */
  /* Contains no request data */
  /* Contains no response data */
  /* User service processing finished. */
//  SEG_XDATA U32 DataAddr_Low;
//  SEG_XDATA U32 DataAddr_High;
//  SEG_XDATA U32 DataAddr = (U32)0x00U;
//  SEG_XDATA U16 DataLen = (U16)0x00U;
//  SEG_XDATA U8 buf[16];
//
//  DataAddr_High = (pMsgContext->reqData[1] << 8) | (pMsgContext->reqData[2]);
//  DataAddr_Low = (pMsgContext->reqData[3] << 8) |pMsgContext->reqData[4];
//
//  DataAddr = (DataAddr_High << 16) | DataAddr_Low;
//  DataLen = (pMsgContext->reqData[5] << 8) | pMsgContext->reqData[6];
//
//  if ((DataLen <= 16) && ((DataAddr+DataLen-1) <= 0x1FBFFU))
//  {
//    //Diag_Read_Address(pMsgContext->resData, DataAddr, DataLen);
//	Diag_Read_Address(buf, DataAddr, DataLen);
//	memcpy(&pMsgContext->resData[0], buf, DataLen);
//
//    pMsgContext->resDataLen = DataLen;
//  }
//  else
//  {
//    DescSetNegResponse(kDescNrcRequestOutOfRange);
//  }
//
//  DescProcessingDone();
}
/*  ********************************************************************************
 * Function name:ApplDescWriteDidCAMERA_System_On_Time (Service request header:$2E $F1 $5 )
 * Description: not available
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or later
 * but before calling "DescProcessingDone".
 ********************************************************************************  */
void  ApplDescWriteDidCAMERA_System_On_Time(DescMsgContext* pMsgContext)
{
//  /* <<TBD>> Remove this comment once you have completely implemented this function!!! */
//  /* Dummy example how to access the request data. */
//  /* Assumming the FIRST DATA byte contains important data which has to be less than a constant value. */
//  if(pMsgContext->reqData[0] < 0xFF)
//  {
//    /* Received data is in range process further. */
//    /* Contains no response data */
//	//cjlee_221102_EngCondition
//	if ((IlGetEngCond_ENG() != 0x03U) && (IlGetEngCond_IGN() == 0x03U))
//	{
//	}
//	else
//	{
//	  DescSetNegResponse(kDescNrcConditionsNotCorrect);
//	}
//  }
//  else
//  {
//    /* Request contains invalid data - send negative response! */
//    DescSetNegResponse(kDescNrcRequestOutOfRange);
//  }
//  /* User service processing finished. */
//  DescProcessingDone();
}
/*  ********************************************************************************
 * Function name:ApplDescWriteDidCAMERA_Operating_Time_and_System_On (Service request header:$2E $F1 $6 )
 * Description: not available
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or later
 * but before calling "DescProcessingDone".
 ********************************************************************************  */
void  ApplDescWriteDidCAMERA_Operating_Time_and_System_On(DescMsgContext* pMsgContext)
{
  /* <<TBD>> Remove this comment once you have completely implemented this function!!! */
  /* Dummy example how to access the request data. */
  /* Assumming the FIRST DATA byte contains important data which has to be less than a constant value. */
//  if(pMsgContext->reqData[0] < 0xFF)
//  {
//    /* Received data is in range process further. */
//    /* Contains no response data */
//	//cjlee_221102_EngCondition
//  	if ((IlGetEngCond_ENG() != 0x03U) && (IlGetEngCond_IGN() == 0x03U))
//  	{
//	}
//	else
//	{
//		DescSetNegResponse(kDescNrcConditionsNotCorrect);
//	}
//  }
//  else
//  {
//    /* Request contains invalid data - send negative response! */
//    DescSetNegResponse(kDescNrcRequestOutOfRange);
//  }
//  /* User service processing finished. */
//  DescProcessingDone();
}
/*  ********************************************************************************
 * Function name:ApplDescWriteDidCAMERA_Warning_Text_Language (Service request header:$2E $F1 $7 )
 * Description: not available
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or later
 * but before calling "DescProcessingDone".
 ********************************************************************************  */
void  ApplDescWriteDidCAMERA_Warning_Text_Language(DescMsgContext* pMsgContext)
{
//  /* <<TBD>> Remove this comment once you have completely implemented this function!!! */
//  /* Dummy example how to access the request data. */
//  /* Assumming the FIRST DATA byte contains important data which has to be less than a constant value. */
//  if(pMsgContext->reqData[0] < 0xFF)
//  {
//    /* Received data is in range process further. */
//    /* Contains no response data */
//	//cjlee_221102_EngCondition
//  	if ((IlGetEngCond_ENG() != 0x03U) && (IlGetEngCond_IGN() == 0x03U))
//  	{
//	}
//	else
//	{
//		DescSetNegResponse(kDescNrcConditionsNotCorrect);
//	}
//  }
//  else
//  {
//    /* Request contains invalid data - send negative response! */
//    DescSetNegResponse(kDescNrcRequestOutOfRange);
//  }
//  /* User service processing finished. */
//  DescProcessingDone();
}

/*  ********************************************************************************
 * Function name:ApplDescWriteDidCAMERA_Reset_Counter (Service request header:$2E $F1 $8 )
 * Description: not available
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or later
 * but before calling "DescProcessingDone".
 ********************************************************************************  */
void  ApplDescWriteDidCAMERA_Reset_Counter(DescMsgContext* pMsgContext)
{
  /* <<TBD>> Remove this comment once you have completely implemented this function!!! */
  /* Dummy example how to access the request data. */
  /* Assumming the FIRST DATA byte contains important data which has to be less than a constant value. */
//  if(pMsgContext->reqData[0] < 0xFF)
//  {
//    /* Received data is in range process further. */
//    /* Contains no response data */
//	//cjlee_221102_EngCondition
//  	if ((IlGetEngCond_ENG() != 0x03U) && (IlGetEngCond_IGN() == 0x03U))
//  	{
//	}
//	else
//	{
//		DescSetNegResponse(kDescNrcConditionsNotCorrect);
//	}
//  }
//  else
//  {
//    /* Request contains invalid data - send negative response! */
//    DescSetNegResponse(kDescNrcRequestOutOfRange);
//  }
//  /* User service processing finished. */
//  DescProcessingDone();
}
/*  ********************************************************************************
 * Function name:ApplDescRequestDownload (Service request header:$34 )
 * Description: not available
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request byte after the Sid.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response after the Sid.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the service request length, excluding Sid byte only.
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the total service response length, excluding the Sid only.
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or later
 * but before calling "DescProcessingDone".
 ********************************************************************************  */
void  ApplDescRequestDownload(DescMsgContext* pMsgContext)
{
  /* <<TBD>> Remove this comment once you have completely implemented this function!!! */
  /* Contains no request data */
  /* Contains no response data */
  /* User service processing finished. */

  // cjlee_211012_EngCondition
//	if (Diag_Request_Download(pMsgContext->reqData,pMsgContext->reqDataLen) == 0x01)
//	{
//	pMsgContext->resData[pMsgContext->resDataLen++] = 0x10;
//	pMsgContext->resData[pMsgContext->resDataLen++] = 0xFF;
//	}
//	else
//	{
//    /* Request contains invalid data - send negative response! */
//    DescSetNegResponse(kDescNrcRequestOutOfRange);
//   }
//
//  DescProcessingDone();
}
/*  ********************************************************************************
 * Function name:ApplDescTransferData (Service request header:$36 )
 * Description: not available
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request byte after the Sid.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response after the Sid.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the service request length, excluding Sid byte only.
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the total service response length, excluding the Sid only.
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or later
 * but before calling "DescProcessingDone".
 ********************************************************************************  */
void  ApplDescTransferData(DescMsgContext* pMsgContext)
{
//  /* <<TBD>> Remove this comment once you have completely implemented this function!!! */
//  /* Contains no request data */
//  /* Contains no response data */
//  /* User service processing finished. */
//
//  // cjlee_211012_EngCondition
//    transferdata_number = pMsgContext->resData[0];
//	if (Diag_Write_Transfer_Data(pMsgContext->reqData) == 0x01)
//	{
//	pMsgContext->resData[0] = transferdata_number;
//	pMsgContext->resDataLen = 1;
//	}
//  else
//  {
//    DescSetNegResponse(kDescNrcRequestOutOfRange);
//  }
//
//  DescProcessingDone();
}
/*  ********************************************************************************
 * Function name:ApplDescRequestTransferExit (Service request header:$37 )
 * Description: not available
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request byte after the Sid.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response after the Sid.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the service request length, excluding Sid byte only.
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the total service response length, excluding the Sid only.
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or later
 * but before calling "DescProcessingDone".
 ********************************************************************************  */
void  ApplDescRequestTransferExit(DescMsgContext* pMsgContext)
{
//  /* <<TBD>> Remove this comment once you have completely implemented this function!!! */
//  /* Contains no request data */
//  /* Contains no response data */
//  /* User service processing finished. */
//
//  // cjlee_211012_EngCondition
//  if (Diag_Exit_Update() == 0x01)
//  {
//	}
//	else
//  {
//    DescSetNegResponse(kDescNrcRequestOutOfRange);
//  }
//
//  DescProcessingDone();
}
/*  ********************************************************************************
 * Function name:ApplDescWriteMemoryByAddress (Service request header:$3D )
 * Description: not available
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request byte after the Sid.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response after the Sid.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the service request length, excluding Sid byte only.
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the total service response length, excluding the Sid only.
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or later
 * but before calling "DescProcessingDone".
 ********************************************************************************  */
void  ApplDescWriteMemoryByAddress(DescMsgContext* pMsgContext)
{
//	//SEG_XDATA volatile U8 length;
//	SEG_XDATA volatile U8 aa_temp[60];
//  /* <<TBD>> Remove this comment once you have completely implemented this function!!! */
//  /* Contains no request data */
//  /* Contains no response data */
//  /* User service processing finished. */
//  memset(aa_temp,0x00,sizeof(aa_temp));
//  //if((pMsgContext->reqData[0] < 0xFF) && (IlGetRxCF_Gway_IGNSw() == 0x03)) // 211012 Eng&Diag
//  if(pMsgContext->reqData[0] < 0xFF)
//  {
//#if 1 // cjlee_221102_EngCondition
//  	if ((IlGetEngCond_ENG() != 0x03U) && (IlGetEngCond_IGN() == 0x03U))
//  	{
//#endif
//  	//memcpy(aa_temp,pMsgContext->reqData,pMsgContext->reqDataLen);
//	if ( pMsgContext->reqDataLen == 0x05U)
//	{
//		Diag_Extra_Data_Process(pMsgContext->reqData);
//	}
//	else
//	{
//		Diag_Write_Sys(pMsgContext->reqData,pMsgContext->reqDataLen);
//	}
//#if 1 // cjlee_211012_EngCondition
//  	}
//    else
//    {
//      DescSetNegResponse(kDescNrcConditionsNotCorrect);
//    }
//#endif
//
//  }
//  else
//  {
//    /* Request contains invalid data - send negative response! */
//    DescSetNegResponse(kDescNrcRequestOutOfRange);
//  }
//  /* User service processing finished. */
//
//  DescProcessingDone();
}
/*  ********************************************************************************
 * Function name:ApplDescControlDtcSettingEnable (Service request header:$85 $1 )
 * Description: not available
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or later
 * but before calling "DescProcessingDone".
 ********************************************************************************  */
void  ApplDescControlDtcSettingEnable(DescMsgContext* pMsgContext)
{
//  /* <<TBD>> Remove this comment once you have completely implemented this function!!! */
//  /* Contains no request data */
//  /* Contains no response data */
//  /* User service processing finished. */
//	Diag_DTC_Set_On();
//  DescProcessingDone();
}
/*  ********************************************************************************
 * Function name:ApplDescControlDtcSettingDisable (Service request header:$85 $2 )
 * Description: not available
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or later
 * but before calling "DescProcessingDone".
 ********************************************************************************  */
void  ApplDescControlDtcSettingDisable(DescMsgContext* pMsgContext)
{
//  /* <<TBD>> Remove this comment once you have completely implemented this function!!! */
//  /* Contains no request data */
//  /* Contains no response data */
//  /* User service processing finished. */
//  	Diag_DTC_Set_Off();
//  DescProcessingDone();
}
/*  ********************************************************************************
 * Function name:ApplDescCheckSessionTransition
 * Description:Check if the given session transition is allowed or your ECU is currently not
 * able to perform it.
 * Returns:  nothing
 * Parameter(s):
 *   - newState:
 *       - Contains the new state in which the state group will be set.
 *       - Access type: read
 *   - formerState:
 *       - Contains the current state of the state group.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescSessionTransitionChecked" may be called.
 *   - The function "DescSetNegResponse" may be called.
 ********************************************************************************  */
void  ApplDescCheckSessionTransition(DescStateGroup newState, DescStateGroup formerState)
{
  /* <<TBD>> Remove this comment once you have completely implemented this function!!! */
     /* Avoids warnings */
  DESC_IGNORE_UNREF_PARAM(newState);
  DESC_IGNORE_UNREF_PARAM(formerState);
  /* Just showing how to reject session transition if necessary.
   * The negative response code may be other depending on the rejection reason.
   * This can be done later, but just store the iContext parameter (if exists) globaly to be able to give the correct acknowledgment.
   */
  //DescSetNegResponse(DESC_CONTEXT_PARAM_FIRST kDescNrcConditionsNotCorrect);
  /* Confirm the session will be accepted.
   * This can be done later, but just store the iContext parameter (if exists) globaly to be able to give the correct acknowledgment.
   */
  DescSessionTransitionChecked(DESC_CONTEXT_PARAM_ONLY);;
}
