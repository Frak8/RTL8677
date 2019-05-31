/**********************************************************************
 Copyright (c), 1991-2008, T&W ELECTRONICS(SHENTHEN) Co., Ltd.

 �ļ�����: dns.h

 �ļ�����: dnsģ���ͷ�ļ�

 �޶���¼:
           1 ���� : Annan Li (madbug)
             ���� : 2008-3-17
             ���� : ����

           2 ���� : ������
             ���� : 2008-10-10
             ���� : �ع�
**********************************************************************/
#ifndef _TBS_DNS_H
#define _TBS_DNS_H

#include "common.h"

#include "new_msg.h"
#include "common_func.h"
#include "common_msg_handler.h"


/******************************************************************************
 *                                 MACRO                                      *
 ******************************************************************************/

/* ��Ԫ����*/
#ifdef UNIT_TEST
#define fprintf stub_fprintf
#define malloc stub_malloc
#define fopen stub_fopen
#define fclose stub_fclose
#endif

/* ������Ϣ��*/

#ifdef DNS_DEBUG
#define DNS_TRACE(fmt, args...)         COMMON_TRACE(MID_DNS,fmt"\n", ##args)

#else
#define DNS_TRACE(fmt, args...)         do{}while(0)

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

#define DNS_LOG_ERR(fmt, args...)       COMMON_LOG_ERROR(MID_DNS, fmt, ##args)

/* ��������*/
#define RESOLVER_FILE                   "/etc/resolv.conf"
#define DPROXY_FILE                     "/etc/dproxy.conf"
#define HOSTS_FILE                      "/etc/hosts"
#define MAX_LEN_DNS                     48

/* �Ǳ�׼�ڵ���ǰ׺*/
#define CUST_NODE_NAME_PREFIX           "X_TWSZ-COM_"

/* ����·���ĸ�ʽ�ַ���*/
#define PATH_LAN_DEV_PREFIX             "InternetGatewayDevice.LANDevice."
#define PATH_WAN_DEV_PREFIX             "InternetGatewayDevice.WANDevice."

#define PATH_LAN_HOST_CM                "InternetGatewayDevice.LANDevice.1.LANHostConfigManagement."
#define PATH_WAN_IP_CONN                "InternetGatewayDevice.WANDevice.%lu.WANConnectionDevice.%lu.WANIPConnection.%lu."
#define PATH_WAN_PPP_CONN               "InternetGatewayDevice.WANDevice.%lu.WANConnectionDevice.%lu.WANPPPConnection.%lu."
#define PATH_WAN_IP_CONN_PATH           "InternetGatewayDevice.WANDevice.%lu.WANConnectionDevice.%lu.WANIPConnection.%lu"
#define PATH_WAN_PPP_CONN_PATH          "InternetGatewayDevice.WANDevice.%lu.WANConnectionDevice.%lu.WANPPPConnection.%lu"

#define MID_PATH_WAN_PPP_CONN           "InternetGatewayDevice.WANDevice.{i}.WANConnectionDevice.{i}.WANPPPConnection.{i}."
#define MID_PATH_WAN_IP_CONN            "InternetGatewayDevice.WANDevice.{i}.WANConnectionDevice.{i}.WANIPConnection.{i}."


#define PATH_DEFAULTGW                  "InternetGatewayDevice.X_TWSZ-COM_DEFAULT_GW"
#define NODE_DNSEnabled                 "DNSEnabled"
#define NODE_DNSOverrideAllowed         "DNSOverrideAllowed"
#define NODE_UseIPRoutersAsDNSServer    "X_TWSZ-COM_UseIPRoutersAsDNSServer"
#define NODE_DNSServers                 "DNSServers"
#define NODE_WANDNSServers              "X_TWSZ-COM_WANDNSServers"
#define NODE_DNSRelayEnable             CUST_NODE_NAME_PREFIX"DNSRelayEnable"
#define NODE_UsrDNSServers              CUST_NODE_NAME_PREFIX"UsrDNSServers"
#define NODE_IP_IF                      "IPInterface"
#define NODE_IF_IP_ADDRESS              "IPInterfaceIPAddress"
#define NODE_DNSConnectionStatus        "ConnectionStatus"

#define DNS_WAN_CONN_SERVICE_TYPE_NODE "X_TWSZ-COM_ServiceList"
#define DNS_WAN_CONN_SERVICE_TYPE_VALUE "Internet"

#define CALL_FUCTION_FAILED     "Call function \"%s\" failed\n"

#ifdef CONFIG_APPS_LOGIC_IPV6_ENABLE
#define IPV6_ENABLE_NODE "InternetGatewayDevice.X_TWSZ-COM_IPv6Enable.Enable"
#endif



/******************************************************************************
 *                                 ENUM                                       *
 ******************************************************************************/



/******************************************************************************
 *                                STRUCT                                      *
 ******************************************************************************/
#ifdef CONFIG_TBS_SUPPORT_IPV6
int DNS_DproxyStatemachine(unsigned char ucSignal);
#endif


/******************************************************************************
 *                               FUNCTION                                     *
 ******************************************************************************/
/* Init && Destroy */
int DNS_ModuleInit();
int DNS_ModuleDestroy();


#endif //_TBS_DNS_H
