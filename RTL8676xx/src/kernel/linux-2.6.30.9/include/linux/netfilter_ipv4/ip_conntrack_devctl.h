/**********************************************************************
 Copyright (c), 1991-2008, T&W ELECTRONICS(SHENTHEN) Co., Ltd.

 �ļ�����: ip_conntrack_devctl.h

 �ļ�����: ���ն������ں�ģ��ͷ�ļ�
           ������һЩӦ�ò�Ҫ�õ�������

 �����б�:


 �޶���¼:
           1 ���� : ������
             ���� : 2009-03-26
             ���� : ����

**********************************************************************/
#ifndef _IP_CONNTRACK_DEVCTL_H
#define _IP_CONNTRACK_DEVCTL_H


#ifdef __KERNEL__
    #include <linux/types.h>
    #include <linux/ctype.h>
    #include <linux/tbs_msg.h>
#else
    typedef unsigned char       unchar;
    typedef unsigned short      ushort;
    typedef unsigned int        uint;
    typedef unsigned long       ulong;
    #include "tbstype.h"
    #define NETLINK_TBS_DEVCTL  18
#endif /* __KERNEL__ */



/******************************************************************************
*                                 MACRO                                      *
******************************************************************************/

#ifndef TRUE
#define TRUE    1
#define FALSE   0
typedef unsigned int BOOL;
#endif

typedef unsigned char byte;

/* ������ */
#define ERR_DEVCTL_INVALID_PARA         1
#define ERR_DEVCTL_INVALID_MSG          2
#define ERR_DEVCTL_INVALID_IP           3
#define ERR_DEVCTL_INVALID_DEVTYPE      4
#define ERR_DEVCTL_INVALID_CONFIG       5
#define ERR_DEVCTL_NOT_FOUND            6
#define ERR_DEVCTL_MEM_OUT              7
#define ERR_DEVCTL_LIST_FULL            8


/* ���ȶ��� */
#define MAX_DEVICE_COUNT	256
#define DEV_MAC_LEN         6

/* IP��ַ */
#define NIPQUAD(addr) \
	((unsigned char *)&addr)[0], \
	((unsigned char *)&addr)[1], \
	((unsigned char *)&addr)[2], \
	((unsigned char *)&addr)[3]
#define NIPQUAD_FMT "%u.%u.%u.%u"


/* Mac��ַ */
#define NMACQUAD(addr) \
    ((unsigned char *)addr)[0], \
	((unsigned char *)addr)[1], \
	((unsigned char *)addr)[2], \
	((unsigned char *)addr)[3], \
	((unsigned char *)addr)[4], \
	((unsigned char *)addr)[5]
#define NMACQUAD_FMT "%02x:%02x:%02x:%02x:%02x:%02x"

#define NET_ADDR(ip, mask)              ((ip) & (mask))
#define HOST_ADDR(ip, mask)             ((ip) & ~(mask))

#define IS_NET_ADDR(ip, mask)           (!HOST_ADDR(ip, mask))
#define IS_BROAD_ADDR(ip, mask)         (HOST_ADDR(ip, mask) == ~(mask))
#define IN_SAME_SUBNET(ip1, ip2, mask)  (NET_ADDR(ip1, mask) == NET_ADDR(ip2, mask))



/******************************************************************************
*                                 ENUM                                       *
******************************************************************************/

/*
�豸����:
 0:none
 1:Computer
 3:Camera
 2:STB
 4:Phone
*/
typedef enum tagEN_DC_DevType
{
    DC_DevType_None = 0,
    DC_DevType_Computer,
    DC_DevType_Camera,
    DC_DevType_STB,
    DC_DevType_Phone,
    
    DC_DevType_Max     //5
}EN_DC_DevType;


/*
��������:
 0: by ip
 1: by mac
 2: by devtype
*/
typedef enum
{
    DC_FT_ByIP,
    DC_FT_ByMac,
    DC_FT_ByDevType
} EN_DC_FilterType;



/******************************************************************************
*                                STRUCT                                      *
******************************************************************************/

/* �豸���Ͷ�Ӧ��Option60�ַ��� */
typedef struct tagST_DC_DevTypeName
{
    EN_DC_DevType enType;
    char *szName;
} ST_DC_DevTypeName;

/* �豸��Ϣ */
typedef struct tagST_DC_DeviceInfo
{
	ulong ipaddr;
    char mac[8];
    EN_DC_DevType type;
    ulong time;
} ST_DC_DeviceInfo;

/* �������� */
typedef struct tagST_DC_SearchFilter
{
   EN_DC_FilterType type;
   char value[8];
} ST_DC_SearchFilter;

/* ������� */
typedef struct tagST_DC_SearchResult
{
    ulong count;
    ST_DC_DeviceInfo devlist[0];
} ST_DC_SearchResult;


/* ������ */
typedef struct tagST_DC_DevTypeStatus
{
    uint currentNumber;
} ST_DC_DevTypeStatus;

/* ������Ϣ */
typedef struct tagST_DC_Config
{
    uint    mode;
    uint    maxTotalNumber;
    ulong   lanIP;
    ulong   lanMask;
    ulong   pppBeginIP;
    ulong   pppEndIP;
    uint    timeout;
    uint    print;
    struct tagDevTypeSetting
    {
        uint    enable;
        uint    maxNumber;
    } devTypeSettings[DC_DevType_Max];
} ST_DC_Config;


/******************************************************************************
*                                FUNCTION                                     *
******************************************************************************/

#ifdef __KERNEL__

/* ����mac��ַ����ip��ַ����ȡ��Ӧ�ն˵��豸���� */
EN_DC_DevType dc_get_devtype(EN_DC_FilterType type, char *value);

#endif /* __KERNEL__ */


/******************************************************************************
*                                 END                                        *
******************************************************************************/


#endif /* _IP_CONNTRACK_DEVCTL_H */

