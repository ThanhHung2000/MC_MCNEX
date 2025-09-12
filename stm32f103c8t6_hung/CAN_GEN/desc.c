#include "desc.h"
#include"tp_cfg.h"
#include "appdesc.h"
#include "il_par.h"
# define DESC_TPCONTEXT_PARAM_VALUE                                  ((uint8_t)0)
#define kDescPrimBufferLen                                           256U
/* Used for additional information about each network */
#define kDescUsdtNetInvalidDescContext                               (uint8_t)(0xFF)
# define kDescTpNumContexts                                          1
# define DescUsdtNetStartReception(infoPool)                         (DescUsdtNetAbsStartReception((infoPool)))
/* Default setting for "response on request" (phys - yes, func - no) */
#define kDescDefaultResOnReq                                         0x01
#define kDescResOnAllReq                                             0x03
#define kDescSvcHeadNumItems                                         19U
#define kDescInvalidSvcHandle                                        (uint8_t)kDescSvcHeadNumItems
#define kDescSvcIdMapSize                                            70U
#define kDescNumSvcHeadConstGapBits                                  1U
#define kDescNumSvcHeadSessionGapBits                                3
/* SvcInst table size. */
#define kDescSvcInstNumItems                                         29
/* SvcInstHeadEx table size. */
#define kDescSvcInstHeadExtNumItems                                  22
#define kDescInvalidSvcInstHandle                                    (uint8_t)kDescSvcInstNumItems

# define V_BOOL_EXPR(exp)                                            ((uint8_t)((exp)?kDescTrue:kDescFalse))
#define kDescStateGroupNumTransition                                  9
#define kDescStateNoTransition                                       ((uint8_t)kDescStateGroupNumTransition)
#define DescOemOnValidService(iContext)                              /* Not used */
# define DescExtractResExtHeadLen(resExtHeadLen)                     (resExtHeadLen)
# define DescExtractReqExtHeadLen(reqExtHeadLen)                     (reqExtHeadLen)

# define kDescOemNrcParamIdNotSupportedInSession                     kDescNrcRequestOutOfRange

# define kDescP2InitValue                                            kDescP2Ticks
# define kDescP2ReloadValue                                          kDescP2StarTicks

#define DescActivateT2Timer(iContext)                                (g_descT2Timer[iContext] = kDescP2InitValue)

static uint16_t  g_descT2Timer[kDescNumContexts];

static uint8_t g_descBuffer[kDescTpNumContexts][kDescPrimBufferLen];
/* forward declaration of t_descUsdtNetInfoPool */
typedef struct t_descUsdtNetInfoPoolTag t_descUsdtNetInfoPool;
typedef struct t_descUsdtNetInfoPoolTag* t_descUsdtNetInfoPoolPtr;
static t_descUsdtNetResult DescUsdtNetAbsStartReception(t_descUsdtNetInfoPoolPtr infoPool);
static void DescDispatcher(void );
static uint8_t DescFindSvc(uint8_t reqSvcId);

/* Network dependent data type */
struct t_descUsdtNetInfoPoolTag
{
  t_descUsdtNetBus        busInfo;
  t_descUsdtNetReqType    reqType;
  t_descUsdtNetResType    resType;
  uint8_t                 busHandle;
  uint8_t                 descHandle;
  uint16_t                dataLength;
  uint8_t *               reqDataPtr;
  uint8_t *               resDataPtr;
};
/* ---- Context management state machine ---- */
typedef struct
{
  t_descUsdtNetResult txState;/* Tp ErrorCode */
  uint8_t         contextMode                :3;/* 0x00 - Normal, 0x01 - PIDListProcess */
  uint8_t         isApplError                :1;/* 0x00 - CANdesc NR, 0x01 - APPL NR */
  uint8_t         dummy                      :4;
} DescContextCtrl;
/* ---- Interrupt context management state machine ---- */
typedef struct
{
  t_descUsdtNetInfoPoolPtr infoPoolPtr;
  uint8_t             activity                 ;/* 0x00 - Idle, 0x01 - ActiveRx, 0x02 - ActiveProcess, 0x04 - ActiveTxSingleResponse ,0x08 - ActiveTxPeriodicRes, 0x10 - ActiveTxRoeResponder */
  uint8_t        isContextLocked        :1;
} DescInterruptContextCtrl;

typedef struct DescStateInfoTag
{
  DescStateGroup stateSession : 5;
  DescStateGroup stateSecurityAccess : 2;
  DescStateGroup stateGap_0 : 1;
} DescStateInfo;
static t_descUsdtNetInfoPool  g_descUsdtNetInfoPoolIsoTp[kDescNumContexts];
/* State machine of each received request */
static DescContextCtrl          g_descContextCtrl[kDescNumContexts];
static DescInterruptContextCtrl g_descInterruptContextCtrl[kDescNumContexts];
/* Context information to accompany the complete request processing */
static DescMsgContext     g_descMsgContext[kDescNumContexts];
/* Store the service entry index for the current request */
static uint8_t      g_descCurReqSvc[kDescNumContexts];
/* Store the instance entry index for the current request */
static uint8_t   g_descCurReqSvcInst[kDescNumContexts];
/* Store detected diagnostic error */
static DescNegResCode     g_descNegResCode[kDescNumContexts];

/* Security access sequence state */
static uint8_t                   g_descIsKeyAwaited;
static uint8_t g_descRcrrpBuffer[kDescNumContexts][3];
static DescStateInfo g_descCurState;
static void DescStateOnceInit(void);
static void DescStateOnceInit(void);
typedef struct
{
  uint8_t                reqHeadByteSpec   :8; /* Each bit represents sub-function/identifier - 0: identifier; 1: any subfunction */
  uint8_t                reqHeadExLen      :4; /* Request HeadEx length up to 7 Byte  */
  uint8_t                resHeadExLen      :4; /* Response HeadEx length up to 7 Byte */


  uint8_t             reqType           :2; /* 0x01: Phys 0x02: Func */
  uint8_t             resOnReq          :2; /* 0x01: Phys 0x02: Func */

  uint8_t             suppPosRes        :1; /* 0x00: No   0x01: Yes  */
  uint8_t             isReqHeadExtEchoed:1; /* 0x00: No   0x01: Yes  */
  uint8_t             hasSubFunction    :1; /* 0x00: No   0x01: Yes  */
  uint8_t             svcHeadPlaceHolder:kDescNumSvcHeadConstGapBits; /* Gap holder  */
  DescStateGroup          checkSessionState :kDescNumStateSession;/* State group Session on SId level */
  uint8_t             sessionPlaceHolder:kDescNumSvcHeadSessionGapBits; /* Gap holder  */

  uint8_t                  minReqLength;


/* First item in Service Instance table */
  uint8_t        svcInstFirstItem;
  uint8_t svcInstHeadExtFirstItem;
} DescSvcHead;

typedef struct
{
  uint16_t             reqLen;
  DescMsgAddInfo         msgAddInfo;
  DescStateInfo          checkState;
  uint8_t      setStateIndex;
  uint8_t   postHandlerRef;
  DescMainHandler        mainHandler;
} DescSvcInst;

/* State machine of CANdesc described by CANdelaStudio */
static uint8_t DescFindSvcInst(uint8_t * reqHeadPtr,  DescSvcHead   * pSvcHead, uint8_t* failedByteMask);
static uint8_t DescGetSvcInstHeadExtEntrySize( DescSvcHead   * pSvcHead);

static void  DescOemStartSessionDefault(DescMsgContext* pMsgContext);
static void  DescOemStartSessionProgramming(DescMsgContext* pMsgContext);
static void  DescOemStartSessionExtended(DescMsgContext* pMsgContext);
static void  DescOemStartSessionProgramming_Session_for_KWP2000(DescMsgContext* pMsgContext);
static void  DescOemStartSessionExtended_Session_for_KWP2000(DescMsgContext* pMsgContext);
static void  DescOemStopDiagnosticSession(DescMsgContext* pMsgContext);
static void  DescReadDataByIdentifier(DescMsgContext* pMsgContext);
static void  DescOemGetRequestSeed_Unlock_Level_1(DescMsgContext* pMsgContext);
static void  DescOemSendSendKey_Unlock_Level_1(DescMsgContext* pMsgContext);
static void  DescOemCommonCommCtrl(DescMsgContext* pMsgContext);
static void  DescOemEnableCommunication(DescMsgContext* pMsgContext);
static void  DescRoutineControlByIdentifier(DescMsgContext* pMsgContext);
static void  DescOemProcessTesterPresent(DescMsgContext* pMsgContext);
//static void  DescOemPostStartSessionDefault(uint8_t status);
//static void  DescOemPostStartSessionProgramming(uint8_t status);
//static void  DescOemPostStartSessionExtended(uint8_t status);
//static void  DescOemPostStartSessionProgramming_Session_for_KWP2000(uint8_t status);
//static void  DescOemPostStartSessionExtended_Session_for_KWP2000(uint8_t status);
//static void  DescPostReadDataByIdentifier(uint8_t status);
//static void  DescOemPostCommonCommCtrl(uint8_t status);
//static void  DescOemPostEnableCommunication(uint8_t status);
static DescNegResCode DescCheckState( DescStateInfo  * refState);
/* Function prototypes */
static void DescOemPrepareSessionControl(DescMsgContext *pMsgContext, DescStateGroup targetSession);
static void DescOemOnInvalidReq_27(void);
#define DescOemOnInvalidRequest(iContext)                          (DescOemOnInvalidReq_27())

/* &~MsgTables */
/* Table of reference indexes for fast SID look-up. */
static  uint8_t  g_descSidMap[kDescSvcIdMapSize] =
{
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0x00, 0x01, 0xFF, 0xFF, 0x02, 0xFF, 0xFF, 0xFF, 0xFF, 0x03, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0x04, 0xFF, 0x05, 0x06, 0xFF, 0xFF, 0xFF, 0x07, 0x08, 0x09, 0xFF, 0xFF, 0xFF, 0xFF, 0x0A, 0xFF,
  0xFF, 0x0B, 0xFF, 0xFF, 0x0C, 0xFF, 0x0D, 0x0E, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0x10, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x11
};
/* Table of service ID relevant information. */
static  DescSvcHead  g_descSvcHead[kDescSvcHeadNumItems] =
{
  { 0x01, 1, 1, 0x03, 0x03, 1, 1, 1, 0x00, 0x1F, 0x00,  2,  0,  0 } /* $10 */,
  { 0x01, 1, 1, 0x03, 0x03, 1, 1, 1, 0x00, 0x1F, 0x00,  2,  5,  5 } /* $11 */,
  { 0x01, 0, 0, 0x03, 0x03, 0, 1, 0, 0x00, 0x0D, 0x00,  1,  7,  7 } /* $14 */,
  { 0x01, 1, 1, 0x03, 0x03, 1, 1, 1, 0x00, 0x0D, 0x00,  2,  8,  7 } /* $19 */,
  { 0x01, 0, 0, 0x03, 0x03, 0, 1, 0, 0x00, 0x1F, 0x00,  1, 10,  9 } /* $20 */,
  { 0x01, 0, 0, 0x03, 0x03, 0, 1, 0, 0x00, 0x1F, 0x00,  1, 11,  9 } /* $22 */,
  { 0x01, 0, 0, 0x03, 0x03, 0, 1, 0, 0x00, 0x0D, 0x00,  1, 12,  9 } /* $23 */,
  { 0x01, 1, 1, 0x03, 0x03, 1, 1, 1, 0x00, 0x1E, 0x00,  2, 13,  9 } /* $27 */,

  { 0x01, 0, 0, 0x03, 0x03, 1, 1, 0, 0x00, 0x1E, 0x00,  1, 15, 11 } /* $28 */,
  { 0x01, 0, 0, 0x03, 0x03, 0, 1, 0, 0x00, 0x1E, 0x00,  1, 16, 11 } /* $29 */,
  { 0x07, 2, 2, 0x03, 0x03, 0, 1, 0, 0x00, 0x1F, 0x00,  4, 17, 11 } /* $2E */,
  { 0x01, 0, 0, 0x03, 0x03, 1, 1, 1, 0x00, 0x1E, 0x00,  1, 21, 19 } /* $31 */,
  { 0x01, 0, 0, 0x03, 0x03, 0, 1, 0, 0x00, 0x1A, 0x00,  1, 22, 19 } /* $34 */,
  { 0x01, 0, 0, 0x03, 0x03, 0, 1, 0, 0x00, 0x1A, 0x00,  1, 23, 19 } /* $36 */,
  { 0x01, 0, 0, 0x03, 0x03, 0, 1, 0, 0x00, 0x1A, 0x00,  1, 24, 19 } /* $37 */,
  { 0x01, 0, 0, 0x03, 0x03, 0, 1, 0, 0x00, 0x1F, 0x00,  1, 25, 19 } /* $3D */,

  { 0x01, 1, 1, 0x03, 0x03, 1, 1, 1, 0x00, 0x1F, 0x00,  2, 26, 19 } /* $3E */,
  { 0x01, 1, 1, 0x03, 0x03, 1, 1, 1, 0x00, 0x1E, 0x00,  2, 27, 20 } /* $85 */,
  { 0x00, 0, 0, 0x00, 0x00, 0, 1, 0, 0x00, 0x00, 0x00,  1, 29, 22 } /* $FF */
};

/* Table of sub-service protocol information. */
static  uint8_t  g_descSvcInstHeadExt[kDescSvcInstHeadExtNumItems] =
{
  0x01, 0x02, 0x03, 0x05, 0x10 /* $10 */,
  0x01, 0x03 /* $11 */,
  0x01, 0x02 /* $19 */,
  0x01, 0x02 /* $27 */,
  0xF1, 0x05, 0xF1, 0x06, 0xF1, 0x07, 0xF1, 0x08 /* $2E */,
  0x00 /* $3E */,
  0x01, 0x02 /* $85 */
};
/* Table of service (instance) relevant information. */
static  DescSvcInst  g_descSvcInst[kDescSvcInstNumItems] =
{
  {   2, { 0x03, 0x03, 0 }, { 0x1F, 0x03, 0x00 },                      0,    4, DescOemStartSessionDefault } /* $10 $1 */,
  {   2, { 0x03, 0x03, 0 }, { 0x1F, 0x03, 0x00 },                      1,    5, DescOemStartSessionProgramming } /* $10 $2 */,
  {   2, { 0x03, 0x03, 0 }, { 0x1F, 0x03, 0x00 },                      2,    6, DescOemStartSessionExtended } /* $10 $3 */,
  {   2, { 0x03, 0x03, 0 }, { 0x1F, 0x03, 0x00 },                      3,    7, DescOemStartSessionProgramming_Session_for_KWP2000 } /* $10 $5 */,
  {   2, { 0x03, 0x03, 0 }, { 0x1F, 0x03, 0x00 },                      4,    8, DescOemStartSessionExtended_Session_for_KWP2000 } /* $10 $10 */,
  {   2, { 0x03, 0x03, 0 }, { 0x1F, 0x03, 0x00 },                      5,    0, ApplDescEcuResetHard } /* $11 $1 */,
  {   2, { 0x03, 0x03, 0 }, { 0x1F, 0x03, 0x00 },                      6,    0, ApplDescEcuResetSoft } /* $11 $3 */,
  {   4, { 0x03, 0x03, 0 }, { 0x0D, 0x03, 0x00 }, kDescStateNoTransition,    0, ApplDescClearDiagInfo } /* $14 */,

  {   3, { 0x03, 0x03, 0 }, { 0x0D, 0x03, 0x00 }, kDescStateNoTransition,    0, ApplDescReadDtcRNODTCBSM } /* $19 $1 */,
  {   3, { 0x03, 0x03, 0 }, { 0x0D, 0x03, 0x00 }, kDescStateNoTransition,    0, ApplDescReadDtcRDTCBSM } /* $19 $2 */,
  {   1, { 0x03, 0x03, 0 }, { 0x1F, 0x03, 0x00 },                      7,    0, DescOemStopDiagnosticSession } /* $20 */,
  {   0, { 0x03, 0x03, 0 }, { 0x1F, 0x03, 0x00 }, kDescStateNoTransition,    3, DescReadDataByIdentifier } /* $22 */,
  {   0, { 0x03, 0x03, 0 }, { 0x0D, 0x03, 0x00 }, kDescStateNoTransition,    0, ApplDescReadMemoryByAddress } /* $23 */,
  {   2, { 0x03, 0x03, 0 }, { 0x1E, 0x03, 0x00 }, kDescStateNoTransition,    0, DescOemGetRequestSeed_Unlock_Level_1 } /* $27 $1 */,
  {   6, { 0x03, 0x03, 0 }, { 0x1E, 0x03, 0x00 },                      8,    0, DescOemSendSendKey_Unlock_Level_1 } /* $27 $2 */,
  {   0, { 0x03, 0x03, 0 }, { 0x1E, 0x03, 0x00 }, kDescStateNoTransition,    1, DescOemCommonCommCtrl } /* $28 */,

  {   0, { 0x03, 0x03, 0 }, { 0x1E, 0x03, 0x00 }, kDescStateNoTransition,    2, DescOemEnableCommunication } /* $29 */,
  {   5, { 0x03, 0x03, 0 }, { 0x1F, 0x02, 0x00 }, kDescStateNoTransition,    0, ApplDescWriteDidCAMERA_System_On_Time } /* $2E $F1 $5 */,
  {   5, { 0x03, 0x03, 0 }, { 0x1F, 0x02, 0x00 }, kDescStateNoTransition,    0, ApplDescWriteDidCAMERA_Operating_Time_and_System_On } /* $2E $F1 $6 */,
  {   4, { 0x03, 0x03, 0 }, { 0x1F, 0x02, 0x00 }, kDescStateNoTransition,    0, ApplDescWriteDidCAMERA_Warning_Text_Language } /* $2E $F1 $7 */,
  {   4, { 0x03, 0x03, 0 }, { 0x1F, 0x02, 0x00 }, kDescStateNoTransition,    0, ApplDescWriteDidCAMERA_Reset_Counter } /* $2E $F1 $8 */,
  {   0, { 0x03, 0x03, 0 }, { 0x1E, 0x03, 0x00 }, kDescStateNoTransition,    0, DescRoutineControlByIdentifier } /* $31 */,
  {   0, { 0x03, 0x03, 0 }, { 0x1A, 0x02, 0x00 }, kDescStateNoTransition,    0, ApplDescRequestDownload } /* $34 */,
  {   0, { 0x03, 0x03, 0 }, { 0x1A, 0x02, 0x00 }, kDescStateNoTransition,    0, ApplDescTransferData } /* $36 */,

  {   1, { 0x03, 0x03, 0 }, { 0x1A, 0x02, 0x00 }, kDescStateNoTransition,    0, ApplDescRequestTransferExit } /* $37 */,
  {   0, { 0x03, 0x03, 0 }, { 0x1F, 0x02, 0x00 }, kDescStateNoTransition,    0, ApplDescWriteMemoryByAddress } /* $3D */,
  {   2, { 0x03, 0x03, 0 }, { 0x1F, 0x03, 0x00 }, kDescStateNoTransition,    0, DescOemProcessTesterPresent } /* $3E $0 */,
  {   2, { 0x03, 0x03, 0 }, { 0x1E, 0x03, 0x00 }, kDescStateNoTransition,    0, ApplDescControlDtcSettingEnable } /* $85 $1 */,
  {   2, { 0x03, 0x03, 0 }, { 0x1E, 0x03, 0x00 }, kDescStateNoTransition,    0, ApplDescControlDtcSettingDisable } /* $85 $2 */
};
/*******************************************************************************
* NAME:              DescUsdtNetIsoTpInitContext
*
* CALLED BY:         CANdesc
* PRECONDITIONS:
*
* DESCRIPTION:       Re-initialization
*
*
*******************************************************************************/
static void DescUsdtNetIsoTpInitContext(void)
{
  g_descUsdtNetInfoPoolIsoTp[DESC_TPCONTEXT_PARAM_VALUE].descHandle = kDescUsdtNetInvalidDescContext;
  g_descUsdtNetInfoPoolIsoTp[DESC_TPCONTEXT_PARAM_VALUE].reqDataPtr = g_descBuffer[DESC_TPCONTEXT_PARAM_VALUE];
  g_descUsdtNetInfoPoolIsoTp[DESC_TPCONTEXT_PARAM_VALUE].resDataPtr = g_descBuffer[DESC_TPCONTEXT_PARAM_VALUE];
  g_descUsdtNetInfoPoolIsoTp[DESC_TPCONTEXT_PARAM_VALUE].resType = kDescUsdtResponseNone;
  g_descInterruptContextCtrl[DESC_CONTEXT_PARAM_VALUE].isContextLocked = 0;
  g_descInterruptContextCtrl[DESC_CONTEXT_PARAM_VALUE].activity = kDescContextIdle;

}
void Dest_Init(void)
{
	DescUsdtNetIsoTpInitContext();
	DescStateOnceInit();
}
uint8_t * DescGetBuffer(uint16_t dataLenght)
{
	uint8_t * returnValue =(uint8_t *)NULL;
	t_descUsdtNetResult result;

    g_descUsdtNetInfoPoolIsoTp[DESC_TPCONTEXT_PARAM_VALUE].dataLength        = dataLenght;
    g_descUsdtNetInfoPoolIsoTp[DESC_TPCONTEXT_PARAM_VALUE].descHandle        = DESC_TPCONTEXT_PARAM_VALUE; /* Write used context */
    result = DescUsdtNetStartReception(&g_descUsdtNetInfoPoolIsoTp[DESC_TPCONTEXT_PARAM_VALUE]);/// ham nay lien quan den store frame
    if(result==kDescUsdtNetworkOk)
    {
    	returnValue = g_descUsdtNetInfoPoolIsoTp[DESC_TPCONTEXT_PARAM_VALUE].reqDataPtr;
    }
	return returnValue;

}
void DescStateTask(void)
{

	  /*-------------------------*/

	  /*-------------------------*/
	  g_descInterruptContextCtrl[DESC_CONTEXT_PARAM_VALUE].activity = kDescContextActiveRxEnd;
	  if(g_descInterruptContextCtrl[DESC_CONTEXT_PARAM_VALUE].activity == kDescContextActiveRxEnd)
	  {
	    /* Switch to process state */
		g_descInterruptContextCtrl[DESC_CONTEXT_PARAM_VALUE].activity = kDescContextActiveProcess;

	#if defined (DESC_ENABLE_DESCICN_SUPPORT)
	    /* Check if this instance is a internal request */
	    if(g_descInterruptContextCtrl[DESC_CONTEXT_PARAM_VALUE].infoPoolPtr->reqType != kDescUsdtNetReqTypeInternal)
	#endif
	    {
	      /* transfer addressing information from infoPool into MsgContext */
	      if (g_descInterruptContextCtrl[DESC_CONTEXT_PARAM_VALUE].infoPoolPtr->reqType == kDescUsdtNetReqTypeFunctional)
	      {
	        /* Specify that the current request was actually a functional one */
	        g_descMsgContext[DESC_CONTEXT_PARAM_VALUE].msgAddInfo.reqType = kDescFuncReq;
	      }
	      else
	      {
	        /* Set request addressing method */
	        g_descMsgContext[DESC_CONTEXT_PARAM_VALUE].msgAddInfo.reqType = kDescPhysReq;
	      }
	      /* Copy the SID for RCR-RP response.
	       * necessary for multi session diag to know which one will be sent
	       */
	      g_descRcrrpBuffer[DESC_CONTEXT_PARAM_VALUE][1] = *g_descInterruptContextCtrl[DESC_CONTEXT_PARAM_VALUE].infoPoolPtr->reqDataPtr;

	      /* Activate always the T2 timer. Once the timeout occurs,
	       * there will be decision about sending a response or not.
	       */
	      DescActivateT2Timer(DESC_CONTEXT_PARAM_VALUE);

	#if defined(DESC_ENABLE_ALL_RES_AFTER_RCRRP)
	      /*Reset Rcr-Rp Flag*/
	      g_descInterruptContextCtrl[DESC_CONTEXT_PARAM_VALUE].rcrRpSent = 0;
	#endif
	    }
	#if defined (DESC_ENABLE_DESCICN_SUPPORT)
	    else
	    {
	      /* Set request addressing method always to simulate physical reception */
	      g_descMsgContext[DESC_CONTEXT_PARAM_VALUE].msgAddInfo.reqType = kDescPhysReq;
	    }
	#endif
		/* Stores the length */
		g_descMsgContext[0].reqDataLen = g_descInterruptContextCtrl[0].infoPoolPtr->dataLength;

		/* Clear the error code */
		//g_descNegResCode[DESC_CONTEXT_PARAM_VALUE] = kDescNrcNone;
		/* Process the dispatcher */
		DescDispatcher();
	  }
}
/*******************************************************************************
* NAME:              DescUsdtNetAbsStartReception
*
* CALLED BY:         Desc Network Layer
* PRECONDITIONS:
*
* DESCRIPTION:       StartOfFrame reception function
*
*
*******************************************************************************/
static t_descUsdtNetResult DescUsdtNetAbsStartReception(t_descUsdtNetInfoPoolPtr infoPool)
{
  t_descUsdtNetResult returnValue = kDescUsdtNetworkFailed;

  //if(g_descInterruptContextCtrl[0].isContextLocked == 0)
  {
    {
      /* Lock the buffer */
      //g_descInterruptContextCtrl[0].isContextLocked = 1;
      g_descInterruptContextCtrl[0].activity |= kDescContextActiveRxBegin;
      /* Store infoPool reference */
      g_descInterruptContextCtrl[0].infoPoolPtr = infoPool;//

      returnValue = kDescUsdtNetworkOk;
    }
  }
  return returnValue;
}
static void DescDispatcher(void)
{
	  /* Code optimization */
	  uint8_t * msg;
	  msg = g_descInterruptContextCtrl[0].infoPoolPtr->reqDataPtr;/// cai nay là dau cuoi
	  g_descContextCtrl[0].isApplError = 0;
	  /* Set default response on request type (phys - yes, func - no) */
	  g_descMsgContext[0].msgAddInfo.resOnReq = kDescDefaultResOnReq;
	  /* Update the context phase */
	  g_descInterruptContextCtrl[0].activity = kDescContextActiveProcess;
	  /*---------------------------------------------*/
	  /*              Search service ID              */
	  /*---------------------------------------------*/
	  /* If service not supported */
	  /* -------------------------*/
	  g_descCurReqSvc[0] = DescFindSvc(msg[0]);
	  if(g_descCurReqSvc[0] < kDescInvalidSvcHandle)
	  {
			DescSvcHead   * refDescSvcHead;
			refDescSvcHead = &g_descSvcHead[g_descCurReqSvc[0]];
		    /* -----------------------------------------------*/
		    /*      Set the default state for the response    */
		    /* -----------------------------------------------*/
		    g_descMsgContext[0].msgAddInfo.resOnReq = refDescSvcHead->resOnReq;
			/*------------------------------------*/
			/*     Service ID session check       */
			/*------------------------------------*/

			if ((refDescSvcHead->checkSessionState & g_descCurState.stateSession) != 0)// Hung dang code o doan nay
			{

		        if(g_descMsgContext[0].reqDataLen >= refDescSvcHead->minReqLength)
		        {
		            uint8_t supPosResBit;
		            uint8_t svcInstFailedBytePosMask = 0;
		            /* SID is no more of relevance - position to the first sub-service byte */
		            msg++;
		            /*---------------------------------------------*/
		            /*   Suppress positive response bit calc       */
		            /*---------------------------------------------*/
		            supPosResBit = refDescSvcHead->suppPosRes;
		            /* compose 0x80 or 0x00 depending on the CDD info */
		            supPosResBit <<= 7;
		            /* Extract the information to the application */
		            g_descMsgContext[0].msgAddInfo.suppPosRes =  (((supPosResBit & *msg)!= 0)?0x01:0x00);
		            /* Mask out the SPRMB */
		            *msg &= (uint8_t)(~supPosResBit);/* compose 0x80 or 0x00 depending on the CDD info */
		            /*---------------------------------------------*/
		            /*              Search service instance        */
		            /*---------------------------------------------*/
		            /* if at least one instance is defined - check if it is valid */

		            if(refDescSvcHead->reqHeadExLen > 0)
		            { /*lint -e{644}*/
		              g_descCurReqSvcInst[0] = (uint8_t)DescFindSvcInst(msg, refDescSvcHead, &svcInstFailedBytePosMask);
		            }
		            else
		            {
		              g_descCurReqSvcInst[0] = refDescSvcHead->svcInstFirstItem;
		            }
		            if((g_descCurReqSvcInst[0]) < kDescInvalidSvcInstHandle)
		            {

						DescSvcInst   * refDescSvcInst;
						refDescSvcInst = &g_descSvcInst[g_descCurReqSvcInst[0]];
			            /* -----------------------------------------------*/
			            /*      Set the default state for the response    */
			            /* -----------------------------------------------*/
			            /* Set service instance specific addressing information */
			            g_descMsgContext[0].msgAddInfo.resOnReq = refDescSvcInst->msgAddInfo.resOnReq;

			            /*---------------------------------------------*/
			            /*  Service instance addressing method check   */
			            /*---------------------------------------------*/
			            if ((refDescSvcInst->msgAddInfo.reqType &
			                 g_descMsgContext[0].msgAddInfo.reqType) != 0)
			            {
			              /*---------------------------------------------*/
			              /*      Service instance related checks        */
			              /*---------------------------------------------*/
			              /* Request length (top limit) check */

			              /*---------------------------------------------*/
			              /*      Service instance related checks        */
			              /*---------------------------------------------*/
			              /* Generated session check */
				#if defined (DESC_ENABLE_SESSION_FORMAT_STATE_CHECK)
						  if ((refDescSvcInst->checkState.stateSession & g_descCurState.stateSession) != 0)
				#endif
						  {
							/* Request length (top limit) check */
							/* If the table stored length is not zero -
							check it for matching with the request length */
							if((refDescSvcInst->reqLen == 0) ||
							  (refDescSvcInst->reqLen == g_descMsgContext[0].reqDataLen))
							{
				#if 1
								GPIOA->ODR ^=(1<<5);
							  /* Generated state checks */
							  g_descNegResCode[DESC_CONTEXT_PARAM_VALUE] = DescCheckState(&(refDescSvcInst->checkState));

							  /* If any error was detected - go out */
							  if(g_descNegResCode[DESC_CONTEXT_PARAM_VALUE] == kDescNrcNone)
				#endif
							  {
								/* Release the PostHandler call feature */
								g_descContextCtrl[DESC_CONTEXT_PARAM_VALUE].isApplError = 1;

								/* OEM specific hook for pre-service execution */
								DescOemOnValidService(DESC_CONTEXT_PARAM_VALUE);

								/* If any error was detected - go out */
								/* Do not use pre-processor encapsulation - since good compilers would optimize this check. */
								if(g_descNegResCode[DESC_CONTEXT_PARAM_VALUE] == kDescNrcNone)
								{
				#if defined (DESC_ENABLE_PREHANDLER_USAGE)
								  /*---------------------------------------------*/
								  /*    Application specific condition check     */
								  /*---------------------------------------------*/
								  /* Call the pre handler */
								  g_descPreHandlerTable[refDescSvcInst->preHandlerRef](DESC_CONTEXT_PARAM_ONLY);
								  /* If any error was detected - go out */
								  if(g_descNegResCode[DESC_CONTEXT_PARAM_VALUE] == kDescNrcNone)
				#endif
								  {
									/* Prepare the application information */
									g_descMsgContext[DESC_CONTEXT_PARAM_VALUE].reqData    = (DescMsg)(msg + DescExtractReqExtHeadLen(refDescSvcHead->reqHeadExLen));
									/* This is the extension of the response header (the header without the resposne SID), so for the complete length - add 1 */
									g_descMsgContext[DESC_CONTEXT_PARAM_VALUE].resData    = (DescMsg)(msg + DescExtractResExtHeadLen(refDescSvcHead->resHeadExLen));
									/* The length contains up to now the whole request length, so just substract the header length */
									g_descMsgContext[DESC_CONTEXT_PARAM_VALUE].reqDataLen -= (DescMsgLen)(DescExtractReqExtHeadLen(refDescSvcHead->reqHeadExLen) + 1);
									/* Zero the response length - optimization for no data responses */
									g_descMsgContext[DESC_CONTEXT_PARAM_VALUE].resDataLen = 0;

									/*---------------------------------------------*/
									/*      Call the main handler function         */
									/*---------------------------------------------*/
				#if defined (DESC_ENABLE_PERMANENT_MAINHANDLER_MULTICALL)
									/* If each service needs it - start always here to save ROM for the application */
									_DescStartRepeatedServiceCall(DESC_CONTEXT_PARAM_VALUE, refDescSvcInst->mainHandler);
				#endif
									refDescSvcInst->mainHandler(&g_descMsgContext[DESC_CONTEXT_PARAM_VALUE]);

									//DescAssertContext((g_descMsgContext[DESC_CONTEXT_PARAM_VALUE].iContext == DESC_CONTEXT_PARAM_VALUE), kDescAssertContextIdWasModified);
									//DescAssertContext((DESC_CONTEXT_PARAM_VALUE < kDescNumContexts), kDescAssertInvalidContextId);
									/* Skip the processing done call - let the application decide when */
									return;
								  }
								}/* else: error code already set - skip further processing */
							  }
				#if 1
							  else
							  {
								/* Correct the negative response code dependent on the sub-function instantiation */
								if((g_descNegResCode[DESC_CONTEXT_PARAM_VALUE] == kDescNrcSubfunctionNotSupportedInActiveSession)
								  &&(refDescSvcHead->hasSubFunction == 0))
								{
								  g_descNegResCode[DESC_CONTEXT_PARAM_VALUE] = kDescOemNrcParamIdNotSupportedInSession;
								}

								/* EXAMPLE: Additional activities may be necessary */
								DescOemOnInvalidRequest(DESC_CONTEXT_PARAM_VALUE);
							  }
				#endif
							}
							else
							{
							  g_descNegResCode[DESC_CONTEXT_PARAM_VALUE] = kDescNrcInvalidFormat;
							  /* EXAMPLE: Additional activities may be necessary */
							  DescOemOnInvalidRequest(DESC_CONTEXT_PARAM_VALUE);
							}
						  }

			            }

		           }
		        }
			}
	  }
	  else
	  {
		//  GPIOA->ODR &=~(1<<5);
	  }
}

/******************************************************************************
* NAME:              DescFindSvc
*
* CALLED BY:         Dispatcher,FuncReception
* PRECONDITIONS:
*
* DESCRIPTION:       Search a service ID.
*
*******************************************************************************/
static uint8_t DescFindSvc(uint8_t reqSvcId)
{
  uint8_t result  = kDescInvalidSvcHandle;
  uint8_t     byteVar = 0;

  /* check if it is in the correct range (0x00-0x3f or 0x80-bf => bit2 must be 0) */
  /* check if it is not bigger that the maximum defined SID */
  if (((reqSvcId & kDescPosResIdOffset) == 0)&&
       (reqSvcId <= kDescMaxReqSid))
  {
    /* find in which response range is the request. */
    if ((reqSvcId & 0x80) != 0)
    {
      byteVar = kDescPosResIdOffset;
    }
    result = g_descSidMap[reqSvcId - byteVar];
  }

  return result;
}
/*******************************************************************************
* NAME:              DescStateOnceInit
*
* CALLED BY:         DescInit
* PRECONDITIONS:
*
* DESCRIPTION:       Initilizes the state subcomponent common data.
*
*******************************************************************************/
static void DescStateOnceInit(void)
{
  DescStateInit();
}

void  DescStateInit(void)
{
  g_descCurState.stateSession = kDescStateSessionDefault;
  g_descCurState.stateSecurityAccess = kDescStateSecurityAccessLocked;
}
/******************************************************************************
* NAME:              DescFindSvcInst
*
* CALLED BY:         Dispatcher
* PRECONDITIONS:
*
* DESCRIPTION:       Linear search for service instance.
*
*******************************************************************************/
static uint8_t DescFindSvcInst(uint8_t * reqHeadPtr,  DescSvcHead   * pSvcHead, uint8_t* failedByteMask)
{
  uint8_t        iter;
  uint8_t        reqSvcInstHandle;
  uint8_t        offset;
  uint8_t            currColFound;
  uint8_t            incStep;
  uint8_t                isEqual;
  incStep = DescGetSvcInstHeadExtEntrySize(pSvcHead);
  /* Mark subserviceInstance handle as invalid */
  reqSvcInstHandle = kDescInvalidSvcInstHandle;
  /* No error still found */
  *failedByteMask = 0;
  offset = 0;
	  for(iter = pSvcHead->svcInstHeadExtFirstItem;
		  iter < (pSvcHead + 1)->svcInstHeadExtFirstItem;
		  iter+= incStep)
	  {
		currColFound = 0;
		do
		{
		  isEqual = V_BOOL_EXPR(reqHeadPtr[currColFound] == g_descSvcInstHeadExt[iter + currColFound]);
		  currColFound++;
		}
		while ((currColFound < pSvcHead->reqHeadExLen)&&(isEqual != kDescFalse));
		if(isEqual != kDescFalse)
		{
		  reqSvcInstHandle = (uint8_t)(offset + pSvcHead->svcInstFirstItem);
		  break;
		}
		else
		{
		  /* Compensate post increment */
		  currColFound--;
		  /* Check for deepest error column */
		  if(currColFound > *failedByteMask)
		  {
			/* Use 0x02, to skip the SID bit */
			*failedByteMask = currColFound;
		  }
		  /* Check if it makes sense to continue the search */
		  if(reqHeadPtr[currColFound] < g_descSvcInstHeadExt[iter + currColFound])
		  {
			/* That was it */
			break;
		  }
		}
		/* Increment the reference */
		offset++;
	  }

  /* Use 0x02, to skip the SID bit */
  *failedByteMask = (uint8_t)(0x02 << *failedByteMask);
  return reqSvcInstHandle;
}
/* NAME:              DescGetSvcInstHeadExtEntrySize
*
* CALLED BY:         Dispatcher
* PRECONDITIONS:
*
* DESCRIPTION:       Binary search for service instance.
*
*******************************************************************************/
static uint8_t DescGetSvcInstHeadExtEntrySize( DescSvcHead   * pSvcHead)
{
  return (uint8_t)((pSvcHead->isReqHeadExtEchoed != 0)?
                  (pSvcHead->reqHeadExLen):(pSvcHead->reqHeadExLen + pSvcHead->resHeadExLen));
}

/*  ********************************************************************************
 * Function name:DescOemStartSessionDefault
 * Description:Processes the session change request, parametrizing the common processing
 * function.
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext:
 *       - Contains all request properties.
 *       - Access type: read/write
 * Particularitie(s) and limitation(s): none
 ********************************************************************************  */
static void  DescOemStartSessionDefault(DescMsgContext* pMsgContext)
{
	//GPIOA->ODR ^=(1<<5);
	DescOemPrepareSessionControl(pMsgContext, kDescStateSessionDefault);
}
/*  ********************************************************************************
 * Function name:DescOemStartSessionProgramming
 * Description:Processes the session change request, parametrizing the common processing
 * function.
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext:
 *       - Contains all request properties.
 *       - Access type: read/write
 * Particularitie(s) and limitation(s): none
 ********************************************************************************  */
static void  DescOemStartSessionProgramming(DescMsgContext* pMsgContext)
{
  //DescOemPrepareSessionControl(pMsgContext, kDescStateSessionProgramming);
}
/*  ********************************************************************************
 * Function name:DescOemStartSessionExtended
 * Description:Processes the session change request, parametrizing the common processing
 * function.
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext:
 *       - Contains all request properties.
 *       - Access type: read/write
 * Particularitie(s) and limitation(s): none
 ********************************************************************************  */
static void  DescOemStartSessionExtended(DescMsgContext* pMsgContext)
{
 // DescOemPrepareSessionControl(pMsgContext, kDescStateSessionExtended);
}


/*  ********************************************************************************
 * Function name:DescOemStartSessionProgramming_Session_for_KWP2000
 * Description:Processes the session change request, parametrizing the common processing
 * function.
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext:
 *       - Contains all request properties.
 *       - Access type: read/write
 * Particularitie(s) and limitation(s): none
 ********************************************************************************  */
static void  DescOemStartSessionProgramming_Session_for_KWP2000(DescMsgContext* pMsgContext)
{
 // DescOemPrepareSessionControl(pMsgContext, kDescStateSessionECUProgrammingMode);
}

/*  ********************************************************************************
 * Function name:DescOemStartSessionExtended_Session_for_KWP2000
 * Description:Processes the session change request, parametrizing the common processing
 * function.
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext:
 *       - Contains all request properties.
 *       - Access type: read/write
 * Particularitie(s) and limitation(s): none
 ********************************************************************************  */
static void  DescOemStartSessionExtended_Session_for_KWP2000(DescMsgContext* pMsgContext)
{
 // DescOemPrepareSessionControl(pMsgContext, kDescStateSessionExtendedDiagnosticMode);
}


/*  ********************************************************************************
 * Function name:DescOemGetRequestSeed_Unlock_Level_1
 * Description:Manages the security get seed function.
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext:
 *       - Contains all request properties.
 *       - Access type: read/write
 * Particularitie(s) and limitation(s): none
 ********************************************************************************  */
static void  DescOemGetRequestSeed_Unlock_Level_1(DescMsgContext* pMsgContext)
{
 // DescOemSecuritySeed(pMsgContext, kDescStateSecurityAccessUnlockedL1);
}

/*  ********************************************************************************
 * Function name:DescOemSendSendKey_Unlock_Level_1
 * Description:Manages the security send key function.
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext:
 *       - Contains all request properties.
 *       - Access type: read/write
 * Particularitie(s) and limitation(s): none
 ********************************************************************************  */
static void  DescOemSendSendKey_Unlock_Level_1(DescMsgContext* pMsgContext)
{
  //DescOemSecurityKey(pMsgContext, kDescStateSecurityAccessUnlockedL1);
}


/*  ********************************************************************************
 * Function name:DescOemPostStartSessionDefault (Service request header:$10 $1 )
 * Description: not available
 * Returns:  nothing
 * Parameter(s):
 *   - status:
 *       - Shows the status of the process accomplishment. Can be: kDescPostHandlerStateOk or kDescPostHandlerStateNegResSent or/and kDescPostHandlerStateTxFailed.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" may not be called (within this post-handler or
 * later before a main-handler is called).
 *   - The function "DescSetNegResponse" may not be called (within this post-handler or
 * later before at least a pre-handler is called).
 ********************************************************************************  */
//static void  DescOemPostStartSessionDefault(uint8_t status)
//{
//  //DescOemCommonSessionPostProcessing(status, kDescStateSessionDefault);
//}
///*  ********************************************************************************
// * Function name:DescOemPostStartSessionProgramming (Service request header:$10 $2 )
// * Description: not available
// * Returns:  nothing
// * Parameter(s):
// *   - status:
// *       - Shows the status of the process accomplishment. Can be: kDescPostHandlerStateOk or kDescPostHandlerStateNegResSent or/and kDescPostHandlerStateTxFailed.
// *       - Access type: read
// * Particularitie(s) and limitation(s):
// *   - The function "DescProcessingDone" may not be called (within this post-handler or
// * later before a main-handler is called).
// *   - The function "DescSetNegResponse" may not be called (within this post-handler or
// * later before at least a pre-handler is called).
// ********************************************************************************  */
//static void  DescOemPostStartSessionProgramming(uint8_t status)
//{
// // DescOemCommonSessionPostProcessing(status, kDescStateSessionProgramming);
//}
//
//
///*  ********************************************************************************
// * Function name:DescOemPostStartSessionExtended (Service request header:$10 $3 )
// * Description: not available
// * Returns:  nothing
// * Parameter(s):
// *   - status:
// *       - Shows the status of the process accomplishment. Can be: kDescPostHandlerStateOk or kDescPostHandlerStateNegResSent or/and kDescPostHandlerStateTxFailed.
// *       - Access type: read
// * Particularitie(s) and limitation(s):
// *   - The function "DescProcessingDone" may not be called (within this post-handler or
// * later before a main-handler is called).
// *   - The function "DescSetNegResponse" may not be called (within this post-handler or
// * later before at least a pre-handler is called).
// ********************************************************************************  */
//static void  DescOemPostStartSessionExtended(uint8_t status)
//{
//  //DescOemCommonSessionPostProcessing(status, kDescStateSessionExtended);
//}

/************************************************/
/* $3E Tester Present */
/************************************************/
/*******************************************************************************
* NAME:              DescOemProcessTesterPresent
*
* CALLED BY:         DescDispatcher
* PRECONDITIONS:
*
* DESCRIPTION:
*
*******************************************************************************/
static void  DescOemProcessTesterPresent(DescMsgContext *pMsgContext)
{
//#if (kDescNumContexts == 1)
//  /* Avoid warnings */
//  DESC_IGNORE_UNREF_PARAM(pMsgContext);
//#endif
//  DescProcessingDone(DESC_CONTEXT_PARAM_WRAPPER_ONLY(pMsgContext->iContext));
}
/*  ********************************************************************************
 * Function name:DescOemPostStartSessionProgramming_Session_for_KWP2000 (Service request header:$10 $5 )
 * Description: not available
 * Returns:  nothing
 * Parameter(s):
 *   - status:
 *       - Shows the status of the process accomplishment. Can be: kDescPostHandlerStateOk or kDescPostHandlerStateNegResSent or/and kDescPostHandlerStateTxFailed.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" may not be called (within this post-handler or
 * later before a main-handler is called).
 *   - The function "DescSetNegResponse" may not be called (within this post-handler or
 * later before at least a pre-handler is called).
 ********************************************************************************  */
//static void  DescOemPostStartSessionProgramming_Session_for_KWP2000(uint8_t status)
//{
// // DescOemCommonSessionPostProcessing(status, kDescStateSessionECUProgrammingMode);
//}
/*******************************************************************************
* NAME:              DescOemEnableCommunication
*
* CALLED BY:         Dispatcher
* PRECONDITIONS:
*
* DESCRIPTION:       Central communciation Dispatching
*
*******************************************************************************/
static void  DescOemEnableCommunication(DescMsgContext *pMsgContext)
{
//  DescNegResCode errorCode = kDescNrcNone;
//
//  /*Check Length*/
//  if (pMsgContext->reqDataLen != 1)
//  {
//    errorCode = kDescNrcConditionsNotCorrect;
//  }
//  else
//  {
//    /*CheckSubfunction*/
//    if( (pMsgContext->reqData[0]!= 0x01) && (pMsgContext->reqData[0] != 0x02) )
//    {
//      errorCode = kDescNrcConditionsNotCorrect;
//    }
//    else
//    {
//      /*Suppress Response?*/
//      if( pMsgContext->reqData[0] == 0x02)
//      {
//        /*No Response is sent*/
//        pMsgContext->msgAddInfo.suppPosRes  =  0x01;
//      }
//    }
//  }
//
//  if (errorCode == kDescNrcNone)
//  {
//    /*The subfunction is not needed anymore */
//    DescOemCommonCommCtrlProcess((vuint8)0x03, pMsgContext);
//  }
//  else
//  {
//    DescSetNegResponse(DESC_CONTEXT_PARAM_WRAPPER_FIRST(pMsgContext->iContext) errorCode);
//    DescProcessingDone(DESC_CONTEXT_PARAM_WRAPPER_ONLY(pMsgContext->iContext));
//  }
}
/*******************************************************************************
* NAME:              DescRoutineControlByIdentifier
*
* CALLED BY:         DescDispatcher
* PRECONDITIONS:
*
* DESCRIPTION:       Main handler for RID handling.
*
*******************************************************************************/
static void  DescRoutineControlByIdentifier(DescMsgContext* pMsgContext)
{
//  DescRidInstIndex   ridInstRef;
//  /* Use iContext when necessary */
//  DESC_CONTEXT_PARAM_DEF_LOCAL
//#if (kDescNumContexts > 1)
//  /* Use iContext as name to be able to use the DESC_CONTEXT_PARAM_VALUE macro */
//  DESC_CONTEXT_PARAM_VALUE = pMsgContext->iContext;
//#endif
//
//#if defined (DESC_ENABLE_RID_POSTHANDLER_USAGE)
//  /* Set reference to invalid (skip application post-handler call) */
//  g_descRidCurrInstRef = kDescInvalidRidInstHandle;
//#endif
//  /* Check minimum length (SubFunction and RID must be inside) */
//  if(pMsgContext->reqDataLen > 2)
//  {
//    ridInstRef = DescRidFindRoutineId(DescMake16Bit(pMsgContext->reqData[1], pMsgContext->reqData[2]));
//
//    /* Is the RID supported? */
//    // if(ridInstRef < kDescInvalidRidHandle) //cjlee_211012_EngCondition
//    if((ridInstRef < kDescInvalidRidHandle))
//    {
//     if((IlGetEngCond_ENG() != 0x03U) && (IlGetEngCond_IGN() == 0x03U))
//    {
//      /* analyze the RID specific information */
//      DescRidAnalyseRoutine(DESC_CONTEXT_PARAM_FIRST &(g_descRIDInfo[ridInstRef]), kDescTrue);
//      /* Dispatching successful?*/
//      if(g_descNegResCode[DESC_CONTEXT_PARAM_VALUE] == kDescNrcNone)
//      {
//        /* Check if sub-function valid (at all and for RID) */
//        ridInstRef = DescRidFindSubFunction(pMsgContext->reqData[0], ridInstRef);
//
//#if defined (DESC_ENABLE_RID_POSTHANDLER_USAGE)
//        /* Store the RID reference for later */
//        g_descRidCurrInstRef = (DescMemRidInstIndex)ridInstRef;
//#endif
//
//        /* Does the RID support the control type? */
//        if(ridInstRef < kDescInvalidRidInstHandle)
//        {
//          /* If dynamic request length OR valid one - process the service */
//          if((g_descRIDInstInfo[ridInstRef].reqDataLen == 0) ||
//             (pMsgContext->reqDataLen == g_descRIDInstInfo[ridInstRef].reqDataLen))
//          {
//            /* analyze the sub-function specific information */
//            DescRidAnalyseRoutine(DESC_CONTEXT_PARAM_FIRST &(g_descRIDInstInfo[ridInstRef].tinyInfo), kDescFalse);
//
//            /* Dispatching successful?*/
//            if(g_descNegResCode[DESC_CONTEXT_PARAM_VALUE] == kDescNrcNone)
//            {
//#if defined (DESC_ENABLE_PERMANENT_MAINHANDLER_MULTICALL)
//              /* Redirect main-handler call */
//              _DescStartRepeatedServiceCall(DESC_CONTEXT_PARAM_VALUE, g_descRIDInstInfo[ridInstRef].mainHandler);
//#endif
//              /* Skip the routine Id and sub-function */
//              pMsgContext->reqData+=3;
//              pMsgContext->resData+=3;
//              pMsgContext->reqDataLen-=3;
//              /* Set the context mode */
//              g_descContextCtrl[DESC_CONTEXT_PARAM_VALUE].contextMode = kDescContextModeRoutineControl;
//              /* Call the main-handler */
//              g_descRIDInstInfo[ridInstRef].mainHandler(pMsgContext);
//              /* Let the application to take care about the service */
//              return;
//            }
//            /*else - NRC is already registered */
//          }
//          else
//          {
//            /* Invalid service length */
//            g_descNegResCode[DESC_CONTEXT_PARAM_VALUE] = kDescNrcInvalidFormat;
//          }
//        }
//        else
//        {
//          /* the RID is available but the sub-function not */
//          g_descNegResCode[DESC_CONTEXT_PARAM_VALUE] = kDescNrcSubfunctionNotSupported;
//        }
//      }
//      /*else - NRC is already registered */
//    }
//    else
//    {
//          /* the eng_condition disable */
//          g_descNegResCode[DESC_CONTEXT_PARAM_VALUE] = kDescNrcConditionsNotCorrect;
//     }
//    }
//    else
//    {
//      /* the RID is not available but the sub-function not */
//      g_descNegResCode[DESC_CONTEXT_PARAM_VALUE] = kDescNrcRequestOutOfRange;
//    }
//  }
//  else
//  {
//    /* Too short service length */
//    g_descNegResCode[DESC_CONTEXT_PARAM_VALUE] = kDescNrcInvalidFormat;
//  }
//
//  /* Error Case */
//  DescProcessingDone(DESC_CONTEXT_PARAM_ONLY);
}
/*******************************************************************************
* NAME:              DescOemStopDiagnosticSession
*
* CALLED BY:         DescDispatcher/DescTask (in case of real answer is sent)
* PRECONDITIONS:
*
* DESCRIPTION:       Session management.
*
*******************************************************************************/
static void DescOemStopDiagnosticSession(DescMsgContext *pMsgContext)
{
  pMsgContext->resDataLen = pMsgContext->reqDataLen;
  /* Ask the application for permission to accept the session transition */
 // ApplDescCheckSessionTransition(DESC_CONTEXT_PARAM_WRAPPER_FIRST(pMsgContext->iContext) kDescStateSessionDefault, g_descCurState.stateSession);
}
/*******************************************************************************
* NAME:              DescOemCommonCommCtrl
*
* CALLED BY:         Dispatcher
* PRECONDITIONS:
*
* DESCRIPTION:       Central communciation Dispatching
*
*******************************************************************************/
static void  DescOemCommonCommCtrl(DescMsgContext *pMsgContext)
{
//  vuint8 reqInfo;
//  DescNegResCode errorCode = kDescNrcNone;
//
//  /* hgpark_221108_ES95486-02 Engine condition*/
//  if ( (IlGetEngCond_ENG() == 0x03U) || (IlGetEngCond_IGN() != 0x03U) )
//  {
//  	errorCode = kDescNrcConditionsNotCorrect;
//  }
//  else if (pMsgContext->reqDataLen >= 1)
//  {
//    switch(pMsgContext->reqData[0])
//    {
//    case 0x00:
//    case 0x03:
//    /*UDS like CommCtrl request*/
//
//      /*Check Length*/
//      if (pMsgContext->reqDataLen != 2)
//      {
//        errorCode = kDescNrcInvalidFormat;
//        break;
//      }
//
//      /*Check communicationType*/
//	 // cjlee 210820 if (pMsgContext->reqData[1] != kDescCommControlMsgAppl)
//     if ((pMsgContext->reqData[1] != kDescCommControlMsgAppl) && (pMsgContext->reqData[1] != kDescCommControlMsgTypeAll))
//      {
//        errorCode = kDescNrcRequestOutOfRange;
//        break;
//      }
//
//    /*Prepare Response*/
//      pMsgContext->resDataLen = 1;
//      pMsgContext->resData[0] = pMsgContext->reqData[0];
//
//      /*Set reqInfo - convert subfunc bit map into boolean info (e.g. EnableRxEnableTx SubFunc = 0x00 -> 0x03*/
//      reqInfo = (vuint8)((~pMsgContext->reqData[0])&0x03);
//       break;
//
//#if defined (DESC_ENABLE_COMM_SVC_29)
//    case 0x02:
//    case 0x01:
//      if(pMsgContext->msgAddInfo.suppPosRes != 0x00)
//      {
//       /*Response is sent*/
//        errorCode = kDescNrcSubfunctionNotSupported;
//        break;
//      }
//      /* Now consider default KWP response behavior */
//      if(pMsgContext->reqData[0] == 0x02)
//      {
//        /*No positive response is sent*/
//        pMsgContext->msgAddInfo.suppPosRes  =  0x01;
//      }
//      /*Check Length*/
//      if (pMsgContext->reqDataLen != 1)
//      {
//        errorCode = kDescNrcInvalidFormat;
//        break;
//      }
//      reqInfo = (vuint8) 0x00;/* Disable Rx and Tx */
//      break;
//#endif
//    default:
//      errorCode = kDescNrcSubfunctionNotSupported;
//      break;
//    }
//  }
//  else
//  {
//    errorCode = kDescNrcInvalidFormat;
//  }
//
//  if (errorCode == kDescNrcNone)
//  {
//    /*The subfunction is not needed anymore */
//    DescOemCommonCommCtrlProcess(reqInfo, pMsgContext);
//  }
//  else
//  {
//    DescSetNegResponse(DESC_CONTEXT_PARAM_WRAPPER_FIRST(pMsgContext->iContext) errorCode);
//    DescProcessingDone(DESC_CONTEXT_PARAM_WRAPPER_ONLY(pMsgContext->iContext));
//  }
}
/*******************************************************************************
* NAME:              DescReadDataByIdentifier
*
* CALLED BY:         DescTask
* PRECONDITIONS:
*
* DESCRIPTION:       Main handler for PID list handling.
*
*******************************************************************************/
static void  DescReadDataByIdentifier(DescMsgContext *pMsgContext)
{
//  /* Use iContext when necessary */
//  DESC_CONTEXT_PARAM_DEF_LOCAL
//
//#if (kDescNumContexts > 1)
//  /* Use iContext as name to be able to use the DESC_CONTEXT_PARAM_VALUE macro */
//  DESC_CONTEXT_PARAM_VALUE = pMsgContext->iContext;
//#endif
//
//#if defined (DESC_ENABLE_PERMANENT_MAINHANDLER_MULTICALL)
//  /* Disable this multiple service call */
//  _DescStartRepeatedServiceCall(DESC_CONTEXT_PARAM_VALUE, V_NULL);
//#endif
//  /* The length for this service shall be ZERO to allow the application (CANdesc)
//   * to check it */
//  /* Check if at least one PID is available (at least 2 Byte) */
//  /* Check if the length is multiple of 2, since each PID is 2 Byte.*/
//  if((g_descMsgContext[DESC_CONTEXT_PARAM_VALUE].reqDataLen > 1)&&
//    ((g_descMsgContext[DESC_CONTEXT_PARAM_VALUE].reqDataLen & 0x01) == 0))
//  {
//    /* Also the capacity of the ECU shall not be exceeded */
//    if((g_descMsgContext[DESC_CONTEXT_PARAM_VALUE].reqDataLen >> 1) <= kDescNumMaxPidList)
//    {
//      /* Set the CANdesc in a PID List mode */
//      g_descContextCtrl[DESC_CONTEXT_PARAM_VALUE].contextMode = kDescContextModePidList;
//      /* Copy the original structure */
//      g_descPidMsgContext[DESC_CONTEXT_PARAM_VALUE] = *pMsgContext;
//      /* Dispatch all PID */
//      DescPidDispatcher(DESC_CONTEXT_PARAM_ONLY);
//      /* Check if any error was found */
//      if(g_descNegResCode[DESC_CONTEXT_PARAM_VALUE] == kDescNrcNone)
//      {
//        /* Prepare for datat processing */
//#if defined (DESC_ENABLE_RES_RINGBUFFER)
//        /* Start from the begining */
//        g_descRingBufferCtrl[DESC_CONTEXT_PARAM_VALUE].txReadIndex = 0;
//        /* Write after the SID */
//        g_descRingBufferCtrl[DESC_CONTEXT_PARAM_VALUE].txWriteIndex = 1;
//        /* Once started transmission - there are no data rather than the PID inside (will be written later) */
//        /* g_descRingBufferCtrl[DESC_CONTEXT_PARAM_VALUE].bytesWritten = 0;*/
//        /* Initiate the none application data length - 1Byte SID*/
//        g_descRingBufferCtrl[DESC_CONTEXT_PARAM_VALUE].protocolDataLen = 1;
//        g_descPidProcessorState[DESC_CONTEXT_PARAM_VALUE].isPidProcessed = 0;
//#endif
//        /* Process the first PID
//        * stack utilization optimization */
//        g_descPidProcessorState[DESC_CONTEXT_PARAM_VALUE].isPidReady = 1;
//        return;
//      }
//    }
//    else
//    {
//      /* Too many PIDs */
//      g_descNegResCode[DESC_CONTEXT_PARAM_VALUE] = kDescOemNrcTooManyPIDs;
//    }
//  }
//  else
//  {
//    /* Invalid length */
//    g_descNegResCode[DESC_CONTEXT_PARAM_VALUE] = kDescNrcInvalidFormat;
//  }
//#if defined (DESC_ENABLE_PROTOCOL_KWP)
//  /* KWP shall not respond on failed functionally requested service $22 with NR $31 ($33 is mapped to $31)*/
//  if(g_descMsgContext[DESC_CONTEXT_PARAM_VALUE].msgAddInfo.reqType == kDescFuncReq)
//  {
//    switch(g_descNegResCode[DESC_CONTEXT_PARAM_VALUE])
//    {
//      case kDescNrcRequestOutOfRange:
//        /* Fall through */
//      case kDescNrcAccessDenied:
//        /* Suppress response */
//        g_descMsgContext[DESC_CONTEXT_PARAM_VALUE].msgAddInfo.resOnReq = 0;
//        break;
//      default:/* Normal processing */ break;
//    }
//  }
//#endif
//  /* Reaching this point means diagnostic error was found */
//  DescFinalProcessingDone(DESC_CONTEXT_PARAM_ONLY);
}
/*******************************************************************************
* NAME:              DescOemPrepareSessionControl
*
* CALLED BY:         CANdesc
* PRECONDITIONS:
*
* DESCRIPTION:       Prepares the response for DiagnosticSessionControl.
*
*******************************************************************************/
static void DescOemPrepareSessionControl(DescMsgContext *pMsgContext, DescStateGroup targetSession)
{
	if (targetSession == kDescStateSessionProgramming)
	{
		if ((IlGetEngCond_ENG() != 0x03U) && (IlGetEngCond_IGN() == 0x03U))
		{

		}
		else
		{
			DescSetNegResponse(kDescNrcConditionsNotCorrect);
			//DescProcessingDone((pMsgContext->iContext));
			return;
		}
	}
	ApplDescCheckSessionTransition(targetSession, g_descCurState.stateSession);
}
/*******************************************************************************
* NAME:              DescSetNegResponse
*
* CALLED BY:         any application function
* PRECONDITIONS:
*
* DESCRIPTION:       Sets the error.
*
*******************************************************************************/
void  DescSetNegResponse( DescNegResCode errorCode)
{
  //DescAssertContext((DESC_CONTEXT_PARAM_VALUE < kDescNumContexts),kDescAssertInvalidContextId);
  /* Ignore setting an error if already set */
  if (g_descNegResCode[0] == kDescNrcNone)
  {
    g_descNegResCode[0] = errorCode;
  }
}
/*******************************************************************************
* NAME:              DescOemStopDiagnosticSession
*
* CALLED BY:         DescDispatcher/DescTask (in case of real answer is sent)
* PRECONDITIONS:
*
* DESCRIPTION:       Session management.
*
*******************************************************************************/


/*******************************************************************************
* NAME:              DescProcessingDone
*
* CALLED BY:         Dispatch,  any application function
* PRECONDITIONS:
*
* DESCRIPTION:       Depending on the current situation, provides the right
*                    action.
*******************************************************************************/
void  DescProcessingDone(void)
{
	uint8_t a=0x00U;
	a +=1;
//  DescAssertContext((DESC_CONTEXT_PARAM_VALUE < kDescNumContexts),kDescAssertInvalidContextId);
//
//  switch(g_descContextCtrl[DESC_CONTEXT_PARAM_VALUE].contextMode)
//  {
//    case kDescContextModeNormal:
//      /* Finalize the request processing */
//      DescFinalProcessingDone(DESC_CONTEXT_PARAM_ONLY);
//      break;
//# if defined (DESC_ENABLE_PID_LIST_MODE) ||
//     defined (DESC_ENABLE_SIM_PID_LIST_MODE)
//    case kDescContextModePidList:
//      /* Current Pid list process finalization */
//      DescPidProcessingDone(DESC_CONTEXT_PARAM_ONLY);
//      break;
//# endif
//# if defined (DESC_ENABLE_PERIODIC_MODE)
//    case kDescContextModePeriodic:
//      /* Current P-Pid list process finalization */
//      DescSchedulerProcessingDone(DESC_CONTEXT_PARAM_ONLY);
//      break;
//# endif
//# if defined (DESC_ENABLE_ROUTINE_CONTROL_MODE)
//    case kDescContextModeRoutineControl:
//      /* The routine control service is ready - finalize it */
//      DescRidProcessingDone(DESC_CONTEXT_PARAM_ONLY);
//      break;
//# endif
//# if defined (DESC_ENABLE_MEMORY_BY_ADDRESS_MODE)
//    case kDescContextModeMemByAddr:
//      /* The memory by address service is ready - finalize it */
//      DescMemByAddrProcessingDone(DESC_CONTEXT_PARAM_ONLY);
//      break;
//# endif
//    default:
//      /* Unknown context mode */
//      DescAssertCommonAlways(kDescAssertInvalidContextMode);
//      break;
//  }
}

static DescNegResCode DescCheckState( DescStateInfo  * refState)
{
  if((refState->stateSession & g_descCurState.stateSession) == 0)
  {
    return kDescNrcRejectStateSession;
  }
  if((refState->stateSecurityAccess & g_descCurState.stateSecurityAccess) == 0)
  {
    return kDescNrcRejectStateSecurityAccess;
  }

  return kDescNrcNone;
}
#if defined (DESC_ENABLE_RESET_AWAIT_KEY_ON_INVALID_REQ)
/*******************************************************************************
* NAME:              DescOemOnInvalidReq_27
*
* CALLED BY:         DescDispatcher
* PRECONDITIONS:
*
* DESCRIPTION:       Special security state handling on invalid request
*
*******************************************************************************/
static void DescOemOnInvalidReq_27(void)
{
  /* Take only service $27 into account */
  if(g_descInterruptContextCtrl[DESC_CONTEXT_PARAM_VALUE].infoPoolPtr->reqDataPtr[0] == 0x27)
  {
    g_descIsKeyAwaited = kDescFalse;
  }
}
#endif
