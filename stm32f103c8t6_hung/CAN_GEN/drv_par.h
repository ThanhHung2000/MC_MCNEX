/*
 * drv_par.h
 *
 *  Created on: Apr 28, 2025
 *      Author: R&D
 */

#ifndef DRV_PAR_H_
#define DRV_PAR_H_
#include "main.h"


typedef struct _c_CGW1_msgTypeTag
{
	uint8_t CF_Gway_IGNSw : 3;
	uint8_t unused0 : 5;
	uint8_t unused1 : 4;
	uint8_t CF_Gway_TrunkTgSw : 2;
	uint8_t unused2 : 2;
	uint8_t unused3 : 8;
	uint8_t unused4 : 8;
	uint8_t unused5 : 8;
	uint8_t unused6 : 8;
	uint8_t unused7 : 6;
	uint8_t CF_Gway_InhibitRMT : 2;
	uint8_t unused8 : 4;
	uint8_t CF_Gway_ParkBrakeSw : 2;
	uint8_t unused9 : 2;
} _c_CGW1_msgType;
typedef struct _c_ENG_COND_msgTypeTag
{
  uint8_t ENG_STAT : 3;
  uint8_t IGN_STAT : 3;
  uint8_t COMM_CTRL_INFO : 2;
} _c_ENG_COND_msgType; // cjlee_211012_EngCondition

typedef struct _c_EMS16_msgTypeTag
{
	uint8_t unused0 : 8;
	uint8_t unused1 : 8;
	uint8_t unused2 : 8;
	uint8_t unused3 : 4;
	uint8_t ENG_STAT : 3;
	uint8_t unused4 : 1;
} _c_EMS16_msgType;

typedef union _c_CGW1_bufTag
{
  uint8_t _c[8];
  _c_CGW1_msgType CGW1;
} _c_CGW1_buf;

typedef union _c_EMS16_bufTag
{
  uint8_t _c[4];
  _c_EMS16_msgType EMS16;
} _c_EMS16_buf;
typedef union _c_ENG_COND_bufTag
{
  uint8_t _c[1];
  _c_ENG_COND_msgType ENG_COND;
} _c_ENG_COND_buf; // cjlee_211012_EngCondition
extern _c_EMS16_buf  EMS16;
extern _c_CGW1_buf  CGW1;
extern _c_ENG_COND_buf  ENG_COND; // cjlee_211012_EngCondition
#endif /* DRV_PAR_H_ */
