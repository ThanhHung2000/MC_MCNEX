/*
 * appdesc.h
 *
 *  Created on: Apr 26, 2025
 *      Author: R&D
 */

#ifndef APPDESC_H_
#define APPDESC_H_
#include "desc.h"

void  ApplDescOnTransitionSession(DescStateGroup newState, DescStateGroup formerState);
void  ApplDescOnTransitionSecurityAccess(DescStateGroup newState, DescStateGroup formerState);
void  ApplDescEcuResetHard(DescMsgContext* pMsgContext);
void  ApplDescEcuResetSoft(DescMsgContext* pMsgContext);
void  ApplDescClearDiagInfo(DescMsgContext* pMsgContext);
void  ApplDescReadDtcRNODTCBSM(DescMsgContext* pMsgContext);
void  ApplDescReadDtcRDTCBSM(DescMsgContext* pMsgContext);
/* Request length check can not be performed. Please, implement length check in your Main-Handler!! */
void  ApplDescReadMemoryByAddress(DescMsgContext* pMsgContext);
void  ApplDescWriteDidCAMERA_System_On_Time(DescMsgContext* pMsgContext);
void  ApplDescWriteDidCAMERA_Operating_Time_and_System_On(DescMsgContext* pMsgContext);
void  ApplDescWriteDidCAMERA_Warning_Text_Language(DescMsgContext* pMsgContext);
void  ApplDescWriteDidCAMERA_Reset_Counter(DescMsgContext* pMsgContext);
/* Request length check can not be performed. Please, implement length check in your Main-Handler!! */
void  ApplDescRequestDownload(DescMsgContext* pMsgContext);
/* Request length check can not be performed. Please, implement length check in your Main-Handler!! */
void  ApplDescTransferData(DescMsgContext* pMsgContext);
/* Request length check can not be performed. Please, implement length check in your Main-Handler!! */
void  ApplDescRequestTransferExit(DescMsgContext* pMsgContext);
/* Request length check can not be performed. Please, implement length check in your Main-Handler!! */
void  ApplDescWriteMemoryByAddress(DescMsgContext* pMsgContext);
void  ApplDescControlDtcSettingEnable(DescMsgContext* pMsgContext);
void  ApplDescControlDtcSettingDisable(DescMsgContext* pMsgContext);




void  ApplDescCheckSessionTransition(DescStateGroup newState, DescStateGroup formerState);



#endif /* APPDESC_H_ */
