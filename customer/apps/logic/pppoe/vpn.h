/**********************************************************************
 Copyright (c), 1991-2008, T&W ELECTRONICS(SHENTHEN) Co., Ltd.

 �ļ�����: vpn.h
 �ļ�����: PPPOE���� ͷ�ļ�

 �޶���¼:
        1. ����: �
           ����: 2009-07-22
           ����: ����vpn����

**********************************************************************/

#ifndef __VPN_H__

/* ������Ϣ��*/

//#define VPN_DEBUG

#ifdef VPN_DEBUG
#define VPN_TRACE(fmt, args...)   		printf("%s:%4d "fmt, __FUNCTION__, __LINE__, ##args)
#else
#define VPN_TRACE(fmt, args...)         
#endif

#define L2TPD_NAME                 "l2tpd"
#define PPTP_NAME                  "pptp"

/* VPN��صĽڵ� */
#define NODE_VPN_SERVER 			"X_TWSZ-COM_VPN_SERVER"
#define NODE_VPN_CLIENT 			"X_TWSZ-COM_VPN_CLIENT"
#define NODE_VPN_NETMASK 			"X_TWSZ-COM_VPN_NETMASK"
#define NODE_VPN_GATEWAY 			"X_TWSZ-COM_VPN_GATEWAY"
#define NODE_VPN_TYPE				"ConnectionType"

//#define NODE_WAN_DEV_NAME		"^InternetGatewayDevice\\.WANDevice\\.[0-9]+\\.WANConnectionDevice\\.[0-9]+\\.X_TWSZ-COM_DeviceName"
#define NODE_WAN_DEV_NAME		"X_TWSZ-COM_DeviceName"

#ifndef PPPOE_SET_BASE_PATH
#define PPPOE_SET_BASE_PATH     "^InternetGatewayDevice\\.WANDevice\\.[0-9]+\\.WANConnectionDevice\\.[0-9]+\\.WANPPPConnection\\.[0-9]+\\."
#endif

#define VPN_SET_SERVER	PPPOE_SET_BASE_PATH NODE_VPN_SERVER
#define VPN_SET_CLIENT	PPPOE_SET_BASE_PATH NODE_VPN_CLIENT
#define VPN_SET_NETMASK	PPPOE_SET_BASE_PATH NODE_VPN_NETMASK
#define VPN_SET_GATEWAY	PPPOE_SET_BASE_PATH NODE_VPN_GATEWAY
#define VPN_SET_TYPE	PPPOE_SET_BASE_PATH NODE_VPN_TYPE

/* ǰ������ */
struct tag_ST_PPPOE_Instance;

/**************************** VPN ģ��Ķ���API ****************************/

/* ��Ȩ������������ص�API */
int VPN_CheckServer(const char *pVpnServer);
int VPN_CheckClient(const char *pVpnClient);
int VPN_CheckNetmask(const char *pNetmask);
int VPN_CheckGateway(const char *pGateway);

/* �Խ����������Ӧ������ */
int VPN_L2tpdStarted(void *pInstance, const char *pOutput);

/* ������������ */
int VPN_IsVpnConnection(struct tag_ST_PPPOE_Instance *pPppInstance);
int VPN_ProcCtrl(struct tag_ST_PPPOE_Instance *pPppInstance, const char *pcName,
				unsigned long ulInstNo, unsigned char ucOpt);
//int VPN_IsVpnConnection(ST_PPPOE_Instance *pPppInstance);

#endif /* __VPN_H__ */

