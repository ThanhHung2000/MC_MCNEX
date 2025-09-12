

#ifndef INC_DESC_H_
#define INC_DESC_H_
#include"main.h"
#define kDescMaxReqSid        0x85U
#define kDescPosResIdOffset  ((uint8_t)0x40U)

#define kDescNumContexts                                             1
/* CANdesc supports at maximum as many states pro StateGroup as given below: */
#define kDescMaxAllowedNumStates                                     16
#define kDescNumStateSession                                         5
#define kDescStateSessionDefault                                     ((uint8_t)0x01)
#define kDescStateSessionProgramming                                 ((uint8_t)0x02)
#define kDescStateSessionExtended                                    ((uint8_t)0x04)
#define kDescStateSessionExtendedDiagnosticMode                      ((uint8_t)0x08)
#define kDescStateSessionECUProgrammingMode                          ((uint8_t)0x10)
#define kDescNumStateSecurityAccess                                  2
#define kDescStateSecurityAccessLocked                               ((DescStateGroup)0x01)
#define kDescStateSecurityAccessUnlockedL1                           ((DescStateGroup)0x02)
# define DESC_CONTEXT_PARAM_DEF_LOCAL
#define kDescNrcNone                                                 ((uint8_t)0x00)

/* -- Context specific activity states -- */
#define kDescContextIdle                                             ((uint8_t)0x00)
#define kDescContextActiveRxBegin                                    ((uint8_t)0x01)
#define kDescContextActiveRxEnd                                      ((uint8_t)0x02)
#define kDescContextActiveProcess                                    ((uint8_t)0x04)
#define kDescContextActiveProcessEnd                                 ((uint8_t)0x08)
#define kDescContextActiveTxReady                                    ((uint8_t)0x10)
#define kDescContextActiveTx                                         ((uint8_t)0x20)
#define kDescContextActivePostProcess                                ((uint8_t)0x40)

/* CANdesc API types use the CANdesc namings convention! */
#define kDescFalse                                                   (uint8_t)0
#define kDescTrue                                                    (uint8_t)1
#define DESC_IGNORE_UNREF_PARAM(paramName)                           ((void)paramName)
# define DESC_CONTEXT_PARAM_ONLY
# define DESC_CONTEXT_PARAM_VALUE                                    ((uint8_t)0)
#define DESC_ENABLE_RESET_AWAIT_KEY_ON_INVALID_REQ

/* ---  Addressing methods  --- */
#define kDescPhysReq                                                 ((uint8_t)0x01)
#define kDescFuncReq                                                 ((uint8_t)0x02)
/* -----------------------------------------------------------------------------
    &&&~ Negative response codes
 ----------------------------------------------------------------------------- */

#define kDescNrcGeneralReject                                        0x10
#define kDescNrcServiceNotSupported                                  0x11
#define kDescNrcSubfunctionNotSupported                              0x12
#define kDescNrcInvalidFormat                                        0x13
#define kDescNrcResponseTooLong                                      0x14
#define kDescNrcBusyRepeatRequest                                    0x21
#define kDescNrcConditionsNotCorrect                                 0x22
#define kDescNrcRequestSequenceError                                 0x24
#define kDescNrcRequestOutOfRange                                    0x31
#define kDescNrcAccessDenied                                         0x33
#define kDescNrcInvalidKey                                           0x35
#define kDescNrcExceedNumOfAttempts                                  0x36
#define kDescNrcTimeDelayNotExpired                                  0x37
#define kDescNrcUploadDownloadNotAccepted                            0x70
#define kDescNrcTransferDataSuspended                                0x71
#define kDescNrcGeneralProgrammingFailure                            0x72
#define kDescNrcWrongBlockSequenceCounter                            0x73
#define kDescNrcResponsePending                                      0x78
#define kDescNrcSubfunctionNotSupportedInActiveSession               0x7E
#define kDescNrcServiceNotSupportedInActiveSession                   0x7F
#define kDescNrcService_not_supported_in_active_diagnostic_mode      0x80
#define kDescNrcRpmTooHigh                                           0x81
#define kDescNrcRpmTooLow                                            0x82
#define kDescNrcEngineIsRunning                                      0x83
#define kDescNrcEngineIsNotRunning                                   0x84
#define kDescNrcEngineRunTimeTooLow                                  0x85
#define kDescNrcTemperatureTooHigh                                   0x86
#define kDescNrcTemperatureTooLow                                    0x87
#define kDescNrcVehicleSpeedTooHigh                                  0x88
#define kDescNrcVehicleSpeedTooLow                                   0x89
#define kDescNrcThrottleSpeedTooHigh                                 0x8A
#define kDescNrcThrottleSpeedTooLow                                  0x8B
#define kDescNrcTransmissionRangeInNeutral                           0x8C
#define kDescNrcTransmissionRangeInGears                             0x8D
#define kDescNrcBrakeSwitchNotClosed                                 0x8F
#define kDescNrcShifterLeverNotInPark                                0x90
#define kDescNrcTorqueConverterClutchLocked                          0x91
#define kDescNrcVoltageTooHigh                                       0x92
#define kDescNrcVoltageTooLow                                        0x93
void  DescStateInit(void);
#define kDescNrcRejectStateSession                                   kDescNrcSubfunctionNotSupportedInActiveSession
#define kDescNrcRejectStateSecurityAccess                            kDescNrcAccessDenied


/* P2 timeout in DescTask cycles. */
#define kDescP2Ticks                                                 4
/* P2Ex timeout in DescTask cycles. */
#define kDescP2StarTicks                                             499
void  DescProcessingDone(void);

# define DescSessionTransitionChecked(DESC_CONTEXT_PARAM_ONLY)       (DescProcessingDone(DESC_CONTEXT_PARAM_ONLY))

typedef enum
{
  kDescUsdtResponseNone,
  kDescUsdtResponsePositive,
  kDescUsdtResponseRingBuffer,
  kDescUsdtResponseNegative,
  kDescUsdtResponseNegativeRCR_RP,
  kDescUsdtResponseNegativeApplRCR_RP,
  kDescUsdtResponseRoe,
  kDescUsdtResponsePeriodic,
  kDescUsdtResponseSpontaneous,
  kDescUsdtResponseSpontaneousAppl
} t_descUsdtNetResType;

typedef enum
{
  kDescUsdtNetReqTypePhysical,
  kDescUsdtNetReqTypeFunctional,
  kDescUsdtNetReqTypeInternal
} t_descUsdtNetReqType;
typedef enum
{
  kDescUsdtNetBusTypeNone,
  kDescUsdtNetBusTypeIsoTp
} t_descUsdtNetBusType;


typedef struct
{
  uint8_t                comChannel; /* relevant only on multi-channel systems (e.g. multi CAN channels) */
  uint8_t                testerId;   /* in some cases the tester address (ID) is from importance - store here if you have it */
  t_descUsdtNetBusType  busType;        /* from DescUsdtNetDispatcher.h */
} t_descUsdtNetBus;
typedef enum
{
  kDescUsdtNetworkOk = 0,
  kDescUsdtNetworkFailed = 1,
  kDescUsdtNetBufferUnderrun = 2,
  kDescUsdtNetworkAbort = 3
} t_descUsdtNetResult;

/* Additional information about a request */
typedef struct
{
  uint8_t reqType   :2; /* 0x01: Phys 0x02: Func */
  uint8_t resOnReq  :2; /* 0x01: Phys 0x02: Func */
  uint8_t suppPosRes:1; /* 0x00: No   0x02: Yes  */
} DescMsgAddInfo;
/* Some groups can have more than 8 states */
typedef unsigned int DescStateGroup;
/* Context information of a request */
typedef struct
{
  uint8_t *              reqData;
  uint16_t               reqDataLen;
  uint8_t *              resData;
  uint16_t               resDataLen;
  DescMsgAddInfo         msgAddInfo;
  DESC_CONTEXT_PARAM_DEF_LOCAL /* No iContext in non-parallel system */
  t_descUsdtNetBus    busInfo;
} DescMsgContext;
typedef uint8_t *                    DescMsg;
typedef uint8_t                      DescMsgLen;

/* Main handler function pointer type */
typedef void  (*DescMainHandler)     (DescMsgContext*);
/* Negative response codes is normally single Byte */
typedef uint8_t DescNegResCode;
void DescStateTask(void);
/* Set a diagnostic error */
void  DescSetNegResponse( DescNegResCode errorCode);
void Dest_Init(void);
#endif /* INC_DESC_H_ */
