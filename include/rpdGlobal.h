/***********************************************************************
 * Copyright (c) 2020 Synamedia. All rights reserved
 * rpdGlobal.h
 ************************************************************/
#ifndef _RPD_GLOBAL_
#define _RPD_GLOBAL_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef NEVER_DEFINED
  extern "C" }
/* Keep editor auto-indent happy */
#endif

#include "gcpMsgDefs.h"

/* MAX_CCAP_CORES see section 14.2.1 "Configured Core Table" in Rphy-spec */
#define MAX_CCAP_CORES 10
/***********************************************************************
 *
 ***********************************************************************/
typedef enum RcpAdminStateType
  {
   OtherRcpAdminState = 1,
   UpRcpAdminState = 2,
   DownRcpAdminState = 3,
   TestingRcpAdminState = 4
  } RcpAdminStateType;

/***********************************************************************
 *
 ***********************************************************************/
typedef enum ThrottleAdminStatus
  {
   UnconstrainedAdminStatus = 1,
   MaintainBelowAdminStatus = 2,
   StopAtThresholdAdminStatus = 3,
   InhibitiedAdminStatus = 4
  } ThrottleAdminStatus;


/***********************************************************************
 *
 ***********************************************************************/
typedef enum GcpRecoveryActionType
{
  gcpWaitForAction = 1,
  gcpReconnectToSameCore = 2,
  gcpHandoverToBackup = 3,
  waitAndReboot = 4
} GcpRecoveryActionType;

/***********************************************************************
 *
 ***********************************************************************/
#define cinIfTimeoutIsSet 0
#define eapReqTimeoutIsSet 1
#define eapolStartRetriesIsSet 2
#define noIraRcvdTimeoutIsSet 3
#define noRexRcvdTimeoutIsSet 4

typedef struct PerRPDInitializationTimers /* PerRPDInitializationTimers Complex TLV 15.9 variable */
{
  uint16_t cinIfTimeout; /* CinIfTimeout UnsignedShort 15.9,1 2 R */
  uint16_t eapReqTimeout; /* EapReqTimeout UnsignedShort 15.9.2 2 R */
  uint8_t eapolStartRetries; /* EapolStartRetries UnsignedByte 15.9.3 1 R */
  uint16_t noIraRcvdTimeout; /* NoIraRcvdTimeout UnsignedShort 15.9.4 2 R */
  uint16_t noRexRcvdTimeout; /* NoRexRcvdTimeout UnsignedShort 15.9.5 2 R */
  uint8_t presenceBits[1];
  uint8_t readBits[1];
} PerRPDInitializationTimers;

/***********************************************************************
 *
 ***********************************************************************/
#define pcitc_coreIdIsSet 0
#define initialConfigCompleteTimeoutIsSet 1
#define initialConfigCompleteRetryCountIsSet 2
#define initialConfigCompleteRetryTimeoutIsSet 3
#define waitOperationalTimeoutIsSet 4
#define waitOperationalRetryCountIsSet 5
#define waitOperationalRetryTimeoutIsSet 6

typedef struct PerCoreInitializationTimerConfig /* PerCoreInitializationTimerConfig Complex TLV 15.8 variable */
{
  uint16_t coreIdLen;
  uint8_t coreId[6]; /* CoreId HexBinary 15.8.1 6 N/A key */
  
  uint16_t initialConfigCompleteTimeout; /* InitialConfigCompleteTimeout UnsignedShort 15.8.2 2 R/W */
  uint8_t initialConfigCompleteRetryCount; /* InitialConfigCompleteRetryCount UnsignedByte 15.8.3 1 R/W */
  uint16_t initialConfigCompleteRetryTimeout; /* InitialConfigCompleteRetryTimeout UnsignedShort 15.8.4 2 R/W */
  uint16_t waitOperationalTimeout; /* WaitOperationalTimeout UnsignedShort 15.8.5 2 R/W */
  uint8_t waitOperationalRetryCount; /* WaitOperationalRetryCount UnsignedByte 15.8.6 1 R/W */
  uint16_t waitOperationalRetryTimeout; /* WaitOperationalRetryTimeout UnsignedShort 15.8.7 2 R/W */
  uint8_t presenceBits[1];
  uint8_t readBits[1];
} PerCoreInitializationTimerConfig;

/***********************************************************************
 *
 ***********************************************************************/
#define lldpEnableIsSet 0
#define msgTxIntervalIsSet 1

typedef struct LldpConfig /* LldpConfig Complex TLV 15.6 Variable */
{
  uint8_t lldpEnable; /* LldpEnable Boolean 15.6.1 1 R/W */
  uint16_t msgTxInterval; /* MsgTxInterval UnsignedShort 15.6.2 2 R/W */
  uint8_t presenceBits[1];
  uint8_t readBits[1];
} LldpConfig;

/***********************************************************************
 *
 ***********************************************************************/
#define scQamUseRngPwIsSet 0
#define ofdmaMaxNumPayloadUnitsIsSet 1
#define ofdmaMaxNumTrailerUnitsIsSet 2

typedef struct UepiControl /* UepiControl Complex TLV 15.4 variable */
{
  uint8_t scQamUseRngPw; /* ScQamUseRngPw Boolean 15.4.1 1 R/W */
  uint8_t ofdmaMaxNumPayloadUnits; /* OfdmaMaxNumPayloadUnits UnsignedByte 15.4.2 1 R/W */
  uint8_t ofdmaMaxNumTrailerUnits; /* OfdmaMaxNumTrailerUnits UnsignedByte 15.4.3 1 R/W */
  uint8_t presenceBits[1];
  uint8_t readBits[1];
} UepiControl;

/***********************************************************************
 *
 ***********************************************************************/
#define useIcmpBasedPmtudIsSet 0
#define usePacketizationBasedPmtudIsSet 1

typedef struct PmtudControl /* PmtudControl Complex TLV 15.3.2 variable */
{
  uint8_t useIcmpBasedPmtud; /* UseIcmpBasedPmtud Boolean 15.3.2.1 1 R/W */
  uint8_t usePacketizationBasedPmtud; /* UsePacketizationBasedPmtud Boolean 15.3.2.2 1 R/W */
  uint8_t presenceBits[1];
  uint8_t readBits[1];
} PmtudControl;

/***********************************************************************
 *
 ***********************************************************************/
#define ipStackControlIsSet 0
#define pmtudControlIsSet 1

typedef struct IpConfig /* IpConfig Complex TLV 15.3 variable */
{
  uint8_t ipStackControl; /* IpStackControl UnsignedByte 15.3.1 1 R/W */
  PmtudControl pmtudControl; /* PmtudControl Complex TLV 15.3.2 variable */
  uint8_t presenceBits[1];
  uint8_t readBits[1];
} IpConfig;

/***********************************************************************
 *
 ***********************************************************************/
#define gcv_coreIdIsSet 0
#define maxGcpIdleTimeIsSet 1
#define gcpRecoveryActionIsSet 2
#define gcpRecoveryActionRetryIsSet 3
#define gcpRecoveryActionDelayIsSet 4
#define gcpReconnectTimeoutIsSet 5
#define gcpHandoverTimeoutIsSet 6
#define checkForDisconnectedCoresPeriodIsSet 7
#define auxReconnectFailResetIsSet 8

typedef struct GcpConnVerification /* GcpConnVerification Complex TLV 15.2 variable */
{
  uint16_t coreIdLen;
  uint8_t coreId[6]; /* CoreId HexBinary 15.2.1 6 N/A key */
  uint16_t maxGcpIdleTime; /* MaxGcpIdleTime UnsignedShort 15.2.2 2 R/W */
  uint8_t gcpRecoveryAction; /* GcpRecoveryAction UnsignedByte 15.2.3 1 R/W */
  uint8_t gcpRecoveryActionRetry; /* GcpRecoveryActionRetry UnsignedByte 15.2.4 1 R/W */
  uint16_t gcpRecoveryActionDelay; /* GcpRecoveryActionDelay UnsignedShort 15.2.5 2 R/W */
  uint16_t gcpReconnectTimeout; /* GcpReconnectTimeout UnsignedShort 15.2.6 2 R/W */
  uint16_t gcpHandoverTimeout; /* GcpHandoverTimeout UnsignedShort 15.2.7 2 R/W */
  uint16_t checkForDisconnectedCoresPeriod; /* CheckForDisconnectedCoresPeriod UnsignedShort 15.2.8 2 R/W */
  uint8_t auxReconnectFailReset; /* AuxReconnectFailReset Boolean 15.2.9 2 R/W */ 
  uint8_t presenceBits[2];
  uint8_t readBits[2];
} GcpConnVerification;

/***********************************************************************
 *
 ***********************************************************************/
#define syslogThrottleAdminStatusIsSet 0
#define syslogThrottleThresholdIsSet 1
#define syslogThrottleIntervalIsSet 2

typedef struct SyslogThrottleCfg /* SyslogThrottleCfg Complex TLV 15.1.6.3 variable N/A */
{
  /* see ThrottleAdminStatus enum */
  uint8_t syslogThrottleAdminStatus; /* SyslogThrottleAdminStatus UnsignedByte 15.1.6.3.1 1 R/W */
  uint32_t syslogThrottleThreshold; /* SyslogThrottleThreshold UnsignedInt 15.1.6.3.2 4 R/W */
  uint32_t syslogThrottleInterval; /* SyslogThrottleInterval UnsignedInt 15.1.6.3.3 4 R/W */
  uint8_t presenceBits[1];
  uint8_t readBits[1];
} SyslogThrottleCfg;
  
/***********************************************************************
 *
 ***********************************************************************/
#define syslogPriorityIsSet 0
#define syslogReportingIsSet 1

typedef struct SyslogControlCfg /* SyslogControlCfg Complex TLV 15.1.6.2 variable N/A */
{
  uint8_t syslogPriority; /* SyslogPriority UnsignedByte 15.1.6.2.1 1 R/W */
  uint8_t syslogReporting; /* SyslogReporting UnsignedByte 15.1.6.2.2 1 R/W */
  uint8_t presenceBits[1];
  uint8_t readBits[1];
} SyslogControlCfg;
  
/***********************************************************************
 *
 ***********************************************************************/
#define syslogServerIndexIsSet 0
#define syslogServerIpAddrIsSet 1
#define syslogServerAdminStateIsSet 2

typedef struct SyslogServerCfg /* SyslogServerCfg Complex TLV 15.1.6.1 variable N/A */
{
  uint8_t syslogServerIndex; /* SyslogServerIndex UnsignedByte 15.1.6.1.1 1 R/W */
  uint16_t syslogServerIpAddrLen;
  uint8_t syslogServerIpAddr[16]; /* SyslogServerIpAddr IpAddress 15.1.6.1.2 4 or 16 R/W */
  RcpAdminStateType syslogServerAdminState; /* SyslogServerAdminState AdminStateType 15.1.6.1.3 1 R/W */
  uint8_t presenceBits[1];
  uint8_t readBits[1];
} SyslogServerCfg;
  
/***********************************************************************
 *
 ***********************************************************************/
#define syslogServerCfgIsSet 0
#define syslogControlCfgIsSet 1
#define syslogThrottleCfgIsSet 2

typedef struct SyslogCfg /* SyslogCfg Complex TLV 15.1.6 variable N/A */
{
  SyslogServerCfg syslogServerCfg; /* SyslogServerCfg Complex TLV 15.1.6.1 variable N/A */
  SyslogControlCfg syslogControlCfg; /* SyslogControlCfg Complex TLV 15.1.6.2 variable N/A */
  SyslogThrottleCfg syslogThrottleCfg; /* SyslogThrottleCfg Complex TLV 15.1.6.3 variable N/A */
  uint8_t presenceBits[1];
  uint8_t readBits[1];
} SyslogCfg;

/***********************************************************************
 *
 ***********************************************************************/
#define evPriorityIsSet 0
#define evReportingIsSet 1

typedef struct EvControl /* EvControl Complex TLV 15.1.1 variable N/A */
{
  uint8_t evPriority; /* EvPriority UnsignedByte 15.1.1.1 1 N/A */
  uint8_t evReporting; /* EvReporting UnsignedByte 15.1.1.2 1 R/W */
  uint8_t presenceBits[1];
  uint8_t readBits[1];
} EvControl;

/***********************************************************************
 *
 ***********************************************************************/
#define evControlIsSet 0
#define evThrottleAdminStatusIsSet 1
#define evThrottleThresholdIsSet 2
#define evThrottleIntervalIsSet 3
#define notifyEnableIsSet 4
#define syslogCfgIsSet 5

typedef struct EvCfg /* EvCfg Complex TLV 15.1 variable */
{
  EvControl evControl; /* EvControl Complex TLV 15.1.1 variable N/A */
  /* see ThrottleAdminStatus enum */
  uint8_t evThrottleAdminStatus; /* EvThrottleAdminStatus UnsignedByte 15.1.2 1 R/W */
  uint32_t evThrottleThreshold; /* EvThrottleThreshold UnsignedInt 15.1.3 4 R/W */
  uint32_t evThrottleInterval; /* EvThrottleInterval UnsignedInt 15.1.4 4 R/W */
  uint8_t notifyEnable; /* NotifyEnable UnsignedByte 15.1.5 1 R/W */  
  SyslogCfg syslogCfg; /* SyslogCfg Complex TLV 15.1.6 variable N/A */
  uint8_t presenceBits[1];
  uint8_t readBits[1];
} EvCfg;

/***********************************************************************
 *
 ***********************************************************************/
#define evCfgIsSet 0
#define gcpConnVerificationIsSet 1
#define ipConfigIsSet 2
#define uepiControlIsSet 3
#define gcpDscpIsSet 4
#define lldpConfigIsSet 5
#define coreConnectTimeoutIsSet 6
#define perCoreInitializationTimerConfigIsSet 7
#define perRPDInitializationTimersIsSet 8

typedef struct RpdGlobal /* RpdGlobal Complex TLV 15 variable */
{
  EvCfg evCfg; /* EvCfg Complex TLV 15.1 variable */
  uint16_t gcpConnVerificationCount;
  GcpConnVerification gcpConnVerification[MAX_CCAP_CORES]; /* GcpConnVerification Complex TLV 15.2 variable */
  IpConfig ipConfig; /* IpConfig Complex TLV 15.3 variable */
  
  UepiControl uepiControl; /* UepiControl Complex TLV 15.4 variable */
  uint8_t gcpDscp; /* GcpDscp UnsignedByte 15.5 1 R/W */
  LldpConfig lldpConfig; /* LldpConfig Complex TLV 15.6 Variable */
  
  uint16_t coreConnectTimeout; /* CoreConnectTimeout UnsignedShort 15.7 2 R/W */
  /* PerCoreInitializationTimerConfig Complex TLV 15.8 variable */
  uint16_t perCoreInitializationTimerConfigCount;
  PerCoreInitializationTimerConfig perCoreInitializationTimerConfig[MAX_CCAP_CORES]; /* Complex TLV 15.8 variable */
  PerRPDInitializationTimers perRPDInitializationTimers; /* PerRPDInitializationTimers Complex TLV 15.9 variable */
  uint8_t presenceBits[2];
  uint8_t readBits[2];
} RpdGlobal;

uint8_t getPerRPDInitializationTimers(uint8_t *msgBuf, uint16_t bufLen, uint16_t *curNdx,
            PerRPDInitializationTimers* perRPDInitializationTimers);

uint8_t getPerCoreInitializationTimerConfig(uint8_t *msgBuf, uint16_t bufLen,
                                            uint16_t *curNdx,
            PerCoreInitializationTimerConfig* perCoreInitializationTimerConfig);

uint8_t getLldpConfig(uint8_t *msgBuf, uint16_t bufLen, uint16_t *curNdx,
                      LldpConfig* lldpConfig);

uint8_t getUepiControl(uint8_t *msgBuf, uint16_t bufLen, uint16_t *curNdx,
                       UepiControl *uepiControl);

uint8_t getPmtudControl(uint8_t *msgBuf, uint16_t bufLen, uint16_t *curNdx,
                        PmtudControl *pmtudControl);

uint8_t getIpConfig(uint8_t *msgBuf, uint16_t bufLen, uint16_t *curNdx,
                    IpConfig* ipConfig);

uint8_t getGcpConnVerification(uint8_t *msgBuf, uint16_t bufLen, uint16_t *curNdx,
                               GcpConnVerification* gcpConnVerification);

uint8_t getSyslogThrottleCfg(uint8_t *msgBuf, uint16_t bufLen, uint16_t *curNdx,
                             SyslogThrottleCfg* syslogThrottleCfg);

uint8_t getSyslogControlCfg(uint8_t *msgBuf, uint16_t bufLen, uint16_t *curNdx,
                            SyslogControlCfg* syslogControlCfg);

uint8_t getSyslogServerCfg(uint8_t *msgBuf, uint16_t bufLen, uint16_t *curNdx,
                           SyslogServerCfg* syslogServerCfg);

uint8_t getSyslogCfg(uint8_t *msgBuf, uint16_t bufLen, uint16_t *curNdx,
                     SyslogCfg* syslogCfg);

uint8_t getEvControl(uint8_t *msgBuf, uint16_t bufLen, uint16_t *curNdx,
                     EvControl* evControl);

uint8_t getEvCfg(uint8_t *msgBuf, uint16_t bufLen, uint16_t *curNdx,
                 EvCfg* evCfg);

uint8_t getRpdGlobal(uint8_t *msgBuf, uint16_t bufLen, uint16_t *curNdx,
                     RpdGlobal *rpdGlobal);

uint8_t putRpdGlobal(RpdGlobal *rpdGlobal,
                     uint8_t *buffer, uint16_t bufLen, uint16_t *curNdx);
uint8_t serializeRpdGlobal(uint8_t *recBuf, uint16_t maxBufLen, uint16_t *bufSize, RpdGlobal rec);				 

void logPutRpdGlobal(EMHANDLE* emHandle, enum LOGMODE_T logLevel,
                     char *msg, RpdGlobal *rpdGlobal);
#ifdef __cplusplus
__cplusplus
 }
#endif

#endif
/* End _RPD_GLOBAL_*/
