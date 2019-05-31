/**********************************************************************
  Copyright (c), 1991-2007, T&W ELECTRONICS(SHENTHEN) Co., Ltd.

  �ļ�����: events.h
  �ļ�����: eventsά��һ���Ƚ��ȳ��Ķ��С����е���Ԫ�ذ���event��������event��commandkey��
  �޶���¼:
         1. ����: chenzhuo
            ����: 2007-11-13
            ����: �����ļ�

         2. ����: huangjidong
            ����: 2008-11-13
            ����: �ع�,ɾ�����滻һЩ�¼ܹ��ò����Ĵ���

**********************************************************************/

#ifndef _EVENTS_H_
#define _EVENTS_H_

#include "tr069.h"

/*
 * �����events������صĺ꼰�ӿں���
 */
#define BOOTSTRAP_0	        "0 BOOTSTRAP"
#define BOOT_1	            "1 BOOT"
#define PERIODIC_2          "2 PERIODIC"
#define SCHEDULED_3         "3 SCHEDULED"
#define VALUECHANGE_4       "4 VALUE CHANGE"
#define CONNECTIONREQUEST_6 "6 CONNECTION REQUEST"
#define TRANSFERCOMPLETE_7  "7 TRANSFER COMPLETE"
#define DIAGNOSTICSTATE_8	"8 DIAGNOSTICS COMPLETE"
#define REBOOT_M            "M Reboot"
#define DOWNLOAD_M          "M Download"
#define UPLOAD_M            "M Upload"
#define SCHEDULEINFORM_M    "M ScheduleInform"

#define TR069_CFG_EVENT "InternetGatewayDevice.ManagementServer.X_TWSZ-COM_Events"

#define IS_MULTI_EVENT(x) (('M'==x[0])?1:0)


int TR069_AddEvent(char* szEvent, char* szCommandKey);
int TR069_ClearEvents (void);
int TR069_ReportEvents(ST_MSG* pstMsg);
int TR069_FindEvent (char *szEvent);
int TR069_DeleteEvent(char *szEvent);

#endif

