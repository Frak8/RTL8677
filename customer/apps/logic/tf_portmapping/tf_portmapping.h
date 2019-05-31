/**********************************************************************
 Copyright (c), 1991-2010, T&W ELECTRONICS(SHENTHEN) Co., Ltd.

 �ļ�����: tf_portmapping.h
 �ļ�����: tf_portmapping ģ��ͷ�ļ�

 �޶���¼:
        1. ����: oujunhe
           ����: 2010-06-29
           ����: �����ļ�
**********************************************************************/

#ifndef __TF_PORTMAPPING_H__
#define __TF_PORTMAPPING_H__

#include "warnlog.h"

/******************************************************************************
*                                 MACRO                                      *
******************************************************************************/

/*********************************************************************
 *                              DEBUG                                *
 *********************************************************************/

#ifdef TF_PORTMAPPING_DEBUG
    #define TF_PM_TRACE(fmt, args...) COMMON_TRACE(MID_TF_PORTMAPPING, ECHO_GREEN fmt ECHO_NONE, ##args)
    #define TF_PM_LOG_ERR(fmt, args...) COMMON_TRACE(MID_TF_PORTMAPPING, ECHO_RED fmt ECHO_NONE, ##args)
    #define CMD_PRINT 1   /* ����tbsSystemʱ��ӡ���� */
#else
    #define TF_PM_TRACE(fmt, args...) 
    #define TF_PM_LOG_ERR(fmt, args...) COMMON_LOG_ERROR(MID_TF_PORTMAPPING, fmt, ##args)
    #define CMD_PRINT 0
#endif

#define TF_PM_TRACE_INTO_FUNC TF_PM_TRACE("##In## %s\n", __func__)

/********************************************************************
 *                   �Ժ�������ֵ������ļ򻯶���                 *
 ********************************************************************/
#define TF_PM_RET_GOTO(Ret, gotoTag, fmt, args...) \
        if ( RET_FAILED(Ret) )  \
        {\
            TF_PM_TRACE(fmt, ##args); \
            goto gotoTag; \
        }

#define TF_PM_RET_RETURN(Ret, fmt, args...)  \
        RET_RETURN(MID_TF_PORTMAPPING, Ret, fmt, ##args)

#define TF_PM_PTR_GOTO(Pointer,gotoTag, fmt, args...)  \
        POINTER_GOTO(MID_TF_PORTMAPPING, Pointer, gotoTag, fmt, ##args)

#define TF_PM_PTR_RETURN(Pointer, Ret, fmt, args...) \
        if (!Pointer)    \
        { \
            TF_PM_TRACE(fmt, ##args); \
            return Ret; \
        }

#define TF_PM_CHECK_NULL_PTR(ptr)   TF_PM_PTR_RETURN(ptr, TBS_NULL_PTR, "TBS_NULL_PTR\n")


/*******************************************************************
 *                          �ڵ�·������                           *
 *******************************************************************/
#define TF_PM_PATH_PREFIX  "InternetGatewayDevice.X_TELEFONICA_PORTMAPPING."
#define LAN_DHCP_PATH_PREFIX "InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.4."
#define DHCP_ENABLE "DhcpServerEnable"
#define TF_PM_NODE_ENABLE                   "Enable"
#define TF_PM_NODE_PROVIDER          "Provider"
#define TF_PM_NODE_CONFIGURATION     "Configuration"

//bridge info
#define TF_PM_BR2_PATH "InternetGatewayDevice.Layer2Bridging.Bridge.2"
#define TF_PM_BR3_PATH "InternetGatewayDevice.Layer2Bridging.Bridge.3"
#define TF_PM_BR_ENABLE "BridgeEnable"
#define TF_PM_BR_ASSOCIATE_LAN "X_TWSZ-COM_AssociatedLANDevice"

//filter info
#define TF_PM_FL2_PATH "InternetGatewayDevice.Layer2Bridging.Filter.2"
#define TF_PM_FL3_PATH "InternetGatewayDevice.Layer2Bridging.Filter.3"
#define TF_PM_FL4_PATH "InternetGatewayDevice.Layer2Bridging.Filter.4"
#ifdef CONFIG_TELEFONICA_COLOMBIA
#define TF_PM_FL8_37_PATH "InternetGatewayDevice.Layer2Bridging.Filter.21"
//#define TF_PM_FL10_35_PATH "InternetGatewayDevice.Layer2Bridging.Filter.10"
#else
#define TF_PM_FL8_37_PATH "InternetGatewayDevice.Layer2Bridging.Filter.9"
#define TF_PM_FL10_35_PATH "InternetGatewayDevice.Layer2Bridging.Filter.10"
#endif
#define TF_PM_FL_BRREF "FilterBridgeReference"

//lan info
#define TF_PM_DHCPS_EN_NODE "LANHostConfigManagement.DHCPServerEnable"
#define TF_PM_PPPOE_CONN "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANDSLLinkConfig"
#define TF_PM_PPPOE_PVC "DestinationAddress"
#define TF_PM_PVC_PREFIX "PVC:"

#define TF_PM_PPPOE_PATH 
#define TF_PM_BR_8_37_CONN "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.2.WANIPConnection.1.Enable"
 
/*****************************************************************
 *                     �ڵ�·��ƥ��������ʽ                    *
 *****************************************************************/
#define TF_PM_PATH_PREFIX_REGEX "^InternetGatewayDevice\\.X_TELEFONICA_PORTMAPPING\\."
#define TF_PM_NODE_REGEX(node) TF_PM_PATH_PREFIX_REGEX node "$"

/*****************************************************************
 *                       ��ӡģʽ�ַ���                          *
 *****************************************************************/
#define CALL_FUCTION_FAILED     "Call function \"%s\" failed\n"
#define SET_NODE_VAL_FAILED     "Set node \"%s\" value failed\n"
#define GET_NODE_VAL_FAILED     "Get node \"%s\" value failed\n"


/*****************************************************************
 *                       �������ֵ                          *
 *****************************************************************/
 struct TF_PM_CaseVal
{
    char szBr2En[8];
    char szBr3En[8];
    char szFilter2BrRef[8];
    char szFilter3BrRef[8];
    char szFilter4BrRef[8];
    char szFilter8_37BrRef[8];
#ifdef CONFIG_TELEFONICA_COLOMBIA
    char szBr8_37Enable[8];
    //char szFilter10_35BrRef[8];
    //char szBr10_35Enable[8];
	char szProvider[8];
#else
    char szFilter10_35BrRef[8];
#endif
};

/*****************************************************************
 *                       ����timerout��Ӧ�Ĳ���                          *
 *****************************************************************/
#define TF_PM_TIMEOUT_SET_BR3 0
#define TF_PM_TIMEOUT_SET_OTHERS 1
#define TF_PM_TIMEOUT_REBOOT 2

/******************************************************************************
 *                                      END                                   *
 ******************************************************************************/

#endif /*__TF_PORTMAPPING_H__*/
