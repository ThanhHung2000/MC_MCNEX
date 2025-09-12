/*
 * il_par.h
 *
 *  Created on: Apr 28, 2025
 *      Author: R&D
 */

#ifndef IL_PAR_H_
#define IL_PAR_H_
#include "drv_par.h"
#include "il_cfg.h"

// cjlee_211012_EngCondition
#define IlGetEngCond_ENG()            (ENG_COND.ENG_COND.ENG_STAT)
#define IlGetEngCond_IGN()            (ENG_COND.ENG_COND.IGN_STAT)
#define IlGetEngCond_CommCtrlInfo()   (ENG_COND.ENG_COND.COMM_CTRL_INFO)
/* Handle:   12,Name:              CF_Tcu_GarSelDisp,Size:  4,UsedBytes:  1,SingleSignal */
#ifdef IL_ENABLE_RX
#define IlGetRxENG_STAT()                    (EMS16.EMS16.ENG_STAT)
#endif

/* Handle:    1,Name:                  CF_Gway_IGNSw,Size:  3,UsedBytes:  1,SingleSignal */
#ifdef IL_ENABLE_RX
#define IlGetRxCF_Gway_IGNSw()               (CGW1.CGW1.CF_Gway_IGNSw)
#endif


// cjlee_211012_EngCondition
extern void IlPutEngCond_ENG(uint8_t sigData);
extern void IlPutEngCond_IGN(uint8_t sigData);
extern void IlPutEngCond_CommCtrlInfo(uint8_t sigData);

extern void IlMsgEMS16Indication(uint8_t rxObject);
extern void IlMsgCGW1Indication(uint8_t rxObject);
#endif /* IL_PAR_H_ */
