#define _POSIX_C_SOURCE 200112L

#include <string.h>

#include "logSupport.h"
#include "elementmodel.h"
#include "emMisc.h"
#include "rpdGlobal.h"

/************************************************************************
 * getGcpConnVerification()
 * - Complex TLV 15.2 variable R/W
 * - Elements: 
 *   - 1 to 13
 *   - CandidateBackupCoreTable Complex TLV 15.2 variable R
 *   - 
 ************************************************************************/
uint8_t getGcpConnVerification(uint8_t *msgBuf, uint16_t bufLen, uint16_t *curNdx,
                                  GcpConnVerification *gcpConnVerification)
{
  uint8_t success = 1;
  uint8_t curTLVtype = 0;
  uint16_t curTLVlen = 0;
  char logMsg[500];
  
  while (*curNdx < bufLen && success)
  {
    sprintf(logMsg, "%s(%d) %s lastTLVtype(%d) lastTLVlen(%d)  bufLen(%d) curNdx(%d)",
            __FILE__, __LINE__, "getGcpConnVerification()",
            curTLVtype, curTLVlen, bufLen, *curNdx);
    LogPut(getGcpMsgHandle(), LOG_CAPTURE, logMsg);
    if(!getByte(msgBuf, bufLen, curNdx, &curTLVtype))
    {
      success = 0;
      sprintf(logMsg, "%s(%d) %s, bufLen(%d) curNdx(%d)",
              __FILE__, __LINE__,
              "getGcpConnVerification() failed to get curTLVtype", bufLen, *curNdx);
      LogPut(getGcpMsgHandle(), LOG_MIN, logMsg);
    }
    else /* We have curTLVtype */
    {
      if (!getShort(msgBuf, bufLen, curNdx, &curTLVlen))
      {
        sprintf(logMsg, "%s(%d) %s curTLVtype(%d) bufLen(%d) curNdx(%d)",
                __FILE__, __LINE__,
                "getGcpConnVerification() failed to get curTLVlen ",
                curTLVtype, bufLen, *curNdx);
        success = 0;
      }
      else /* We have curTLVlen */
      {
        sprintf(logMsg, "%s(%d) %s tlvType(%d) tlvLen(%d) bufLen(%d) curNdx(%d)",
                __FILE__, __LINE__, "getGcpConnVerification() ",
                curTLVtype, curTLVlen, bufLen, *curNdx);
        LogPut(getGcpMsgHandle(), LOG_CAPTURE, logMsg);
      
        switch (curTLVtype)
        {
        case 1: /* coreId HexBinary 15.2.1 R/W */
          gcpConnVerification->coreIdLen = curTLVlen;
          if (!getVariable(msgBuf, bufLen, curNdx, 
                           (uint8_t*)gcpConnVerification->coreId,
                           curTLVlen, sizeof(gcpConnVerification->coreId)))
          {
            sprintf(logMsg, "%s(%d) %s tlvType(%d) tlvLen(%d) bufLen(%d) curNdx(%d)",
                    __FILE__, __LINE__, "getGcpConnVerification() coreId-failed",
                    curTLVtype, curTLVlen, bufLen, *curNdx);
            LogPut(getGcpMsgHandle(), LOG_MIN, logMsg);
            success = 0;
          }
          else
          {
            setIsSet(gcpConnVerification->presenceBits, gcv_coreIdIsSet);
          }
          break;
        case 2: /* maxGcpIdleTime - UnsignedShort 15.2.2 R/W */
          if (!getShort(msgBuf, (*curNdx + curTLVlen), curNdx,
                        &gcpConnVerification->maxGcpIdleTime))
          {
            sprintf(logMsg, "%s(%d) %s tlvType(%d) tlvLen(%d) bufLen(%d) curNdx(%d)",
                    __FILE__, __LINE__, "getGcpConnVerification() MaxGcpIdleTime",
                    curTLVtype, curTLVlen, bufLen, *curNdx);
            LogPut(getGcpMsgHandle(), LOG_MIN, logMsg);
            success = 0;
          }
          else
          {
            setIsSet(gcpConnVerification->presenceBits, maxGcpIdleTimeIsSet);
          }
          break;
        case 3: /* gcpRecoveryAction - UnsignedShort 15.2.3 R/W */
          if (!getByte(msgBuf, (*curNdx + curTLVlen), curNdx,
                        &gcpConnVerification->gcpRecoveryAction))
          {
            sprintf(logMsg, "%s(%d) %s tlvType(%d) tlvLen(%d) bufLen(%d) curNdx(%d)",
                    __FILE__, __LINE__, "getGcpConnVerification() GcpRecoveryAction",
                    curTLVtype, curTLVlen, bufLen, *curNdx);
            LogPut(getGcpMsgHandle(), LOG_MIN, logMsg);
            success = 0;
          }
          else
          {
            setIsSet(gcpConnVerification->presenceBits, gcpRecoveryActionIsSet);
          }
          break;
        case 4: /* gcpRecoveryActionRetry - UnsignedShort 15.2.4 R/W */
          if (!getByte(msgBuf, (*curNdx + curTLVlen), curNdx,
                        &gcpConnVerification->gcpRecoveryActionRetry))
          {
            sprintf(logMsg, "%s(%d) %s tlvType(%d) tlvLen(%d) bufLen(%d) curNdx(%d)",
                    __FILE__, __LINE__, "getGcpConnVerification() GcpRecoveryActionRetry",
                    curTLVtype, curTLVlen, bufLen, *curNdx);
            LogPut(getGcpMsgHandle(), LOG_MIN, logMsg);
            success = 0;
          }
          else
          {
            setIsSet(gcpConnVerification->presenceBits, gcpRecoveryActionRetryIsSet);
          }
          break;
        case 5: /* gcpRecoveryActionDelay - UnsignedShort 15.2.5 R/W */
          if (!getShort(msgBuf, (*curNdx + curTLVlen), curNdx,
                        &gcpConnVerification->gcpRecoveryActionDelay))
          {
            sprintf(logMsg, "%s(%d) %s tlvType(%d) tlvLen(%d) bufLen(%d) curNdx(%d)",
                    __FILE__, __LINE__, "getGcpConnVerification() GcpRecoveryActionDelay",
                    curTLVtype, curTLVlen, bufLen, *curNdx);
            LogPut(getGcpMsgHandle(), LOG_MIN, logMsg);
            success = 0;
          }
          else
          {
            setIsSet(gcpConnVerification->presenceBits, gcpRecoveryActionDelayIsSet);
          }
          break;		  
        case 6: /* gcpReconnectTimeout - UnsignedShort 15.2.5 R/W */
          if (!getShort(msgBuf, (*curNdx + curTLVlen), curNdx,
                        &gcpConnVerification->gcpReconnectTimeout))
          {
            sprintf(logMsg, "%s(%d) %s tlvType(%d) tlvLen(%d) bufLen(%d) curNdx(%d)",
                    __FILE__, __LINE__, "getGcpConnVerification() GcpReconnectTimeout",
                    curTLVtype, curTLVlen, bufLen, *curNdx);
            LogPut(getGcpMsgHandle(), LOG_MIN, logMsg);
            success = 0;
          }
          else
          {
            setIsSet(gcpConnVerification->presenceBits, gcpReconnectTimeoutIsSet);
          }
          break;
        case 7: /* gcpReconnectTimeout - UnsignedShort 15.2.5 R/W */
          if (!getShort(msgBuf, (*curNdx + curTLVlen), curNdx,
                        &gcpConnVerification->gcpHandoverTimeout))
          {
            sprintf(logMsg, "%s(%d) %s tlvType(%d) tlvLen(%d) bufLen(%d) curNdx(%d)",
                    __FILE__, __LINE__, "getGcpConnVerification() GcpHandoverTimeout",
                    curTLVtype, curTLVlen, bufLen, *curNdx);
            LogPut(getGcpMsgHandle(), LOG_MIN, logMsg);
            success = 0;
          }
          else
          {
            setIsSet(gcpConnVerification->presenceBits, gcpHandoverTimeoutIsSet);
          }
          break;		  
        case 8: /* checkForDisconnectedCoresPeriod - UnsignedShort 15.2.5 R/W */
          if (!getShort(msgBuf, (*curNdx + curTLVlen), curNdx,
                        &gcpConnVerification->checkForDisconnectedCoresPeriod))
          {
            sprintf(logMsg, "%s(%d) %s tlvType(%d) tlvLen(%d) bufLen(%d) curNdx(%d)",
                    __FILE__, __LINE__, "getGcpConnVerification() CheckForDisconnectedCoresPeriod",
                    curTLVtype, curTLVlen, bufLen, *curNdx);
            LogPut(getGcpMsgHandle(), LOG_MIN, logMsg);
            success = 0;
          }
          else
          {
            setIsSet(gcpConnVerification->presenceBits, checkForDisconnectedCoresPeriodIsSet);
          }
          break;
        case 9: /* auxReconnectFailReset - UnsignedShort 15.2.4 R/W */
          if (!getByte(msgBuf, (*curNdx + curTLVlen), curNdx,
                        &gcpConnVerification->auxReconnectFailReset))
          {
            sprintf(logMsg, "%s(%d) %s tlvType(%d) tlvLen(%d) bufLen(%d) curNdx(%d)",
                    __FILE__, __LINE__, "getGcpConnVerification() AuxReconnectFailReset",
                    curTLVtype, curTLVlen, bufLen, *curNdx);
            LogPut(getGcpMsgHandle(), LOG_MIN, logMsg);
            success = 0;
          }
          else
          {
            setIsSet(gcpConnVerification->presenceBits, auxReconnectFailResetIsSet);
          }
          break;		  
        default:
          sprintf(logMsg, "%s(%d) getGcpConnVerification() unknown tlvType(%d).",
                  __FILE__, __LINE__, (int) curTLVtype);
          LogPut(getGcpMsgHandle(), LOG_MIN, logMsg);
          *curNdx += curTLVlen;
          break;
        } /* End switch curTLVtype */
      } /* End else We have curTLVlen */
    } /* End else We have curTLVtype */
  } /* End while *curNdx < bufLen && success */
  sprintf(logMsg, "%s(%d) %s, bufLen(%d) curNdx(%d) success(%d)",
          __FILE__, __LINE__,
          "getGcpConnVerification() completed", bufLen, *curNdx, (int8_t) success);
  LogPut(getGcpMsgHandle(), LOG_CAPTURE, logMsg);
  return success;
} /* End getGcpConnVerification() */

/************************************************************************
 * getGcpConnVerification()
 * - Complex TLV 15.2 variable R/W
 * - Elements: 
 *   - 1 to 13
 *   - CandidateBackupCoreTable Complex TLV 15.2 variable R
 *   - 
 ************************************************************************/
uint8_t getRpdGlobal(uint8_t *msgBuf, uint16_t bufLen, uint16_t *curNdx, RpdGlobal *rpdGlobal)
{
  uint8_t success = 1;
  uint8_t curTLVtype = 0;
  uint16_t curTLVlen = 0;
  uint16_t gcvNdx = 0;
  char logMsg[500];
  
  while (*curNdx < bufLen && success)
  {
    sprintf(logMsg, "%s(%d) %s lastTLVtype(%d) lastTLVlen(%d)  bufLen(%d) curNdx(%d)",
            __FILE__, __LINE__, "getCcapCoreIdentification()",
            curTLVtype, curTLVlen, bufLen, *curNdx);
    LogPut(getGcpMsgHandle(), LOG_CAPTURE, logMsg);
    if(!getByte(msgBuf, bufLen, curNdx, &curTLVtype))
    {
      success = 0;
      sprintf(logMsg, "%s(%d) %s, bufLen(%d) curNdx(%d)",
              __FILE__, __LINE__,
              "getCcapCoreIdentification() failed to get curTLVtype", bufLen, *curNdx);
      LogPut(getGcpMsgHandle(), LOG_MIN, logMsg);
    }
    else /* We have curTLVtype */
    {
      if (!getShort(msgBuf, bufLen, curNdx, &curTLVlen))
      {
        sprintf(logMsg, "%s(%d) %s curTLVtype(%d) bufLen(%d) curNdx(%d)",
                __FILE__, __LINE__,
                "getCcapCoreIdentification() failed to get curTLVlen ",
                curTLVtype, bufLen, *curNdx);
        LogPut(getGcpMsgHandle(), LOG_MIN, logMsg);
        success = 0;
      }
      else /* We have curTLVlen */
      {
        if (!getGcpConnVerification(msgBuf, (*curNdx + curTLVlen), curNdx,
                            &rpdGlobal->gcpConnVerification[gcvNdx]))
        {
          sprintf(logMsg, "%s(%d) %s bufLen(%d) tlvType(%d) tlvLen(%d) curNdx(%d)",
                  __FILE__, __LINE__,
                  "getResourceSet() gcpConnVerification-failed",
                  bufLen, curTLVtype, curTLVlen, *curNdx);
          LogPut(getGcpMsgHandle(), LOG_MIN, logMsg);
          success = 0;
        }
        else
        {
          setIsSet(rpdGlobal->presenceBits, gcv_coreIdIsSet);
          gcvNdx++;
           (rpdGlobal->gcpConnVerificationCount)++;
        }
        break;		  
      } /* End else We have curTLVlen */
    } /* End else We have curTLVtype */
  } /* End while *curNdx < bufLen && success */
  /*sprintf(logMsg, "%s(%d) %s, bufLen(%d) curNdx(%d) success(%d)",*/
  sprintf(logMsg, "%s(%d) getGcpConnVerification() completed, bufLen(%d) curNdx(%d) success(%d)",
          __FILE__, __LINE__, bufLen, *curNdx, (int8_t) success);
  LogPut(getGcpMsgHandle(), LOG_CAPTURE, logMsg);
  return success;
} /* End getGcpConnVerification() */

/* Serialize GcpConnVerification and return the serialized buffer (recBuf)
 * and resultent buffer size is available in bufSize. 
 * Returns 1 on success, otherwise 0
 */
uint8_t serializeGcpConnVerification(uint8_t *gcpConnVerBuf, uint16_t maxBufLen, uint16_t *bufSize, GcpConnVerification rec)
{
    uint8_t success = 1;
    char logMsg[500] = {"\0"};

    if (fieldIsSet(rec.presenceBits, gcv_coreIdIsSet) && success)
    {
        if (!putByte(1, gcpConnVerBuf, maxBufLen, bufSize))
        {
            success  = 0;
            snprintf(logMsg,sizeof(logMsg),"Error:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
            LogPut(getGcpMsgHandle(), LOG_MIN,logMsg);
        }
        else if (!putShort(rec.coreIdLen, gcpConnVerBuf, maxBufLen, bufSize))
        {
            success = 0;
            snprintf(logMsg,sizeof(logMsg),"Error:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
            LogPut(getGcpMsgHandle(), LOG_MIN,logMsg);
        }
        else if (!putVariable(gcpConnVerBuf, maxBufLen, bufSize, rec.coreId, rec.coreIdLen, sizeof(rec.coreId)))
        {
            success = 0;
            snprintf(logMsg,sizeof(logMsg),"Error:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
            LogPut(getGcpMsgHandle(), LOG_MIN,logMsg);
        }
    }
	
    if (fieldIsSet(rec.presenceBits, maxGcpIdleTimeIsSet) && success)
    {
        if (!putByte(2, gcpConnVerBuf, maxBufLen, bufSize))
        {
            success  = 0;
            snprintf(logMsg,sizeof(logMsg),"Error:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
            LogPut(getGcpMsgHandle(), LOG_MIN,logMsg);
        }
        else if (!putShort(2, gcpConnVerBuf, maxBufLen, bufSize))
        {
            success = 0;
            snprintf(logMsg,sizeof(logMsg),"Error:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
            LogPut(getGcpMsgHandle(), LOG_MIN,logMsg);
        }		
        else if (!putShort(rec.maxGcpIdleTime, gcpConnVerBuf, maxBufLen, bufSize))
        {
            success = 0;
            snprintf(logMsg,sizeof(logMsg),"Error:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
            LogPut(getGcpMsgHandle(), LOG_MIN,logMsg);
        }
    } 

    if (fieldIsSet(rec.presenceBits, gcpRecoveryActionIsSet) && success)
    {
        if (!putByte(3, gcpConnVerBuf, maxBufLen, bufSize))
        {
            success  = 0;
            snprintf(logMsg,sizeof(logMsg),"Error:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
            LogPut(getGcpMsgHandle(), LOG_MIN,logMsg);
        }
        else if (!putShort(1, gcpConnVerBuf, maxBufLen, bufSize))
        {
            success = 0;
            snprintf(logMsg,sizeof(logMsg),"Error:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
            LogPut(getGcpMsgHandle(), LOG_MIN,logMsg);
        }		
        else if (!putByte(rec.gcpRecoveryAction, gcpConnVerBuf, maxBufLen, bufSize))
        {
            success = 0;
            snprintf(logMsg,sizeof(logMsg),"Error:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
            LogPut(getGcpMsgHandle(), LOG_MIN,logMsg);
        }
    }

    if (fieldIsSet(rec.presenceBits, gcpRecoveryActionRetryIsSet) && success)
    {
        if (!putByte(4, gcpConnVerBuf, maxBufLen, bufSize))
        {
            success  = 0;
            snprintf(logMsg,sizeof(logMsg),"Error:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
            LogPut(getGcpMsgHandle(), LOG_MIN,logMsg);
        }
        else if (!putShort(1, gcpConnVerBuf, maxBufLen, bufSize))
        {
            success = 0;
            snprintf(logMsg,sizeof(logMsg),"Error:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
            LogPut(getGcpMsgHandle(), LOG_MIN,logMsg);
        }		
        else if (!putByte(rec.gcpRecoveryActionRetry, gcpConnVerBuf, maxBufLen, bufSize))
        {
            success = 0;
            snprintf(logMsg,sizeof(logMsg),"Error:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
            LogPut(getGcpMsgHandle(), LOG_MIN,logMsg);
        }
    }

    if (fieldIsSet(rec.presenceBits, gcpRecoveryActionDelayIsSet) && success)
    {
        if (!putByte(5, gcpConnVerBuf, maxBufLen, bufSize))
        {
            success  = 0;
            snprintf(logMsg,sizeof(logMsg),"Error:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
            LogPut(getGcpMsgHandle(), LOG_MIN,logMsg);
        }
        else if (!putShort(2, gcpConnVerBuf, maxBufLen, bufSize))
        {
            success = 0;
            snprintf(logMsg,sizeof(logMsg),"Error:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
            LogPut(getGcpMsgHandle(), LOG_MIN,logMsg);
        }		
        else if (!putShort(rec.gcpRecoveryActionDelay, gcpConnVerBuf, maxBufLen, bufSize))
        {
            success = 0;
            snprintf(logMsg,sizeof(logMsg),"Error:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
            LogPut(getGcpMsgHandle(), LOG_MIN,logMsg);
        }
    }

    if (fieldIsSet(rec.presenceBits, gcpReconnectTimeoutIsSet) && success)
    {
        if (!putByte(6, gcpConnVerBuf, maxBufLen, bufSize))
        {
            success  = 0;
            snprintf(logMsg,sizeof(logMsg),"Error:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
            LogPut(getGcpMsgHandle(), LOG_MIN,logMsg);
        }
        else if (!putShort(2, gcpConnVerBuf, maxBufLen, bufSize))
        {
            success = 0;
            snprintf(logMsg,sizeof(logMsg),"Error:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
            LogPut(getGcpMsgHandle(), LOG_MIN,logMsg);
        }		
        else if (!putShort(rec.gcpReconnectTimeout, gcpConnVerBuf, maxBufLen, bufSize))
        {
            success = 0;
            snprintf(logMsg,sizeof(logMsg),"Error:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
            LogPut(getGcpMsgHandle(), LOG_MIN,logMsg);
        }
    }
	
    if (fieldIsSet(rec.presenceBits, gcpHandoverTimeoutIsSet) && success)
    {
        if (!putByte(7, gcpConnVerBuf, maxBufLen, bufSize))
        {
            success  = 0;
            snprintf(logMsg,sizeof(logMsg),"Error:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
            LogPut(getGcpMsgHandle(), LOG_MIN,logMsg);
        }
        else if (!putShort(2, gcpConnVerBuf, maxBufLen, bufSize))
        {
            success = 0;
            snprintf(logMsg,sizeof(logMsg),"Error:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
            LogPut(getGcpMsgHandle(), LOG_MIN,logMsg);
        }		
        else if (!putShort(rec.gcpHandoverTimeout, gcpConnVerBuf, maxBufLen, bufSize))
        {
            success = 0;
            snprintf(logMsg,sizeof(logMsg),"Error:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
            LogPut(getGcpMsgHandle(), LOG_MIN,logMsg);
        }
    }
	
    if (fieldIsSet(rec.presenceBits, checkForDisconnectedCoresPeriodIsSet) && success)
    {
        if (!putByte(8, gcpConnVerBuf, maxBufLen, bufSize))
        {
            success  = 0;
            snprintf(logMsg,sizeof(logMsg),"Error:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
            LogPut(getGcpMsgHandle(), LOG_MIN,logMsg);
        }
        else if (!putShort(2, gcpConnVerBuf, maxBufLen, bufSize))
        {
            success = 0;
            snprintf(logMsg,sizeof(logMsg),"Error:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
            LogPut(getGcpMsgHandle(), LOG_MIN,logMsg);
        }		
        else if (!putShort(rec.checkForDisconnectedCoresPeriod, gcpConnVerBuf, maxBufLen, bufSize))
        {
            success = 0;
            snprintf(logMsg,sizeof(logMsg),"Error:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
            LogPut(getGcpMsgHandle(), LOG_MIN,logMsg);
        }
    }

    if (fieldIsSet(rec.presenceBits, auxReconnectFailResetIsSet) && success)
    {
        if (!putByte(9, gcpConnVerBuf, maxBufLen, bufSize))
        {
            success  = 0;
            snprintf(logMsg,sizeof(logMsg),"Error:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
            LogPut(getGcpMsgHandle(), LOG_MIN,logMsg);
        }
        else if (!putShort(1, gcpConnVerBuf, maxBufLen, bufSize))
        {
            success = 0;
            snprintf(logMsg,sizeof(logMsg),"Error:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
            LogPut(getGcpMsgHandle(), LOG_MIN,logMsg);
        }		
        else if (!putByte(rec.auxReconnectFailReset, gcpConnVerBuf, maxBufLen, bufSize))
        {
            success = 0;
            snprintf(logMsg,sizeof(logMsg),"Error:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
            LogPut(getGcpMsgHandle(), LOG_MIN,logMsg);
        }
    }
	
    if (success)
    {
        snprintf(logMsg,sizeof(logMsg),"%s:%s:SUCCESS\n",__FILE__,__FUNCTION__);
        LogPut(getGcpMsgHandle(), LOG_CAPTURE,logMsg);
    }	
    return success;
}

/* Serialize RpdGlobal and return the serialized buffer (recBuf)
 * and resultent buffer size is available in bufSize. 
 * Returns 1 on success, otherwise 0
 */
uint8_t serializeRpdGlobal(uint8_t *rpdGlobalBuf, uint16_t maxBufLen, uint16_t *bufSize, RpdGlobal rec)
{
  uint8_t tempBuf[MAX_GCP_MSG_SIZE] = {"\0"};
  uint16_t tempBufSize = 0;	
  uint8_t gcpConnVerBuf[MAX_GCP_MSG_SIZE] = {"\0"};
  uint16_t gcpConnVerBufSize = 0;
  uint16_t gcpConnVerNdx = 0;  
  uint8_t success = 1;
  char logMsg[500] = {"\0"};

  snprintf(logMsg,sizeof(logMsg),"%s:%d:%s: rec.gcpConnVerificationCount: %d ", __FILE__, __LINE__, __FUNCTION__, rec.gcpConnVerificationCount);
  LogPut(getGcpMsgHandle(), LOG_MIN,logMsg);
   
  if (fieldIsSet(rec.presenceBits, gcpConnVerificationIsSet) && success)
  {
    gcpConnVerNdx = 0;	
    while (success && gcpConnVerNdx < rec.gcpConnVerificationCount)
    {
      if (!putByte(2, tempBuf, maxBufLen, &tempBufSize))
      {
        success = 0;
        snprintf(logMsg,sizeof(logMsg),"Error:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
        LogPut(getGcpMsgHandle(), LOG_MIN,logMsg);
      }
      else if (!serializeGcpConnVerification(gcpConnVerBuf, maxBufLen, &gcpConnVerBufSize, rec.gcpConnVerification[gcpConnVerNdx]))
      {
        success = 0;
        snprintf(logMsg,sizeof(logMsg),"Error:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
        LogPut(getGcpMsgHandle(), LOG_MIN,logMsg);
      }	  
      else if (!putShort(gcpConnVerBufSize, tempBuf, maxBufLen, &tempBufSize))
      {
        success = 0;
        snprintf(logMsg,sizeof(logMsg),"Error:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
        LogPut(getGcpMsgHandle(), LOG_MIN,logMsg);
      }
      else if (!putComplexTlv(tempBuf, maxBufLen, &tempBufSize, gcpConnVerBuf, gcpConnVerBufSize))
      {
        success = 0;
        snprintf(logMsg,sizeof(logMsg),"Error:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
        LogPut(getGcpMsgHandle(), LOG_MIN,logMsg);
      }
      gcpConnVerNdx++;
    } /* End while pwaNdx */
  }
 
  if (success)
  {
    if (!putComplexTlv(rpdGlobalBuf, maxBufLen, bufSize, tempBuf, tempBufSize))
    {
      snprintf(logMsg,sizeof(logMsg),"Error:%s:%s:FAILED\n",__FILE__,__FUNCTION__);
      LogPut(getGcpMsgHandle(), LOG_MIN,logMsg);
    }
    else 
    {
      snprintf(logMsg,sizeof(logMsg),"%s:%s:SUCCESS\n",__FILE__,__FUNCTION__);
      LogPut(getGcpMsgHandle(), LOG_CAPTURE,logMsg);
    }
  }

  snprintf(logMsg,sizeof(logMsg),"%s:%d:%s: maxBufLen: %d ", __FILE__, __LINE__, __FUNCTION__, maxBufLen);
  LogPut(getGcpMsgHandle(), LOG_MIN,logMsg);

  return success;
}

/************************************************************************
 * 
 ************************************************************************/
void logPutGcpConnVerification(EMHANDLE* emHandle, enum LOGMODE_T logLevel,
                                char *msg, GcpConnVerification *gcpConnVerification)
{
  char logMsg[1024];
  int logThreshold = 0;
	
  if (logLevel == LOG_NONE)
  {
    return;
  }
  else if (emHandle != NULL)
  {
    logThreshold = EMGetIntValuebyName(emHandle,"logmode",0);
    if (logThreshold < logLevel)
    {
      return;
    }
  }
  if (gcpConnVerification == NULL)
  {
    snprintf(logMsg,sizeof(logMsg), "%s:%d:%s:InvalidParamsError",
             __FILE__,__LINE__,__FUNCTION__);
    LogPut(emHandle, LOG_MIN, logMsg);
  }
  else
  {
    snprintf(logMsg,sizeof(logMsg),"%s", msg);
    strnCatMacAddrBin(logMsg, sizeof(logMsg),
                      gcpConnVerification->presenceBits,
                      gcpConnVerification->readBits,
                      gcv_coreIdIsSet,
                      "coreId=", gcpConnVerification->coreId);
    strnCatShort(logMsg, sizeof(logMsg),
                 gcpConnVerification->presenceBits,
                 gcpConnVerification->readBits,
                 maxGcpIdleTimeIsSet,
                 ",maxGcpIdleTime=", gcpConnVerification->maxGcpIdleTime);
    strnCatByte(logMsg, sizeof(logMsg),
                gcpConnVerification->presenceBits,
                gcpConnVerification->readBits,
                gcpRecoveryActionIsSet,
                ",gcpRecoveryAction=", gcpConnVerification->gcpRecoveryAction);
    strnCatByte(logMsg, sizeof(logMsg),
                gcpConnVerification->presenceBits,
                gcpConnVerification->readBits,
                gcpRecoveryActionRetryIsSet,
                ",gcpRecoveryActionRetry=", gcpConnVerification->gcpRecoveryActionRetry);				 
    strnCatShort(logMsg, sizeof(logMsg),
                 gcpConnVerification->presenceBits,
                 gcpConnVerification->readBits,
                 gcpRecoveryActionDelayIsSet,
                 ",gcpRecoveryActionDelay=", gcpConnVerification->gcpRecoveryActionDelay);
    strnCatShort(logMsg, sizeof(logMsg),
                 gcpConnVerification->presenceBits,
                 gcpConnVerification->readBits,
                 gcpReconnectTimeoutIsSet,
                 ",gcpReconnectTimeout=", gcpConnVerification->gcpReconnectTimeout);				 
    strnCatShort(logMsg, sizeof(logMsg),
                 gcpConnVerification->presenceBits,
                 gcpConnVerification->readBits,
                 checkForDisconnectedCoresPeriodIsSet,
                 ",checkForDisconnectedCoresPeriod=", gcpConnVerification->checkForDisconnectedCoresPeriod);
	strnCatShort(logMsg, sizeof(logMsg),
                 gcpConnVerification->presenceBits,
                 gcpConnVerification->readBits,
                 gcpHandoverTimeoutIsSet,
                 ",gcpHandoverTimeout=", gcpConnVerification->gcpHandoverTimeout);
    strnCatByte(logMsg, sizeof(logMsg),
                 gcpConnVerification->presenceBits,
                 gcpConnVerification->readBits,
                 auxReconnectFailResetIsSet,
                 ",auxReconnectFailReset=", gcpConnVerification->auxReconnectFailReset);				 
    LogPut(emHandle, logLevel, logMsg);
  }
}

/************************************************************************
 *
 ************************************************************************/
void logPutRpdGlobal(EMHANDLE* emHandle, enum LOGMODE_T logLevel,
                                  char *msg, RpdGlobal *rpdGlobal)
{
  char logMsg[500];
  char tmpLogMsg[500];
  int logThreshold = 0;
  int ndx;

  if (logLevel == LOG_NONE)
  {
    return;
  }
  else if (emHandle != NULL)
  {
    logThreshold = EMGetIntValuebyName(emHandle,"logmode",0);
    if (logThreshold < logLevel)
    {
      return;
    }
  }
  if (rpdGlobal == NULL)
  {
    snprintf(logMsg,sizeof(logMsg), "%s:%d:%s:InvalidParamsError",
             __FILE__,__LINE__,__FUNCTION__);
    LogPut(emHandle, LOG_MIN, logMsg);
  }
  else
  {
    snprintf(logMsg,sizeof(logMsg),"%s", msg);
    if (fieldIsSet(rpdGlobal->readBits, gcpConnVerificationIsSet))
    {
      strncat(logMsg, getReadBitSetStr(), sizeof(logMsg));
      LogPut(emHandle, logLevel, logMsg);
    }
    else
    {
      snprintf(tmpLogMsg,sizeof(tmpLogMsg), "Len=%d",
               rpdGlobal->gcpConnVerificationCount);
      strncat(logMsg, tmpLogMsg, sizeof(logMsg));
      LogPut(emHandle, logLevel, logMsg);

      for (ndx = 0; ndx < rpdGlobal->gcpConnVerificationCount; ndx++)
      {
        logPutGcpConnVerification(emHandle, logLevel,
            logMsg, &rpdGlobal->gcpConnVerification[ndx]);
      }
    }
  }
}
