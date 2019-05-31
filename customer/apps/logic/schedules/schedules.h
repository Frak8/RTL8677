/**********************************************************************

 Copyright (c), 1991-2008, T&W ELECTRONICS(SHENTHEN) Co., Ltd.

 �ļ�����: schedules.h

 �ļ�����: schedulesģ��ͷ�ļ�

 �޶���¼:

        1. ����: 

           ����: 2009-7-10

           ����: �����ļ�

**********************************************************************/

#ifndef __SCHED_H__
#define __SCHED_H__

#include "warnlog.h"

/******************************************************************************
 *                                 MACRO                                      *
 ******************************************************************************/

/*********************************************************************
 *                              DEBUG                                *
 *********************************************************************/
#ifdef SCHED_DEBUG
#define SCHED_TRACE(fmt, args...) COMMON_TRACE(MID_SCHED, ECHO_GREEN fmt ECHO_NONE, ##args)
#define SCHED_LOG_ERR(fmt, args...) COMMON_TRACE(MID_SCHED, ECHO_RED fmt ECHO_NONE, ##args)
#define SCHED_TRACE_INTO_FUNC(fmt, args...)  SCHED_TRACE("##In## %s\n", __func__)
#define SCHED_TRACE_OUT_FUNC(fmt, args...)  SCHED_TRACE("##Out## %s\n", __func__)
#define APCMD_PRINT 1 /* ����tbsSystemʱ��ӡ���� */
#else
    #define SCHED_TRACE(fmt, args...)
    #define SCHED_LOG_ERR(fmt, args...)  COMMON_LOG_ERROR(MID_SCHED, fmt, ##args)
    #define SCHED_TRACE_INTO_FUNC(fmt, args...)
    #define SCHED_TRACE_OUT_FUNC(fmt, args...) 
    #define APCMD_PRINT 0 
#endif

/********************************************************************
 *                   �Ժ�������ֵ������ļ򻯶���                 *
 ********************************************************************/
#define SCHED_RET_GOTO(Ret, gotoTag, fmt, args...) \
        if ( RET_FAILED(Ret) )  \
        {\
            SCHED_TRACE(fmt, ##args); \
            goto gotoTag; \
        }

#define SCHED_RET_RETURN(Ret, fmt, args...)  \
        if ( RET_FAILED(Ret) ) \
        { \
            SCHED_TRACE(fmt, ##args); \
            return Ret; \
        }

#define SCHED_PTR_GOTO(Pointer,gotoTag, fmt, args...)  \
        if (!Pointer)    \
        { \
            SCHED_TRACE(fmt, ##args); \
            goto gotoTag; \
        }

#define SCHED_PTR_RETURN(Pointer, Ret, fmt, args...) \
        if (!Pointer)    \
        { \
            SCHED_TRACE(fmt, ##args); \
            return Ret; \
        }

#define SCHED_CHECK_NULL_PTR(ptr)   SCHED_PTR_RETURN(ptr, TBS_NULL_PTR, "TBS_NULL_PTR\n")

/*******************************************************************
 *                       ���ýڵ�ֵ�ļ򻯺궨��                    *
 *******************************************************************/
#define SET_NODE_VAL(szPath, szValue)  \
        if (CFG_RET_FAILED(CFG_SetNodeVal(szPath, szValue, NULL))) \
        {\
            SCHED_TRACE("Set cfg value fail: %s=%s\n", szPath, szValue);  \
            return TBS_FAILED;  \
        }

/*******************************************************************
 *                          �ڵ�·������                           *
 *******************************************************************/
#define SCHED_PATH_PREFIX     "InternetGatewayDevice.X_TWSZ-COM_SCHEDULES."
#define SCHED_PATH_SCHED         SCHED_PATH_PREFIX "SchedEntry."

#define SCHED_NODE_ENABLE     "Enable"
#define SCHED_NODE_MAX      "MaxEntries"
#define SCHED_NODE_NUMOFSCHED    "SchedulesNumberOfEntries"

#define SCHED_NODE_NAME      "SchedName"
#define SCHED_NODE_USED     "SchedUsed"
#define SCHED_NODE_ENABLEALLDAYS   "EnableAllDays"
#define SCHED_NODE_DAYS   "SelectDays"
#define SCHED_NODE_ENABLEALLTIMES   "EnableAllTimes"
#define SCHED_NODE_STARTTIME   "StartTime"
#define SCHED_NODE_ENDTIME   "EndTime"

#define TIME_PATH_PREFIX    "InternetGatewayDevice.Time."
#define TIME_NODE_ENABLE    "Enable"

/*****************************************************************
 *                     �ڵ�·��ƥ��������ʽ                    *
 *****************************************************************/
#define SCHED_PATH_PREFIX_REGEX "^InternetGatewayDevice\\.X_TWSZ-COM_SCHEDULES\\."
#define SCHED_PATH_SCHED_REGEX     SCHED_PATH_PREFIX_REGEX "SchedEntry\\.[0-9]+\\."
#define END "$"
#define SCHED_ADD_INST_REGEX SCHED_PATH_PREFIX_REGEX "SchedEntry\\." END

#define SCHED_SCHED_NODE_REGEX(node)  SCHED_PATH_SCHED_REGEX node END

/*****************************************************************
 *                       ��ӡģʽ�ַ���                          *
 *****************************************************************/
#define CALL_FUCTION_FAILED     "Call function \"%s\" failed\n"

/*****************************************************************
 *                             Others                            *
 *****************************************************************/
#define MAX_INDEX_COUNT     1
#define SCHED_MAX_IFNAME_LEN  16
#define SCHED_SUBCMD_LEN_1    16
#define SCHED_SUBCMD_LEN_2    32
#define SCHED_SUBCMD_LEN_3    64

/******************************************************************************
 *                                 ENUM                                       *
 ******************************************************************************/
/* ʵ�����Ͷ��� */
enum {
    QOS_IF = 1,
    QOS_QUEUE,
    QOS_CLASS
};

/* ��Ч������ʽ */
typedef enum {
    QOS_APPLY_NON,
    QOS_APPLY_ADD,
    QOS_APPLY_DEL
} EN_QOS_APPLY;

/*�������÷�ʽ*/
enum {
    QOS_CLS_QUEUE = 1,   //����ƥ���󶨶���
    QOS_CLS_MARKING      //����ƥ�����б��
};

/******************************************************************************
 *                                      END                                   *
 ******************************************************************************/

#endif /* __SCHED_H__ */

