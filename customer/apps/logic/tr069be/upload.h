/**********************************************************************
  Copyright (c), 1991-2007, T&W ELECTRONICS(SHENTHEN) Co., Ltd.

 �ļ�����: upload.h
 �ļ�����:

 �޶���¼:
        1. ����: chenzhuo
           ����: 2008-05-19
           ����: ��������

        2. ����: huangjidong
           ����: 2008-11-13
           ����: TR069BE�ع����ע��

**********************************************************************/
#ifndef _UPLOAD_H_
#define _UPLOAD_H_

#include "common.h"


enum EN_TR069_UpLoadState {ulIdle, ulProcess, ulDelay, ulInformWait };
#define DOWNLOADDIAG_PATH_DOWNLOADURL "InternetGatewayDevice.DownloadDiagnostics.DownloadURL"
/*
 * upload.c �к�������
 */

int TR069_UploadMethod(ST_MSG *pstMsg) ;
int TR069_DelayedUpload(ST_MSG *pstMsg, const int iFlag ) ;
int TR069_FirmwareUpload(char *pcBaseUrl, char *pcUserName, char *pcPassword) ;
int TR069_UploadDone(ST_MSG *pstMsg) ;
int TR069_PrintUploadMsg(ST_MSG *pstMsg) ;
int TR069_SendUploadResponse (ST_MSG *pstMsg, int iStatus);
int TR069_GetCfgFileAddress();
int TR069_GetCfgFile(ST_MSG* pstMsg);
int TR069_UploadProcCtrl(const char *pcName, unsigned long ulInstNo, unsigned char ucOpt);


#endif

