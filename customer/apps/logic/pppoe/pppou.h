/******************************************************************************
 Copyright (c), 1991-2008, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 �ļ�����: ppp_usb.h
 �ļ�����: PPP_USB���� ͷ�ļ�
 �޶���¼:
        1. ����: ����
           ����: 2009-7-20
           ����: �����ļ�
******************************************************************************/
#ifndef _PPPOU_H
#define _PPPOU_H

/******************************************************************************
*                                 DEBUG                                      *
******************************************************************************/

/******************************************************************************
*                                 MACRO                                      *
******************************************************************************/
#ifdef CONFIG_APP_LOGIC_PPPOU
#define MAX_USB_DEV_LEN             20
#define MAX_APN_LEN                 20
#define MAX_DIAL_NUM_LEN            20
#define MAX_COUNTRY_LEN             20
#define MAX_CONN_TYPE_LEN           20
#define MAX_CFG_LEN                 512
#define MAXIDLEDELAYTIME			600

#define PPPOU_CHAT_FILE             "/var/ppp/ppp-on-chat"

/* �������� */
#define NODE_APN                    "X_TWSZ-COM_Apn"
/* ���������ĺ��� */
#define NODE_DIAL_NUMBER            "X_TWSZ-COM_DialNumber"
/* ���� */
#define NODE_COUNTRY                "X_TWSZ-COM_Country"
/* ISP�ṩ�� */
#define NODE_ISP                    "X_TWSZ-COM_Isp"
/* ģ�鲨���� */
#define NODE_MODEM_BAUD             "X_TWSZ-COM_ModemBaud"
/* ��Ʒ�ͺ� */
#define NODE_MODEM_PRODUCT          "X_TWSZ-COM_Product"
/* ���ݵ��ӳ�ʱ�� */
#define NODE_IDLE_DELAY_TIME		"IdleDelayTime"

#define PPPOE_SET_APN           PPPOE_SET_BASE_PATH     NODE_APN            /*add by zhh*/
#define PPPOE_SET_DIAL_NUMBER   PPPOE_SET_BASE_PATH     NODE_DIAL_NUMBER    /*add by zhh*/
#define PPPOE_SET_COUNTRY       PPPOE_SET_BASE_PATH     NODE_COUNTRY        /*add by zhh*/
#define PPPOE_SET_ISP           PPPOE_SET_BASE_PATH     NODE_ISP            /*add by zhh*/
#define PPPOE_SET_MODEM_BAUD    PPPOE_SET_BASE_PATH     NODE_MODEM_BAUD     /*add by zhh*/
#define PPPOE_SET_MODEM_PRODUCT PPPOE_SET_BASE_PATH     NODE_MODEM_PRODUCT  /*add by zhh*/
#define PPPOE_SET_IDLE_DELAY_TIME	PPPOE_SET_BASE_PATH     NODE_IDLE_DELAY_TIME  /*add by zhh*/

/* COMMIT��Ϣƥ��·�� */
#define COMMIT_NODE_PPPUSB      PPPOE_SET_APN"|"\
                                PPPOE_SET_DIAL_NUMBER"|"\
                                PPPOE_SET_COUNTRY"|"\
                                PPPOE_SET_ISP"|"\
                                PPPOE_SET_MODEM_BAUD"|"\
                                PPPOE_SET_MODEM_PRODUCT
/******************************************************************************
*                                STRUCT                                      *
******************************************************************************/
struct tag_ST_PPPOE_Instance;

/******************************************************************************
*                               FUNCTION                                     *
******************************************************************************/
int PPPOU_NodeInit(void);
int PPPOU_IsUsbModem(struct tag_ST_PPPOE_Instance *PPPInstance);
int PPPOU_ProcCtrl(struct tag_ST_PPPOE_Instance *PPPInstance, char *pszExecInfo[]);
int PPPOU_ModemInfoSet(struct tag_ST_PPPOE_Instance *PPPInstance );
int PPPOU_IsUsbModem(struct tag_ST_PPPOE_Instance *PPPInstance);
int PPPOU_ConnIsOverload(char *pszPath, char *pszValue);
int PPPOU_CommitProcess(char *pszPath, char *pszValue);
int My_MaxStrCpy(char *dest, char *src, int maxlen);
int CheckDialNumber(const char *pszDialNumber);
int CheckApn(const char *pszApn);
int CheckModemBaud(const char *pszModemBaud);
int CheckIdleDelayTime(const char *pszIdleDelayTime);
int PPPOU_MsgUsbModemChgHandler(ST_MSG *pstMsg);

#endif
#endif
/******************************************************************************
*                                 END                                        *
******************************************************************************/

