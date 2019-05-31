/**********************************************************************
 Copyright (c), 1991-2008, T&W ELECTRONICS(SHENTHEN) Co., Ltd.

 �ļ�����: antiattack.h

 �ļ�����: ������ģ���ͷ�ļ�

 �޶���¼:

           1 ���� : ������
             ���� : 2008-11-29
             ���� : ��FIREWALLģ���з������

**********************************************************************/
#ifndef __TBS_ANTIATTACK_H
#define __TBS_ANTIATTACK_H

#include "common.h"

#include "new_msg.h"
#include "common_func.h"
#include "common_msg_handler.h"


/******************************************************************************
*                                 MACRO
******************************************************************************/
/* ������Ϣ��*/
//#define ANTI_DEBUG
#ifdef ANTI_DEBUG
#define ANTI_TRACE(fmt, args...)                 COMMON_TRACE(MID_ANTIATTACK, fmt"\n", ##args)

#define ANTI_FUNC_IN(fmt, args...)               ANTI_TRACE("++I %s, "fmt"\n", __FUNCTION__, ##args)
#define ANTI_FUNC_OUT()                          ANTI_TRACE("--O %s\n", __FUNCTION__)

#else
#define ANTI_TRACE(str, args...)

#define ANTI_FUNC_IN(fmt, args...)
#define ANTI_FUNC_OUT()


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

#define ANTI_LOG_ERR(fmt, args...)                          COMMON_LOG_ERROR(MID_ANTIATTACK, fmt, ##args)

#define ANTI_RET_LOG(iRet, strError, args...)               RET_LOG(MID_ANTIATTACK, iRet, strError, ##args)
#define ANTI_POINTER_LOG(pRet, strError, args...)           POINTER_LOG(MID_ANTIATTACK, pRet, strError, ##args)

#define ANTI_RET_RETURN(iRet, strError, args...)            RET_RETURN(MID_ANTIATTACK, iRet, strError, ##args)
#define ANTI_POINTER_RETURN(pRet, strError, args...)        POINTER_RETURN(MID_ANTIATTACK, pRet, strError, ##args)

#define ANTI_RET_GOTO(iRet, strError, gotoTag)              RET_GOTO(MID_ANTIATTACK, iRet, strError, gotoTag)
#define ANTI_POINTER_GOTO(pRet, gotoTag, strError, args...) POINTER_GOTO(MID_ANTIATTACK, pRet, gotoTag ,strError, ##args)

#define ANTI_RET_FALSE(iRet, strError, args...)             RET_FALSE(MID_ANTIATTACK, iRet, strError, ##args)
#define ANTI_POINTER_FALSE(pRet, strError, args...)         POINTER_FALSE(MID_ANTIATTACK, pRet, strError, ##args)

#define ERR_GET(node)                       "Get Node["node"] value failed.\n"
#define ERR_SET(node)                       "Set Node["node"] value failed.\n"
#define ERR_SET_VAL(node)                   "Set Node["node"] value failed, value = %s\n"


/************************** �������� **************************/

/* �ڵ������� */
#define NODE_AntiDosEnable                  "AntiDosEnable"
#define NODE_AntiLogEnable                  "AntiLogEnable"

#define NODE_SynCookieEnable                "SynCookieEnable"
#define NODE_SynMaxConnections              "SynMaxConnections"
#define NODE_IcmpEchoIgnrBcastEnable        "IcmpEchoIgnrBcastEnable"
#define NODE_IgnrPortScanEnable             "IgnrPortScanEnable"
#define NODE_AntiFraggleEnable              "AntiFraggleEnable"
#define NODE_AntiEchoCargenEnable           "AntiEchoCargenEnable"
#define NODE_AntiIPLandEnable               "AntiIPLandEnable"

#define NODE_AntiScanSetSynFinEnable        "AntiScanSetSynFinEnable"
#define NODE_AntiScanSetSynRstEnable        "AntiScanSetSynRstEnable"
#define NODE_AntiScanSetFinRstEnable        "AntiScanSetFinRstEnable"
#define NODE_AntiScanUnAckSetFinEnable      "AntiScanUnAckSetFinEnable"
#define NODE_AntiScanUnAckSetPshEnable      "AntiScanUnAckSetPshEnable"
#define NODE_AntiScanUnAckSetUrgEnable      "AntiScanUnAckSetUrgEnable"
#define NODE_AntiScanUnsetAllEnable         "AntiScanUnsetAllEnable"
#define NODE_AntiScanSetAllEnable           "AntiScanSetAllEnable"
#define NODE_AntiScanForAllSetSynRstAckFinUrgEnable  "AntiScanForAllSetSynRstAckFinUrgEnable"
#define NODE_AntiScanForAllSetFinEnable              "AntiScanForAllSetFinEnable"
#define NODE_AntiScanForAllSetFinUrgPshEnable        "AntiScanForAllSetFinUrgPshEnable"


/* �ڵ�·������ */

/* chain name */
#define ANTI_DOS_CHAIN                      "ANTI_DOS_INPUT"
#define ANTI_SCAN_CHAIN                     "ANTI_SCAN_INPUT"
#define ANTI_SYN_PRE_CHAIN                  "ANTI_SYN_PRE"
#define ANTI_SYN_INPUT_CHAIN                "ANTI_SYN_INPUT"

#define ANTI_PATH_PREFIX                    "InternetGatewayDevice.X_TWSZ-COM_AntiAttack."
#define ANTI_DOS_PREFIX                     "InternetGatewayDevice.X_TWSZ-COM_AntiAttack.AntiDos"
#define ANTI_SCAN_PREFIX                    "InternetGatewayDevice.X_TWSZ-COM_AntiAttack.AntiScan"

/* ������ */
#define ANTI_REGEX_PREFIX                   "^InternetGatewayDevice\\.X_TWSZ-COM_AntiAttack\\."

/* WAN dev regex */
#define ANTI_REGEX_IPCONN_INST              "^InternetGatewayDevice\\.WANDevice\\.[0-9]{1,5}\\.WANConnectionDevice\\.[0-9]{1,5}\\.WANIPConnection\\.[0-9]\{1,5}\\.$"
#define ANTI_REGEX_PPPCONN_INST             "^InternetGatewayDevice\\.WANDevice\\.[0-9]{1,5}\\.WANConnectionDevice\\.[0-9]{1,5}\\.WANPPPConnection\\.[0-9]\{1,5}\\.$"

#define ANTI_REGEX_WAN_CONN_IP              "^InternetGatewayDevice\\.WANDevice\\.[0-9]{1,5}\\.WANConnectionDevice\\.[0-9]{1,5}\\.WANIPConnection\\.[0-9]\{1,5}$"
#define ANTI_REGEX_WAN_CONN_PPP             "^InternetGatewayDevice\\.WANDevice\\.[0-9]{1,5}\\.WANConnectionDevice\\.[0-9]{1,5}\\.WANPPPConnection\\.[0-9]\{1,5}$"



#define ANTI_MAX_SYNCONNECTIONS_DEF         50

/* ��ӹ���/ɾ������ */
#define ANTI_CMD_NONE                       0
#define ANTI_CMD_ADD                        1
#define ANTI_CMD_DEL                        2



/******************************************************************************
*                                ENUM
******************************************************************************/



/******************************************************************************
*                                STRUCT
******************************************************************************/

/* TCP��־ */
typedef struct tag_ST_TCP_SCAN_FLAG
{
    char *pszName;
    char *pszFlag1;
    char *pszFlag2;
    int iExists;
} ST_TCP_SCAN_FLAG;



/*******************************************************************
*
* FUNC DECLEARATION
*
*******************************************************************/
/* Init && Destroy */
int ANTI_ModuleRegister();
int ANTI_ModuleInit();
int ANTI_ModuleDestroy();


#endif//__TBS_ANTIATTACK_H
