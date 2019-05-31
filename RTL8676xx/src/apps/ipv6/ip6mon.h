/**********************************************************************
 Copyright (c), 1991-2009, T&W ELECTRONICS(SHENTHEN) Co., Ltd.

 �ļ�����: ip6mon.h
 �ļ�����: ip6monģ���ͷ�ļ� 

 �޶���¼:
        1. ����: XuXiaodong
           ����: 2009-12-24
           ����: �����ļ�
**********************************************************************/
#ifndef _IP6MON_H_
#define _IP6MON_H_

#ifdef IP6MON_DEBUG
    #define IP6MON_TRACE(fmt, args...) COMMON_TRACE(MID_IP6MON, fmt, ##args)
    #define IP6MON_LOG_ERR(fmt, args...) COMMON_TRACE(MID_IP6MON, fmt, ##args)
#else
    #define IP6MON_TRACE(fmt, args...) 
    #define IP6MON_LOG_ERR(fmt, args...) COMMON_LOG_ERROR(MID_IP6MON, fmt, ##args)
#endif


#define IP6MON_RET_GOTO(Ret, gotoTag, fmt, args...) \
        if ( RET_FAILED(Ret) )  \
        {\
            IP6MON_TRACE(fmt, ##args); \
            goto gotoTag; \
        }

#define IP6MON_RET_RETURN(Ret, fmt, args...)  \
        RET_RETURN(MID_IP6MON, Ret, fmt, ##args)

#define IP6MON_PTR_GOTO(Pointer,gotoTag, fmt, args...)  \
        POINTER_GOTO(MID_IP6MON, Pointer, gotoTag, fmt, ##args)

#define IP6MON_PTR_RETURN(Pointer, Ret, fmt, args...) \
        if (!Pointer)    \
        { \
            IP6MON_TRACE(fmt, ##args); \
            return Ret; \
        }
        
#define CALL_FUCTION_FAILED     "Call function \"%s\" failed\n"

#define IFA_F_GLBAUTOCONF 0x0
#define IFA_F_AUTOCONF 0x04
#define IFA_F_STATIC   0x08

#endif /*_IP6MON_H_*/

