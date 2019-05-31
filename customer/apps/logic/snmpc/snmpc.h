/**********************************************************************
 Copyright (c), 1991-2008, T&W ELECTRONICS(SHENTHEN) Co., Ltd.

 �ļ�����: snmpc.h

 �ļ�����: snmpcģ���ͷ�ļ�

 �޶���¼:
           1 ���� : ����ƽ
             ���� : 2008-4-16
             ���� :

           2 ���� : ������
             ���� : 2008-11-10
             ���� : �ع�
**********************************************************************/
#ifndef __TBS_SNMPC_H
#define __TBS_SNMPC_H

#include <time.h>
#include "common.h"

#include "new_msg.h"
#include "common_func.h"
#include "common_msg_handler.h"

#include "autoconf.h"



/******************************************************************************
 *                                 MACRO                                      *
 ******************************************************************************/
/* ������Ϣ��*/

#ifdef SNMPC_DEBUG
#define SNMPC_TRACE(fmt, args...)               COMMON_TRACE(MID_SNMPC, fmt"\n", ##args)
#define SNMPC_FUNC_IN(fmt, args...)             SNMPC_TRACE("++I %s, "fmt"\n", __FUNCTION__, ##args)
#define SNMPC_FUNC_OUT()                        SNMPC_TRACE("--O %s\n", __FUNCTION__)


#else
#define SNMPC_TRACE(str, args...)
#define SNMPC_FUNC_IN(fmt, args...)
#define SNMPC_FUNC_OUT()

/* ģ��رյ���ʱ�ص��ڴ���� */
#ifdef malloc
#undef malloc
#endif
#ifdef free
#undef free
#endif
#ifdef calloc
#undef calloc
#endif
#ifdef realloc
#undef realloc
#endif

#endif

#define SNMPC_LOG_ERR(fmt, args...)                             COMMON_LOG_ERROR(MID_SNMPC, fmt, ##args)

#define SNMPC_RET_LOG(iRet, strError, args...)                  RET_LOG(MID_SNMPC, iRet, strError, ##args)
#define SNMPC_POINTER_LOG(pRet, strError, args...)              POINTER_LOG(MID_SNMPC, pRet, strError, ##args)

#define SNMPC_RET_RETURN(iRet, strError, args...)               RET_RETURN(MID_SNMPC, iRet, strError, ##args)
#define SNMPC_POINTER_RETURN(pRet, strError, args...)           POINTER_RETURN(MID_SNMPC, pRet, strError, ##args)

#define SNMPC_RET_GOTO(iRet, strError, gotoTag)                 RET_GOTO(MID_SNMPC, iRet, strError, gotoTag)
#define SNMPC_POINTER_GOTO(pRet, gotoTag, strError, args...)    POINTER_GOTO(MID_SNMPC, pRet, gotoTag ,strError, ##args)

#define SNMPC_RET_FALSE(iRet, strError, args...)                RET_FALSE(MID_SNMPC, iRet, strError, ##args)
#define SNMPC_POINTER_FALSE(pRet, strError, args...)            POINTER_FALSE(MID_SNMPC, pRet, strError, ##args)

#define ERR_GET(node)                   "Get Node["node"] value failed.\n"
#define ERR_SET(node)                   "Set Node["node"] value failed.\n"
#define ERR_SET_VAL(node)               "Set Node["node"] value failed, value = %s\n"



/************************** �������� ************************/
#define SNMPC_AGENT_NAME			    "/usr/sbin/snmpd"
#define SNMPC_AGENT_CONF_FILE		    "/tmp/snmpd.conf"
#define SNMPC_AGENT_MAX_ARGS  	        16                  /* snmpd���������� */


/* ���� */
#define SNMPC_REGEX_PREFIX			    "^InternetGatewayDevice\\.X_TWSZ-COM_SNMPAgent\\."

/* �ڵ�·������ */
#define SNMPC_PATH_PREFIX		        "InternetGatewayDevice.X_TWSZ-COM_SNMPAgent."

#define NODE_Enable				        "Enable"
#define NODE_Status				        "Status"

#define NODE_ROCommunity		        "ROCommunity"
#define NODE_RWCommunity		        "RWCommunity"
#define NODE_WarnStartFlag              "WarnStartFlag"

#define NODE_TrapVersion		        "TrapVersion"
#define NODE_TrapHost			        "TrapHost"
#define NODE_TrapCommunity	            "TrapCommunity"

#define NODE_PollInterval		        "PollInterval"
#define NODE_CPUDuration	            "CPUDuration"
#define NODE_CPUThreshold               "CPUThreshold"
#define NODE_MemDuration	            "MemDuration"
#define NODE_MemThreshold	            "MemThreshold"

/* �ڵ�ȫ·���� */
#define SNMPC_PATH_ENABLE				SNMPC_PATH_PREFIX NODE_Enable
#define SNMPC_PATH_STATUS				SNMPC_PATH_PREFIX NODE_Status


/* �ڵ�ö��ֵ���� */
#define SNMPC_AGENT_STATUS_STOPPED	    0
#define SNMPC_AGENT_STATUS_STARTED	    1
#define SNMPC_AGENT_STATUS_STOPPING	    2
#define SNMPC_AGENT_STATUS_STARTING	    3

/* Agent��������ʧ��������Դ��� */
#define SNMPC_AGENT_MAX_START_FAIL	    3

/* Ĭ�϶˿ں� */
#define SNMPC_DEF_AGENT_PORT		    161
#define SNMPC_DEF_TRAP_PORT			    162

#define SNMP_V1		                    1
#define SNMP_V2C	                    2
#define SNMP_V3		                    3

/******************************************************************************
 *                                ENUM                                      *
 ******************************************************************************/



/******************************************************************************
 *                                STRUCT                                      *
 ******************************************************************************/

typedef struct tag_ST_SNMPC_Nodes
{
    int     bEnable;
    int     iStatus;
    char    *szROCommunity;
    char    *szRWCommunity;
    char    *szTrapCommunity;
    char    *szTrapHost;
    int     szTrapVersion;
    int     iPollInterval;
    int     iCPUDuration;
    int     iCPUThreshold;
    int     iMemDuration;
    int     iMemThreshold;
}ST_SNMPC_Nodes;

typedef struct tag_ST_SNMPC_Private
{
    int                     iStatus;            /* Agent����״̬: SNMPC_AGENT_STATUS_xxxxx */
    unsigned long           ulStartFail;        /* Agent��������ʧ�ܼ��� */
    unsigned long           ulAbnormallyExit;   /* Agent�����쳣�˳����� */
    unsigned char           ucMachineState;     /* Agent����״̬,PC��¼ */
} ST_SNMPC_Private;                             /* Agent����ʱ��Ϣ */


/* SNMPC������Ϣ */
typedef struct tagST_SNMPC_RUNTIME_INFO
{
    ST_SNMPC_Nodes stNodes;
    ST_SNMPC_Private stPrivate;
} ST_SNMPC_RUNTIME_INFO;


/*******************************************************************
*
* FUNC DECLEARATION
*
*******************************************************************/
/* Init and Destroy */
int SNMPC_ModuleInit(void);
int SNMPC_ModuleDestroy(void);


#endif //__TBS_SNMPC_H

