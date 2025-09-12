/*
 * can_par.c
 *
 *  Created on: Mar 1, 2025
 *      Author: Hi
 */
#include <can_par.h>
#include "tpmc.h"
#include <drive_gpio.h>
#include "drv_par.h"
#include "il_par.h"
 _c_cLED9 LED_UV;




RxDataPtr RX_DataPtr[CAN_NUBER_RX_OBJECT]={
    (RxDataPtr)NULL,      /*0x240 */
	(RxDataPtr)EMS16._c,  /*0x260*/
	(RxDataPtr)NULL,      /*0x16 */
	(RxDataPtr)NULL,      /*0x18*/
	(RxDataPtr)NULL,      /*0x20*/
	(RxDataPtr)NULL,      /*0x22*/
	(RxDataPtr)NULL,      /*0x28*/
	(RxDataPtr)NULL,      /*0x16 */
	(RxDataPtr)NULL,      /*0x18*/
	(RxDataPtr)NULL,      /*0x20*/
	(RxDataPtr)NULL,      /*0x22*/
	(RxDataPtr)CGW1._c,   /*0x541*/

	(RxDataPtr)NULL,      /*0x16 */
	(RxDataPtr)NULL,      /*0x18*/
	(RxDataPtr)NULL,      /*0x20*/

};
const uint8_t Data_Rx_Leght[CAN_NUBER_RX_OBJECT]=
{
	4,/* ID: 0x00000240, Handle: 11, CGW_PC11 */
	4,/* ID: 0x00000240, Handle: 11, CGW_PC11 */
	8,/* ID: 0x00000240, Handle: 11, CGW_PC11 */
	8,
	8,
	8,
	8,
	8,
	8,
	8,
	8,
	8,
	8,
	8,
	8
};
const uint32_t CanRxId0[CAN_NUBER_RX_OBJECT] = {0x240U,0x260U,0x2b0U,0x367U, 0x368U, 0x386U, 0x490U, 0x4A4U, 0x4A6U, 0x4F1U, 0x50EU, 0x541U,0x57FU,0x796U, 0x7DFU};

void CAN_RX_IndicationFct(CANObject object)
{
	//call funtion drv_led
	ISR_Led_HU(object);

}

const CAN_Rx_Fun_Ptr CAN_Rx_Fun_Index[CAN_NUBER_RX_OBJECT]=
{
	NULL,
	(CAN_Rx_Fun_Ptr)IlMsgEMS16Indication,/* 0x260*/
	(CAN_Rx_Fun_Ptr)CAN_RX_IndicationFct,/* 0x260*/
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	(CAN_Rx_Fun_Ptr)CanRxCGW1IndicationFct,/* 0x541*/
	NULL,
	NULL,
	NULL
};
const AppPreCopy_Ptr CanRxPreCoppyPtr[CAN_NUBER_RX_OBJECT]=
{
	(AppPreCopy_Ptr)NULL,
	(AppPreCopy_Ptr)NULL,/* */
	(AppPreCopy_Ptr)NULL,
	(AppPreCopy_Ptr)NULL,
	(AppPreCopy_Ptr)NULL,
	(AppPreCopy_Ptr)NULL,
	(AppPreCopy_Ptr)NULL,
	(AppPreCopy_Ptr)NULL,
	(AppPreCopy_Ptr)NULL,
	(AppPreCopy_Ptr)NULL,
	(AppPreCopy_Ptr)NULL,
	(AppPreCopy_Ptr)NULL,
	(AppPreCopy_Ptr)NULL,
	(AppPreCopy_Ptr)TpPreCopy,
	(AppPreCopy_Ptr)NULL,

};
void CanRxCGW1IndicationFct(CANObject rcvObject)
{
  IlMsgCGW1Indication(rcvObject);
}
