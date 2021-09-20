#include "RpdAdapter.h"
#include "EMIPAddressUtils.h"
#include "emAPIs.h"
#include "RemotePHYen.h"
#include "gcpMsg.h"
#include "MultiCore_RexOpn.h"
#define MAX_INT32 0x7FFFFFFF

void logPutGcpConnVerification(EMHANDLE* emHandle, enum LOGMODE_T logLevel, 
             char *msg, GcpConnVerification *gcpConnVerification);

/* Handle the CCapCoreIdentification response received from RPD */
uint8_t handleRpdGlobalRsp(void *rpdHandlerVptr, void *rpdClientVptr,
                                      void *pendingMsgVptr, GcpMsg *gcpRsp)
{
  char logMsg[500] = {'\0'};
  uint8_t success = 1;
  int permitConfig = 0;
  RpdGlobal *rpdGlobal = NULL;
  pthread_t handlerThreadId = 0;
  RcpMsg *rcpMsg = NULL;
  uint16_t seqNdx = 0;
  int tlvNdx = 0;
  int errorRsp = 0;
  PendingGcpMsg *lastPendingMsg = (PendingGcpMsg*)pendingMsgVptr;
  RpdHandler *rpdHandler = (RpdHandler*)rpdHandlerVptr;
  RPD_CLIENT *rpdClient = (RPD_CLIENT*)rpdClientVptr;
  EMHANDLE *rpdEmHandle = NULL;
  /*
  uint8_t tempBuf[MAX_GCP_MSG_SIZE] = {'\0'};
  uint16_t tempBufSize = 0;
  uint16_t bufLen = 0;
  */

  if (rpdHandler == NULL || rpdClient == NULL || lastPendingMsg == NULL)
  {
    sprintf(logMsg,
      "%s:%d:%s: Error.InvalidParams,rpdHandler=%p,rpdClient=%p,lastPendingMsg=%p",
      __FILE__, __LINE__, __FUNCTION__,
      (void*)rpdHandler, (void*)rpdClient, (void*)lastPendingMsg);
    LogPut(getGcpMsgHandle(), LOG_MIN, logMsg);
  }
  else if ((rpdEmHandle = getClientEmHandle(rpdClient)) == NULL)
  {
    sprintf(logMsg,"%s:%d:%s,InvalidRpdEmHandle,clientIp=%s",
      __FILE__, __LINE__, __FUNCTION__,getClientIpStr(rpdClient));
    LogPut(getGcpMsgHandle(), LOG_MIN, logMsg);
  }
  else if (gcpRsp == NULL)
  {
    /* Null response indicates timeout or cancellation of transaction */
    setClientWipState(rpdClient,IdleWipState);
    setClientState(rpdClient, RpdClientIdle);
    setClientKeepAliveInProgress(rpdClient, 0);
    sprintf(logMsg,
      "%s:%d:%s: Warn.NullResponse.XactionEnded,rpdHandler=%p,rpdClient=%p,lastPendingMsg=%p,gcpRsp=%p",
       __FILE__, __LINE__, __FUNCTION__,
      (void*)rpdHandler, (void*)rpdClient, (void*)lastPendingMsg, (void*)gcpRsp);
    LogPut(getGcpMsgHandle(), LOG_MIN, logMsg);
    EMPutIntValue(rpdEmHandle, EN_RPD_OPERATIONALSTATE, 0, EN_RPD_OPERATIONALSTATE_PROVISIONFAILURE);
    EMIncCounter(rpdEmHandle, EN_RPD_GCPEDSRSPTIMEOUTCNT, 0, MAX_INT32);
    errorRsp = 1;
  }
  else
  {

    handlerThreadId = getHandlerThreadId(rpdHandler);
    if (gcpRsp->msgId == GcpEdsErrRspMsgId)
    {
      sprintf(logMsg,
              "%s:%d:%s: threadId=0x%08lx,GcpEdsErrorResponse,xactionId=%d,ExceptionCode=%s,clientIp=%s",
              __FILE__, __LINE__, __FUNCTION__, handlerThreadId,
              (int)gcpRsp->msgData.gcpErrRsp.xactionId,
      getGcpReturnCodeStr((int)gcpRsp->msgData.gcpErrRsp.returnCode),
      getClientIpStr(rpdClient));
      LogPut(getGcpMsgHandle(), LOG_MIN, logMsg);
      EMPutIntValue(rpdEmHandle, EN_RPD_OPERATIONALSTATE, 0, EN_RPD_OPERATIONALSTATE_PROVISIONFAILURE);
      errorRsp = 1;
    }
    else if (gcpRsp->msgId == GcpEdsRspMsgId)
    {
      sprintf(logMsg,
        "%s:%d:%s: threadId=0x%08lx,GcpEdsRsp,xactionId=%d,sequenceCount=%d,clientIp=%s",
        __FILE__, __LINE__, __FUNCTION__, handlerThreadId,
        (int)gcpRsp->msgData.edsRsp.xactionId,
        (int)gcpRsp->msgData.edsRsp.rcpMsg.sequenceCount,
      getClientIpStr(rpdClient));
      LogPut(getGcpMsgHandle(), LOG_NORMAL, logMsg);

      rcpMsg = &gcpRsp->msgData.edsRsp.rcpMsg;
      for (seqNdx = 0; seqNdx < gcpRsp->msgData.edsRsp.rcpMsg.sequenceCount; seqNdx++)
      {
         sprintf(logMsg, 
           "%s:%d:%s: threadId=0x%08lx,GcpEdsRspSequence, rcpMsg->seqList[seqNdx].tlvCount=%d,", __FILE__, __LINE__, __FUNCTION__, handlerThreadId, rcpMsg->seqList[seqNdx].tlvCount);
         LogPut(getGcpMsgHandle(), LOG_MIN, logMsg);
        
        for (tlvNdx = 0; tlvNdx < rcpMsg->seqList[seqNdx].tlvCount; tlvNdx++)
        {
          sprintf(logMsg,
            "%s:%d:%s: threadId=0x%08lx,GcpEdsRspSequence,xactionId=%d,seqNdx=%d,tlvCount=%d,clientIp=%s,",
            __FILE__, __LINE__, __FUNCTION__, handlerThreadId,
            (int)gcpRsp->msgData.edsRsp.xactionId,
          seqNdx, rcpMsg->seqList[seqNdx].tlvCount,getClientIpStr(rpdClient));
          strnCatRcpSequence(logMsg, sizeof(logMsg),
            &gcpRsp->msgData.edsRsp.rcpMsg.seqList[seqNdx]);
          LogPut(getGcpMsgHandle(), LOG_MIN, logMsg);


          if (rcpMsg->seqList[seqNdx].rcpTlv[tlvNdx].type == RpdGlobalTlvType)
          {
            rpdGlobal = &rcpMsg->seqList[seqNdx].rcpTlv[tlvNdx].data.rpdGlobal;
            /*bufLen = rcpMsg->seqList[seqNdx].rcpTlv[tlvNdx].len; */
            if (rcpMsg->seqList[seqNdx].responseCode_IsSet && rcpMsg->seqList[seqNdx].responseCode != 0)
            {
              sprintf(logMsg, "%s:%d:%s,threadId=0x%08lx,ReceivedRpdGlobalRspWithErrorResponseCode,xactionId=%d,seqNdx=%d,tlvNdx=%d,rspCode=%d,clientIp=%s,",
                 __FILE__, __LINE__, __FUNCTION__, handlerThreadId,
                (int)gcpRsp->msgData.edsRsp.xactionId,
              seqNdx,tlvNdx,rcpMsg->seqList[seqNdx].responseCode,getClientIpStr(rpdClient));
              LogPut(getGcpMsgHandle(), LOG_MIN, logMsg);
              EMPutIntValue(rpdEmHandle, EN_RPD_OPERATIONALSTATE, 0, EN_RPD_OPERATIONALSTATE_PROVISIONFAILURE);
              errorRsp = 1;
            }
            else
            {
              /*                  
              memset(tempBuf,0,sizeof(tempBuf));
              tempBufSize = 0;
                  logPutGcpConnVerification(rpdEmHandle, LOG_NORMAL, logMsg,
                    &gcpMsg->msgData.edsReq.rcpMsg.seqList[0].rcpTlv[0].data.rpdGlobal.gcpConnVerification[0]);
              */
              /*uint8_t getRpdGlobal(uint8_t *msgBuf, uint16_t bufLen, uint16_t *curNdx, RpdGlobal *rpdGlobal) */
              /*getRpdGlobal(tempBuf,  bufLen, &tempBufSize, rpdGlobal);*/
              logPutRpdGlobal(rpdEmHandle, LOG_NORMAL, logMsg, rpdGlobal);
            }
            sprintf(logMsg, 
            "%s:%d:%s: threadId=0x%08lx,GcpEdsRspSequence, Received TLVType=%d,", __FILE__, __LINE__, __FUNCTION__, handlerThreadId, rcpMsg->seqList[seqNdx].rcpTlv[tlvNdx].type);
            LogPut(getGcpMsgHandle(), LOG_MIN, logMsg);
          }
          else
          {
            sprintf(logMsg, 
            "%s:%d:%s: threadId=0x%08lx,GcpEdsRspSequence, Received TLVType=%d,", __FILE__, __LINE__, __FUNCTION__, handlerThreadId, rcpMsg->seqList[seqNdx].rcpTlv[tlvNdx].type);
            LogPut(getGcpMsgHandle(), LOG_MIN, logMsg);
          }
        }
      }
    }

    permitConfig = EMGetIntValue(rpdEmHandle, EN_RPD_PERMITAUXSELFCONFIG, 0);
    setClientState(rpdClient, WaitForRpdMultiCoreReadRsp);
    requestRpdPermitAuxSelfConfiguration(rpdHandler,rpdClient);
  }

  return success;
}


/****************************************************************
 * setGcpConnVerification()
 ****************************************************************/
uint8_t setGcpConnVerification(GcpConnVerification *gcpConnVerification, RPD_CLIENT *rpdClient)
{
  char logMsg[1024] = {'\0'};
  uint8_t success = 0;
  CoreData coreData;

  memset(&coreData, 0, sizeof(CoreData));
  if (rpdClient == NULL)
  {
    sprintf(logMsg, "%s:%s:%d,InvalidInputParameters,clientIp=%p",
       __FILE__,__FUNCTION__,__LINE__,(void*)rpdClient);
    LogPut(getGcpMsgHandle(), LOG_MIN, logMsg);
  }
  else if (gcpConnVerification == NULL)
  {
    sprintf(logMsg, "%s:%s:%d,InvalidInputParameters,rpcGlobal=%p",
       __FILE__,__FUNCTION__,__LINE__,(void*)gcpConnVerification);
    LogPut(getGcpMsgHandle(), LOG_MIN, logMsg);
  }	
  else
  {
    populateCoreData(getClientFd(rpdClient), &coreData);
    gcpConnVerification->coreIdLen = 6;
    memcpy(gcpConnVerification->coreId, coreData.coreId,sizeof(gcpConnVerification->coreId));
    setIsSet(gcpConnVerification->presenceBits, gcv_coreIdIsSet);  /* CoreId 0 */

    gcpConnVerification->maxGcpIdleTime = EMGetIntValue(getGcpMsgHandle(), EN_REMOTEPHY_MAXGCPIDLETIME, 0);
    setIsSet(gcpConnVerification->presenceBits, maxGcpIdleTimeIsSet);  /* MaxGcpIdleTime 1 */

    gcpConnVerification->gcpRecoveryAction = EMGetIntValue(getGcpMsgHandle(), EN_REMOTEPHY_GCPRECOVERYACTION, 0);
    setIsSet(gcpConnVerification->presenceBits, gcpRecoveryActionIsSet);  /* GcpRecoveryAction 2 */
	
	gcpConnVerification->gcpRecoveryActionRetry = EMGetIntValue(getGcpMsgHandle(), EN_REMOTEPHY_GCPRECOVERYACTIONRETRY, 0);
    setIsSet(gcpConnVerification->presenceBits, gcpRecoveryActionRetryIsSet);  /* GcpRecoveryActionRetry 3 */
	
	gcpConnVerification->gcpRecoveryActionDelay = EMGetIntValue(getGcpMsgHandle(), EN_REMOTEPHY_GCPRECOVERYACTIONDELAY, 0);
    setIsSet(gcpConnVerification->presenceBits, gcpRecoveryActionDelayIsSet);  /* GcpRecoveryActionDelay 4 */

	gcpConnVerification->gcpReconnectTimeout = EMGetIntValue(getGcpMsgHandle(), EN_REMOTEPHY_GCPRECONNECTTIMEOUT, 0);
    setIsSet(gcpConnVerification->presenceBits, gcpReconnectTimeoutIsSet);  /* GcpRecoveryActionDelay 5 */

    /*
	gcpConnVerification->gcpHandoverTimeout = 45;
    setIsSet(gcpConnVerification->presenceBits, gcpHandoverTimeoutIsSet);
    */

	gcpConnVerification->checkForDisconnectedCoresPeriod = EMGetIntValue(getGcpMsgHandle(), EN_REMOTEPHY_CHECKFORDISCONNECTEDCORESPERIOD, 0);
    setIsSet(gcpConnVerification->presenceBits, checkForDisconnectedCoresPeriodIsSet);

    gcpConnVerification->auxReconnectFailReset = EMGetIntValue(getGcpMsgHandle(), EN_REMOTEPHY_AUXRECONNECTFAILRESET, 0); 
    setIsSet(gcpConnVerification->presenceBits, auxReconnectFailResetIsSet);

    success = 1;	
  }
  return success;
}

/****************************************************************
 * configureRpdGcpConnVerification()
 ****************************************************************/
uint8_t configureAuxCoreRpdGlobal(RpdHandler *rpdHandler, RPD_CLIENT *rpdClient)
{
  uint8_t success = 0;
  char logMsg[1024] = {'\0'};
  GcpTcpData gcpTcpData;

  pthread_t handlerThreadId = 0;
  GcpMsg *gcpMsg = NULL;
  EMHANDLE *rpdEmHandle = NULL;
  
  memset(&gcpTcpData, 0, sizeof(GcpTcpData));

  if (rpdHandler == NULL || rpdClient == NULL)
  {
    sprintf(logMsg, "%s:%s:%d,InvalidInputParams,rpdHandler=%p,clientIp=%p",
      __FILE__,__FUNCTION__,__LINE__,(void*)rpdHandler, (void*)rpdClient);
    LogPut(getGcpMsgHandle(), LOG_MIN, logMsg);
  }
  else if ((gcpMsg = getAvailableMsg(rpdHandler)) == NULL)
  {
    sprintf(logMsg, "%s:%s:%d,OutOfMemory,CannotProvisionGcpConnVerification,clientIp=%s",
            __FILE__,__FUNCTION__,__LINE__,getClientIpStr(rpdClient));
        LogPut(getGcpMsgHandle(), LOG_MIN, logMsg);
  }
  else if (!setGcpConnVerification(&gcpMsg->msgData.edsReq.rcpMsg.seqList[0].rcpTlv[0].data.rpdGlobal.gcpConnVerification[0], rpdClient))
  {
    sprintf(logMsg, "%s:%s:%d, setGcpConnVerification,clientIp=%s",
            __FILE__,__FUNCTION__,__LINE__,getClientIpStr(rpdClient));
    LogPut(getGcpMsgHandle(), LOG_MIN, logMsg);
  }
  else
  {
	sprintf(logMsg, "%s:%s:%d,PROVISION:MaxGcpIdleTime,clientIp=%s",
                __FILE__,__FUNCTION__,__LINE__,getClientIpStr(rpdClient));
    LogPut(getGcpMsgHandle(), LOG_NORMAL, logMsg);
    handlerThreadId = getHandlerThreadId(rpdHandler);
    gcpMsg->msgData.edsReq.rcpMsg.msgType = REX_RCP_MSG_TYPE;
    gcpMsg->msgData.edsReq.rcpMsg.sequenceCount = 1;
    gcpMsg->msgData.edsReq.rcpMsg.seqList[0].sequenceNumber = getClientNextSeqenceNumber(rpdClient);
    gcpMsg->msgData.edsReq.rcpMsg.seqList[0].operation = WriteOpType;
    gcpMsg->msgData.edsReq.rcpMsg.seqList[0].tlvCount = 1;
    gcpMsg->msgData.edsReq.rcpMsg.seqList[0].rcpTlv[0].type = RpdGlobalTlvType;
    gcpMsg->msgData.edsReq.rcpMsg.seqList[0].responseCode_IsSet = 0;
    gcpMsg->msgData.edsReq.rcpMsg.seqList[0].rcpTlv[0].data.rpdGlobal.gcpConnVerificationCount = 1; /* We configure only for AuxCore */
    setIsSet(gcpMsg->msgData.edsReq.rcpMsg.seqList[0].rcpTlv[0].data.rpdGlobal.presenceBits, gcpConnVerificationIsSet);    
 
    rpdEmHandle = getClientEmHandle(rpdClient);
	memset(logMsg, 0, sizeof(logMsg));
    
    logPutGcpConnVerification(rpdEmHandle, LOG_NORMAL, logMsg,
      &gcpMsg->msgData.edsReq.rcpMsg.seqList[0].rcpTlv[0].data.rpdGlobal.gcpConnVerification[0]);
    
    logPutRpdGlobal(rpdEmHandle, LOG_NORMAL, logMsg,
      &gcpMsg->msgData.edsReq.rcpMsg.seqList[0].rcpTlv[0].data.rpdGlobal);
	
    if (!RPDSendGcpEdsReqMsg(rpdHandler, rpdClient, gcpMsg, NULL, handleRpdGlobalRsp))
    {
      sprintf(logMsg, "%s:%s:%d,FailedToSendGcpMsgForGcpConnVerificationRequest,clientIp=%s",
                    __FILE__,__FUNCTION__,__LINE__,getClientIpStr(rpdClient));
      LogPut(getGcpMsgHandle(),LOG_NORMAL,logMsg);
    }
    else
    {
      sprintf(logMsg,"%s:%s:%d,GcpConnVerificationSuccess, clientIp=%s",
                    __FILE__,__FUNCTION__,__LINE__,getClientIpStr(rpdClient));
      LogPut(getGcpMsgHandle(),LOG_NORMAL,logMsg);
      success = 1;
    }
    releaseMsgFromUse(rpdHandler, gcpMsg);
  }
  return success;
} /* End configureRpdGcpConnVerification */

