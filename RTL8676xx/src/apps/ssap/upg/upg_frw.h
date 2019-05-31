/**********************************************************************
 Copyright (c), 1991-2007, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 �ļ����� : upg_frw.h
 �ļ����� : upg_frw ��ͷ�ļ�
 �����б� :
 �޶���¼ :
          1 ���� : ���
            ���� : 2008-4-14
            ���� :

          2 ���� : ������
            ���� : 2009-2-23
            ���� : ���ӷ��ش�����Ĺ������ֶΣ��Խ�����ش�
            	   ��������Ϣ����Ӧ������

          3 ���� : ������
            ���� : 2009-07-03
            ���� : ����ע��

**********************************************************************/
#ifndef __UPGFRW_H__
#define __UPGFRW_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "common.h"
#include "flash_layout.h"


/******************************************************************************
*                                 MACRO
******************************************************************************/

/* ���Կ��غ� */
#define UPG_DEBUG   1

#ifdef UPG_DEBUG
#define UPG_LogTrace(fmt, args...)      UPG_Log("%s:%4d %20s: " fmt, __FILE__, __LINE__, __func__, ##args)
#else
#define UPG_LogTrace(fmt, args...)      {do { ; } while(0);}
#endif

#ifdef CONFIG_DUAL_IMAGE
#ifndef CONFIG_DOUBLE_BACKUP
#define CONFIG_DOUBLE_BACKUP
#endif
#endif


/* ��С���� */
#define BLOCKSIZE               1024*4
#define MAX_STR_LEN             256    /* �ַ�����󳤶�*/

#define FIRMWARE_FILE           "/var/image.img"
#define LOGFILE                 "/var/upg_log.txt"

/* �����ļ�·�� */
#define UPDATE_IMG              "/var/update_img"
#define UPDATE_BIN              "/var/update_bin"
#define UPG_ITEM_UPGRADING      "upgrading"


/* procϵͳ����·�� */
#define IMG_SPACE               "/proc/llconfig/img_space"
#define FLASH_SIZE              "/proc/llconfig/flash_size"

/* �����־�õ��Ľڵ� */
#define CFG_NODE_LEN 256
#define CLEAN_LOG_TRIGGER "InternetGatewayDevice.X_TWSZ-COM_Logger.LogClearTrigger"

/******************************************************************************
*                                ENUM
******************************************************************************/
/* �������� */
enum
{
	NONE,
 	IMG_WRITE,
 	CONFIG_READ,
 	CONFIG_WRITE,
 	CONFIG_BAKUP
};


/* �ļ����� */
enum
{
        TYPE_BINARY,
        TYPE_TEXT
};

/* �ɱ�������� */
typedef char *va_list;


/******************************************************************************
*                                STRUCT
******************************************************************************/



/******************************************************************************
*                                FUNCTION
******************************************************************************/
/* API Functions */
int UPG_UpdateImg(char * filename, BOOL bClearCfg, BOOL bClearLog, unsigned short sMID);
int UPG_UpdateConfig(char * filename, unsigned short sMID);
int UPG_BackupConfig(char * filename,unsigned short sMID);
int UPG_ClearConfig(void);

int UPG_ModuleInit(void);
int UPG_ModuleDestory(void);

/* Utils Funtions */
int UPG_UpgradeSystem(void);
int UPG_SendSimpleMsg(unsigned short usDstMid, unsigned short usMsgType);
int UPG_SendSetMsg(int nValNum, char aszName[][CFG_NODE_LEN], char aszVal[][CFG_NODE_LEN]);
int UPG_ProcessAckMsg(void);
int UPG_ReplyStateToCaller(unsigned long ulResult, unsigned short sMID);

int UPG_CheckValid(char *filename);
int UPG_CheckSum(char * filename);
void UPG_Log(char *szFmt, ...);

#endif

