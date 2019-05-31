/**********************************************************************
 Copyright (c), 1991-2007, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 �ļ�����: staticrouting.h
 �ļ�����: ��̬·��ͷ�ļ�
 �޶���¼:
        1. ����: �Ƽ̶�
           ����:
           ����: �����ļ�
**********************************************************************/

#ifndef _V6SROUTE_H_
#define _V6SROUTE_H_

#define V6SROUTE_MOD  "[V6SROUTE] "
/******************************************************************************
*                                 DEBUG                                      *
******************************************************************************/
#ifdef  V6SROUTE_DEBUG
#define V6SROUTETRACE1  /* ��ӡ��Ҫ����Ϣ */
#define V6SROUTETRACE2  /* ��ӡ����Ҫ����Ϣ */
#define V6SROUTETRACE3
#define V6SROUTEASSERT
#endif

#ifdef  V6SROUTETRACE1
#define V6SROUTE_TRACE1(fmt, args...)  printf(V6SROUTE_MOD"%s: %d: " fmt, __FUNCTION__, __LINE__, ##args)
#else
#define V6SROUTE_TRACE1(fmt, args...)  do{;}while(0);
#endif
#ifdef  V6SROUTETRACE2
#define V6SROUTE_TRACE2(fmt, args...)  printf(V6SROUTE_MOD"%s: %d: " fmt, __FUNCTION__, __LINE__, ##args)
#else
#define V6SROUTE_TRACE2(fmt, args...)  do{;}while(0);
#endif
#ifdef  V6SROUTETRACE3
#define V6SROUTE_TRACE3(fmt, args...)  printf(fmt, ##args)
#else
#define V6SROUTE_TRACE3(fmt, args...)  do{;}while(0);
#endif

#ifdef V6SROUTEASSERT
#define V6SROUTE_RT_ASSERT(exp) \
if (!(exp)) \
{ \
    V6SROUTE_TRACE1("ASSERT FAIL"); \
    exit(-1); \
}
#else
#define V6SROUTE_RT_ASSERT(exp)  do{;}while(0);
#endif

#define CALL_FUCTION_FAILED "Call function \"%s\" failed\n"

#define V6SROUTE_RET_RETURN(iRet, strError, args...) RET_RETURN(V6SROUTE_MOD, iRet, strError, ##args)
#define V6SROUTE_PTR_RETURN(pRet, strError, args...) POINTER_RETURN(V6SROUTE_MOD, pRet, strError, ##args)

/******************************************************************************
*                                 MACRO                                      *
******************************************************************************/
#define  V6SROUTE_RT_TRACE_INTO_FUNC  V6SROUTE_TRACE1("##In## %s\n", __func__)

/* �����ʱ���ӡ�궨�� */
#define V6SROUTE_PRINTF(fmt, args...)  printf(V6SROUTE_MOD "[ERROR] " "%s: %d: " fmt, __FUNCTION__, __LINE__, ##args)
#define V6SROUTE_GOTO(iRet,strError,gotoTag)         \
      {\
        if ( RET_FAILED(iRet) )    \
        {   \
            V6SROUTE_PRINTF("%s, %d %s \n\r", __func__, __LINE__, strError); \
            goto gotoTag; \
        }\
      }

#define FOR_EACH_LIST(pos, list)            for ((pos) = list; (pos) != NULL; (pos) = (pos)->pstNext)

/* ���õĺ궨�� */
#define V6SROUTE_System(para0)              tbsSystem(para0, 1)
#define V6SROUTE_MOD_ID                     MID_V6SROUTE
#define V6SROUTE_NODE_LENGTH                256
#define V6SROUTE_PATH_LENGTH                256
#define MAX_INDEX_COUNT                     1
#define MAX_CMD_LENGTH                      256
#define MAX_RULE_LENGTH                     256
#define MAX_IPV6_LEN                        40
#define MAX_COUNT_PER_ITEM                  16
#define V6SROUTE_NAME_INIT                  ""
#define V6SROUTE_ROUTE_RULE_INIT            ""
#define ROUTE_ADD_RULE                      "route add"
#define ROUTE_DEL_RULE                      "route del"
#define DEFAULT_ROUTE_DEL                   "route del default"

#define NOT_ERROR_INFO                      ""
#define DEFAULT_ERROR_INFO                  "ERROR"
#define DEVICE_NOT_CONNECTED_ERROR          "Device not connected"

#define LAN_DEVICE_NAME                     "X_TWSZ-COM_DeviceName"
#define LAN_DEVICE_PPP_IF_NAME              "X_TWSZ-COM_ConnectedDeviceName"
#define LAN_PHY_DEVICE_PATH                 "InternetGatewayDevice.LANDevice"

#define V6SROUTE_IPCONN_DEVICE              "InternetGatewayDevice.WANDevice"
#define V6SROUTE_IPCONN_CONNDEVICE          "WANConnectionDevice"
#define V6SROUTE_IPCONN_INSTANCE            "WANIPConnection"
#define V6SROUTE_V6WANCONN_INSTANCE         "X_TWSZ-COM_IPv6Config"
#define V6SROUTE_PPPCONN_INSTANCE           "WANPPPConnection"
#define V6SROUTE_IPCONN_NODE_CONN_STATUS    "ConnectionStatus"
#define V6SROUTE_IPCONN_NODE_DEF_RT_LIST    "IPv6DefaultRouterList"
#define V6SROUTE_IPCONN_NODE_DEF_RT_ADDR    "DefaultRouter"
#define V6SROUTE_LAN_DEVICE_FEATURE         ".LANDevice."
#define V6SROUTE_WAN_SERVICE_TYPE           "X_TWSZ-COM_ServiceList"
#define V6SROUTE_WAN_SERVICE_INTERNET       "Internet"
#define V6SROUTE_WAN_SERVICE_NAME           "Name"

#define V6SROUTE_IPCONN_CONN_STATUS         "GlobalConnected"
#define V6SROUTE_IPCONN_CONN_STATUS_DISCONNECTED         "Disconneted"
#define V6SROUTE_IPCONN_DEF_RT_STATUS       "Valid"
#define V6SROUTE_LAN_DEVICE_NAME            "br0"

/* �ڲ���Ϣ�ؼ��ֶ� */
#define V6SROUTE_VALUE_ENABLE               VALUE_ENABLE
#define V6SROUTE_VALUE_DISABLE              VALUE_DISABLE
#define V6SROUTE_VALUE_ERROR                VALUE_ERROR
#define V6SROUTE_KEY_PATH                   KEY_PATH
#define V6SROUTE_KEY_INTERFACE_NAME         KEY_INTERFACE_NAME
#define V6SROUTE_KEY_REMOVE_IP              KEY_REMOTE_IP

/* ���ù����ʱ������LOG�ļ� */
#define ROUTE_LOG_FILE                      "/var/log/staticrouting"
#define DEFAULT_ROUTE_LOG_FILE              "/var/log/defaultrouting"

/* ��̬·�ɽڵ�·�� */
#define V6SROUTE_MAIN_PATH                  "InternetGatewayDevice.X_TELEFONICA-ES_IPv6Layer3Forwarding"
#define V6SROUTE_FORWARD_PATH               "InternetGatewayDevice.X_TELEFONICA-ES_IPv6Layer3Forwarding.IPv6Forwarding"

/* tr069�ڵ� */
#define V6SROUTE_NODE_DEFAULT_ROUTE   "DefaultConnectionService"
#define V6SROUTE_NODE_INSTANCE_NUM    "ForwardNumberOfEntries"

#define V6SROUTE_NODE_ENABLE          "Enable"
#define V6SROUTE_NODE_STATUS          "Status"
#define V6SROUTE_NODE_DESTIP          "DestIPv6Address"
#define V6SROUTE_NODE_DESTMASK        "DestPrefixLength"
#define V6SROUTE_NODE_SRC_IP          "SourceIPv6Address"
#define V6SROUTE_NODE_SRC_MASK        "SourcePrefixLength"
#define V6SROUTE_NODE_GATEWAY         "GatewayIPv6Address"
#define V6SROUTE_NODE_INTERFACE       "Interface"
#define V6SROUTE_NODE_METRIC          "ForwardingMetric"
#define V6SROUTE_NODE_MTU             "MTU"

#define V6SROUTE_NODE_FLAG            "StaticRoute"
#define V6SROUTE_NODE_ERRORINFO       "X_TWSZ-COM_ErrorInfo"

/* ƥ��·�� */
#define V6SROUTE_DEFAULT_ROUTE_PATH   "^InternetGatewayDevice\\.X_TELEFONICA-ES_IPv6Layer3Forwarding\\."
#define V6SROUTE_BASE_PATH            "^InternetGatewayDevice\\.X_TELEFONICA-ES_IPv6Layer3Forwarding\\.IPv6Forwarding\\."
#define V6SROUTE_INSTANCE_PATH        "^InternetGatewayDevice\\.X_TELEFONICA-ES_IPv6Layer3Forwarding\\.IPv6Forwarding\\.[0-9]+\\."

/* WAN�ڵ�·�� */
#define WAN_IPCONN_INST_MATCH         "InternetGatewayDevice.WANDevice.{i}.WANConnectionDevice.{i}.WANIPConnection.{i}."
#define WAN_PPPCONN_INST_MATCH        "InternetGatewayDevice.WANDevice.{i}.WANConnectionDevice.{i}.WANPPPConnection.{i}."
#define WAN_CONNECTION_PATH           "InternetGatewayDevice.WANDevice.1.WANConnectionDevice."
#define WAN_IP_CONN_ENTRIES           "WANIPConnectionNumberOfEntries"
#define WAN_PPP_CONN_ENTRIES          "WANPPPConnectionNumberOfEntries"
#define WAN_IP_CONNECTION             "WANIPConnection"
#define WAN_PPP_CONNECTION            "WANPPPConnection"
#define WAN_CONNECTION_NAME           "Name"
#define WAN_IPV6_ENABLE               "X_TELEFONICA-ES_IPv6Enabled"
#define WAN_IPV6_STATUS               "X_TELEFONICA-ES_IPv6ConnStatus"

/* ADD��Ϣƥ�� */
#define V6SROUTE_ADD_MATCH_PATH       V6SROUTE_BASE_PATH"$"

/* SET��Ϣƥ�� */
#define V6SROUTE_SET_ENABLE         V6SROUTE_INSTANCE_PATH    V6SROUTE_NODE_ENABLE"$"
#define V6SROUTE_SET_DESTIP         V6SROUTE_INSTANCE_PATH    V6SROUTE_NODE_DESTIP "$"
#define V6SROUTE_SET_DESTMASK       V6SROUTE_INSTANCE_PATH    V6SROUTE_NODE_DESTMASK "$"
#define V6SROUTE_SET_SRCTIP         V6SROUTE_INSTANCE_PATH    V6SROUTE_NODE_SRC_IP "$"
#define V6SROUTE_SET_SRCMASK        V6SROUTE_INSTANCE_PATH    V6SROUTE_NODE_SRC_MASK "$"
#define V6SROUTE_SET_GATEWAY        V6SROUTE_INSTANCE_PATH   V6SROUTE_NODE_GATEWAY "$"
#define V6SROUTE_SET_INTERFACE      V6SROUTE_INSTANCE_PATH   V6SROUTE_NODE_INTERFACE "$"
#define V6SROUTE_SET_METRIC         V6SROUTE_INSTANCE_PATH     V6SROUTE_NODE_METRIC"$"
#define V6SROUTE_SET_DEFAULT_ROUTE  V6SROUTE_DEFAULT_ROUTE_PATH   V6SROUTE_NODE_DEFAULT_ROUTE "$"

/* COMMIT��Ϣƥ�� */
#define V6SROUTE_COMMIT_DEFAULT_ROUTE V6SROUTE_SET_DEFAULT_ROUTE
#define V6SROUTE_COMMIT_NODE        V6SROUTE_SET_ENABLE "|" V6SROUTE_SET_DESTIP "|" \
                                    V6SROUTE_SET_GATEWAY "|" V6SROUTE_SET_INTERFACE "|" V6SROUTE_SET_METRIC

/* Ĭ��·��ƥ�� */
#define V6SROUTE_DEL_MATCH_PATH     V6SROUTE_INSTANCE_PATH"$"

/* IPv6·�ɱ���Ϣ�ļ� */
#define V6SROUTE_ROUTE_INFO         "/var/ip6routeinfo.txt"

/* IPv6·�ɱ���Ϣ�ڵ� */
#define V6SROUTE_INFO_PATH          "InternetGatewayDevice.X_TWSZ-COM_IPv6RouteInfo"

/******************************************************************************
*                                 ENUM                                       *
******************************************************************************/
enum
{
    V6SROUTE_RT_ADD = 0,
    V6SROUTE_RT_DEL,
};

/******************************************************************************
*                                STRUCT                                      *
******************************************************************************/
/* V6Ĭ��·����Ϣ */
typedef struct tag_ST_V6DEFAULT_INFO{
    struct tag_ST_V6DEFAULT_INFO *pNext;
    struct tag_ST_V6DEFAULT_INFO *pPre;
    char szIfAddr[MAX_IPV6_LEN];        /* Ĭ��·�ɵ�IP��ַ */
    char szIfName[MAX_IF_NAME_LEN];     /* Ĭ��·�ɵĽӿ��� */
}ST_V6DEFAULT_INFO;

/* V6 ��̬·����Ϣ */
typedef struct tag_ST_V6SROUTE_RT_INFO{
    struct tag_ST_V6SROUTE_RT_INFO *pNext;
    char szV6DstAddr[MAX_IPV6_LEN];     /* Ŀ��IP��ַ */
    //char szV6DstPrefix[32];             /* Ŀ��IP������ */
    char szV6SrcAddr[MAX_IPV6_LEN];     /* ԴIP��ַ */
    //char szV6SrcPrefix[32];             /* ԴIP������ */
    char szMetric[32];                  /* metric */
    char szV6GWAddr[MAX_IPV6_LEN];      /* Ĭ������IP��ַ*/
    char szDevName[MAX_IF_NAME_LEN];    /* �豸�� */
}ST_V6SROUTE_RT_INFO;

/* V6Ĭ��·����Ϣ����ͷ */
typedef struct tag_ST_V6DEFAULT_INFO_HEAD{
    struct tag_ST_V6DEFAULT_INFO *pNext;
    struct tag_ST_V6DEFAULT_INFO *pPre;
}ST_V6DEFAULT_INFO_HEAD;

/* V6·�ɱ���Ϣ�ṹ */
typedef struct tag_ST_V6ROUTE_INFO
{
	char *pszDestination;
	char *pszNextHop;
	char *pszFlags;
	char *pszMetric;
	char *pszRef;
	char *pszUse;
	char *pszIface;
} ST_V6ROUTE_INFO;


#define TBS_E8_TELECOM

/******************************************************************************
*                                FUNCTION                                      *
******************************************************************************/
int V6SROUTE_GetDefault(char *pszValue);
#endif

/******************************************************************************
*                                 END                                        *
******************************************************************************/
