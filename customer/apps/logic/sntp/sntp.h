/**********************************************************************
 Copyright (c), 1991-2009, T&W ELECTRONICS(SHENTHEN) Co., Ltd.

 �ļ�����: sntp.h
 �ļ�����: sntpģ��ͷ�ļ�

 �޶���¼:
        1. ����: XuXiaodong
           ����: 2008-12-23
           ����: �����ļ�
**********************************************************************/

#ifndef __SNTP_H__
#define __SNTP_H__

#include "warnlog.h"

/******************************************************************************
*                                 MACRO                                      *
******************************************************************************/

/*********************************************************************
 *                              DEBUG                                *
 *********************************************************************/

#ifdef SNTP_DEBUG
    #define SNTP_TRACE(fmt, args...) COMMON_TRACE(MID_SNTP, ECHO_GREEN fmt ECHO_NONE, ##args)
    #define SNTP_LOG_ERR(fmt, args...) COMMON_TRACE(MID_SNTP, ECHO_RED fmt ECHO_NONE, ##args)
    #define CMD_PRINT 1   /* ����tbsSystemʱ��ӡ���� */
#else
    #define SNTP_TRACE(fmt, args...) 
    #define SNTP_LOG_ERR(fmt, args...) COMMON_LOG_ERROR(MID_SNTP, fmt, ##args)
    #define CMD_PRINT 0
#endif

#define SNTP_TRACE_INTO_FUNC SNTP_TRACE("##In## %s\n", __func__)

/********************************************************************
 *                   �Ժ�������ֵ������ļ򻯶���                 *
 ********************************************************************/
#define SNTP_RET_GOTO(Ret, gotoTag, fmt, args...) \
        if ( RET_FAILED(Ret) )  \
        {\
            SNTP_TRACE(fmt, ##args); \
            goto gotoTag; \
        }

#define SNTP_RET_RETURN(Ret, fmt, args...)  \
        RET_RETURN(MID_SNTP, Ret, fmt, ##args)

#define SNTP_PTR_GOTO(Pointer,gotoTag, fmt, args...)  \
        POINTER_GOTO(MID_SNTP, Pointer, gotoTag, fmt, ##args)

#define SNTP_PTR_RETURN(Pointer, Ret, fmt, args...) \
        if (!Pointer)    \
        { \
            SNTP_TRACE(fmt, ##args); \
            return Ret; \
        }

#define SNTP_CHECK_NULL_PTR(ptr)   SNTP_PTR_RETURN(ptr, TBS_NULL_PTR, "TBS_NULL_PTR\n")

/*******************************************************************
 *                       ���ýڵ�ֵ�ļ򻯺궨��                    *
 *******************************************************************/
#define SNTP_SET_NODE_VAL(pszPath, pszVal) \
    { \
        iRet = CFG_SetNodeVal(pszPath, pszVal, NULL); \
        SNTP_RET_RETURN(iRet, "Set node \"%s\" value failed\n", pszPath); \
    }

/*******************************************************************
 *                          �ڵ�·������                           *
 *******************************************************************/
#define SNTP_PATH_PREFIX  "InternetGatewayDevice.Time."

#define SNTP_NODE_ENABLE          "Enable"
#define SNTP_NODE_SYN_STATE       "X_TWSZ-COM_Status"
#define SNTP_NODE_UP_INTERVAL     "X_TWSZ-COM_UpdateInterval"
#define SNTP_NODE_RETRY_INTERVAL  "X_TWSZ-COM_RetryInterval"
#define SNTP_NODE_NTPSVR1         "NTPServer1"
#define SNTP_NODE_NTPSVR2         "NTPServer2"
#define SNTP_NODE_CUR_LOCALTIME   "CurrentLocalTime"
#define SNTP_NODE_LOCAL_TZ        "LocalTimeZone"
#define SNTP_NODE_LOCAL_TZ_NAME   "LocalTimeZoneName"
#define SNTP_NODE_DS_USED         "DaylightSavingsUsed"
#define SNTP_NODE_DS_START        "DaylightSavingsStart"
#define SNTP_NODE_DS_END          "DaylightSavingsEnd"

#define WAN_DEV_PREFIX      "InternetGatewayDevice.WANDevice."
#define KEY_WAN_CONN_DEV    "WANConnectionDevice"
#define KEY_WAN_IP_CONN     "WANIPConnection"

#define DEFAULT_ROUTE_PATH   "InternetGatewayDevice.X_TWSZ-COM_DEFAULT_GW"

/*****************************************************************
 *                     �ڵ�·��ƥ��������ʽ                    *
 *****************************************************************/
#define SNTP_PATH_PREFIX_REGEX "^InternetGatewayDevice\\.Time\\."
#define SNTP_NODE_REGEX(node) SNTP_PATH_PREFIX_REGEX node "$"

#define WAN_IPCONN_INST_REGEX  "^InternetGatewayDevice\\.WANDevice\\.[0-9]+\\." KEY_WAN_CONN_DEV \
                               "\\.[0-9]+\\." KEY_WAN_IP_CONN "\\.[0-9]+$"

/*****************************************************************
 *                       ��ӡģʽ�ַ���                          *
 *****************************************************************/
#define CALL_FUCTION_FAILED     "Call function \"%s\" failed\n"
#define SET_NODE_VAL_FAILED     "Set node \"%s\" value failed\n"
#define GET_NODE_VAL_FAILED     "Get node \"%s\" value failed\n"

#define WAN_IPCONN_INST_PATH_FMT  WAN_DEV_PREFIX "%lu." KEY_WAN_CONN_DEV ".%lu." KEY_WAN_IP_CONN ".%lu"

/****************************************************************
 *                         ntpͬ��״̬                          *
 ****************************************************************/
#define UNSYN      "Unsynchronized"
#define SYNING     "Synchronizing"
#define SYNED      "Synchronized"
#define SYNFAILED  "Error_FailedToSynchronize"

/*****************************************************************
 *                             Others                            *
 *****************************************************************/
#define SNTP_TIMEZONE_FILE "/etc/TZ"
#define TZ_BUF_LEN        56
#define MAX_DATE_TIME_LEN 21
#define TIME_RATIO        60
#define MAX_NTP_SVRS_NUM  7
#define MAX_START_PARAM_NUM (5+MAX_NTP_SVRS_NUM)

/****************************************************************
 *                         WAN����״̬                          *
 ****************************************************************/
#define WAN_STATE_CONNECTED         "Connected"
#define NODE_ConnectionStatus       "ConnectionStatus"
#define NODE_TriggerType            "ConnectionTrigger"

/* ���㷢�͵���ϢID */
#define SNTP_GET_MSG_ID  (++g_stSntpStateInfo.ulMsgId == 0) ? 1 : g_stSntpStateInfo.ulMsgId

/******************************************************************************
 *                                  STRUCT                                    *
 ******************************************************************************/
typedef struct tag_ST_SNTP_WANIPCONN_INST{
    struct {
        unsigned long ulWanDevIdx;
        unsigned long ulWanConnDevIdx;
        unsigned long ulWanIpConnIdx;
    } stIndex;
    BOOL bNtpsState;
    struct tag_ST_SNTP_WANIPCONN_INST *pstNext;
}ST_SNTP_WANIPCONN_INST;

/* SNTPģ���ȫ����Ϣ */
typedef struct tag_ST_SNTP_STATE_INFO {
    unsigned long ulTimerId;
    unsigned long ulMsgId;
    unsigned char ucStateMachine;
    unsigned char ucPad[3];
} ST_SNTP_STATE_INFO;

/******************************************************************************
 *                                      END                                   *
 ******************************************************************************/

#endif /*__SNTP_H__*/
