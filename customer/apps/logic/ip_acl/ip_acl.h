/**********************************************************************
 Copyright (c), 1991-2008, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 
 �ļ�����: ip_acl.h

 �ļ�����: IP_ACLģ��ͷ�ļ�

 �޶���¼:
             ���� : kevin
             ���� : 2009-07-26
             ���� : ����
**********************************************************************/
#ifndef __TBS_IP_ACL_H__
#define __TBS_IP_ACL_H__
/*********************************************************************
**************************����ͷ�ļ�*****************************
**********************************************************************/
#include "common.h"
#include <arpa/inet.h>
#include "new_msg.h"
#include "common_func.h"
#include "common_msg_handler.h"
#include "autoconf.h"

/*********************************************************************
**************************���������*****************************
**********************************************************************/
//���Ժ���Ϣ
//#define IP_ACL_DEBUG

#ifdef IP_ACL_DEBUG
#define IP_ACL_TRACE(fmt, args...)                 COMMON_TRACE(MID_IP_ACL, fmt"\n", ##args)
#define IP_ACL_RET_GOTO(iRet, strError, gotoTag)               RET_GOTO(MID_IP_ACL, iRet, strError, gotoTag)
#else
#define IP_ACL_TRACE(fmt, args...)
#define IP_ACL_FUNC_IN(fmt, args...)
#define IP_ACL_RET_GOTO(iRet, strError, gotoTag) 
#endif


//IP_ACLģ��·��ǰ׺
#define IP_ACL_PATH_PREFIX                 "InternetGatewayDevice.X_TWSZ-COM_IP_ACL."
//IP_ACL ACLEnable·��
#define IP_ACL_PATH_ENABLE                 "InternetGatewayDevice.X_TWSZ-COM_IP_ACL.ACLEnable"
//IP_ACL ACL·��
#define IP_ACL_PATH_ACL                       "InternetGatewayDevice.X_TWSZ-COM_IP_ACL.ACL"

// �ڵ�·��������ʽ
//ģ��·��ǰ׺
#define IP_ACL_REGEX_PREFIX                "^InternetGatewayDevice\\.X_TWSZ-COM_IP_ACL\\."
//IP_ACL ACLEnable�ڵ�
#define IP_ACL_REGEX_ENABLE        IP_ACL_REGEX_PREFIX "ACLEnable" "$"
//IP_ACL ACL�ڵ�Ļ���·��
#define IP_ACL_REGEX_ADD_PATH    IP_ACL_REGEX_PREFIX "ACL." "$"
//����IP_ACL ACL�ڵ�ʵ����·��
#define IP_ACL_REGEX_SET_NODE        "^InternetGatewayDevice\\.X_TWSZ-COM_IP_ACL\\.ACL\\.[0-9]+\\.SrcIP" "$"
//ɾ��IP_ACL ACL�ڵ�ʵ����·��
#define IP_ACL_REGEX_DEL_NODE        "^InternetGatewayDevice\\.X_TWSZ-COM_IP_ACL\\.ACL\\.[0-9]+\\." "$"


/***********************************************************************
**************************����ԭ��**********************************
************************************************************************/
int IP_ACL_ModuleInit();
static int Static_IP_ACL_ApplyACLSet(char *pszPath, char *pszValue);
static int Static_IP_ACL_AddIPAddr(char *pszPath, unsigned long ulIndex);
static int Static_IP_ACL_GetRuleCount( int * piVaue);
static int Static_IP_ACL_CheckIPAddr(const char *pszValue);
static int Static_IP_ACL_CheckEnable(const char *pszValue);
static int Static_IP_ACL_ApplyIPAddr(char *pszPath, char *pszValue);
static int Static_IP_ACL_DelIPAddr(char *pszPath);
static int Static_IP_ACL_Init_All(void);
static int Static_IP_ACL_Init_Iptables(void);
static int Static_IP_ACL_Init_ACLRules(void);

#endif
