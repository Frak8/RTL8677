/******************************************************************
 Copyright (c), 1991-2010, T&W ELECTRONICS(SHENTHEN) Co., Ltd.

 �ļ�����: os_info.h
 �ļ�����:
           OS_INFO ģ��
 �޶���¼:
        1. ����: yuanchaoqun@twsz.com
           ����:
           ����: ����

        2. ����: yuanchaoqun@twsz.com
           ����: 20100612
           ����: �ع�

******************************************************************/

#ifndef _OS_INFO_H_
#define _OS_INFO_H_

#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "tbsmsg.h"
#include "tbserror.h"
#include "cfg_api.h"
#include "pc_api.h"
#include "tbsutil.h"
#include "tbstype.h"
#include "tbsflash.h"
#include "new_msg.h"
#include "common_func.h"
#include "flash_layout.h"
#include "common_msg_handler.h"

#define OSINFO_TREE "InternetGatewayDevice.X_TELEFONICA-ES_SystemState."
#define OSINFO_TREE_Mem "InternetGatewayDevice.X_TELEFONICA-ES_SystemState.Mem."
#define OSINFO_TREE_CPU "InternetGatewayDevice.X_TELEFONICA-ES_SystemState.CPU."
#define OSINFO_TREE_Nat "InternetGatewayDevice.X_TELEFONICA-ES_SystemState.NatSessionTable."




#define OSINFO_TREE_REG "InternetGatewayDevice\\.X_TELEFONICA-ES_SystemState\\."

#define OSINFO_MemTotal              OSINFO_TREE_REG"Mem\\.MemTotal$"
#define OSINFO_MemTotalFree          OSINFO_TREE_REG"Mem\\.MemTotalFree$"
#define OSINFO_CPUTotal              OSINFO_TREE_REG"CPU\\.CPULoad$"
#define OSINFO_NatSessionTableTotal  OSINFO_TREE_REG"NatSessionTable\\.Total$"
#define OSINFO_NatSessionTableUse    OSINFO_TREE_REG"NatSessionTable\\.Use$"




int OS_INFO_ModuleInit();
int OS_INFO_ModuleDestroy();
int OS_INFO_UpdateMeminfo();
int OS_INFO_UpdateCPUinfo();
int OS_INFO_UpdateNatinfo();


int OSINFO_UpdateSN();
/******************************************************************************
*                                 END                                        *
******************************************************************************/
#endif



