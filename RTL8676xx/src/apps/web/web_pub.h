/**********************************************************************
 Copyright (c), 1991-2007, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 �ļ����� : web_pub.h
 �ļ����� : WEB���������ֵĹ�����Ϣͷ�ļ�

 �����б� :


 �޶���¼ :
          1 ���� : ����Ⱥ / 2007-8-21
            ���� :
            ���� :

**********************************************************************/
#ifndef _WEB_PUB_H_
#define _WEB_PUB_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../omlib/om_buffer.h"
#include "../omlib/om_nameval.h"

#include "../include/om.h"

#include "web_stub.h"
#include "web_utils.h"

/******************************************************************************
 *                                 MACRO                                      *
 ******************************************************************************/
//���Կ���
//#define WEBPROC_DEBUG

#ifdef WEBPROC_DEBUG
#define WEB_TRACE(fmt, args...) WEB_LogTrace("%s:%4d %20s: "fmt, __FILE__, __LINE__, __func__, ##args)
#else
#define WEB_TRACE(fmt, args...) do{}while(0)
#endif

#ifdef CONFIG_APPS_LOGIC_NEW3G
extern FILE *cfp;
extern unsigned long g_ulIsNew3g;
extern unsigned long g_ulTagMid;

#ifdef d_printf
#undef d_printf
#endif

#define d_printf(fmt, args...) \
	do{\
		if (cfp <= 0) \
			cfp = fdopen(open("/dev/console", O_RDWR), "w");\
		if (cfp > 0)	\
                        if (access("/var/3g_debug_wan", F_OK) == 0)   \
			    fprintf(cfp, "[WEBPROC] %s:%d "fmt, __FILE__, __LINE__, ##args);\
	}while(0)

#ifdef WEBPROC_DEBUG
#undef  WEB_TRACE
#define WEB_TRACE(fmt, args...) d_printf("%s:%4d %20s: "fmt, __FILE__, __LINE__, __func__, ##args)
#endif

#else
#ifdef d_printf
#undef d_printf
#endif
#define d_printf(fmt, args...) 
#endif


#define WEB_FUNC_IN(fmt, args...)   WEB_TRACE("++I"fmt"\n", ##args)
#define WEB_FUNC_OUT()              WEB_TRACE("--O\n")



#define  WEB_LEN_TEXT_BUF    (1024*24)      /* �ı����壬ÿ�ν���������ı����� */
#define  WEB_NUM_PARAM       10        /* ÿһ����������������� */
#define  WEB_MAX_CMD_DEPTH   5         /* ��������Ƕ������ */
#define  WEB_TIMEOUT_RESP    10        /* ��Ӧ�ĳ�ʱʱ�� */
#define  WEB_MAX_VAL_PER_OBJ 32        /* ÿ��������԰����Ĳ���������������Ƶ� */
#define  WEB_MAX_OBJ_LEN     512       /* ÿ������ĳ��� */
#define  WEB_MAX_OBJ_NUM     260       /* ÿ������ĳ��� */

#define  WEB_NUM_STRING      16        /* һ�����ִ�����󳤶� */

#define  WEB_STRUCT_MAGIC    0x01105AA5

#define Name2MID safe_atol
/*------------��������-----------------*/
#define WEB_NEED_MARK     1
/******************************************************************************
 *                                 ENUM                                       *
 ******************************************************************************/
/* ��Ӧ��Ϣ�ļ���� */
enum en_RESPONSE_STATE
{
    EN_RESPONSE_SUCCESS,    /* ���OK */
    EN_RESPONSE_ERROR,      /* ������� */
    EN_RESPONSE_TIMEOUT,    /* �ȴ���ʱ */
    EN_RESPONSE_UNREPECT,   /* ��Ԥ����Ϣ */

    EN_RESPONSE_BUTT
};


#define WEB_OutToTerm  printf

/******************************************************************************
 *                                STRUCT                                      *
 ******************************************************************************/


/* ��ģ�鶨���ֵ*/
typedef struct tag_ST_WEB_VALUE
{
    struct tag_ST_WEB_VALUE *pNext;
    struct tag_ST_WEB_VALUE *pPrev;
    int                      nCount;
    char                    *value[WEB_MAX_VAL_PER_OBJ];
}ST_WEB_VALUE;

/* POST��Ϣ */
typedef struct tag_ST_POST_INFO
{
    unsigned long   ulMagic;        /* ��ȷ��ʼ��Ϊ WEB_STRUCT_MAGIC*/
    int			    nPostNum;       /* POST���� */

    unsigned long   ulStatus;       /* ״ֵ̬ */
    BOOL            bPostError;     /* �Ƿ����*/
    char		   *szPostResult;
}ST_POST_INFO, *PST_POST_INFO;

/******************************************************************************
 *                               FUNCTION                                     *
 ******************************************************************************/


/* ����һ������� �൱�� szName = szvalue */
void WEB_SetVar(char *szName, char *szValue);
/* ��ȡһ����� */
ST_NAME_VAL * WEB_GetVar(char *szName);

/* �ı�������ڣ����ؽ�����ʵ�ʳ��� */
int WEB_ParseText (char *pText, int nSize);

/* ��ȡ��Ӧ, ����ֵȡֵ��en_RESPONSE_STATE */
int  WEB_GetResponse();

/* POSTINFO���������*/
void WEB_PostInfoClean();

/* ȫ����Դ������ */
void Web_CleanUp();


#if WEB_NEED_MARK
    /* �������ļ� */
    void WEB_MarkFileIn(PCSTR strFile);
    /* �뿪�ļ� */
    void WEB_MarkFileOut();
    /* �����Դ */
    void WEB_MarkClearUp();
    /* �����µص� */
    void WEB_MarkGotoPos(int nPos, PCSTR strCmd, int nLen);
    /* �ۼ��ѽ������� */
    void WEB_MarkPosIncr(int nCount);
    /* ������ʾ */
    #define WEB_MarkError(str) WEB_MarkErrorEx(__FILE__, __LINE__, str);
    void WEB_MarkErrorEx(PCSTR szFile, int nLine, PCSTR strError);
    /* �Ƿ�������� */
    BOOL WEB_MarkIsError();
#else
    #define  WEB_MarkFileIn(...)
    #define  WEB_MarkFileOut(...)
    #define  WEB_MarkClearUp(...)
    #define  WEB_MarkGotoPos(...)
    #define  WEB_MarkPosIncr(...)
    #define  WEB_MarkError(...)
    #define  WEB_MarkIsError() FALSE
#endif


/******************************************************************************
 *                           PUB EXTERN VAR                                   *
 ******************************************************************************/

extern ST_NAME_VAL        *g_pstWebVars;
extern ST_NAME_VAL        *g_pstNetVars;
extern ST_NAME_VAL        *g_pstPostVars;
extern ST_POST_INFO        g_stPostInfo;
extern unsigned long       g_ulMsgSerial;


//int read(int, void *, unsigned int);
//int open(const char *, int, ...);
//int close(int);
//char * getcwd(char *, int);
//int write(int, const void *, unsigned int);


/* ��ȫ���ͷ���Ϣ*/
#define safe_free_msg(pointer) \
    { \
        if ( pointer ) \
        { \
            MSG_ReleaseMessage(pointer); \
            pointer = NULL; \
        } \
    }


/******************************************************************************
 *                                 END                                        *
 ******************************************************************************/




#endif /* _WEB_PUB_H_ */
