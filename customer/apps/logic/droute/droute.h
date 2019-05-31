/**********************************************************************
 Copyright (c), 1991-2007, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 �ļ�����: droute.h
 �ļ�����: ��̬·��ͷ�ļ�
 �޶���¼:
        1. ����: �콡��
           ����:
           ����: �����ļ�
**********************************************************************/
#ifndef _DROUTE_H
#define _DROUTE_H

/******************************************************************************
*                                 DEBUG                                      *
******************************************************************************/

#ifdef  DROUTE_DEBUG
#define DROUTE_TRACE(fmt, args...)  COMMON_TRACE(MID_DROUTE, fmt, ##args)
#else
#define DROUTE_TRACE(fmt, args...)  do{;}while(0);
#endif


/******************************************************************************
*                                 MACRO                                      *
******************************************************************************/
/* �����ʱ���ӡ�궨�� */
#define DROUTE_GOTO(iRet,strError,gotoTag)         \
      {\
        if ( RET_FAILED(iRet) )    \
        {   \
            DROUTE_TRACE(strError); \
            goto gotoTag; \
        }\
      }

/* һ��ĺ궨�� */
#define DROUTE_MOD_ID                   MID_DROUTE
#define DROUTE_NODE_LENGTH              256
#define DROUTE_PATH_LENGTH              256
#define MAX_PROC_NAME_LEN               16
#define DROUTE_ZEBRA_NAME               "zebra"
#define DROUTE_RIP_NAME                 "rip"
#ifdef CONFIG_TBS_SUPPORT_IPV6
#define DROUTE_RIPNG_NAME               "ripng"
#endif
#define DROUTE_ZEBRA_ID                 1
#define DROUTE_RIP_ID                   2
#ifdef CONFIG_TBS_SUPPORT_IPV6
#define DROUTE_RIPNG_ID                 3
#endif
#define DROUTE_INIT_NAME                ""
#define MAX_DROUTE_EXECINFO_COUNT       5
#define MAX_INDEX_COUNT                 3

#define DROUTE_WANDEV                   "InternetGatewayDevice.WANDevice"
#define DROUTE_WANCONNDEV               "WANConnectionDevice"
#define DROUTE_WANPPPCONN               "WANPPPConnection"
#define DROUTE_WANIPCONN                "WANIPConnection"
#define DROUTE_LANDEV                   "InternetGatewayDevice.LANDevice"

/* WAN ����״̬ */
#define WAN_IPV4_CONNECTED              "Connected"
#ifdef CONFIG_TBS_SUPPORT_IPV6
#define WAN_IPV6_CONNECTED              "GlobalConnected"
#endif

/* zebra�������ļ� */
#define ZEBRA_CONFIG                    "/var/zebra.conf"
#define RIPD_CONFIG                     "/var/ripd.conf"
#ifdef CONFIG_TBS_SUPPORT_IPV6
#define RIPNGD_CONFIG                   "/var/ripngd.conf"
#endif

/* rip�汾ֵ */
#define DROUTE_VERSION_OFF              "Off"
#define DROUTE_VERSION_RIP1             "RIPv1"
#define DROUTE_VERSION_RIP2             "RIPv2"
#define DROUTE_VERSION_BOTH             "Both"
#define DROUTE_RIP_PORT            "520"

/* ע��ڵ��ƥ��·�� */
#define REG_NODE_PATH_IPCONN            "InternetGatewayDevice.WANDevice.{i}.WANConnectionDevice.{i}.WANIPConnection.{i}."
#define REG_NODE_PATH_PPPOE             "InternetGatewayDevice.WANDevice.{i}.WANConnectionDevice.{i}.WANPPPConnection.{i}."
#define REG_NODE_PATH_LAN               "InternetGatewayDevice.LANDevice.{i}."

/* ��̬·��ά���Ľڵ� */
#define NODE_ROUTE_RX                   "RouteProtocolRx"
#define NODE_ROUTE_PASSIVE          "RipPassive"
#ifdef CONFIG_TBS_SUPPORT_IPV6
#define NODE_ROUTE_RIPNG_ENABLED        "RIPngEnabled"
#endif
#define NODE_DROUTE_INSTANCE_DEL        "X_TWSZ-COM_DelFlag"
#define NODE_DROUTE_WAN_STATUS          "X_TWSZ-COM_DynamicStatus"

/* WAN Connection �ڵ� */
#ifdef CONFIG_TBS_SUPPORT_IPV6
#define NODE_WAN_IPV6_CONNECTION_STATUS "X_TWSZ-COM_IPv6Config.ConnectionStatus"
#define NODE_WAN_IPV6_DEVICE_NAME       "X_TWSZ-COM_IPv6Config.X_TWSZ-COM_ConnectedDeviceName"
#define NODE_WAN_IPV6_ENABLE            "X_TELEFONICA-ES_IPv6Enabled"
#endif

/* ��̬·�ɽڵ�·�� */
#define DROUTE_PATH_ATTACH              "[0-9]+\\."
#define DROUTE_BASE_PATH_PPP            "^InternetGatewayDevice\\.WANDevice\\.[0-9]+\\.WANConnectionDevice\\.[0-9]+\\.WANPPPConnection\\."
#define DROUTE_BASE_PATH_IPCONN         "^InternetGatewayDevice\\.WANDevice\\.[0-9]+\\.WANConnectionDevice\\.[0-9]+\\.WANIPConnection\\."
#define DROUTE_BASE_PATH_LAN            "^InternetGatewayDevice\\.LANDevice\\.[0-9]\\."

/* ADD��Ϣƥ�� */
#define DROUTE_ADD_PATH_PPP             DROUTE_BASE_PATH_PPP"$"
#define DROUTE_ADD_PATH_IPCONN          DROUTE_BASE_PATH_IPCONN"$"
/* DEL��Ϣƥ�� */
#define DROUTE_DEL_PATH_PPP             DROUTE_BASE_PATH_PPP  DROUTE_PATH_ATTACH"$"
#define DROUTE_DEL_PATH_IPCONN          DROUTE_BASE_PATH_IPCONN   DROUTE_PATH_ATTACH"$"
/* SET��Ϣƥ�� */
#define DROUTE_SET_PPP_ROUTE            DROUTE_BASE_PATH_PPP  DROUTE_PATH_ATTACH  NODE_ROUTE_RX"$"
#define DROUTE_SET_IPCONN_ROUTE         DROUTE_BASE_PATH_IPCONN  DROUTE_PATH_ATTACH   NODE_ROUTE_RX"$"
/* COMMIT��Ϣƥ�� */
#define DROUTE_COMMIT_PPP_ROUTE         DROUTE_BASE_PATH_PPP  DROUTE_PATH_ATTACH  NODE_ROUTE_RX"$"
#define DROUTE_COMMIT_IPCONN_ROUTE      DROUTE_BASE_PATH_IPCONN  DROUTE_PATH_ATTACH   NODE_ROUTE_RX"$"
#define DROUTE_COMMIT_LAN_ROUTE         DROUTE_BASE_PATH_LAN NODE_ROUTE_RX"$"

#define DROUTE_COMMIT_PPP_PASSIVE       DROUTE_BASE_PATH_PPP  DROUTE_PATH_ATTACH  NODE_ROUTE_PASSIVE"$"
#define DROUTE_COMMIT_IPCONN_PASSIVE    DROUTE_BASE_PATH_IPCONN  DROUTE_PATH_ATTACH   NODE_ROUTE_PASSIVE"$"
#define DROUTE_COMMIT_LAN_PASSIVE       DROUTE_BASE_PATH_LAN NODE_ROUTE_PASSIVE"$"

#ifdef CONFIG_TBS_SUPPORT_IPV6
#define DROUTE_COMMIT_PPP_RIPNG_ENABLED       DROUTE_BASE_PATH_PPP  DROUTE_PATH_ATTACH  NODE_ROUTE_RIPNG_ENABLED"$"
#define DROUTE_COMMIT_IPCONN_RIPNG_ENABLED    DROUTE_BASE_PATH_IPCONN  DROUTE_PATH_ATTACH   NODE_ROUTE_RIPNG_ENABLED"$"
#endif

/* �ڲ���Ϣ�ؼ��� */
#define DROUTE_PATH_KEY                 KEY_PATH
#define DROUTE_KEY_INTERFACE_NAME       KEY_INTERFACE_NAME

/******************************************************************************
*                                 ENUM                                       *
******************************************************************************/
/* WAN���ӵ�״̬ */
enum
{
    IF_STATE_DISCONNECTED = 0,
    IF_STATE_CONNECTED,
};

enum
{
    WAN_IPCONN = 0,
    WAN_PPPOE,
    LAN_CONN,
};

/******************************************************************************
*                                STRUCT                                      *
******************************************************************************/
/* ״̬�����ݽṹ */
typedef struct tag_ST_DROUTE_PROC{
    unsigned char   ucZebraState;                /* breza������״̬����״̬ */
    unsigned char   ucRipState;                   /* rip������״̬����״̬ */
#ifdef CONFIG_TBS_SUPPORT_IPV6
    unsigned char   ucRipngState;                   /* ripng������״̬����״̬ */
#else
    unsigned char   pad_char_1;                   /* 4 Bytes aligned */
#endif
    unsigned char   pad_char_2;                   /* 4 Bytes aligned */
    unsigned long   ulZebraId;                    /* breza��Ӧ��id */
    char            szZebraName[MAX_PROC_NAME_LEN];       /*breza����*/
    unsigned long   ulRipId;                    /* rip��Ӧ��id */
    char            szRipName[MAX_PROC_NAME_LEN];       /*rip����*/
#ifdef CONFIG_TBS_SUPPORT_IPV6
    unsigned long   ulRipngId;                    /* ripng��Ӧ��id */
    char            szRipngName[MAX_PROC_NAME_LEN];       /*ripng����*/
#endif
}ST_DROUTE_PROC;

/* ������̬·�������ļ���ʱ��, ��Ҫ��ȡ�˽ڵ��б����Ϣ */
typedef struct tag_ST_DROUTE_REBUILD_FILE_LIST{
    char *szIfStatus;
    char *szIfName;
    char *iDelFlag;
}ST_DROUTE_REBUILD_FILE_LIST;

/*LAN��ڵ��б���Ϣ*/
typedef struct tag_ST_DROUTE_LAN_FILE_LIST{
    char *szIfName;
}ST_DROUTE_LAN_FILE_LIST;

/* �ж��Ƿ�����zebra���̵�ʱ��,��Ҫ���ߴ˽ڵ��б����Ϣ */
typedef struct tag_ST_DROUTE_OPERATE_LIST{
    char *szIfStatus;
    char *szVersion;
    char *iDelFlag;
}ST_DROUTE_OPERATE_LIST;

#endif

/******************************************************************************
*                                 END                                        *
******************************************************************************/
