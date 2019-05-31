/**********************************************************************
 Copyright (c), 1991-2007, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 �ļ����� : mon.h
 �ļ����� : mon ��ͷ�ļ�


 �����б� :


 �޶���¼ :
          1 ���� : ղ��
            ���� : 2008-3-17
            ���� :

**********************************************************************/

#ifndef  _MON_H_
#define  _MON_H_

#include "tbsmsg.h"
#include "tbstype.h"
#include "tbsutil.h"
#include "tbserror.h"
#include "warnlog.h"
#include "common.h"

/******************************************************************************
*                                 MACRO
******************************************************************************/

/* ������Ϣ�� */

#ifdef MON_DEBUG
#define MON_TRACE(fmt, args...) COMMON_TRACE(MID_MON, fmt, ##args)
#else
#define MON_TRACE(str, args...)
#endif

#define LEVEL_1_SPACE " "
#define LEVEL_5_SPACE "     "

#define MON_BACKUP_FILE_NAME "/var/mon.conf"

#define MON_INTE_LEN_MAX 16     /* ifr_name(16 bytes) in struct ifreq */
#define MON_PATH_LEN_MAX 256

#define ELM_RECEIVE_TIME_OUT    3

#define MON_OPTION_PATH     "Path="
#define MON_OPTION_NAME     "Name="
#define MON_OPTION_STATUS   "Status="

#define MON_STR_UNKNOWN         "Unknown"
#define MON_STR_CONNECTED       "Connected"
#define MON_STR_DISCONNECTED    "Disconnected"
#define MON_STR_INITIALIZING    "Initializing"
#define MON_STR_HANDSHAKING    "HandShaking"


#define MON_STR_DUP_OPT_PATH        "DUP_OPT_PATH"
#define MON_STR_DUP_OPT_NAME        "DUP_OPT_NAME"
#define MON_STR_DUP_OPT_STATUS      "DUP_OPT_STATUS"

#define MON_MID_ALL 0xffff
#define MAX_MSG_LEN 1024


/******************************************************************************
*                                 ENUM
******************************************************************************/
typedef enum tag_EN_MON_INTF_TYPE
{
    MON_INTF_TYPE_NORMAL = 1,   /* ��ͨ�豸�ӿڣ��磺��̫���ӿ� */
    MON_INTF_TYPE_WLAN,         /* WLAN �豸�ӿ� */
    MON_INTF_TYPE_DSL,         /* DSL �豸�ӿ� */
} EN_MON_INTF_TYPE;

/* ö�������е�����ö����ֱ��Ӧһ�������豸����������״̬�����ӡ��Ͽ���״̬δ֪ */
typedef enum tag_EN_MON_INTF_LINKSTATE
{
    MON_INTF_LINKSTATE_UNKNOWN = 1,     /* ����״̬δ֪ */
    MON_INTF_LINKSTATE_CONNECTED,    /* ��������״̬ */
    MON_INTF_LINKSTATE_DISCONNECTED, /* ���ڶϿ�״̬ */
    MON_INTF_LINKSTATE_INITIALIZING, /*���ڳ�ʼ�� */
    MON_INTF_LINKSTATE_HANDSHAKING, /*�������� */
} EN_MON_INTF_LINKSTATE;

/* ��Ҫ��صĽӿڵ���Ϣ�Ľṹ��������¼��Ҫ��صĽӿڵ�һЩ��Ϣ */
typedef struct tag_ST_MON_INTE_INFO
{
    struct tag_ST_MON_INTE_INFO *pstNext;
    char szInterface[MON_INTE_LEN_MAX];     /* �ӿ��� */
    char szPath[MON_PATH_LEN_MAX];          /* �ӿ����������ļ��е�·�� */
    EN_MON_INTF_LINKSTATE enLinkState;      /* �ӿ�����״̬��������״̬��δ֪�����ӡ��Ͽ� */
    unsigned short usMID;                   /* �ӿ�������ģ��ID���ĸ�ģ������ģ��ע��������ӿڣ�����ӿھ������Ǹ�ģ�� */
    char acPad[2];
} ST_MON_INTE_INFO;


/******************************************************************************
*                               FUNCTION DECLARE                              *
******************************************************************************/
void MON_SigRoutine(int dunno);
int main(int argc, char **argv);
int MON_ModuleInit(void);
void MON_MsgProcess(ST_MSG *pstMsg);
int MON_MsgRegHandler(const ST_MSG *pstMsg);
int MON_MsgUnRegHandler(const ST_MSG *pstMsg);
int MON_MsgQueryHandler(const ST_MSG *pstMsg);
int MON_LinkStateInform(unsigned long ulMsgID, const ST_MON_INTE_INFO *pstIntfInfo);
int MON_IntfStateDetect(ST_MON_INTE_INFO *pstIntfInfo);
int MON_LinkStateMonitor(void);
int MON_AddNode(ST_MON_INTE_INFO *pstNewNode);
int MON_DelNode(ST_MON_INTE_INFO *pstIntfInfo);
int MON_CreateNode(unsigned short usMID, ST_MON_INTE_INFO ** ppstNewNode);
int MON_GetLinkState(const char *pszInterfaceName, EN_MON_INTF_TYPE enIntfType, EN_MON_INTF_LINKSTATE *penState);
ST_MON_INTE_INFO *MON_FindNode(unsigned short usMID, const char *pszIntfName);
int MON_DelAllNode(void);
int MON_ViewOneIntfInfo(const ST_MON_INTE_INFO *pstIntfInfo);
int MON_ViewIntfInfos(const ST_MON_INTE_INFO *pstIntfInfo);
int MON_CheckIntfName(const char *pszIntfName);
int MON_CheckOneIntfInfo(const ST_MON_INTE_INFO *pstIntfInfo);
int MON_WriteConfToFile(void);
int MON_GetConfFromFile(void);
int MON_ModuleDestroy(void);
int MON_MainProc(void);
int MON_ShowCurrentTime(void);

#endif
