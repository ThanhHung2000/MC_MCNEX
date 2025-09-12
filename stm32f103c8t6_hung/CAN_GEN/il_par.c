/*
 * il_par.c
 *
 *  Created on: Apr 28, 2025
 *      Author: R&D
 */


#include "il_par.h"
#include "can_par.h"



void IlMsgEMS16Indication(uint8_t rxObject)
{
  rxObject = rxObject;
//  IlEnterCriticalFlagAccess();
//  ilRxTimeoutFlags[1] &= (vuint8) 0xEF; //10
//  IlLeaveCriticalFlagAccess();

  IlPutEngCond_ENG(IlGetRxENG_STAT()); // cjlee_211012_EngCondition
}
void IlMsgCGW1Indication(uint8_t rxObject)
{
  rxObject = rxObject;
//  IlEnterCriticalFlagAccess();
//  ilRxTimeoutFlags[0] &= (vuint8) 0xE1; //02, 04, 08, 10
//  IlLeaveCriticalFlagAccess();

  IlPutEngCond_IGN(IlGetRxCF_Gway_IGNSw()); // cjlee_211012_EngCondition
}
// cjlee_211012_EngCondition
void IlPutEngCond_ENG(uint8_t sigData)
{
  ENG_COND.ENG_COND.ENG_STAT = ((uint8_t) (sigData & (uint8_t) 0x07));
}
void IlPutEngCond_IGN(uint8_t sigData)
{
  ENG_COND.ENG_COND.IGN_STAT = ((uint8_t) (sigData & (uint8_t) 0x07));
}
void IlPutEngCond_CommCtrlInfo(uint8_t sigData)
{
  ENG_COND.ENG_COND.COMM_CTRL_INFO = ((uint8_t) (sigData & (uint8_t) 0x03));
}
