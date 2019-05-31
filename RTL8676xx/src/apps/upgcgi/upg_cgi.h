/**********************************************************************
 Copyright (c), 1991-2007, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 �ļ����� : upg_cgi.h
 �ļ����� : upgcgi ģ��ͷ�ļ�

 �����б� :

 �޶���¼ :
          1 ���� : ���
            ���� : 2008-04-10
            ���� : �����ļ�

          2 ���� : ������
            ���� : 2008-07-03
            ���� : ����ע��

**********************************************************************/

#ifndef _UPGCGI_H
#define _UPGCGI_H

#include "common.h"


/******************************************************************************
*                                 MACRO
******************************************************************************/

/* ���Կ��� */
//#define UPGCGI_DEBUG   1

#ifdef UPGCGI_DEBUG
#define UPGCGI_LogTrace(fmt, args...)      UPGCGI_Log("%s:%4d %20s: " fmt, __FILE__, __LINE__, __func__, ##args)
#else
#define UPGCGI_LogTrace(fmt, args...)      {do { ; } while(0);}
#endif

/* �ļ������� */
#define FIRM_FILE_NAME 	        "/var/image.img"
#define AUTH_AS_FILE_NAME       "/var/Wlan_Auth_AS.cer"
#define AUTH_AP_FILE_NAME       "/var/Wlan_Auth_AP.cer"
#define AUTH_TR069_FILE_NAME    "/var/CA_root.pem"
#define AUTH_TR069_CA_PATH      "InternetGatewayDevice.ManagementServer.X_TWSZ-COM_CAFilePath"
#define AUTH_TR069_CA_NAME      "InternetGatewayDevice.ManagementServer.X_TWSZ-COM_CAname"
#define AUTH_TR069_CA_FILE      "InternetGatewayDevice.ManagementServer.X_TWSZ-COM_CAFile"

#define CFG_NODE_LEN            256
#define CFG_NODE_VALUE_LEN            3*2048

#define LOGFILE     		    "/var/upgcgi_log.txt"
#define UPGRADINGPAGE		    "/usr/www/html/page/upgrading.html"
#define RESTARTINGPAGE          "/usr/www/html/page/restarting.html"
#define ERROR_RESTARTINGPAGE          "/usr/www/html/page/error_restarting.html"
#define CERTFILE                "/usr/www/html/page/certificate_ca.html"
#define FACTORY_MODE_PAGE            "/usr/www/factory_mode.html"

/* �����ļ��� */
#define UPGCGI_CONFIG_FILENAME      "config.xml"
/* ��־�ļ��� */
#define UPGCGI_SYSEVENT_FILENAME    "sysevent.txt"

/*����ǽ��־�ļ���*/
#define UPGCGI_FIREWALL_FILENAME "firewalllog.log"
/* ��־�ļ�·�� */
#define UPGCGI_SYSEVENT_PATH        "/var/log/" UPGCGI_SYSEVENT_FILENAME
/*����ǽ��־·��*/
#define UPGCGI_FIREWALL_PATH       "/var/log/"UPGCGI_FIREWALL_FILENAME


/* ��֤ģ����ض��� */
#define WEB_SSID_TOKEN          "sessionid"
#define WEB_SPLIT_CHAR_SET      "&;"

/* webshell����Կ */
#define UPGCGI_KEY              "twmode"

/* -c action */
#define IMG_WRITE			    "1"
#define CONFIG_READ		        "2"
#define CONFIG_WRITE		    "3"
#define CONFIG_BAKUP		    "4"

/* ��С���� */
#define PAGE_SIZE			    1024*10
#define CONFIG_SIZE			    1024

#define MAX_LINE			    80
#define MAX_PATH			    1024
#define MAX_MSG_SIZE		    256

/* �ɱ�������� */
typedef char *va_list;

/******************************************************************************
*                                ENUM
******************************************************************************/
/* �ļ����� */
enum
{
  	TYPE_BINARY,
	TYPE_TEXT
};

/* web���ݱ��뷽ʽ */
enum
{
    CONTENT_TYPE_NONE,
	CONTENT_TYPE_URLENCODE,
  	CONTENT_TYPE_FORMDATA
};

/* ��Ӧ��Ϣ�ļ���� */
enum en_RESPONSE_STATE
{
    EN_RESPONSE_SUCCESS,    /* ���OK */
    EN_RESPONSE_ERROR,      /* ������� */
    EN_RESPONSE_TIMEOUT,    /* �ȴ���ʱ */
    EN_RESPONSE_UNREPECT,   /* ��Ԥ����Ϣ */

    EN_RESPONSE_BUTT
};

/* ������ */
enum
{
    BEGIN_STATUS = -1,

    UPG_SUCCESS = 0,            //0
    UPG_GENERAL_ERROR,
    INIT_MODULE_ERROR,
    DESTROY_MODULE_ERROR,
    REQUEST_METHOD_ERROR,
    CONTENT_TYPE_ERROR,         //5
    CONTENT_LENGTH_ERROR,
    INVALID_VARIABLE_ERROR,
    NO_VARIABLE_ERROR,
    MIN_CONTENT_LENGTH_ERROR,
    MAX_CONTENT_LENGTH_ERROR,   //10
    FOPEN_LOG_ERROR,
    FOPEN_FW_ERROR,
    OPERATE_FW_ERROR,
    FILE_TYPE_ERROR,
    FILE_SIZE_ERROR,            //15
    FILE_PRODUCT_ERROR,
    FILE_CRC_ERROR,
    FILE_UPLOAD_ERROR,
    AUTH_ERROR,
    END_STATUS                  //20
};

/******************************************************************************
*                                STRUCT
******************************************************************************/



/******************************************************************************
*                                FUNCTION
******************************************************************************/
#define UPGCGI_SUCCEED(ret) (ret == UPG_SUCCESS)
#define UPGCGI_FAILED(ret)  (ret != UPG_SUCCESS)
#define MIN(a,b)            ((a)<(b))?(a):(b)


/* in upg_parse.c */
BOOL get_line_from_post(char *sPtr, int maxLen);
void get_var_name_from_post(char *sPtr, int maxLen);
void get_file_name_from_post(char *sPtr, int maxLen);
void get_value_from_post(char *sPtr, int maxLen, const char *pVarName);
int  get_content_type(char *sPtr, int maxLen);
void remove_trailing_CRLF(char *sPtr, int length);
void remove_CRLF_and_save_buffer(char *sPtr, long *nSize, char *pSavePtr);
unsigned char Hex2Uchar(const char *szHex);
void UrlUnEsc(char *szUrl);


void UPGCGI_RemoveAllFile();
void UPGCGI_FlushBuffer(void);
void UPGCGI_Header(int type, const char *pszFile);
void UPGCGI_SendMsgPage(char *data);
void UPGCGI_SendMsgText(char *data);
void UPGCGI_SendHtmlPage(char * pagename);
void UPGCGI_SendReturnPage(char *data);
void UPGCGI_SendClosePage(char *data);


int UPGCGI_WebShell();
int UPGCGI_ModifyMac();
int UPGCGI_UploadAuthFile();
int UPGCGI_UploadTR069AuthFile();

int UPGCGI_SendSetMsg(int nValNum, char aszName[][CFG_NODE_LEN], char aszVal[][CFG_NODE_VALUE_LEN]);
int UPGCGI_ProcessAckMsg();

int UPGCGI_UploadSingleFile(char * filename, unsigned long minSizeLimit , unsigned long maxSizeLimit);
int UPGCGI_DownloadSysEvtLogFile(BOOL bDownOrShow);
int UPGCGI_DownloadFile(void);
int UPGCGI_DoConfigUpgrade(void);
int UPGCGI_DoFirmwareUpgrade(void);
void handle_read_timeout( int sig );
void UPGCGI_DispPostContent();
void UPGCGI_Log(char *szFmt, ...);
int UPGCGI_CheckAuth(char *pszSessionID);
int UPGCGI_SetCertFile();
int UPGCGI_RedirhtmlPage(int flag);


/******************************************************************************
*                                VARIABLES
******************************************************************************/
extern long nPostDataCount;
extern char sDivider[];

#endif /* _UPGCGI_H */

