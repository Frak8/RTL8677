/**********************************************************************
 Copyright (c), 1991-2009, T&W ELECTRONICS(SHENTHEN) Co., Ltd.

 �ļ�����: sch.h
 �ļ�����: ��Schedule��صĹ���ͷ�ļ�
 �޶���¼:
            ���� : ������
            ���� : 2009-08-29
            ���� :

**********************************************************************/
#ifndef _SCH_H_
#define _SCH_H_

#include "common.h"
#include "new_msg.h"
#include "common_func.h"
#include "common_msg_handler.h"


/*********************************************************************
 *                              DEBUG                                *
 *********************************************************************/
#define MID_SCH  0xFF

#ifdef SCH_DEBUG
    #define SCH_TRACE(fmt, args...)     COMMON_TRACE(MID_SCH, fmt, ##args)
    #define SCH_LOG_ERR(fmt, args...)   COMMON_TRACE(MID_SCH, fmt, ##args)
#else
    #define SCH_TRACE(fmt, args...)
    #define SCH_LOG_ERR(fmt, args...)   COMMON_LOG_ERROR(MID_SCH, fmt, ##args)
#endif

#define SCH_TRACE_INTO_FUNC SCH_TRACE("##In## %s\n", __func__)

/********************************************************************
 *                   �Ժ�������ֵ������ļ򻯶���                 *
 ********************************************************************/
#define SCH_RET_LOG(iRet, strError, args...)                 RET_LOG(MID_SCH, iRet, strError, ##args)
#define SCH_POINTER_LOG(pRet, strError, args...)             POINTER_LOG(MID_SCH, pRet, strError, ##args)

#define SCH_RET_RETURN(iRet, strError, args...)              RET_RETURN(MID_SCH, iRet, strError, ##args)
//#define SCH_POINTER_RETURN(pRet, strError, args...)          POINTER_RETURN(MID_SCH, pRet, strError, ##args)
#define SCH_POINTER_RETURN(Pointer, Ret, fmt, args...) \
        if (!Pointer)    \
        { \
            SCH_TRACE(fmt, ##args); \
            return Ret; \
        }


//#define SCH_RET_GOTO(iRet, strError, gotoTag)              RET_GOTO(MID_SCH, iRet, strError, gotoTag)
#define SCH_RET_GOTO(Ret, gotoTag, fmt, args...) \
        if ( RET_FAILED(Ret) )  \
        {\
            SCH_TRACE(fmt, ##args); \
            goto gotoTag; \
        }
#define SCH_POINTER_GOTO(pRet, gotoTag, strError, args...)   POINTER_GOTO(MID_SCH, pRet, gotoTag ,strError, ##args)

#define SCH_RET_FALSE(iRet, strError, args...)               RET_FALSE(MID_SCH, iRet, strError, ##args)
#define SCH_POINTER_FALSE(pRet, strError, args...)           POINTER_FALSE(MID_SCH, pRet, strError, ##args)



#define SCH_CHECK_NULL_PTR(ptr)                              SCH_POINTER_RETURN(ptr, TBS_NULL_PTR, "TBS_NULL_PTR\n")

#define SCH_ASSERT(expr) \
    do{ \
        if(!(expr))\
        {\
            SCH_TRACE("Assert \"%s\" failed\n", #expr); \
            exit(-1); \
        } \
    }while(0)


/*
 ·������
*/

/* ����: schedule����ʵ�� */
#define SCH_REGEX_ENTRY     "^InternetGatewayDevice\\.X_TWSZ-COM_SCHEDULES\\.SchedEntry\\.[0-9]{1,5}"


/******************************************************************************
 *                                    ENUM                                    *
 ******************************************************************************/

/* Schedule������ʽ */
enum
{
    EN_SCH_INC,
    EN_SCH_DEC
};

/******************************************************************************
 *                                  STRUCT                                    *
 ******************************************************************************/

/* schedule���� */
typedef struct tag_ST_SCH_Rule
{
    char *szSchedUsed;
    char *szStartTime;
    char *szEndTime;
    char *szSelectDays;
}ST_SCH_Rule;


/******************************************************************************
 *                             Function Prototype                             *
 ******************************************************************************/

/* ����/����Schedule�������ü��� */
int SCH_UpdateRef(const char *pszSchPath, int iAction);

/* ����Schedule��·�������������л�ȡSchedule����ʵ�� */
int SCH_GetRule(const char *pszSchPath, ST_SCH_Rule *pstSchRule);

/* ����SchedulePath�л�ʱ������Schedule�������ü��� */
int SCH_SwitchRule(const char *pszSchPath_Old, const char *pszSchPath_New);


#endif /*_SCH_H_*/

