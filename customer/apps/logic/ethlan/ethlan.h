/**********************************************************************

 Copyright (c), 1991-2008, T&W ELECTRONICS(SHENTHEN) Co., Ltd.

 �ļ�����: ethlan.h

 �ļ�����: ethlan ��ͷ�ļ�

 �޶���¼:

        1. ����: �����

           ����: 2008-2-22

           ����: �����ļ�

        2. ����: ����

           ����: 2008-10-10

           ����: ����ģ���Ż�

**********************************************************************/

#include <stdio.h>
#include "common.h"
#include "tbsmsg.h"
#include "tbserror.h"
#include "cfg_api.h"
#include "pc_api.h"
#include <net/if.h>
#include <linux/socket.h>
#include <linux/sockios.h>
#include "common_msg_handler.h"
#include "common_func.h"
#include "tbsutil.h"
#include "warnlog.h"

/******************************************************************************
 *                                 MACRO                                      *
 ******************************************************************************/
#ifdef ETHLAN_UTEST

extern void *stub_malloc(unsigned long ulLen);

#define malloc  stub_malloc
#endif

#ifdef ETHLAN_DEBUG
#define ETHLAN_TRACE(fmt, args...) TBS_TRACE(MID_ETHLAN, fmt"\n", ##args)
#else
#define ETHLAN_TRACE(str, args...)  do { ; } while(0);
#endif

//#define CONFIG_APPS_LOGIC_LAN_PORT_MODE

//#define MAX_PATH_LEN              200         /* �ڵ�·����󳤶�*/
//#define MAX_NODE_NAME_LEN         32          /* ���ڵ�������*/
//#define MAX_NODE_VALUE_LEN        48          /* ���ڵ�ֵ����*/
#define MAX_INDEX_COUNT             2           /* �����Ÿ���*/
#define MAX_MSG_LEN                 1024        /* �����Ϣ����*/
#define MAX_IFNAME_LEN              8           /* ���ӿ�������*/
#define MAX_MACADDR_LEN             48          /* ���MAC��ַ����*/
#define MAX_STR_LEN                 1024        /* �ַ�����󳤶�*/
#define MAX_WHITE_MAC_LIST_COUNT    20          /* ���������MAC��*/
//#define MAX_BOOL_LEN              2           /* ��󲼶��ͳ���*/

/* ����������󳤶�*/
#define MAX_ERROR_DESC_LEN    (MAX_ERROR_INFO_LEN + MAX_PATH_LEN)

/* �ڵ����*/
#define NODE_COUNT      sizeof(s_astEthLanNodeInfoList) / sizeof(ST_NodeInfo);

/* TR069�ڵ��� */
#define NODE_IF_NAME                "X_TWSZ-COM_DeviceName"
#define NODE_ENABLE                 "Enable"
#define NODE_STATUS                 "Status"
#define NODE_MAC_ADDR               "MACAddress"
#define NODE_MACADDR_CTL_ENABLED    "MACAddressControlEnabled"
#define NODE_MAX_BIT_RATE           "MaxBitRate"
#define NODE_DUPLEX_MODE            "DuplexMode"
#define NODE_BYTES_SENT             "BytesSent"
#define NODE_BYTES_RECEIVED         "BytesReceived"
#define NODE_PACKETS_SENT           "PacketsSent"
#define NODE_PACKETS_RECEIVED       "PacketsReceived"
#define NODE_PACKETS_RXERROR  "ErrorsReceived"
#define NODE_PACKETS_TXERROR  "ErrorsSent"
#define NODE_PACKETS_RXDROP  "DiscardPacketsReceived"
#define NODE_PACKETS_TXDROP  "DiscardPacketsSent"
/*Ŀǰ�޽ӿڿɶ��������ٽڵ�*/
#define NODE_PACKETS_TXUNICAST  "UnicastPacketsSent"
#define NODE_PACKETS_RXUNICAST  "UnicastPacketsReceived"
#define NODE_PACKETS_TXMULTICAST  "MulticastPacketsSent"
#define NODE_PACKETS_RXMULTICAST  "MulticastPacketsReceived"
#define NODE_PACKETS_TXBROADCAST  "BroadcastPacketsSent"
#define NODE_PACKETS_RXBROADCAST  "BroadcastPacketsReceived"

/* TR069·��*/
#define STR_LANDEV          "InternetGatewayDevice.LANDevice"
#define STR_LANETHIFCFG     "LANEthernetInterfaceConfig"
#define STR_STATS           "Stats"
#define STR_CONNECTED       "Connected"
#define STR_DISCONNECTED    "Disconnected"
#define ETHLAN_SET_PATH     STR_LANDEV".%."STR_LANETHIFCFG".%."

#define FILE_PROCNET_DEV    "/proc/net/dev"

/* MII�Ĵ��� */
/* Basic Mode Control Register */
#define  MII_BMCR               0x00
#define  MII_BMCR_100MBIT       0x2000
#define  MII_BMCR_AN_ENA        0x1000
#define  MII_BMCR_DUPLEX        0x0100

/* Basic Mode Status Register */
#define  MII_BMSR               0x01
#define  MII_BMSR_100BASET4     0x8000
#define  MII_BMSR_100BASETX_FD  0x4000
#define  MII_BMSR_100BASETX_HD  0x2000
#define  MII_BMSR_10BASET_FD    0x1000
#define  MII_BMSR_10BASET_HD    0x0800
#define  MII_BMSR_LINK_VALID    0x0004

/* Auto-Negotiation Advertisement Register */
#define MII_ANAR                0x04

/* Auto-Negotiation Link Partner Ability Register */
#define  MII_ANLPAR             0x05
#define  MII_AN_100BASET4       0x0200
#define  MII_AN_100BASETX_FD    0x0100
#define  MII_AN_100BASETX_HD    0x0080
#define  MII_AN_10BASET_FD      0x0040
#define  MII_AN_10BASET_HD      0x0020

/******************************************************************************
 *                                STRUCT                                      *
 ******************************************************************************/

/* ����������*/
typedef struct tag_ST_ETHLAN_MAC {
	char szMACAddr[MAX_MACADDR_LEN];
	struct tag_ST_ETHLAN_MAC * pstNext;
}ST_ETHLAN_MAC;


/* �ڵ㺯���б�*/
typedef struct tag_ST_ETHLAN_NodeIFunc {
    const char * pszName;
    int (* pGetFunc)(char *pszValue, const char *pszIfName);
}ST_ETHLAN_NodeFunc;

/******************************************************************************
 *                                FUNC DECLEARATION                                      *
 ******************************************************************************/
/* ��ȡ״ֵ̬���� */
int ETHLAN_GetVportLinkStatus(char *pszValue, const char *pszIfName);


/* �ڵ�Ӧ�ú���*/
int ETHLAN_ApplyEnable(char *pszPath,char *pszValue);
int ETHLAN_ApplyMacAddrCtlEnable(char *pszPath,char *pszValue);

#ifdef  CONFIG_APPS_LOGIC_LAN_PORT_MODE 
int ETHLAN_ApplyMaxBitRate(char *pszPath,char *pszValue);
int ETHLAN_ApplyDuplexMode(char *pszPath,char *pszValue);
#endif

int ETHLAN_MsgIfStateInformHandler(ST_MSG *pstMsg);

int ETHLAN_MsgIfRegAckHandler(ST_MSG *pstMsg);

int ETHLAN_MsgIfUnregAckHandler(ST_MSG *pstMsg);

int ETHLAN_MsgMACListUpdateHandler (ST_MSG *pstMsg);

int ETHLAN_MsgCfgUpdateHandler ();

/* ��Ӱ�������������������*/
int ETHLAN_AddMACList(ST_ETHLAN_MAC ** ppstMACList, const char * pszMACStr);

/* �ͷŰ������ռ�*/
int ETHLAN_FreeAllMACList(ST_ETHLAN_MAC **ppstMACList);

/* ���ebtables����*/
int ETHLAN_AddEBTablesRules(ST_ETHLAN_MAC * pstMACList);

/* ɾ��ebtables����*/
int ETHLAN_DelEBTablesRules();

int ETHLAN_ModuleInit();

int ETHLAN_ModuleDestroy();

extern ST_HookEntry stEthLanSetHookTable[];
extern ST_HookEntry stEthLanCommitHookTable[];
extern ST_ETHLAN_NodeFunc g_astEthLanNodeFuncList[];
extern ST_HookEntry stEthLanUpdateHookTable[];


