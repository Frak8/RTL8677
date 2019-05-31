/**********************************************************************
 Copyright (c), 1991-2009, T&W ELECTRONICS(SHENTHEN) Co., Ltd.

 �ļ�����: layer2br.h
 �ļ�����: ��Layer2Bridging������صĹ���ͷ�ļ�
 �޶���¼:  
            ���� : �����
            ���� : 2009-8-20
            ���� : 

**********************************************************************/
#ifndef _LAYER2BR_H_
#define _LAYER2BR_H_


#include <stdlib.h>
#include "warnlog.h"
#include "new_msg.h"

/*********************************************************************
 *                              DEBUG                                *
 *********************************************************************/
#define MID_LAYER2BR  0xFE   

#ifdef LAYER2BR_DEBUG
    #define LAYER2BR_TRACE(fmt, args...) COMMON_TRACE(MID_LAYER2BR, fmt, ##args)
    #define LAYER2BR_LOG_ERR(fmt, args...) COMMON_TRACE(MID_LAYER2BR, fmt, ##args)
    //#define CMD_PRINT 1   /* ����tbsSystemʱ��ӡ���� */
#else
    #define LAYER2BR_TRACE(fmt, args...) 
    #define LAYER2BR_LOG_ERR(fmt, args...) COMMON_LOG_ERROR(MID_LAYER2BR, fmt, ##args)
    #define CMD_PRINT 0
#endif

#define LAYER2BR_TRACE_INTO_FUNC LAYER2BR_TRACE("##In## %s\n", __func__)

/********************************************************************
 *                   �Ժ�������ֵ������ļ򻯶���                 *
 ********************************************************************/
#define LAYER2BR_RET_GOTO(Ret, gotoTag, fmt, args...) \
        if ( RET_FAILED(Ret) )  \
        {\
            LAYER2BR_TRACE(fmt, ##args); \
            goto gotoTag; \
        }

#define LAYER2BR_RET_RETURN(Ret, fmt, args...)  \
        RET_RETURN(MID_LAYER2BR, Ret, fmt, ##args)

#define LAYER2BR_PTR_GOTO(Pointer,gotoTag, fmt, args...)  \
        POINTER_GOTO(MID_LAYER2BR, Pointer, gotoTag, fmt, ##args)

#define LAYER2BR_PTR_RETURN(Pointer, Ret, fmt, args...) \
        if (!Pointer)    \
        { \
            LAYER2BR_TRACE(fmt, ##args); \
            return Ret; \
        }

#define LAYER2BR_CHECK_NULL_PTR(ptr)   LAYER2BR_PTR_RETURN(ptr, TBS_NULL_PTR, "TBS_NULL_PTR\n")

#define LAYER2BR_ASSERT(expr) \
    do{ \
        if(!(expr))\
        {\
            LAYER2BR_TRACE("Assert \"%s\" failed\n", #expr); \
            exit(-1); \
        } \
    }while(0)

/********************************************************************
 *                          �ڵ�·������                            *
 ********************************************************************/
#define NODE_DeviceName             "X_TWSZ-COM_DeviceName"
        
/********************************************************************
 *                       ��ӡģʽ�ַ���                             *
 ********************************************************************/

/******************************************************************************
 *                                    ENUM                                    *
 ******************************************************************************/


/******************************************************************************
 *                                  STRUCT                                    *
 ******************************************************************************/
 
/******************************************************************************
 *                             Function Prototype                             *
 ******************************************************************************/
extern int LAYER2BR_GetBridgeKey(const char *pszBridgeName,
                                 unsigned int *pnBridgeKey);
extern int LAYER2BR_GetInterfaceKey(const char *pszInterfaceName,
                                    unsigned int *pnInterfaceKey);

extern int LAYER2BR_GetFilterIndex(const char *pszInterfaceName,
                                   unsigned long *pulFilterIdx);

#endif /*_LAYER2BR_H_*/
