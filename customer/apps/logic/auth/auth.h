/**********************************************************************
 Copyright (c), 1991-2007, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 �ļ����� : auth.h
 �ļ����� : ������֤ģ��˽��ͷ�ļ�
**********************************************************************/

#ifndef __AUTH_H__
#define __AUTH_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cfg_api.h"
#include "tbserror.h"
#include "tbsmsg.h"
#include "tbstype.h"
#include "tbsutil.h"
#include "common.h"
#include "common_func.h"

/******************************************************************************
 *                                 MACRO                                      *
 ******************************************************************************/

/* ��ӡ������Ϣ */

#ifdef AUTH_DEBUG
#define AUTH_TRACE(fmt, args...)                              COMMON_TRACE(MID_AUTH, fmt, ##args)
#else
#define AUTH_TRACE(fmt, args...)
#endif

#define AUTH_LOG_ERR(fmt, args...)                            COMMON_LOG_ERROR(MID_AUTH, fmt, ##args)

#define AUTH_RET_LOG(iRet, strError, args...)                 RET_LOG(MID_AUTH, iRet, strError, ##args)
#define AUTH_POINTER_LOG(pRet, strError, args...)             POINTER_LOG(MID_AUTH, pRet, strError, ##args)

#define AUTH_RET_RETURN(iRet, strError, args...)              RET_RETURN(MID_AUTH, iRet, strError, ##args)
#define AUTH_POINTER_RETURN(pRet, strError, args...)          POINTER_RETURN(MID_AUTH, pRet, strError, ##args)

#define AUTH_RET_GOTO(iRet, strError, gotoTag)                RET_GOTO(MID_AUTH, iRet, strError, gotoTag)
#define AUTH_POINTER_GOTO(pRet, gotoTag, strError, args...)   POINTER_GOTO(MID_AUTH, pRet, gotoTag ,strError, ##args)

#define AUTH_RET_FALSE(iRet, strError, args...)               RET_FALSE(MID_AUTH, iRet, strError, ##args)
#define AUTH_POINTER_FALSE(pRet, strError, args...)           POINTER_FALSE(MID_AUTH, pRet, strError, ##args)

/*���ص��û���������*/
#define AUTH_HIDDEN_USERNAME        "tw007"
#define AUTH_HIDDEN_PASSWORD        "tw007"

/*AUTHģ�����Ľڵ�*/
#define AUTH_STR_SESSION_ID         "sessionid"
#define AUTH_STR_USERNAME           "username"
#define AUTH_STR_PASSWORD           "password"
#define AUTH_STR_ACTION             "action"
#define AUTH_STR_LEVEL              "level"

#define AUTH_LEVEL_ACCESSOR         "accessor"
#define AUTH_LEVEL_OPERATOR         "operator"
#define AUTH_LEVEL_ADMINISTRATOR    "administrator"

#define AUTH_ACTION_LOGIN           "login"
#define AUTH_ACTION_CONTROL         "control"
#define AUTH_ACTION_LOGOUT          "logout"
#define AUTH_ACTION_QUERY           "query"
#define AUTH_ACTION_CHANGE          "change"

#define AUTH_LAN_PASSWORD           "InternetGatewayDevice.LANConfigSecurity."
#define AUTH_PATH_BASE              "InternetGatewayDevice.X_TWSZ-COM_Authentication"
#define AUTH_NODE_TIMEOUT           "SessionTimeout"
#define AUTH_NODE_MAXUSER           "MaxUserEntries"

#define AUTH_PATH_USERLIST          "InternetGatewayDevice.X_TWSZ-COM_Authentication.UserList"
#define AUTH_NODE_USERNAME          "UserName"
#define AUTH_NODE_PASSWORD          "Password"
#define AUTH_NODE_LEVEL             "Level"
#ifdef CONFIG_APPS_LOGIC_FIRSTLOGIN_CHECK
#define AUTH_NODE_USED				"Used"
#endif
#define AUTH_OTHER_PATH_BASE        "InternetGatewayDevice.DeviceInfo.X_TWSZ-COM_TeleComAccount"
#define AUTH_OTHER_NODE_ENABLE      "Enable"
#define AUTH_OTHER_NODE_PASSWORD    "Password"

#define AUTH_KEY                    "$1$TW" /* ����������ܵ���Կ */

#define MAX_PASSWORD_LEN	64
#define MAX_PASSWORD_LEN1	128

#ifndef AUTH_TIMEOUT_SECOND
#define AUTH_TIMEOUT_SECOND         300     /* ��¼��ʱ������ */
#endif
#define AUTH_TIMEOUT_MIN       300
#define AUTH_TIMEOUT_MAX       1800

#define AUTH_ACTION_LEN             16      /* ��������� */
#define AUTH_SESSION_ID_LEN         16      /* sessionID���� */

#define AUTH_MSG_STRLEN_MAX         1024    /* ��ģ����Ϣ���ȶ������ڸ�ֵ */

#define AUTH_LEVEL_MAX              3       /* �û�Ȩ�޷����� */
#define AUTH_MAX_INDEX_COUNT        1       /* �����Ÿ��� */
#define AUTH_MAX_USER_COUNT         3       /* ����û�������ǰ�û����̶�Ϊ3  */
#define AUTH_SESSION_MAX            8       /* ���ͬʱ��¼�û���   */

/* ��������AUTH ��Ϣ��result ֵ */
#define AUTH_RESULT_SUCCESS         0       /* �ɹ� */
/*  ���������֤��Ϣ */
#define AUTH_RESULT_WRONG_NAME      1       /* �û����� */
#define AUTH_RESULT_WRONG_PASSWORD  2       /* ����� */
#define AUTH_RESULT_SESSION_FULL    3       /* ��¼�û����� */
/* ���ڼ�Ȩ��Ϣ */
#define AUTH_RESULT_CONTROL_FAILED  4       /* δ��¼ */
#define AUTH_RESULT_SESSION_TIMEOUT 5       /* session��ʱ*/
#define AUTH_RESULT_LOGOUT_SUCCESS  6       /*�û�ע���ɹ�*/
#define AUTH_RESULT_LOGIN_FREQUENT  7       /*��¼����Ƶ��*/
#ifdef CONFIG_APPS_LOGIC_FIRSTLOGIN_CHECK
#define AUTH_RESULT_LOGIN_FIRSTUSE  8       /*�˺ŵ�һ�ε�¼*/
#endif
#define ERR_GET(node)                       "Get Node["node"] value failed.\n"


#define ERR_OUT(iRet, str, lable)  RET_GOTO(MID_AUTH, iRet, str, lable);

#define AUTH_FREELIST(list,type) \
{                          \
    type *p = NULL;  \
    while (NULL != list)  \
    {                                 \
        p = list;                    \
        list = list->pstNext;   \
        safe_free(p);           \
    }                                    \
}

#define P_ERR_GOTO(module,Pointer,strError, goTag)         \
    {\
        if ( !Pointer)    \
        {   \
            COMMON_TRACE(module,strError); \
            goto goTag; \
        }\
    }



/******************************************************************************
 *                                 ENUM                                       *
 ******************************************************************************/
/*  �û�Ȩ�޵ȼ�  */
typedef enum tagEN_AUTH_LEVEL
{
    EN_AUTH_LEVEL_ADMINISTRATOR = 1,
    EN_AUTH_LEVEL_OPERATOR,
    EN_AUTH_LEVEL_ACCESSOR,

    EN_AUTH_LEVEL_NONE
}EN_AUTH_LEVEL;




/******************************************************************************
 *                                STRUCT                                      *
 ******************************************************************************/

/*  һ���Ự�Ľṹ,�൱��һ��ʵ�� */
typedef struct tagST_AUTH_SESSION
{
    struct tagST_AUTH_SESSION     *pstNext;            /*  ��һ���Ự */
    char            szSessionID[AUTH_SESSION_ID_LEN];  /*  �Ự��ʶ */
    char            szUsername[MAX_USER_LEN];          /*  �û���*/
    unsigned long            ulInstance;               /* ���������е�ʵ���� */
    unsigned long            ulLevel;                  /* Ȩ�޵ȼ� */
    time_t            tmTime;                          /*  ���һ�η���ʱ�� */
} ST_AUTH_SESSION;

/* һ����ʱ�Ự�ṹ*/
typedef struct tagST_AUTH_TEMP
{
    struct tagST_AUTH_TEMP  *pstNext;                            /* ��һ���Ự*/
    char                    szSessionID[AUTH_SESSION_ID_LEN];    /*�Ự��ʶ*/
    unsigned short     ulCounter;                                /*��¼�������*/
    time_t                 tmTime;                               /*���һ�η���ʱ��*/
} ST_AUTH_TEMP;

/*  �û���Ϣ�ṹ */
typedef struct tagST_AUTH_USER
{
    struct tagST_AUTH_USER     *pstNext;                    /* ��һ���û� */
    char            szUsername [MAX_USER_LEN];              /* �û��� */
    char            szPassword [MAX_USER_LEN];              /* �û����� */
    unsigned long            ulLevel;                       /* �û�Ȩ�޼��� */
    unsigned long            ulInstance;                    /* ���������е�ʵ���� */
} ST_AUTH_USER;

/*   һ������Ľṹ , ��������ʹ������������ڴ�*/
typedef struct tagST_AUTH_ACTION
{
    char            szAction[AUTH_ACTION_LEN];              /* �������������������: login(��¼), control(��֤), logout(ע��) */
    char            szSessionID[AUTH_SESSION_ID_LEN];       /* �û��ỰCookie*/
    char            szUsername[MAX_USER_LEN];               /* �û���*/
    char            szPassword[MAX_USER_LEN];               /* �û�����*/
    unsigned long    ulLevel;                               /* �û�Ȩ�޼���*/
    unsigned long    ulInstance;                            /* ���������е�ʵ���� */
    unsigned long    ulResult;                              /* ����Ĵ����� */
} ST_AUTH_ACTION;


/******************************************************************************
 *                               FUNCTION                                     *
 ******************************************************************************/
static int AUTH_CheckSessionTimeout(char *pcVal);
static int AUTH_CheckLevel(char *pcVal);
static int AUTH_CheckUserName(char *pcVal);
static int AUTH_CheckPassword(char *pcVal);
static int AUTH_CheckNameConflict(char * pszPath,char *pszValue);
static int AUTH_EncryptPassword(char *pszPath,char *pszValue);
#ifdef CONFIG_APPS_LOGIC_FIRSTLOGIN_CHECK
static int AUTH_CheckUsed(char *pcVal);
#endif
static int AUTH_DeleteUserCheck(char *pszPath);
static int AUTH_DeleteUserProcess(char *pszPath);
static int AUTH_DeleteSessionsByName(const char *pszUsername);
static int AUTH_AddSession(ST_AUTH_SESSION **ppstSessionList, ST_AUTH_ACTION *pstActionInfo);
static int AUTH_DeleteSession(ST_AUTH_SESSION **ppstSessionList, char *pcSessionID);
static int AUTH_RespAuthMsg(const ST_MSG *pstOrgMsg, ST_AUTH_ACTION *pstActionInfo, int nAuthRet);
static int AUTH_AddTempSession(ST_AUTH_TEMP **ppstSessionList, ST_AUTH_ACTION *pstActionInfo);
static int AUTH_CheckHandler(ST_AUTH_ACTION *pstActionInfo);
static int AUTH_ControlHandler(ST_AUTH_ACTION *pstActionInfo);
static int AUTH_LogoutHandler(ST_AUTH_ACTION *pstActionInfo);

static int AUTH_MsgAuthHandler(ST_MSG *pstMsg);

static int AUTH_ApplyTimeOut(char *pszPath,char *pszValue);
static int AUTH_Apply(char *pszPath,char *pszValue);
static int AUTH_ApplyTr069Node(char *pszPath,char *pszValue);
static int AUTH_MsgTimeChangeHandler(ST_MSG *pstMsg);
static int AUTH_ApplyLANPassword(char *pszPath,char *pszValue);

int AUTH_ModuleInit(void);
int AUTH_ModuleDestory(void);



/******************************************************************************
 *                                 END                                        *
 ******************************************************************************/


#endif /* __AUTH_H__ */



