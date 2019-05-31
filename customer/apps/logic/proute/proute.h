
#ifndef __P_ROUTE_H__
#define __P_ROUTE_H__

#include "tbstype.h"
#include "tbserror.h"
#include "tbsmsg.h"
#include "cfg_api.h"
#include "tbsutil.h"

#ifdef _cplusplus
    #if _cplusplus
        extern "C" {
    #endif
#endif




/******************************************************************************
*                                 MACRO                                      *
******************************************************************************/

/*********************************************************************
 *                              DEBUG                                *
 *********************************************************************/
 //#define _PROUTE_DEBUG 1
#ifdef _PROUTE_DEBUG
    #define PROUTE_TRACE(fmt, args...) TBS_TRACE(MID_MACFILTER, \
                                                  ECHO_GREEN fmt ECHO_NONE, \
                                                  ##args)
    #define PROUTE_LOG_ERR(fmt, args...) TBS_TRACE(MID_MACFILTER, \
                                                    ECHO_RED fmt ECHO_NONE, \
                                                    ##args)
    #define CMD_PRINT 1   /* ����tbsSystemʱ��ӡ���� */
#else
    #define PROUTE_TRACE(fmt, args...)   do { ; } while(0)
    #define PROUTE_LOG_ERR(fmt, args...) COMMON_LOG_ERROR(MID_MACFILTER, fmt, ##args)
    #define CMD_PRINT 0
#endif

#define PROUTE_TRACE_INTO_FUNC PROUTE_TRACE("##In## %s\n", __func__)

#define PROUTE_PTR_RETURN(Pointer, Ret, fmt, args...) \
        if (!Pointer)    \
        { \
            PROUTE_LOG_ERR(fmt, ##args); \
            return Ret; \
        }

#define PROUTE_CHECK_NULL_PTR(ptr)    PROUTE_PTR_RETURN(ptr, TBS_NULL_PTR, \
                                                    "TBS_NULL_PTR\n")
#ifdef _PROUTE_DEBUG
#define PROUTE_ERR(err, arg...) \
    printf("[PROUTE]: %s %d error code:%04u%04u ", __FILE__, __LINE__, \
                 TBS_ERR_INTERNAL(err), TBS_ERR_STAND(err)); \
    printf("para: " arg); \
    printf("\n")

/*
#define PROUTE_ERR(err, arg...) \
    syslog(LOG_INFO, "%s %d error code:%04u%04u ", __FILE__, __LINE__, \
                 TBS_ERR_INTERNAL(err), TBS_ERR_STAND(err)) \
    syslog(LOG_INFO, "para: " arg)
*/

#define PROUTE_ASSERT(exp) \
if (!(exp)) \
{ \
    PROUTE_ERR(0, "ASSERT FAIL"); \
    exit(-1); \
}

#else

#define PROUTE_ERR(err, arg...)  (void)0
#define PROUTE_ASSERT(exp) (void)0

#endif


typedef int PROUTE_RET;

#define PROUTE_OK   TBS_SUCCESS
#define PROUTE_MALLOC_FAIL  ERR_MALLOC_FAILED
#define PROUTE_FILE_OPEN_ERR   ERR_FILE_OPEN

//#define PROUTE_INST_DEV     "InternetGatewayDevice.WANDevice"
//#define PROUTE_NODE_NAME "PolicyRoute"

#define PROUTE_INST_PATH "InternetGatewayDevice.X_TWSZ-COM_PolicyRoute"

#define PRoute_ADD_PATH_MATCH    "^InternetGatewayDevice\\.X_TWSZ-COM_PolicyRoute"

/*******************************************************************
 *                          �ڵ�·������                           *
 *******************************************************************/


#define PROUTE_NODE_WANDESCRIPTION         "WANDescription"
#define PROUTE_NODE_LANDESCRIPTION         "LANDescription"
#define PROUTE_NODE_WANPATHNAME              "WANPathName"
#define PROUTE_NODE_LANPATHNAME              "LANPathName"

#define PROUTE_NODE_GWADDRESS                   "RemoteIPAddress"
#define PROUTE_NODE_IPADDRESS                     "ExternalIPAddress"

#define PROUTE_NODE_IPGWADDRESS   		"DefaultGateway"

#define PROUTE_NODE_WANNAME   		"Name"


#define PROUTE_PPPOE_DEVICE                            "X_TWSZ-COM_ConnectedDeviceName"
#define PROUTE_PPPOE_LAN_DEVICE                            "X_TWSZ-COM_DeviceName"
#define PROUTE_WANPPPCONNECTION                  "WANPPPConnection"
#define PROUTE_WANIPCONNECTION                     "WANIPConnection"
/*****************************************************************
 *                     �ڵ�·��ƥ��������ʽ                    *
 *****************************************************************/
#define PROUTE_PATH_PREFIX_REGEX \
            "^InternetGatewayDevice\\.X_TWSZ-COM_PolicyRoute."

#define PROUTE_NODE_REGEX(node) \
            PROUTE_PATH_PREFIX_REGEX \
            "[0-9]+\\." node "$"


#define PROUTE_TRACE_INTO_FUNC PROUTE_TRACE("##In## %s\n", __func__)
 

#define PROUTE_GET_ULONG(ptr, i)        (((unsigned long *)ptr)[i])

#define PROUTE_CHNG_MASK_CONN_TYPE   0
#define PROUTE_CHNG_MASK_LAN_INTF    1
#define PROUTE_CHNG_MASK_SVC_TYPE    2
#define PROUTE_CHNG_MASK_ENABLE      3

#define PROUTE_MAX_PATH_LEN   256
#define PROUTE_MAX_WANLANCONNETION_LEN       256
#define PROUTE_MAX_IPSTRING_LEN                            256



#define PROUTE_COMM_ERR_PROC(ret, arg...) \
if (PROUTE_OK != ret) \
{ \
    PROUTE_ERR(ret, ##arg); \
    return ret; \
}


#define PROUTE_CMD(fmt, arg...) \
{ \
    sprintf(acCmd, fmt, ##arg); \
    ret = tbsSystem(acCmd, TBS_PRINT_CMD); \
}

#define PROUTE_RET_RETURN(Ret, fmt, args...)  \
        RET_RETURN(MID_PROUTE, Ret, fmt, ##args)

/*****************************************************************
 *                       ��ӡģʽ�ַ���                          *
 *****************************************************************/
#define CALL_FUCTION_FAILED     "Call function \"%s\" failed\n"
#define Check_PARAMETER_FAILED  "Check parameter \"%s\" failed\n"
#define SET_NODE_VAL_FAILED     "Set node \"%s\" value failed\n"
#define GET_NODE_VAL_FAILED     "Get node \"%s\" value failed\n"


/* �ӿ����ʹ��� */
#define PROUTE_WAN  1
#define PROUTE_LAN  2


/* lan�ӿ����Ͷ��� */
#define PROUTE_LAN_ETH   1
#define PROUTE_LAN_WLAN  2
#define PROUTE_LAN_USB   3
#define PROUTE_LAN_BR    4


/* wan�������Ͷ��� */
#define PROUTE_WAN_IP    1
#define PROUTE_WAN_PPP   2

/*wan ��������*/
enum en_WANCONNECTION_TYPE{
	PROUTE_WANPPPCONNECTIONTYPE,
	PROUTE_WANIPCONNECTIONYTPE ,
	PROUTE_WANCONNECTIONERRTYPE
	
};

/* ����·�ɸ�������*/
typedef enum en_PROUTE_UPDATERULE_TYPE{
	PROUTE_UPDATERULE_CREATE_TYPE,
	PROUTE_UPDATERULE_DELETE_TYPE,
	PROUTE_UPDATERULE_WANDEVICE_TYPE,
	PROUTE_UPDATERULE_INVALID_TYPE
	}PROUTE_UPDATERULE_TYPE;

/* LAN ʵ���ṹ */
typedef struct tag_ST_LAN_INTF
{
    unsigned long ulDevIndex;         /* LANDevice�������� */
    unsigned long ulIndex;            /* �ڸ���ӿ��е������� */
    unsigned char ucType;             /* �ӿ�����: ����μ� PROUTE_LAN_ETH �� */
    unsigned char aucSpace[3];
    char acDevName[16];               /* �ӿ��豸�� */
    struct tag_ST_LAN_INTF *pstNext;

} ST_LAN_INTF;


/* WAN ʵ���ṹ */
typedef struct
{
    unsigned long ulDevIndex;         /* WANDevice�������� */
    unsigned long ulConnDevIndex;     /* WANConnDevice �������� */
    unsigned char ucProtType;         /* ��������: IP, PPP */
    unsigned char aucSpace[3];
    unsigned long ulIndex;            /* �ڸ���ӿڵ������� */
    char acDevName[16];               /* �ӿ��豸�� */
    char acIp[24];                    /* ip��ַ */
    char acGw[24];                    /* ���ص�ַ */

} ST_WAN_INTF;


/* lan-wan �󶨹�ϵʵ�� */
typedef struct tag_ST_PROUTE_CONN
{
    struct tag_ST_PROUTE_CONN *pstNext;
    struct tag_ST_PROUTE_CONN *pstPrev;

    ST_WAN_INTF  stWanIntf;          /* wan ʵ�� */
    ST_LAN_INTF *pstLanIntf;         /* lanʵ������ */

    unsigned long ulMark;            /* ����·�ɱ�� */

} ST_PROUTE_INST;


/* ��ȡwanʵ��Ҷ�ӽڵ������ */
typedef struct
{
    char *pcEnable;
    char *pcConnType;
    char *pcLanIntf;
    char *pcStatus;
    char *pcIp;
    char *pcGw;

} ST_WAN_LEAF;


#define STATIC static


/***********************************************
STRUCT BY HUANGMENGXIANG
**************************************************/
/* lan-wan �ڵ�ʵ�� */
typedef struct tag_ST_PROUTE_NODE
{
    struct tag_ST_PROUTE_NODE *pstNext;
    struct tag_ST_PROUTE_NODE *pstPrev;

    char szNodeCfgPath[PROUTE_MAX_PATH_LEN];

    char PreWANConnectionDevice[PROUTE_MAX_WANLANCONNETION_LEN];

    char WANConnectionDevice[PROUTE_MAX_WANLANCONNETION_LEN];

    char LanConnectionDevice[PROUTE_MAX_WANLANCONNETION_LEN];

    char WANConnection[PROUTE_MAX_WANLANCONNETION_LEN];
    char LANConnection[PROUTE_MAX_WANLANCONNETION_LEN];
    char WANPathName[PROUTE_MAX_PATH_LEN];
    char  LANPathName[PROUTE_MAX_PATH_LEN];
    char szGWAddress[PROUTE_MAX_IPSTRING_LEN];
    char szIPAddress[PROUTE_MAX_IPSTRING_LEN];
    char szWANDescription[32];

    unsigned long ulMark;            /* ����·�ɱ�� */

	int ulWANConnectType;     

} ST_PROUTE_NODE;



#ifdef _cplusplus
    #if _cplusplus
        }
    #endif
#endif


#endif /* __P_ROUTE_H__ */

