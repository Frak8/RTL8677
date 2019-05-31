/**********************************************************************
 Copyright (c), 1991-2012, T&W ELECTRONICS(SHENTHEN) Co., Ltd.

 �ļ�����: tr069hold.h
 �ļ�����: tr069���������ӳٴ������ģ��

 �޶���¼:
        1. ����: ������
           ����: 2012-03-1
           ����: �����ļ�

**********************************************************************/
#ifndef _TR069HOLD_H_
#define _TR069HOLD_H_

#include "warnlog.h"
#include "tbstype.h"
#include "common.h"

//#define TR069HOLD_DEBUG
#ifdef TR069HOLD_DEBUG
    #define TR069HOLD_TRACE(fmt, args...) COMMON_TRACE(MID_TR069HOLD, fmt"\n", ##args)
    #define TR069HOLD_LOG_ERR(fmt, args...) COMMON_TRACE(MID_TR069HOLD, fmt, ##args)
#else
    #define TR069HOLD_TRACE(fmt, args...) 
    #define TR069HOLD_LOG_ERR(fmt, args...) COMMON_LOG_ERROR(MID_TR069HOLD, fmt, ##args)
#endif

#define TR069HOLD_MSGBODY_STATUS(p) *(((unsigned long *)p->szMsgBody)+1)
#define TR069HOLD_MSGBODY_RET(p) *((unsigned long *)p->szMsgBody)
#define TR069HOLD_MSGBODY_COUNT(p) *((unsigned long *)p->szMsgBody)

//�Ӻ�ִ�еĽڵ���������
#define NODE_TYPE_MASK 0xF000
#define NODE_INDEX_MASK 0x0FFF
#define NODE_TYPE_SET 0x1000
#define NODE_TYPE_ADD 0x2000
#define NODE_TYPE_DEL 0x4000

//�Ӻ�ִ�еĽڵ�����
#define LST_SET_INDEX(i) (NODE_TYPE_SET | i)
#define LST_ADD_INDEX(i) (NODE_TYPE_ADD | i)
#define LST_DEL_INDEX(i) (NODE_TYPE_DEL | i)

typedef struct tag_ST_TR069_HOLD_LST {
    char *nameValue;
    unsigned short index;
    struct tag_ST_TR069_HOLD_LST *next;
} ST_TR069_HOLD_LST;


/* Init && Destroy */
int Tr069hold_ModuleInit(void);
int Tr069hold_ModuleDestroy(void);

#endif

