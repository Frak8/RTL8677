/**********************************************************************
 Copyright (c), 1991-2007, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 �ļ�����: dynamicrouting.h
 �ļ�����: ��̬·��ͷ�ļ�
 �޶���¼:
        1. ����: �콡��
           ����: 
           ����: �����ļ�
**********************************************************************/
#ifndef _DYNAMICROUTE_H_
#define _DYNAMICROUTING_H_

#define DYNAMIC_MOD  "[DYNAMIC :]"
/******************************************************************************
*                                 DEBUG                                      *
******************************************************************************/
//#define DYNAMIC_DEBUG 1
#ifdef  DYNAMIC_DEBUG
#define DYNAMICTRACE1  //��ӡ��Ҫ����Ϣ
#define DYNAMICTRACE2  //��ӡ����Ҫ����Ϣ
#define DYNAMICTRACE3
#endif

#ifdef  DYNAMICTRACE1
#define DYNAMIC_TRACE1(fmt, args...)  printf(DYNAMIC_MOD"%s: %d: " fmt, __FUNCTION__, __LINE__, ##args)
#else
#define DYNAMIC_TRACE1(fmt, args...)  do{;}while(0);
#endif
#ifdef  DYNAMICTRACE2
#define DYNAMIC_TRACE2(fmt, args...)  printf(DYNAMIC_MOD"%s: %d: " fmt, __FUNCTION__, __LINE__, ##args)
#else
#define DYNAMIC_TRACE2(fmt, args...)  do{;}while(0);
#endif
#ifdef  DYNAMICTRACE3
#define DYNAMIC_TRACE3(fmt, args...)  printf(fmt, ##args)
#else
#define DYNAMIC_TRACE3(fmt, args...)  do{;}while(0);
#endif

/******************************************************************************
*                                 MACRO                                      *
******************************************************************************/
/* �����ʱ���ӡ�궨�� */
#define DYNAMIC_PRINTF(fmt, args...)  COMMON_TRACE(MID_DYNAMIC_ROUTING, DYNAMIC_MOD "[ERROR] " "%s: %d: " fmt, __FUNCTION__, __LINE__, ##args)
#define DYNAMIC_GOTO(iRet,strError,gotoTag)         \
      {\
        if ( RET_FAILED(iRet) )    \
        {   \
            DYNAMIC_PRINTF("%s, %d %s \n\r", __func__, __LINE__, strError); \
            goto gotoTag; \
        }\
      }

/* һ��ĺ궨�� */
#define DYNAMIC_MOD_ID          MID_DYNAMIC_ROUTING
#define DYNAMIC_NODE_LENGTH     256
#define DYNAMIC_PATH_LENGTH     256
#define MAX_PROC_NAME_LEN       16
#define DYNAMIC_ZEBRA_NAME      "zebra"
#define DYNAMIC_RIP_NAME        "rip"
#define DYNAMIC_ZEBRA_ID        1 
#define DYNAMIC_RIP_ID          2
#define DYNAMIC_INIT_NAME       ""
#define MAX_DYNAMIC_EXECINFO_COUNT  5
#define MAX_INDEX_COUNT             3

#define DYNAMIC_WANDEV          "InternetGatewayDevice.WANDevice"
#define DYNAMIC_WANCONNDEV      "WANConnectionDevice"
#define DYNAMIC_WANPPPCONN      "WANPPPConnection"
#define DYNAMIC_WANIPCONN       "WANIPConnection"
#define DYNAMIC_LANDEV            "InternetGatewayDevice.LANDevice"

/* zebra�������ļ� */
#define ZEBRA_CONFIG            "/var/zebra.conf"
#define RIPD_CONFIG             "/var/ripd.conf"

/* rip�汾ֵ */
#define DYNAMIC_VERSION_OFF     "Off"
#define DYNAMIC_VERSION_RIP1    "RIPv1"
#define DYNAMIC_VERSION_RIP2    "RIPv2"
#define DYNAMIC_VERSION_OSPF    "OSPF"

/* ע��ڵ��ƥ��·�� */
#define REG_NODE_PATH_IPCONN    "InternetGatewayDevice.WANDevice.{i}.WANConnectionDevice.{i}.WANIPConnection.{i}."
#define REG_NODE_PATH_PPPOE     "InternetGatewayDevice.WANDevice.{i}.WANConnectionDevice.{i}.WANPPPConnection.{i}."
#define REG_NODE_PATH_LAN        "InternetGatewayDevice.LANDevice.{i}."

/* ��̬·��ά���Ľڵ� */
#define NODE_ROUTE_RX               "RouteProtocolRx"
#define NODE_ROUTE_PASSIVE          "RipPassive"
#define NODE_DYNAMIC_INSTANCE_DEL   "X_TWSZ-COM_DelFlag"
#define NODE_DYNAMIC_WAN_STATUS     "X_TWSZ-COM_DynamicStatus"

/* ��̬·�ɽڵ�·�� */
#define DYNAMIC_PATH_ATTACH         "[0-9]+\\."
#define DYNAMIC_BASE_PATH_PPP       "^InternetGatewayDevice\\.WANDevice\\.[0-9]+\\.WANConnectionDevice\\.[0-9]+\\.WANPPPConnection\\."
#define DYNAMIC_BASE_PATH_IPCONN    "^InternetGatewayDevice\\.WANDevice\\.[0-9]+\\.WANConnectionDevice\\.[0-9]+\\.WANIPConnection\\."
#define DYNAMIC_BASE_PATH_LAN   "^InternetGatewayDevice\\.LANDevice\\.[0-9]\\."

/* ADD��Ϣƥ�� */
#define DYNAMIC_ADD_PATH_PPP        DYNAMIC_BASE_PATH_PPP"$"
#define DYNAMIC_ADD_PATH_IPCONN     DYNAMIC_BASE_PATH_IPCONN"$"
/* DEL��Ϣƥ�� */
#define DYNAMIC_DEL_PATH_PPP        DYNAMIC_BASE_PATH_PPP  DYNAMIC_PATH_ATTACH"$"
#define DYNAMIC_DEL_PATH_IPCONN     DYNAMIC_BASE_PATH_IPCONN   DYNAMIC_PATH_ATTACH"$"
/* SET��Ϣƥ�� */
#define DYNAMIC_SET_PPP_ROUTE       DYNAMIC_BASE_PATH_PPP  DYNAMIC_PATH_ATTACH  NODE_ROUTE_RX"$"
#define DYNAMIC_SET_IPCONN_ROUTE    DYNAMIC_BASE_PATH_IPCONN  DYNAMIC_PATH_ATTACH   NODE_ROUTE_RX"$"
#define DYNAMIC_SET_LAN_ROUTE       DYNAMIC_BASE_PATH_LAN NODE_ROUTE_RX"$"
/* COMMIT��Ϣƥ�� */
#define DYNAMIC_COMMIT_PPP_ROUTE    DYNAMIC_BASE_PATH_PPP  DYNAMIC_PATH_ATTACH  NODE_ROUTE_RX"$"
#define DYNAMIC_COMMIT_IPCONN_ROUTE    DYNAMIC_BASE_PATH_IPCONN  DYNAMIC_PATH_ATTACH   NODE_ROUTE_RX"$"
#define DYNAMIC_COMMIT_LAN_ROUTE   DYNAMIC_BASE_PATH_LAN NODE_ROUTE_RX"$"

#define DYNAMIC_COMMIT_PPP_PASSIVE    DYNAMIC_BASE_PATH_PPP  DYNAMIC_PATH_ATTACH  NODE_ROUTE_PASSIVE"$"
#define DYNAMIC_COMMIT_IPCONN_PASSIVE    DYNAMIC_BASE_PATH_IPCONN  DYNAMIC_PATH_ATTACH   NODE_ROUTE_PASSIVE"$"
#define DYNAMIC_COMMIT_LAN_PASSIVE   DYNAMIC_BASE_PATH_LAN NODE_ROUTE_PASSIVE"$"
/* �ڲ���Ϣ�ؼ��� */
#define DYNAMIC_PATH_KEY            KEY_PATH
#define DYNAMIC_KEY_INTERFACE_NAME  KEY_INTERFACE_NAME

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
typedef struct tag_ST_DYNAMIC_PROC{
    unsigned char   ucZebraState;                /* breza������״̬����״̬ */
    unsigned char   ucRipState;                   /* rip������״̬����״̬ */
    unsigned long   ulZebraId;                    /* breza��Ӧ��id */
    char            szZebraName[MAX_PROC_NAME_LEN];       /*breza����*/
    unsigned long   ulRipId;                    /* rip��Ӧ��id */
    char            szRipName[MAX_PROC_NAME_LEN];       /*rip����*/
}ST_DYNAMIC_PROC;

/* ������̬·�������ļ���ʱ��, ��Ҫ��ȡ�˽ڵ��б����Ϣ */
typedef struct tag_ST_DYNAMIC_REBUILD_FILE_LIST{
    char *szIfStatus;
    char *szIfName;
    char *iDelFlag;
}ST_DYNAMIC_REBUILD_FILE_LIST;

/*LAN��ڵ��б���Ϣ*/
typedef struct tag_ST_DYNAMIC_LAN_FILE_LIST{
    char *szIfName;
}ST_DYNAMIC_LAN_FILE_LIST;

/* �ж��Ƿ�����zebra���̵�ʱ��,��Ҫ���ߴ˽ڵ��б����Ϣ */
typedef struct tag_ST_DYNAMIC_OPERATE_LIST{
    char *szIfStatus;
    char *szVersion;
    char *iDelFlag;
}ST_DYNAMIC_OPERATE_LIST;

#endif

/******************************************************************************
*                                 END                                        *
******************************************************************************/
