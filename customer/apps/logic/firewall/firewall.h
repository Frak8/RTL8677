/**********************************************************************
 Copyright (c), 1991-2008, T&W ELECTRONICS(SHENTHEN) Co., Ltd.

 �ļ�����: firewall.h

 �ļ�����: ����ǽģ���ͷ�ļ�

 �޶���¼:
           1 ���� : ��ΰ
             ���� : *
             ���� : ����

           2 ���� : ղ��
             ���� : *
             ���� : �޸�

           3 �޸� : ������
             ���� : 2008-11-18
             ���� : �ع�

**********************************************************************/
#ifndef __TBS_FIREWALL_H
#define __TBS_FIREWALL_H

#include "common.h"

#include "new_msg.h"
#include "common_func.h"
#include "common_msg_handler.h"


/******************************************************************************
*                                 MACRO
******************************************************************************/
/* ������Ϣ��*/

#ifdef FW_DEBUG
#define FW_TRACE(fmt, args...)                              COMMON_TRACE(MID_FIREWALL, fmt"\n", ##args)
#define FW_FUNC_IN(fmt, args...)                            FW_TRACE("++I %s, "fmt"\n", __FUNCTION__, ##args)
#define FW_FUNC_OUT()                                       FW_TRACE("--O %s\n", __FUNCTION__)

#else
#define FW_TRACE(str, args...)
#define FW_FUNC_IN(fmt, args...)
#define FW_FUNC_OUT()


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

#define FW_LOG_ERR(fmt, args...)                            COMMON_LOG_ERROR(MID_FIREWALL, fmt, ##args)

#define FW_RET_LOG(iRet, strError, args...)                 RET_LOG(MID_FIREWALL, iRet, strError, ##args)
#define FW_POINTER_LOG(pRet, strError, args...)             POINTER_LOG(MID_FIREWALL, pRet, strError, ##args)

#define FW_RET_RETURN(iRet, strError, args...)              RET_RETURN(MID_FIREWALL, iRet, strError, ##args)
#define FW_POINTER_RETURN(pRet, strError, args...)          POINTER_RETURN(MID_FIREWALL, pRet, strError, ##args)

#define FW_RET_GOTO(iRet, strError, gotoTag)                RET_GOTO(MID_FIREWALL, iRet, strError, gotoTag)
#define FW_POINTER_GOTO(pRet, gotoTag, strError, args...)   POINTER_GOTO(MID_FIREWALL, pRet, gotoTag ,strError, ##args)

#define FW_RET_FALSE(iRet, strError, args...)               RET_FALSE(MID_FIREWALL, iRet, strError, ##args)
#define FW_POINTER_FALSE(pRet, strError, args...)           POINTER_FALSE(MID_FIREWALL, pRet, strError, ##args)

#define ERR_GET(node)                   "Get Node["node"] value failed.\n"
#define ERR_SET(node)                   "Set Node["node"] value failed.\n"
#define ERR_SET_VAL(node)               "Set Node["node"] value failed, value = %s\n"

#define FOR_EACH_LIST(pos, list)        for ((pos) = list; (pos) != NULL; (pos) = (pos)->pstNext)


/************************** �������� **************************/

/* �ڵ������� */
#define FW_NODE_IPFilterEnable          "IPFilterEnable"
#define FW_NODE_SecurityLevel           "SecurityLevel"
#define FW_NODE_CurrentLANFilterTable   "CurrentLANFilterTable"
#define FW_NODE_CurrentWANFilterTable   "CurrentWANFilterTable"

/* ���˱�ڵ� */
#define FW_NODE_Enable                  "Enable"
#define FW_NODE_DevPath                 "DevPath"
#define FW_NODE_DeviceName              "Cache_DeviceName"
#define FW_NODE_SrcIP                   "SrcIP"
#define FW_NODE_DestIP                  "DestIP"
#define FW_NODE_SrcMask                 "SrcMask"
#define FW_NODE_DstMask                 "DstMask"
#define FW_NODE_SrcPort                 "SrcPort"
#define FW_NODE_DestPort                "DestPort"
#define FW_NODE_Protocol                "Protocol"
#define FW_NODE_Description             "Description"

#define FW_TABLE_NAME_LEN               20
#define FW_TALBE_LanBlackFilter         "LanBlackFilter"
#define FW_TALBE_LanWhiteFilter         "LanWhiteFilter"
#define FW_TALBE_WanBlackFilter         "WanBlackFilter"
#define FW_TALBE_WanWhiteFilter         "WanWhiteFilter"



#define LAN_NODE_DeviceName             "X_TWSZ-COM_DeviceName"


/* ���������� */
#define FW_INSTANCE_MAX_NUM             20


/* �ڵ�·������ */
#define FW_PATH_PREFIX                  "InternetGatewayDevice.X_TWSZ-COM_Firewall."

#define FW_PATH_IPFilterEnable          FW_PATH_PREFIX "IPFilterEnable"
#define FW_PATH_AntiDosEnabled          FW_PATH_PREFIX "AntiDosEnabled"
#define FW_PATH_SecurityLevel           FW_PATH_PREFIX "SecurityLevel"
#define FW_PATH_CurrentLANFilterTable   FW_PATH_PREFIX "CurrentLANFilterTable"
#define FW_PATH_CurrentWANFilterTable   FW_PATH_PREFIX "CurrentWANFilterTable"

#define LAN_PATH_PREFIX                 "InternetGatewayDevice.LANDevice."
#define WAN_PATH_PREFIX                 "InternetGatewayDevice.WANDevice."

/* ��ʽ���� */
#define FW_FORMAT_INST                  FW_PATH_PREFIX "%[^.].%u."


/* ������ */
#define FW_REGEX_PREFIX                 "^InternetGatewayDevice\\.X_TWSZ-COM_Firewall\\."
#define FW_REGEX_TABLE_PREFIX           FW_REGEX_PREFIX"(Wan|Lan)(Black|White)Filter\\.[0-9]{1,5}\\."


/* WAN dev regex */
#define FW_REGEX_IPCONN_INST            "^InternetGatewayDevice\\.WANDevice\\.[0-9]{1,5}\\.WANConnectionDevice\\.[0-9]{1,5}\\.WANIPConnection\\.[0-9]\{1,5}\\.$"
#define FW_REGEX_PPPCONN_INST           "^InternetGatewayDevice\\.WANDevice\\.[0-9]{1,5}\\.WANConnectionDevice\\.[0-9]{1,5}\\.WANPPPConnection\\.[0-9]\{1,5}\\.$"

#define FW_REGEX_WAN_CONN_IP            "^InternetGatewayDevice\\.WANDevice\\.[0-9]{1,5}\\.WANConnectionDevice\\.[0-9]{1,5}\\.WANIPConnection\\.[0-9]\{1,5}$"
#define FW_REGEX_WAN_CONN_PPP           "^InternetGatewayDevice\\.WANDevice\\.[0-9]{1,5}\\.WANConnectionDevice\\.[0-9]{1,5}\\.WANPPPConnection\\.[0-9]\{1,5}$"

/* LAN dev regex */
#define FW_REGEX_LAN_CONN               "^InternetGatewayDevice\\.LANDevice\\.[0-9]{1,5}$"
#define FW_REGEX_WLAN_CONN              "^InternetGatewayDevice\\.LANDevice\\.[0-9]{1,5}\\.WLANConfiguration\\.[0-9]{1,5}$"


/*
�������ò���
*/

/* ��������/ɾ��/����/�Ͽ� */
#define FW_CMD_CONN_EST                 1
#define FW_CMD_CONN_FIN                 2
#define FW_CMD_CONN_ADD                 3
#define FW_CMD_CONN_DEL                 4

/* ��ӹ���/ɾ������ */
#define FW_CMD_ADD                      1
#define FW_CMD_DEL                      0

/* ���/ɾ�� wan/lan �豸 */
#define FW_NOCHANG_DEV                  0
#define FW_ADD_WAN_DEV                  1
#define FW_DEL_WAN_DEV                  2
#define FW_ADD_LAN_DEV                  3
#define FW_DEL_LAN_DEV                  4


/* �����ʽ����*/
#define FMT_SUCC                        ECHO_GREEN"[Success] "ECHO_NONE
#define FMT_FAIL                        ECHO_RED"[Failed] "ECHO_NONE


/******************************************************************************
*                                ENUM
******************************************************************************/
/* IP���˱�ö�� */
typedef enum tag_EN_FW_Filter
{
    EN_LanWhiteFilter = 1,
    EN_LanBlackFilter = 2,
    EN_WanWhiteFilter = 3,
    EN_WanBlackFilter = 4,
} EN_FW_Filter;

/* ����ǽ��ȫ���� */
typedef enum tag_EN_FW_SecurityLevel
{
    EN_User_Ddefined_SecurityLevel  = 0, /* �Զ��� */
    EN_Low_SecurityLevel            = 1, /* �� */
    EN_Medium_SecurityLevel         = 2, /* �� */
    EN_High_SecurityLevel           = 3, /* �� */
} EN_FW_SecurityLevel;


/******************************************************************************
*                                STRUCT
******************************************************************************/

/* ·����Ӧ�豸�� */
typedef struct tag_ST_CONNPATH_DEVNAME
{
    struct list_head head;
    /* ע��:û��'.'��׺ */
    char szConnPath[MAX_PATH_LEN];
    char szDevName[MAX_IF_NAME_LEN];
} ST_ConnPath_DevName;

typedef struct tag_ST_FW_Index
{
    unsigned int nTableIdx;
    unsigned int nRuleIdx;
} ST_FW_Index;

/* Firewall����ʵ����Ӧ�ڵ���Ϣ */
typedef struct tag_ST_FW_Nodes
{
    char *szEnable;
    char *szDevPath;
    char *szProtocol;
    char *szSrcIP;
    char *szDestIP;
    char *szSrcPort;
    char *szDestPort;
    char *szSrcMask;
    char *szDstMask;
    char *szDescription;
} ST_FW_Nodes;

/* firewallʵ�� */
typedef struct tag_ST_FW_Instance
{
    char acPath[MAX_PATH_LEN];
    ST_FW_Index stIndex;
    ST_FW_Nodes stNodes;
    struct tag_ST_FW_Instance *pstNext;
} ST_FW_Instance;


typedef struct tag_ST_FW_PARA
{
    char *szDevPath;
    int iType;
} ST_FW_Para;


/*******************************************************************
*
* FUNC DECLEARATION
*
*******************************************************************/
/* Init && Destroy */
int FW_ModuleInit();
int FW_ModuleDestroy();




#endif//__TBS_FIREWALL_H
