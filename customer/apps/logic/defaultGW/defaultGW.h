/**********************************************************************
 Copyright (c), 1991-2009, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 
 �ļ�����: defaultGW.h

 �ļ�����: defaultGWģ��ͷ�ļ�

 �޶���¼:
             ���� : kevin
             ���� : 2009-07-29
             ���� : ����
**********************************************************************/
#ifndef __TBS_DEFAULT_GATEWAY_H__
#define __TBS_DEFAULT_GATEWAY_H__
/*********************************************************************
**************************����ͷ�ļ�*****************************
**********************************************************************/
#include "common.h"
#include <arpa/inet.h>
#include "new_msg.h"
#include "common_func.h"
#include "common_msg_handler.h"
#include "autoconf.h"

/*********************************************************************
**************************���������*****************************
**********************************************************************/

 
//���Ժ���Ϣ
#define DEFAULT_GATEWAY_DEBUG 1

#ifdef DEFAULT_GATEWAY_DEBUG
#define DFGW_TRACE(fmt, args...)                 COMMON_TRACE(MID_DEFAULTGW, fmt"\n", ##args)
#define DFGW_RET_GOTO(iRet, strError, gotoTag)               RET_GOTO(MID_DEFAULTGW, iRet, strError, gotoTag)
#else
#define DFGW_TRACE(fmt, args...)  
#define DFGW_RET_GOTO(iRet, strError, gotoTag)  
#endif

// ���õĺ궨��
#define MAX_RULE_LENGTH         256
#define DEFAULTGW_PATH_LENGTH   256
#define MAX_CMD_LENGTH          128
#define DEFAULTGW_NAME_INIT        ""

//defaultGWģ��·��ǰ׺
#define DEFAULTGW_ASSIGN_PATH                             "InternetGatewayDevice.Layer3Forwarding.DefaultConnectionService"
#define DEFAULTGW_EFFECT_PATH                 "InternetGatewayDevice.X_TWSZ-COM_DEFAULT_GW"
#define DEFAULTGW_PATH_PREFIX                 "InternetGatewayDevice.X_TWSZ-COM_DEFAULT_GW."
#define DEFAULTGW_WAN_STATE   "ConnectionStatus"
#define DEFAULTGW_WAN_CONNECTED      "Connected"
#define DEFAULT_ROUTE_LOG_FILE  "/var/log/defaultrouting"

//device regex
#define DEVICE_REGEX_WAN_CONN_IP                 "^InternetGatewayDevice\\.WANDevice\\.[0-9]{1,5}\\.WANConnectionDevice\\.[0-9]{1,5}\\.WANIPConnection\\.[0-9]\{1,5}$"
#define DEVICE_REGEX_WAN_CONN_PPP               "^InternetGatewayDevice\\.WANDevice\\.[0-9]{1,5}\\.WANConnectionDevice\\.[0-9]{1,5}\\.WANPPPConnection\\.[0-9]\{1,5}$"
#define DEVICE_REGEX_LAN_CONN                       "^InternetGatewayDevice\\.LANDevice\\.[0-9]{1,5}$"

#define DEFAULTGW_WAN_DEV_STATUS "InternetGatewayDevice.WANDevice.1.WANDSLInterfaceConfig.Status"
#define DEFAULTGW_WAN_DSL_UP "Up"

#define DEFAULTGW_DNS_SERVER	    "DNSServers"
#define DEFAULTGW_CONNTYPE	        "ConnectionType"
#define DEFAULTGW_CONNTYPE_IP_BRIDGE      "IP_Bridged"
#define DEFAULTGW_PPP_CONNTRIGGER "ConnectionTrigger"
#define DEFAULTGW_PPP_ONDEMAND "OnDemand"
#define DEFAULTGW_PPP_ENABLE "Enable"
#define DEFAULTGW_USR_DNS_SERVER	"X_TWSZ-COM_UsrDNSServers"
#define DEFAULTGW_WAN_SERVICE_TYPE  "X_TWSZ-COM_ServiceList"
#define DEFAULTGW_TR069_TYPE	    "TR069"
#define DEFAULTGW_WANTYPE_OTHER     "OTHER"
#define DEFAULTGW_WAN_DEVICE        "InternetGatewayDevice.WANDevice"
#define DEFAULTGW_WAN_CONNDEVICE    "WANConnectionDevice"
#define DEFAULTGW_PPPCONN_INSTANCE  "WANPPPConnection"
#define DEFAULTGW_IPCONN_INSTANCE      "WANIPConnection"
#define DEFAULTGW_PPP_NAME          "Name"

#define DEFAULTGW_PATH_LAN_IP     "InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.IPInterfaceIPAddress"
#define DEFAULTGW_PATH_LAN_MASK   "InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.IPInterfaceSubnetMask"

#define NODE_DNSOverrideAllowed         "DNSOverrideAllowed"

int DefaultGW_Rule_Work(int flag);
// �ڵ�·��������ʽ
//DefaultConnectionService�ڵ�
#define DEFAULTGW_REGEX_USERASSIGN            "InternetGatewayDevice.Layer3Forwarding.DefaultConnectionService" "$"

// Ĭ��·��ƥ����
#define DEFAULTGW_WAN_PATH "InternetGatewayDevice.WANDevice.{i}.WANConnectionDevice.{i}."
/***********************************************************************
**************************����ԭ��**********************************
************************************************************************/
int DefaultGateway_ModuleInit(void);

/* �ӿ���Ϣ���ݽṹ */
typedef struct tag_ST_INTERFACE_INFO{
    char szIfName[256];      /* �ӿ����� */
    char szIfPath[256];      /* �ӿڵ�ȫ·�� */
    char szRemoveIp[256];    /* Զ��IP��ַ */
    int  iIfState;                          /* �˽ӿ�����״̬ */
    struct tag_ST_INTERFACE_INFO *pstNext;
}ST_INTERFACE_INFO;

/* Ĭ��·����Ϣ */
typedef struct tag_ST_DEFAULT_INFO{
    int  iDefaultRouteInKernel;         /* Ĭ��·���Ƿ���kernel table�� */
    int  iDefaultRuleFlag;              /* �ڵ�X_TWSZ-COM_DEFAULT_GW�Ƿ����Ĭ��·�ɱ�־ */
    char szIfPath[DEFAULTGW_PATH_LENGTH];  /* �ӿڵ�ȫ·�� */
}ST_DEFAULT_INFO;

extern ST_INTERFACE_INFO *STATIC_FindInterfaceOfInternet(char *pExcludePath);
extern ST_INTERFACE_INFO *g_pstStaticInterfaceList;  /* ��̬·�ɽӿ�����ͷָ�� */
enum
{
    IF_STATE_DISCONNECTED = 0,
    IF_STATE_CONNECTED,
};

/* ·�ɹ����Ƿ���kernel��·�ɱ��� */
enum
{
    ROUTE_IN_KERNEL_TABLE = 0,
    ROUTE_OUT_KERNEL_TABLE,
};

/* ·�ɹ����Ƿ�enable */
enum
{
    ROUTE_RULE_DISENABLE = 0,
    ROUTE_RULE_ENABLE  = 1,
};

/* �ڵ�X_TWSZ-COM_DEFAULT_GW�Ƿ����Ĭ��·�� */
enum
{
    HAS_DEFAULT_RULE = 0,
    NOT_HAS_DEFAULT_RULE  = 1,
};

#endif

