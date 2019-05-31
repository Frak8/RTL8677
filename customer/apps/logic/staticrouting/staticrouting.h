/**********************************************************************
 Copyright (c), 1991-2007, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 �ļ�����: staticrouting.h
 �ļ�����: ��̬·��ͷ�ļ�
 �޶���¼:
        1. ����: �콡��
           ����:
           ����: �����ļ�
**********************************************************************/


#ifndef _STATICROUTING_H_
#define _STATICROUTING_H_

#define STATIC_MOD  "[STATIC :]"
/******************************************************************************
*                                 DEBUG                                      *
******************************************************************************/
#ifdef  STATIC_DEBUG
#define STATICTRACE1  /* ��ӡ��Ҫ����Ϣ */
#define STATICTRACE2  /* ��ӡ����Ҫ����Ϣ */
#define STATICTRACE3
#endif

#ifdef  STATICTRACE1
#define STATIC_TRACE1(fmt, args...)  printf(STATIC_MOD"%s: %d: " fmt, __FUNCTION__, __LINE__, ##args)
#else
#define STATIC_TRACE1(fmt, args...)  do{;}while(0);
#endif
#ifdef  STATICTRACE2
#define STATIC_TRACE2(fmt, args...)  printf(STATIC_MOD"%s: %d: " fmt, __FUNCTION__, __LINE__, ##args)
#else
#define STATIC_TRACE2(fmt, args...)  do{;}while(0);
#endif
#ifdef  STATICTRACE3
#define STATIC_TRACE3(fmt, args...)  printf(fmt, ##args)
#else
#define STATIC_TRACE3(fmt, args...)  do{;}while(0);
#endif

/******************************************************************************
*                                 MACRO                                      *
******************************************************************************/
/* �����ʱ���ӡ�궨�� */
#define STATIC_PRINTF(fmt, args...)  COMMON_TRACE(MID_STATIC_ROUTING, "[ERROR] " "%s: %d: " fmt, __FUNCTION__, __LINE__, ##args)
#define STATIC_GOTO(iRet,strError,gotoTag)         \
      {\
        if ( RET_FAILED(iRet) )    \
        {   \
            STATIC_PRINTF("%s, %d %s \n\r", __func__, __LINE__, strError); \
            goto gotoTag; \
        }\
      }

#define STATIC_PTR_GOTO(pPtr,strError,gotoTag)         \
{\
    if ( NULL == pPtr )    \
    {   \
        STATIC_PRINTF("%s, %d %s \n\r", __func__, __LINE__, strError); \
        iRet = TBS_FAILED;\
        goto gotoTag; \
    }\
}

/* ���õĺ궨�� */
#define STATIC_System(para0)    tbsSystem(para0, TBS_PRINT_CMD)
#define STATIC_MOD_ID           MID_STATIC_ROUTING
#define STATIC_NODE_LENGTH      256
#define STATIC_PATH_LENGTH      256
#define MAX_INDEX_COUNT         1
#define MAX_CMD_LENGTH          128
#define MAX_RULE_LENGTH         256
#define STATIC_NAME_INIT        ""
#define STATIC_ROUTE_RULE_INIT  ""
#define ROUTE_ADD_RULE          "route add"
#define ROUTE_DEL_RULE          "route del"
/* disable debug output */
#define DEFAULT_ROUTE_DEL       "route del default 2> /dev/null"

#define NOT_ERROR_INFO          ""
#define DEFAULT_ERROR_INFO      "ERROR"
#define DEVICE_NOT_CONNECTED_ERROR  "Device not connected"

#define LAN_DEVICE_NAME         "X_TWSZ-COM_DeviceName"
#define LAN_PHY_DEVICE_PATH     "InternetGatewayDevice.LANDevice"

#define STATIC_IPCONN_DEVICE        "InternetGatewayDevice.WANDevice"
#define STATIC_IPCONN_CONNDEVICE    "WANConnectionDevice"
#define STATIC_IPCONN_INSTANCE      "WANIPConnection"
#define STATIC_PPPCONN_INSTANCE    "WANPPPConnection"
#define STATIC_WAN_STATE   "ConnectionStatus"
#define STATIC_WAN_CONNECTED      "Connected"
#define STATIC_IPCONN_GW  "DefaultGateway"

/* 3G backup */
#define STATIC_WAN_BACKUP 	"Backup3GEnable"
#define STATIC_WAN_BACKUP_ENABLED	"1"
#define STATIC_WAN_NODE_ACTCONN_NUM "InternetGatewayDevice.WANDevice.1.WANCommonInterfaceConfig.NumberOfActiveConnections"
#define STATIC_WAN_DEV_STATUS "InternetGatewayDevice.WANDevice.1.WANDSLInterfaceConfig.Status"
#define STATIC_WAN_DSL_UP "Up"

#define STATIC_WAN_BACKUP_IDLETIME "IdleDelayTime"

#define STATIC_PPP_CONNTYPE	"ConnectionType"
#define STATIC_CONNTYPE_IP_BRIDGE      "IP_Bridged"
#define STATIC_PPP_3G	"PPPoU"
#define STATIC_PPP_CONNTRIGGER "ConnectionTrigger"
#define STATIC_PPP_ONDEMAND "OnDemand"
#define STATIC_PPP_ENABLE "Enable"
#define STATIC_UPTIME	"InternetGatewayDevice.DeviceInfo.UpTime"
#define STATIC_WAITTIME 120
#define STATIC_WAN_SERVICE_TYPE "X_TWSZ-COM_ServiceList"
#define STATIC_TR069_TYPE	"TR069"
#define STATIC_WANTYPE_OTHER "OTHER"
#define STATIC_DNS_SERVER	"X_TWSZ-COM_UsrDNSServers"
#define STATIC_USR_DNS_SERVER	"DNSServers"

//defaultGWģ��·��ǰ׺
#define DEFAULTGW_PATH_PREFIX                 "InternetGatewayDevice.X_TWSZ-COM_DEFAULT_GW."

/* �ڲ���Ϣ�ؼ��ֶ� */
#define STATIC_VALUE_ENABLE     VALUE_ENABLE
#define STATIC_VALUE_DISABLE    VALUE_DISABLE
#define STATIC_VALUE_ERROR      VALUE_ERROR
#define STATIC_KEY_PATH         KEY_PATH
#define STATIC_KEY_INTERFACE_NAME   KEY_INTERFACE_NAME
#define STATIC_KEY_REMOVE_IP    KEY_REMOTE_IP

/* ���ù����ʱ������LOG�ļ� */
#define ROUTE_LOG_FILE          "/var/log/staticrouting"
#define DEFAULT_ROUTE_LOG_FILE  "/var/log/defaultrouting"

/* ��̬·�ɽڵ�·�� */
#define STATIC_MAIN_PATH        "InternetGatewayDevice.Layer3Forwarding"
#define STATIC_FORWARD_PATH     "InternetGatewayDevice.Layer3Forwarding.Forwarding"
#define DEFAULTGW_PATH             "InternetGatewayDevice.X_TWSZ-COM_DEFAULT_GW.UserInIf"

//·�ɱ���Ϣ�ڵ�
#define ROUTE_INFO_PATH  "InternetGatewayDevice.X_TWSZ-COM_RouteInfo"

/* tr069�ڵ� */
#define STATIC_NODE_INSTANCE_NUM    "ForwardNumberOfEntries"

#define STATIC_NODE_ENABLE          "Enable"
#define STATIC_NODE_STATUS          "Status"
#define STATIC_NODE_DESTIP          "DestIPAddress"
#define STATIC_NODE_DESTMASK        "DestSubnetMask"
#define STATIC_NODE_GATEWAY         "GatewayIPAddress"
#define STATIC_NODE_INTERFACE       "Interface"
#define STATIC_NODE_METRIC          "ForwardingMetric"
#define STATIC_NODE_FLAG            "StaticRoute"
#define STATIC_NODE_ERRORINFO       "X_TWSZ-COM_ErrorInfo"

/* ƥ��·�� */
#define STATIC_BASE_PATH            "^InternetGatewayDevice\\.Layer3Forwarding\\.Forwarding\\."
#define STATIC_INSTANCE_PATH        "^InternetGatewayDevice\\.Layer3Forwarding\\.Forwarding\\.[0-9]+\\."

/* ADD��Ϣƥ�� */
#define STATIC_ADD_MATCH_PATH       STATIC_BASE_PATH"$"

/* SET��Ϣƥ�� */
#define STATIC_SET_ENABLE   STATIC_INSTANCE_PATH    STATIC_NODE_ENABLE"$"
#define STATIC_SET_DESTIP   STATIC_INSTANCE_PATH    STATIC_NODE_DESTIP "$"
#define STATIC_SET_DESTMASK   STATIC_INSTANCE_PATH    STATIC_NODE_DESTMASK "$"
#define STATIC_SET_GATEWAY   STATIC_INSTANCE_PATH   STATIC_NODE_GATEWAY "$"
#define STATIC_SET_INTERFACE   STATIC_INSTANCE_PATH   STATIC_NODE_INTERFACE "$"
#define STATIC_SET_METRIC   STATIC_INSTANCE_PATH     STATIC_NODE_METRIC"$"

/* COMMIT��Ϣƥ�� */
#define STATIC_COMMIT_NODE  STATIC_SET_ENABLE "|" STATIC_SET_DESTIP "|" STATIC_SET_DESTMASK "|" \
                            STATIC_SET_GATEWAY "|" STATIC_SET_INTERFACE "|" STATIC_SET_METRIC

/* Ĭ��·��ƥ�� */
#define STATIC_DEL_MATCH_PATH STATIC_INSTANCE_PATH"$"
#define STATIC_WAN_PATH "InternetGatewayDevice.WANDevice.{i}.WANConnectionDevice.{i}."

/*3g ״̬�ڵ�·��*/
#define Connetion_Enable "InternetGatewayDevice.WANDevice.1.WANCommonInterfaceConfig.3GConnectionEnable"



/******************************************************************************
*                                 ENUM                                       *
******************************************************************************/
/* ����״̬ */
enum
{
    IF_STATE_DISCONNECTED = 0,
    IF_STATE_CONNECTED,
};

/* ʵ����Ч��־ */
enum
{
    INSTANCE_VALID = 0,
    INSTANCE_DISVALID,
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

/* �ڵ�DefaultConnectionService�Ƿ����Ĭ��·�� */
enum
{
    HAS_DEFAULT_RULE = 0,
    NOT_HAS_DEFAULT_RULE  = 1,
};

enum
{
    WAN_DEVICE = 0,
    LAN_DEVICE,
};


/******************************************************************************
*                                STRUCT                                      *
******************************************************************************/
/* ����ʵ�����ݽṹ */
typedef struct tag_ST_STATIC_Instance{
    unsigned long   ulIndex;                    /* �ڵ�·����index */
    int             iIsValid;                   /* ʵ����Ч��־ */
    char            szIfName[STATIC_NODE_LENGTH];    /* ʵ���󶨽ӿ����� */
    int             iIfState;                        /* ʵ����Ӧ������״̬ */
    int             enable;
    char            szRouteRule[MAX_RULE_LENGTH];   /* ��Ŵ�ʵ����Ӧ��·�ɹ���,�Ա�ɾ�� */
    int             iRouteInKernel;                 /* �Ƿ���kernel route table��־ */
    struct          tag_ST_STATIC_Instance *pstNext;
}ST_STATIC_Instance;


/* �ӿ���Ϣ���ݽṹ */
typedef struct tag_ST_INTERFACE_INFO{
    char szIfName[STATIC_NODE_LENGTH];      /* �ӿ����� */
    char szIfPath[STATIC_NODE_LENGTH];      /* �ӿڵ�ȫ·�� */
    char szRemoveIp[STATIC_NODE_LENGTH];    /* Զ��IP��ַ */
    int  iIfState;                          /* �˽ӿ�����״̬ */
    struct tag_ST_INTERFACE_INFO *pstNext;
}ST_INTERFACE_INFO;

/* Ĭ��·����Ϣ */
typedef struct tag_ST_DEFAULT_INFO{
    int  iDefaultRouteInKernel;         /* Ĭ��·���Ƿ���kernel table�� */
    int  iDefaultRuleFlag;              /* �ڵ�DefaultConnectionService�Ƿ����Ĭ��·�ɱ�־ */
    char szIfPath[STATIC_NODE_LENGTH];  /* �ӿڵ�ȫ·�� */
}ST_DEFAULT_INFO;

//·�ɱ���Ϣ�ṹ
typedef struct tag_ST_ROUTE_INFO
{
    char *pszDestination;
    char *pszGateway;
    char *pszNetmask;
    char *pszFlags;
    char *pszMetric;
    char *pszRef;
    char *pszIface;
}ST_ROUTE_INFO;

/* 3G������Ϣ���ݽṹ */
typedef struct tag_ST_BACKUP_INFO
{
	int iConnected;								/* �Ƿ������� */
	int iBackupEnable;							/* �Ƿ��п���backup��3G���� */
	char szBackupPath[STATIC_PATH_LENGTH];      /* 3G ppp�ӿڵ�ȫ·�� */
	int IdleDelayTime;							/* ��ʱʱ�� */	
}ST_BACKUP_INFO;

#endif

/******************************************************************************
*                                 END                                        *
******************************************************************************/
