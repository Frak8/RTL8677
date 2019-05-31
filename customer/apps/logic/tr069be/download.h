/**********************************************************************
  Copyright (c), 1991-2007, T&W ELECTRONICS(SHENTHEN) Co., Ltd.

  �ļ�����: download.h
  �ļ�����: TR069ģ����������ģ��

  �޶���¼:
         1. ����: chenzhuo
            ����: 2007-11-13
            ����: �����ļ�

         2. ����: huangjidong
            ����: 2008-11-13
            ����: �ع�

**********************************************************************/
#ifndef _DOWNLOAD_H_
#define _DOWNLOAD_H_

#include "common.h"
#define NEW3G_DRIVER_NODE	"InternetGatewayDevice.WANDevice.2.X_TWSZ-COM_WAN3GInterfaceConfig.driver"
#define DOWNLOADDIAG_PATH_DOWNLOADURL "InternetGatewayDevice.DownloadDiagnostics.DownloadURL"
enum EN_TR069_DownLoadState {dlIdle, dlProcess, dlDelay, dlUpgWait };

/*
 * download.c �к�������
 */
char* TR069_GetArgument(char * pcArguments, int iNumber);
char* TR069_MakeUrl(char *pcBaseUrl, char *pcUserName, char *pcPassword);

int TR069_DownloadMethod(ST_MSG *pstMsg) ;
int TR069_DelayedDownload(ST_MSG *pstMsg, const int iFlag ) ;
int TR069_FirmwareDownload(char *pcBaseUrl, char *pcUserName, char *pcPassword) ;
int TR069_DownloadDone(ST_MSG *pstMsg) ;
int TR069_PrintDownloadMsg(ST_MSG *pstMsg) ;
int TR069_SendDownloadResponse (ST_MSG *pstMsg, int iStatus);
int TR069_DownloadProcCtrl(const char *pcName, unsigned long ulInstNo, unsigned char ucOpt);


#endif


