/********************************************************************
  Copyright (c), 1991-2010, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
  
  �ļ����ƣ�dlna.h
  �ļ�������DLNAģ��ͷ�ļ�
  �޶���¼��
         1. ���ߣ�chenkai
            ���ڣ�2011-03-14
            ���ݣ������ļ�
********************************************************************/

#ifndef __DLNA_H__
#define __DLNA_H__

#include "common_msg_handler.h"
#include "warnlog.h"


/******************************************************************************
*                                 MACRO                                       *
******************************************************************************/
/* ��������*/

#define BASE_PATH_DLNA "^InternetGatewayDevice\\.X_TWSZ-COM_DLNA\\."
#define PATH_DLNA      "InternetGatewayDevice.X_TWSZ-COM_DLNA."


//#define NSM_DEBUG         /*���Կ���*/

#ifdef  DLNA_DEBUG
#define DLNA_TRACE(fmt, args...)        TBS_TRACE(MID_DLNA, fmt, ##args)
#else
#define DLNA_TRACE(fmt, args...)
#endif

/* ������־ */
#ifdef  DLNA_DEBUG
#define DLNA_ERROR(fmt, args...)        printf("DLNA:__%d__"fmt, __LINE__, ##args)
#else
#define DLNA_ERROR(fmt, args...)        COMMON_LOG_ERROR(MID_DLNA, fmt, ##args)
#endif


/******************************************************************************
*                                STRUCT                                      *
******************************************************************************/


/******************************************************************************
*                               FUNCTION                                     *
******************************************************************************/


int DLNA_ModuleInit(void);
int DLNA_ModuleDestroy(void);

/******************************************************************************
*                                 END                                        *
******************************************************************************/

#endif

