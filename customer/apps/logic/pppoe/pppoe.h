/**********************************************************************

 Copyright (c), 1991-2008, T&W ELECTRONICS(SHENTHEN) Co., Ltd.



 �ļ�����: pppoe.h

 �ļ�����: PPPOE���� ͷ�ļ�


 �޶���¼:

        1. ����: �콡��

           ����: 2008-12-18

           ����: �����ļ�

**********************************************************************/
#ifndef _PPPOE_H
#define _PPPOE_H

#include "pppou.h"
#define PPPOE_MODULE "[PPPOE: ]"
#ifdef CONFIG_APPS_LOGIC_SUPPORT_PPPV6
#include "tbsipv6.h"
#endif

/* �����Ƿ�����vpn���ܵ���vpn API */
#ifdef CONFIG_APPS_LOGIC_VPN
#include "vpn.h"
#endif
/******************************************************************************
*                                 DEBUG                                      *
******************************************************************************/
#ifdef  PPPOE_DEBUG
#define PPPOETRACE1
#define PPPOETRACE2
#define PPPOETRACE3
#endif

#ifdef  PPPOETRACE1
#define PPPOE_TRACE1(fmt, args...)  printf(PPPOE_MODULE"%s: %d: " fmt, __FUNCTION__, __LINE__, ##args)
#else
#define PPPOE_TRACE1(fmt, args...)  do{;}while(0);
#endif
#ifdef  PPPOETRACE2
#define PPPOE_TRACE2(fmt, args...)  printf(PPPOE_MODULE"%s: %d: " fmt, __FUNCTION__, __LINE__, ##args)
#else
#define PPPOE_TRACE2(fmt, args...)  do{;}while(0);
#endif
#ifdef  PPPOETRACE3
#define PPPOE_TRACE3(fmt, args...)  printf(fmt, ##args)
#else
#define PPPOE_TRACE3(fmt, args...)  do{;}while(0);
#endif

/******************************************************************************
*                                 MACRO                                      *
******************************************************************************/
/* �����ʱ���ӡ�궨�� */
#define PPPOE_PRINTF(fmt, args...)  COMMON_TRACE(MID_PPPOE, PPPOE_MODULE "[ERROR]" "%s: %d: " fmt, __FUNCTION__, __LINE__, ##args)
#define PPPOE_GOTO(iRet,strError,gotoTag)         \
      {\
        if ( RET_FAILED(iRet) )    \
        {   \
            PPPOE_PRINTF("%s, %d %s \n\r", __func__, __LINE__, strError); \
            goto gotoTag; \
        }\
      }
#define CALL_FUCTION_FAILED     "Call function \"%s\" failed\n"
#ifdef CONFIG_APPS_LOGIC_PPPOE_BRIDGE
#define PPPCONN_RET_RETURN(iRet, strError, args...)              RET_RETURN(MID_PPPOE, iRet, strError, ##args)
#define  PPPCONN_POINTER_RETURN(pRet, strError, args...)          POINTER_RETURN(MID_PPPOE, pRet, strError, ##args)
#endif

/* һ��ĺ궨�� */
#define PPPOE_MOD_ID                MID_PPPOE
#define PPPOE_NODE_LENGTH           256
#define PPPOE_PATH_LENGTH           256
#define MAX_INDEX_COUNT             3
#define MAX_WAN_CONNECTONS_DEF      8
#define MAX_ACTIVE_CONNECTONS_DEF   8
#define MAX_PPP_EXECINFO_COUNT      100
#define MAX_PROC_NAME_LEN           16
#define MAX_DNS_SERVERS_LEN	        64  /* ���DNS��������ַ����*/
#define MAX_MAC_STR_LEN             18    /* MAC��ַ�ַ�����󳤶�*/
#define MAX_TIME_STR_LEN            24
#define PPPOE_NAME                  "pppd"

#define MAX_ERROR_DESC_LEN          (MAX_ERROR_INFO_LEN + MAX_PATH_LEN)    /* ����������󳤶�*/
#define MAX_STR_LEN                 256    /* �ַ�����󳤶�*/
#define MAX_SES_STR_LEN             1024    /* ppp�Ự����󳤶�*/
#define MAX_IDX_STR_LEN             16    /* ppp�Ự���е�ʵ������ַ�����󳤶�*/
#define MAX_MAC_STR_LEN             18    /* MAC��ַ�ַ�����󳤶�*/
#define MAX_PPPSESSION_STR_LEN      (MAX_MAC_STR_LEN + 18)    /* PPP�Ự�ַ�����󳤶�*/

#define MAX_USERNAME_LEN    64  /* ����û�������*/
#define MAX_PASSWORD_LEN    64  /* ������볤��*/
#define MAX_CONNNAME_LEN    256  /* �������������*/
/* �޸�MTU��MRU�ķ�ΧΪ576~1492 */
#define MAX_MRU             1492  /* ���MRU */
#define MIN_MRU             576   /* ��СMRU */
#define MAX_RETRY_TIMES     255   /* ��󲦺Ŵ��� */

#define PPPOE_LOCAL_FLAG 1
#define PPPSESSION_INTERVAL_CHAR    '#'
#define PPPOE_INFINITE_RETRY  0xff


/* �ؼ��ֶεĶ��� */
#define PPPOE_PATH_KEY              KEY_PATH
#define PPPOE_NAME_KEY              KEY_NAME
#define PPPOE_STATUS_KEY            KEY_STATUS
#define PPPOE_ENABLE_KEY            KEY_ENABLE
#define PPPOE_CONNECTED_KEY         VALUE_CONNECTED
#define PPPOE_DISTCONNECTED_KEY     VALUE_DISCONNECTED
#define PPPOE_VALUE_UNABLE          "Unavailable"
#define PPPOE_KEY_INTERFACE_NAME    KEY_INTERFACE_NAME
#define PPPOE_KEY_REMOTE_IP         KEY_REMOTE_IP
#define PPPOE_KEY_EXTERNAL_IP       KEY_EXTERNAL_IP
#define PPPOE_KEY_DNS_SERVER        KEY_DNS_SERVER
#define PPPOE_BUTTON_DISCONNETED    BUTTON_DISCONNETED
#define PPPOE_BUTTON_CONNECTED      BUTTON_CONNECTED
#define PPPOE_VALUE_UP              VALUE_UP
#define PPPOE_VALUE_DOWN            VALUE_DOWN
#ifdef CONFIG_APPS_LOGIC_NATPUBIPADDR
#define PPPOE_NAT_PUBLIC_IP            VALUE_NATIP
#endif

/* TR069 path*/
#define DSL_CONFIG_PATH         "InternetGatewayDevice.WANDevice.%lu.WANConnectionDevice.%lu.WANDSLLinkConfig."
#define PPPOE_WANDEV            "InternetGatewayDevice.WANDevice"
#define PPPOE_WANCONNDEV        "WANConnectionDevice"
#define PPPOE_WANPPPCONN        "WANPPPConnection"
#define PPPOE_WANCOMMETHIFCFG   "WANCommonInterfaceConfig"
#ifdef CONFIG_APPS_LOGIC_Argentina //syn bug[30512]: pvc auto_switch 8/35 <-> 0/33
#define PPPOE_PVCNUM            "DestinationAddress"
#endif
#define PPPOE_WANDEV_FMT        "InternetGatewayDevice.WANDevice.%lu.%s"
#define PPPOE_WANCONNDEV_FMT    "InternetGatewayDevice.WANDevice.%lu.WANConnectionDevice.%lu.%s"
#define PPPOE_PPPCONN_FMT       "InternetGatewayDevice.WANDevice.%lu.WANConnectionDevice.%lu.WANPPPConnection.%lu.%s"


#define PPPOE_MAXIMUM_CONNS     "WANCommonInterfaceConfig.X_TWSZ-COM_MaximumPPPConnections"

/* TR069�ڵ��� */
#define NODE_ENABLE                 "Enable"
#define NODE_CONNECTION_STATUS      "ConnectionStatus"
#define NODE_CONNECTION_TIME       "ConnectionTime"
#define NODE_CONN_ACTION            "X_TWSZ-COM_ConnectionAction"
#define NODE_CONNNAME               "Name"
#define NODE_LAST_CONNECTION_ERROR  "LastConnectionError"
#define NODE_NAT_ENABLED            "NATEnabled"
#define NODE_USERNAME               "Username"
#define NODE_PASSWORD               "Password"
#define NODE_PPP_AUTH_PROTOCOL      "PPPAuthenticationProtocol"
#define NODE_EXTERNAL_IP_ADDRESS    "ExternalIPAddress"
#define NODE_REMOTE_IP_ADDRESS      "RemoteIPAddress"
#define NODE_MAX_MRU_SIZE           "MaxMRUSize"
#define NODE_MAX_MTU_SIZE           "MaxMTUSize"
#define NODE_CONN_TRIGGER           "ConnectionTrigger"
#define NODE_CONN_TYPE              "ConnectionType"
#define NODE_PPP_LCP_ECHO           "PPPLCPEcho"
#define NODE_PPP_LCP_ECHO_RETRY     "PPPLCPEchoRetry"
#define NODE_PPP_CONNECTION_NAME    "X_TWSZ-COM_ConnectedDeviceName"
#define NODE_PPP_SESSIONID          "X_TWSZ-COM_SessionID"
#define NODE_PPP_PASSTHROUGH        "PassThrough"
#define NODE_NAT_PUBIPADDR          "Natipaddr"

#define NODE_BACKUP_ENABLE			"Backup3GEnable"
#define NODE_IDLE_DELAY_TIME		"IdleDelayTime"

#define NODE_IPCONN_NUM_OF_ENTRIES  "WANIPConnectionNumberOfEntries"
#define NODE_PPPCONN_NUM_OF_ENTRIES "WANPPPConnectionNumberOfEntries"
#define NODE_NUM_OF_ACTIVE_CONNS    "NumberOfActiveConnections"
#define NODE_UP_TIME                "Uptime"
#define NODE_IDLE_DISCONNECT_TIME   "IdleDisconnectTime"
#define NODE_IF_NAME                "X_TWSZ-COM_DeviceName"
#define NODE_RETRY_NUM              "X_TWSZ-COM_PppoeRetryNum"
#define NODE_STATIC_IP_ADDRESS      "X_TWSZ-COM_StaticIPAddress"

#define NODE_AC_NAME                "PPPoEACName"
#define NODE_SERVICE_NAME           "PPPoEServiceName"

#define NODE_DNSServers                     "DNSServers"
#define NODE_UsrDNSServers                  "X_TWSZ-COM_UsrDNSServers"

#ifdef CONFIG_APPS_LOGIC_SUPPORT_PPPV6
#define NODE_PROTO_IPV6_ENABLE           "X_TELEFONICA-ES_IPv6Enabled"
#else
#define NODE_PROTO_IPV6_ENABLE           ""
#endif
#define NODE_PROTO_IPV4_ENABLE           "X_TELEFONICA-ES_IPv4Enabled"

#ifdef CONFIG_APPS_LOGIC_Argentina //syn bug[30512]: pvc auto_switch 8/35 <-> 0/33
#define PVC1  "PVC:0/33"
#define PVC2  "PVC:8/35"
#endif

/* ƥ��·�� */
#define PPPOE_CONN_DEV_PATH      "^InternetGatewayDevice\\.WANDevice\\.[0-9]+\\.WANConnectionDevice\\.[0-9]+$"
#define PPPOE_CONN_DEV_INST  "InternetGatewayDevice.WANDevice.%lu.WANConnectionDevice.%lu"

/* ADD��Ϣƥ��·�� */
#define PPPOE_ADD_PATH_MATCH    "^InternetGatewayDevice\\.WANDevice\\.[0-9]+\\.WANConnectionDevice\\.[0-9]+\\.WANPPPConnection\\.$"

/* DEL��Ϣƥ��·�� */
#define PPPOE_DEL_PATH1_MATCH   "^InternetGatewayDevice\\.WANDevice\\.[0-9]+\\.WANConnectionDevice\\.[0-9]+\\.WANPPPConnection\\.[0-9]+\\.$"
#define PPPOE_DEL_PATH2_MATCH   "^InternetGatewayDevice\\.WANDevice\\.[0-9]+\\.WANConnectionDevice\\.[0-9]+\\.$"

/* SET��Ϣƥ��·�� */
#define PPPOE_SET_BASE_PATH     "^InternetGatewayDevice\\.WANDevice\\.[0-9]+\\.WANConnectionDevice\\.[0-9]+\\.WANPPPConnection\\.[0-9]+\\."
#define PPPOE_SET_ENABLE    PPPOE_SET_BASE_PATH     NODE_ENABLE
#define PPPOE_SET_USERNAME  PPPOE_SET_BASE_PATH     NODE_USERNAME
#define PPPOE_SET_PASSWD    PPPOE_SET_BASE_PATH     NODE_PASSWORD
#define PPPOE_SET_CONNECTNAME   PPPOE_SET_BASE_PATH     NODE_CONNNAME
#define PPPOE_SET_MRU   PPPOE_SET_BASE_PATH     NODE_MAX_MRU_SIZE
#define PPPOE_SET_MTU   PPPOE_SET_BASE_PATH     NODE_MAX_MTU_SIZE
#define PPPOE_SET_AUTH  PPPOE_SET_BASE_PATH     NODE_PPP_AUTH_PROTOCOL
#define PPPOE_SET_TRIGGER   PPPOE_SET_BASE_PATH     NODE_CONN_TRIGGER
#define PPPOE_SET_CONNECTACTION     PPPOE_SET_BASE_PATH     NODE_CONN_ACTION
#define PPPOE_SET_LCPECHO   PPPOE_SET_BASE_PATH     NODE_PPP_LCP_ECHO
#define PPPOE_SET_LCPECHO_RETRY     PPPOE_SET_BASE_PATH     NODE_PPP_LCP_ECHO_RETRY
#define PPPOE_SET_RETRY_NUM     PPPOE_SET_BASE_PATH    NODE_RETRY_NUM
#define PPPOE_SET_IDLE_DISCONNECT_TIME     PPPOE_SET_BASE_PATH    NODE_IDLE_DISCONNECT_TIME
#define PPPOE_SET_STATIC_IP_ADDRESS     PPPOE_SET_BASE_PATH    NODE_STATIC_IP_ADDRESS
#define PPPOE_SET_CONN_TYPE PPPOE_SET_BASE_PATH     NODE_CONN_TYPE
#define PPPOE_SET_AC_NAME     PPPOE_SET_BASE_PATH    NODE_AC_NAME
#define PPPOE_SET_SERVICE_NAME     PPPOE_SET_BASE_PATH    NODE_SERVICE_NAME

/*3G����*/
#define PPPOE_SET_BACKUP_ENABLE		PPPOE_SET_BASE_PATH  NODE_BACKUP_ENABLE
#define PPPOE_SET_IDLE_DELAY_TIME	PPPOE_SET_BASE_PATH 	NODE_IDLE_DELAY_TIME

/* IPv6 */
#define PPPOE_SET_IPV6_ENABLE      PPPOE_SET_BASE_PATH NODE_PROTO_IPV6_ENABLE
#define PPPOE_SET_IPV4_ENABLE      PPPOE_SET_BASE_PATH NODE_PROTO_IPV4_ENABLE
#define PPPOE_SET_PASSTHROUGH_ENABLE      PPPOE_SET_BASE_PATH NODE_PPP_PASSTHROUGH

/*** NAT PUBLIC IP ADDRESS ***/
#define PPPOE_SET_NAT_PUBIPADDR      PPPOE_SET_BASE_PATH NODE_NAT_PUBIPADDR

#define IPCONN_SET_BASE_PATH    "^InternetGatewayDevice\\.WANDevice\\.[0-9]+\\.WANConnectionDevice\\.[0-9]+\\.WANIPConnection\\.[0-9]+\\."
#define IPCONN_SET_CONNECTNAME  IPCONN_SET_BASE_PATH   NODE_CONNNAME

/*
 * ipv6 enable/disable �ڵ�
 */
#ifdef CONFIG_APPS_LOGIC_IPV6_ENABLE
#define IPV6_ENABLE_NODE "InternetGatewayDevice.X_TWSZ-COM_IPv6Enable.Enable"
#define COMMIT_IPV6_SET_ENABLE "InternetGatewayDevice\\.X_TWSZ-COM_IPv6Enable\\.Enable"
#endif

#ifdef   CONFIG_APPS_LOGIC_WANSELECT
#define WAN_PPPOE_WANSELECT_PATH_PREFIX            "InternetGatewayDevice.Services."\
                                         "X_TWSZ-COM_WANSelect."

#define WAN_PPPOE_WANSELECT_NODE_DEFCFG          "DefaultCfg"
#endif

/* COMMIT��Ϣƥ��·�� */
#define COMMIT_NODE_LIST_PPPOE  PPPOE_SET_ENABLE"|"PPPOE_SET_USERNAME"|"PPPOE_SET_PASSWD"|" \
				PPPOE_SET_MRU"|"PPPOE_SET_MTU"|"PPPOE_SET_AUTH"|"   \
				PPPOE_SET_TRIGGER"|"PPPOE_SET_CONNECTACTION"|"PPPOE_SET_LCPECHO"|" \
				PPPOE_SET_LCPECHO_RETRY"|"PPPOE_SET_IDLE_DISCONNECT_TIME"|" \
				PPPOE_SET_STATIC_IP_ADDRESS"|"\
				PPPOE_SET_IPV6_ENABLE"|"\
				PPPOE_SET_CONN_TYPE "|"\
				PPPOE_SET_IPV4_ENABLE "|"\
				PPPOE_SET_PASSTHROUGH_ENABLE "|"\
				PPPOE_SET_NAT_PUBIPADDR
				


/* Ĭ��MTU/MRUֵ */
#define PPPOE_DEFAULT_MTU "1500"
typedef enum LINK_TYPE
{
    TYPE_PPPOE,
    TYPE_PPPOA,
    TYPE_OTHER
}emLINK_TYPE;
#ifdef CONFIG_APPS_LOGIC_PPPOE_BRIDGE
/* �������͸ı� */
enum
{
    EN_PPPCONN_NoChanged = 0,
    EN_PPPCONN_UnconfiguredToBridged,
    EN_PPPCONN_UnconfiguredToRouted,
    EN_PPPCONN_BridgedToUnconfigured,
    EN_PPPCONN_BridgedToRouted,
    EN_PPPCONN_RoutedToUnconfigured,
    EN_PPPCONN_RoutedToBridged
};
#endif


/******************************************************************************
*                                STRUCT                                      *
******************************************************************************/
/* PPPoE ��״̬*/
typedef enum tag_EN_PPPOE_STATE{
    EN_PPPOE_STATE_UNKNOWN = 0x00,    		/* δ֪״̬ */
    EN_PPPOE_STATE_CONNECTED,               /* ����״̬ */
    EN_PPPOE_STATE_DISCONNECTED,            /*  �Ͽ�״̬ */
    EN_PPPOE_STATE_INVALID = 0xFF    		/* ��Ч״̬ */
}EN_PPPOE_STATE;

#ifdef CONFIG_APPS_LOGIC_SUPPORT_PPPV6
typedef enum tag_EN_PPPVX_STATE{
    PPPVX_STATE_UNKNOWN = 0x00,    	/* δ֪״̬ */
    PPPVX_STATE_CONNECTED,          /* ����״̬ */
    PPPVX_STATE_PSEUDO_CONNECTED,   /* ��ٵ�����״̬, ���ڰ��貦��*/
    PPPVX_STATE_DISCONNECTED,       /* �Ͽ�״̬ */
    PPPVX_STATE_INVALID = 0xFF      /* ��Ч״̬ */
}EN_PPPVX_STATE;
#endif

/* PPPOEʵ�����ݽṹ */
typedef struct tag_ST_PPPOE_Instance{
    struct{
        unsigned long ulWanDevIdx;                  /* �ڵ�·��WANDevice ��index */
        unsigned long ulWanConnDevIdx;              /* �ڵ�·��WANConnectionDevice ��index */
        unsigned long ulWanPPPConnIdx;              /* �ڵ�·��WANPPPConnection ��index */
    }stIndex;
    struct{
        unsigned char enSMCurState;                 /* ʵ��������״̬����״̬ */
        EN_PPPOE_STATE enPppoeState;                /* PPPoE ��״̬ */
        unsigned char ucOnDemandState;
#ifdef CONFIG_APPS_LOGIC_SUPPORT_PPPV6
        EN_PPPVX_STATE enPPPv6State;                /* ��ʾPPPv6��״̬ */
        struct in6_addr stLocalLLAddr;
        struct in6_addr stRemoteLLAddr;
	char strLocalLLAddr[MAX_IP6_ADDR_LEN];
	char strRemoteLLAddr[MAX_IP6_ADDR_LEN];
#endif
        unsigned long ulPPPId;                      /* ʵ����Ӧ��id */
        char szModuleName[MAX_PROC_NAME_LEN];
        char szPPPName[PPPOE_NODE_LENGTH];          /* PPP �� */
        char szWanIfName[PPPOE_NODE_LENGTH];        /* Wan �豸�� */
        char szDNSServers[MAX_DNS_SERVERS_LEN];     /* DNS��������ַ */
        char szExternalIP[MAX_IP_LEN];              /* ����IP��ַ */
        char szRemoteIP[MAX_IP_LEN];                /* Զ��IP��ַ */
        unsigned int nPppSessId;                    /* PPP�Ựid */
        char szPeerMAC[MAX_MAC_STR_LEN];            /* �Զ�MAC��ַ */
        unsigned char bWanIfState;                  /* Wan �豸״̬ */
		unsigned char ucPppoeRetryNum;              /* PPPOE �ز����� */
        unsigned char ucLinkType;                   /* pppoa/oe*/ 
        unsigned char ucUnitX;                      /* pppoa session ���ܵı��*/
        unsigned char ucSetATMAndPPPoE;             /* ͬʱ����ATM������PPPOE����ʱֻ����Чһ�� */

#ifdef CONFIG_APPS_LOGIC_PPP_RETRY
        int first_retry_count;
        int first_retry_timeout;
        int next_retry_count;
        int next_retry_timeout;
        int last_retry_count;
        int last_retry_timeout;
#endif		
#ifdef CONFIG_APPS_LOGIC_NATPUBIPADDR
        char szNatPubIp[MAX_IP_LEN];
#endif
    }stPrivate;
#ifdef CONFIG_APP_LOGIC_PPPOU
	struct
	{
        char szConnectionType[MAX_CONN_TYPE_LEN];
        char szApn[MAX_APN_LEN];
        char szUsbDev[MAX_USB_DEV_LEN];
        int bUsbIfState;                            /* usb �豸״̬ */
	}stPPPOU_modem;
#endif
    struct tag_ST_PPPOE_Instance *pstNext;
}ST_PPPOE_Instance;

typedef struct tag_ST_PPPOE_Auth{
	char szPPPAuthPro[32];
	char szPPPOption[64];
}ST_PPPOE_Auth;

extern int PPPOE_CommitProcess(char *pszPath, char *pszValue);
//extern void PPPOE_SetWanStatusFile(int pppConId, bool status);
extern int PPPOE_SetStatusMachine(ST_PPPOE_Instance *PPPInstance);
extern void PPPOE_PrintInnerMsg(char *msg);
extern int PPPOE_StopHandle(ST_PPPOE_Instance *PPPInstance);
extern int PPPOE_SetStatusRestart(ST_PPPOE_Instance *PPPInstance);
extern int PPPOE_SetStatusStop(ST_PPPOE_Instance *PPPInstance);
extern ST_PPPOE_Instance* PPPOU_FindInstanceByCnnType(const char *pType);
extern int PPPOE_ConnIsValid(ST_PPPOE_Instance *PPPInstance);
int PPPOE_NotifyWanConnectMsg(unsigned short usMsgType,  ST_PPPOE_Instance * PPPInstance);
#endif

/******************************************************************************
*                                 END                                        *
******************************************************************************/
