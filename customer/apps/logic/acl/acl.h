/**********************************************************************
 Copyright (c), 1991-2008, T&W ELECTRONICS(SHENTHEN) Co., Ltd.

 �ļ�����: acl.h

 �ļ�����: ACLģ��ͷ�ļ�

 �޶���¼:
           1 ���� : ղ��
             ���� : 2008-08-12
             ���� : ����

           2 ���� : ������
             ���� : 2008-12-04
             ���� : �ع�

**********************************************************************/
#ifndef __TBS_ACL_H__
#define __TBS_ACL_H__

#include "common.h"
#include <arpa/inet.h>

#include "new_msg.h"
#include "common_func.h"
#include "common_msg_handler.h"
#include "autoconf.h"



/******************************************************************************
*                                 MACRO
******************************************************************************/
/* ������Ϣ��*/
#ifdef ACL_DEBUG
#define ACL_TRACE(fmt, args...)                 COMMON_TRACE(MID_ACL, fmt"\n", ##args)
#define ACL_FUNC_IN(fmt, args...)               ACL_TRACE("++I %s, "fmt"\n", __FUNCTION__, ##args)
#define ACL_FUNC_OUT()                          ACL_TRACE("--O %s\n", __FUNCTION__)

#else
#define ACL_TRACE(str, args...)
#define ACL_FUNC_IN(fmt, args...)
#define ACL_FUNC_OUT()

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

#endif //#ifdef ACL_DEBUG

#define ACL_LOG_ERR(fmt, args...)                           COMMON_LOG_ERROR(MID_ACL, fmt, ##args)

#define ACL_RET_LOG(iRet, strError, args...)                RET_LOG(MID_ACL, iRet, strError, ##args)
#define ACL_POINTER_LOG(pRet, strError, args...)            POINTER_LOG(MID_ACL, pRet, strError, ##args)

#define ACL_RET_RETURN(iRet, strError, args...)             RET_RETURN(MID_ACL, iRet, strError, ##args)
#define ACL_POINTER_RETURN(pRet, strError, args...)         POINTER_RETURN(MID_ACL, pRet, strError, ##args)

#define ACL_RET_GOTO(iRet, strError, gotoTag)               RET_GOTO(MID_ACL, iRet, strError, gotoTag)
#define ACL_POINTER_GOTO(pRet, gotoTag, strError, args...)  POINTER_GOTO(MID_ACL, pRet, gotoTag ,strError, ##args)

#define ACL_RET_FALSE(iRet, strError, args...)              RET_FALSE(MID_ACL, iRet, strError, ##args)
#define ACL_POINTER_FALSE(pRet, strError, args...)          POINTER_FALSE(MID_ACL, pRet, strError, ##args)

#define ERR_GET(node)                   "Get Node["node"] value failed.\n"
#define ERR_SET(node)                   "Set Node["node"] value failed.\n"
#define ERR_SET_VAL(node)               "Set Node["node"] value failed, value = %s\n"

#define FOR_EACH_LIST(pos, list)        for ((pos) = list; (pos) != NULL; (pos) = (pos)->pstNext)


/*
 * ��������
 */
#define NODE_CONN_STATUS				"Connected"

/* iptables ���� */
#define ACL_CHAIN_LOCAL_SUB             "ACL_LOCAL_SUB"         /* ���ص� filter ��� INPUT ���� */
#define ACL_CHAIN_LOCAL_INPUT           "ACL_LOCAL_INPUT"       /* ���ص� filter ��� INPUT ���� */

#define ACL_CHAIN_REMOTE_SUB            "ACL_REMOTE_SUB"        /* ���ص� nat ��� PREROUTING ���� */
#define ACL_CHAIN_REMOTE_PRE            "ACL_REMOTE_PRE"        /* ���ص� nat ��� PREROUTING ���� */
#define ACL_CHAIN_REMOTE_INPUT          "ACL_REMOTE_INPUT"      /* ���ص� filter ��� INPUT ���� */

/* ��ӹ���/ɾ������ */
#define ACL_CMD_ADD                     1
#define ACL_CMD_DEL                     0

/* ��������/ɾ��/����/�Ͽ� */
#define ACL_CMD_CONN_EST                1
#define ACL_CMD_CONN_FIN                2
#define ACL_CMD_CONN_ADD                3
#define ACL_CMD_CONN_DEL                4

/* ʧ�ܷ���/�ɹ����� */
#define ACL_RETURN_ON_FAIL              1
#define ACL_RETURN_ON_SUCCESS           0

/* ���ACL���� */
#define ACL_SEPARATOR_MAX      8
#define ACL_SEPARATOR_ERROR  -1

/* ACL ��ص� TR069 �ڵ��� */
#define ACL_NODE_RACLEnable             "RACLEnable"
#define ACL_NODE_RACLNumberOfEntries    "RACLNumberOfEntries"
#define ACL_NODE_RACL                   "RACL"

#define ACL_NODE_LACLEnable             "LACLEnable"
#define ACL_NODE_LACLNumberOfEntries    "LACLNumberOfEntries"
#define ACL_NODE_LACL                   "LACL"

/* Device node */
#define ACL_NODE_DevPath                "DevPath"
#define ACL_NODE_DeviceName             "Cache_DeviceName"
#define ACL_NODE_DeviceIP               "Cache_DeviceIP"
#define ACL_NODE_ServiceNumberOfEntries "ServiceNumberOfEntries"
#define ACL_NODE_Service                "Service"

/* Service node */
#define ACL_NODE_Name                   "Name"
#define ACL_NODE_Enable                 "Enable"
#define ACL_NODE_Protocol               "Protocol"
#define ACL_NODE_SrcIP                  "SrcIP"
#define ACL_NODE_SrcMask                "SrcMask"
#define ACL_NODE_ExternalPort           "ExternalPort"
#define ACL_NODE_InternalPort           "InternalPort"
#define ACL_NODE_Description            "Description"

/* ������ؽڵ�·������ */
#define LAN_NODE_DeviceName             "X_TWSZ-COM_DeviceName"
#define WAN_NODE_ConnectedDeviceName    "X_TWSZ-COM_ConnectedDeviceName"
#define WAN_NODE_ExternalIPAddress      "ExternalIPAddress"
#define ACL_LAN_IPADDRESS                   "InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.IPInterfaceIPAddress"


/*
·��ǰ׺
*/
#define ACL_PATH_PREFIX                 "InternetGatewayDevice.X_TWSZ-COM_ACL."

/* �ڵ�·�� */
#define ACL_PATH_LACLEnable             ACL_PATH_PREFIX ACL_NODE_LACLEnable
#define ACL_PATH_LACLNumberOfEntries    ACL_PATH_PREFIX ACL_NODE_LACLNumberOfEntries
#define ACL_PATH_LACL                   ACL_PATH_PREFIX ACL_NODE_LACL

#define ACL_PATH_RACLEnable             ACL_PATH_PREFIX ACL_NODE_RACLEnable
#define ACL_PATH_RACLNumberOfEntries    ACL_PATH_PREFIX ACL_NODE_RACLNumberOfEntries
#define ACL_PATH_RACL                   ACL_PATH_PREFIX ACL_NODE_RACL


/*
������
*/
#define ACL_REGEX_PREFIX                "^InternetGatewayDevice\\.X_TWSZ-COM_ACL\\."

/* WAN dev regex */
#define ACL_REGEX_WAN_CONN_IP            "^InternetGatewayDevice\\.WANDevice\\.[0-9]{1,5}\\.WANConnectionDevice\\.[0-9]{1,5}\\.WANIPConnection\\.[0-9]\{1,5}$"
#define ACL_REGEX_WAN_CONN_PPP           "^InternetGatewayDevice\\.WANDevice\\.[0-9]{1,5}\\.WANConnectionDevice\\.[0-9]{1,5}\\.WANPPPConnection\\.[0-9]\{1,5}$"


/* LAN dev regex */
#define ACL_REGEX_LAN_CONN              "^InternetGatewayDevice\\.LANDevice\\.[0-9]{1,5}$"

#define ACL_REGEX_WLAN_CONN              "^InternetGatewayDevice\\.LANDevice\\.[0-9]{1,5}\\.WLANConfiguration\\.[0-9]{1,5}$"



/******************************************************************************
*                                ENUM
******************************************************************************/
/* ʵ������ */
typedef enum tag_EN_ACL_INST_TYPE
{
    ACL_INST_TYPE_UNKNOWN = 0,          /* UNKNOWN */
    ACL_INST_TYPE_DEVICE,           /* devʵ�� */
    ACL_INST_TYPE_SERVICE,          /* Serviceʵ�� */
} EN_ACL_InstType;

/* ACL���� */
typedef enum tag_EN_ACL_ACLType
{
    ACL_ACLType_UNKNOWN = 0,        /* UNKNOWN */
    ACL_ACLType_LACL,               /* Local���ʿ��� */
    ACL_ACLType_RACL,               /* Remote���ʿ���  */
} EN_ACL_ACLType;


/******************************************************************************
*                                STRUCT
******************************************************************************/

/* �����ṹ */
typedef struct tag_ST_ACL_Index
{
    unsigned int nDevIdx;                 /* ��Ӧ�� DevPath ������index */
    unsigned int nServIdx;                /* ��Ӧ�� Service ������index */
} ST_ACL_Index;

/* Deviceʵ����Ӧ�ڵ���Ϣ */
typedef struct tag_ST_ACL_DeviceNodes
{
    /* �豸�ڵ� */
    char *szDevPath;
    char *szDeviceName;
    char *szDeviceIP;
} ST_ACL_DeviceNodes;


/* Serviceʵ����Ӧ�ڵ���Ϣ */
typedef struct tag_ST_ACL_ServiceNodes
{
    /* ����ڵ� */
    char *szName;
    char *szEnable;
    char *szProtocol;
    char *szSrcIP;
    char *szSrcMask;
    char *szInternalPort;
    char *szExternalPort;
} ST_ACL_ServiceNodes;


/* ʵ���ṹ */
typedef struct tag_ST_ACL_Instance
{
    char acPath[MAX_PATH_LEN];              /* ʵ��·�� */
    EN_ACL_ACLType  enACLType;              /* �������� */
    EN_ACL_InstType enInstType;             /* ʵ������ */
    ST_ACL_Index stIndex;                   /* ����     */

    BOOL  bEnable;                          /* ��ʶ�Ƿ����ã���Ӧ�� LACLEnable/RACLEnable */
    ST_ACL_DeviceNodes  stDeviceInfo;
    ST_ACL_ServiceNodes stServiceInfo;
} ST_ACL_Instance;


/* ���������ṹ */
typedef struct tag_ST_ACL_Para
{
    const char *szDevPath;
    ST_ACL_Instance *pstFoundInst;
} ST_ACL_Para;


/******************************************************************************
*                                FUNCTION
******************************************************************************/

/* Init && Destroy */
int ACL_ModuleRegister();
int ACL_ModuleInit(void);
int ACL_ModuleDestroy(void);


#endif //__TBS_ACL_H__

