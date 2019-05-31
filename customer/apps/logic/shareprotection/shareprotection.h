/**********************************************************************
 Copyright (c), 1991-2008, T&W ELECTRONICS(SHENTHEN) Co., Ltd.

 �ļ�����: antiattack.h

 �ļ�����: ������ģ���ͷ�ļ�

 �޶���¼:

           1 ���� : ������
             ���� : 2008-11-29
             ���� : ��FIREWALLģ���з������

**********************************************************************/
#ifndef __TBS_ANTIATTACK_H
#define __TBS_ANTIATTACK_H

#include "common.h"

#include "new_msg.h"
#include "common_func.h"
#include "common_msg_handler.h"


/******************************************************************************
*                                 MACRO
******************************************************************************/
/* ������Ϣ��*/
//#define SPT_DEBUG
#ifdef SPT_DEBUG
#define SPT_TRACE(fmt, args...)                 COMMON_TRACE(MID_SPT, fmt"\n", ##args)

#define SPT_FUNC_IN(fmt, args...)               SPT_TRACE("++I %s, "fmt"\n", __FUNCTION__, ##args)
#define SPT_FUNC_OUT()                          SPT_TRACE("--O %s\n", __FUNCTION__)

#else
#define SPT_TRACE(str, args...)

#define SPT_FUNC_IN(fmt, args...)
#define SPT_FUNC_OUT()


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

#define SPT_LOG_ERR(fmt, args...)                          COMMON_LOG_ERROR(MID_SPT, fmt, ##args)

#define SPT_RET_LOG(iRet, strError, args...)               RET_LOG(MID_SPT, iRet, strError, ##args)
#define SPT_POINTER_LOG(pRet, strError, args...)           POINTER_LOG(MID_SPT, pRet, strError, ##args)

#define SPT_RET_RETURN(iRet, strError, args...)            RET_RETURN(MID_SPT, iRet, strError, ##args)
#define SPT_POINTER_RETURN(pRet, strError, args...)        POINTER_RETURN(MID_SPT, pRet, strError, ##args)

#define SPT_RET_GOTO(iRet, strError, gotoTag)              RET_GOTO(MID_SPT, iRet, strError, gotoTag)
#define SPT_POINTER_GOTO(pRet, gotoTag, strError, args...) POINTER_GOTO(MID_SPT, pRet, gotoTag ,strError, ##args)

#define SPT_RET_FALSE(iRet, strError, args...)             RET_FALSE(MID_SPT, iRet, strError, ##args)
#define SPT_POINTER_FALSE(pRet, strError, args...)         POINTER_FALSE(MID_SPT, pRet, strError, ##args)

#define ERR_GET(node)                       "Get Node["node"] value failed.\n"
#define ERR_SET(node)                       "Set Node["node"] value failed.\n"
#define ERR_SET_VAL(node)                   "Set Node["node"] value failed, value = %s\n"


/************************** �������� **************************/

/* �ڵ������� */

#define NODE_IPIDEnable                		"IPIDEnable"
#define NODE_TTLEnable                			"TTLEnable"
#define NODE_CONNLIMITEnable                	"CONNLIMITEnable"
#define NODE_TTLValue			              "TTLValue"
#define NODE_TCPCONNValue        			"TCPCONNValue"
#define NODE_UDPCONNValue		             "UDPCONNValue"


/* �ڵ�·������ */
#define SPT_PATH_PREFIX                    "InternetGatewayDevice.X_TWSZ-COM_ShareProtection."

/* ������ */
#define SPT_REGEX_PREFIX                   "^InternetGatewayDevice\\.X_TWSZ-COM_ShareProtection\\."



/******************************************************************************
*                                ENUM
******************************************************************************/
#define CHAIN_SPT					"ShareProtection"



/*******************************************************************
*
* FUNC DECLEARATION
*
*******************************************************************/
/* Init && Destroy */
int SPT_ModuleInit();
int SPT_ModuleDestroy();


#endif//__TBS_ANTIATTACK_H

