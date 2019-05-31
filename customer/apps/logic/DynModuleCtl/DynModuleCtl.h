/******************************************************************
 Copyright (c), 1991-2009, T&W ELECTRONICS(SHENTHEN) Co., Ltd.

 �ļ�����: DynModuleCtl.c
 �ļ�����:

 �޶���¼:
        1. ����: ŷ����, oujunhe@twsz.com
           ����: 2009��05��31��
           ����: Create

******************************************************************/

#ifndef __DYN_MODULE_CTRL_H__
#define __DYN_MODULE_CTRL_H__

#include "common_msg_handler.h"
#include "warnlog.h"


/******************************************************************************
*                                 MACRO                                      *
******************************************************************************/
#define NO_FREE_ITEM -1
#define ITEM_NOT_EXIST -1
#define IN_PARAMETER_POINTER_NULL -2
#define GET_NODE_VALUE_FAILE    -3
#define UNEXCEPT_RESULT    -4
#define FIND_MSG_NO_ITEM -5
#define DYN_CTRL_INFO_LIST_FULL -6
#define MODULE_NO_REGISTERED -7
#define OPEN_DYNAMIC_LIBRARY_FAIL -8
#define CLOSE_DYNAMIC_LIBRARY_FAIL -9
#define LIBRARY_NOT_LOADED -10
#define LIBRARY_LOADED -11

#define MAX_CARE 20


#define DynModuleCtl_TRACE(fmt, args...)        TBS_TRACE(MID_DYN_MODULE_CTRL, fmt, ##args)


/******************************************************************************
*                               STRUCT                                     *
******************************************************************************/


struct DynCtrlInfo
{
    unsigned short usMID;
    unsigned short ucIsModuleAlive;
    int iEnable;
    int iTimerScanCount;
    void *pDynLinkHandle;
    char szModuleName[64];
    char szEnablePath[256];
};
/*
usMID: ģ��MID
ucIsModuleAlive:
        0: δ����
        1:�Ѿ�����
        ����1:�޷�����
        Ĭ��: 0
iEnable:���ڼ�¼ģ����enabled�Ļ���disabled��
iTimerScanCount����ʱ��ɨ��Ĵ���������SET��Ϣʱ��0����ʱ��ɨ��ʱ��1��
        ����5ʱģ��ᱻж��
pDynLinkHandle:
        ��̬���ӿ�ľ��
szModuleName:ģ�����֣����Թ���ģ��Ķ�̬��������ʼ����������
        ���ٺ�����
szEnablePath:
        enable�ڵ��·��
*/


/******************************************************************************
*                               FUNCTION                                     *
******************************************************************************/

int	        DynModuleCtl_RequestDynCtrl(struct DynCtrlInfo* pstDynCtrlInfo);
int         DynModuleCtl_LoadModule(unsigned short usMID);
int         DynModuleCtl_UnloadModule(unsigned short usMID);

int         DynModuleCtl_GetModuleState(unsigned short usMID);
int         DynModuleCtl_GetEnablePath(unsigned short usMID, char* pBuf, int iBufSize);
int         DynModuleCtl_SetModuleState(unsigned short usMID, unsigned short usNewState);
void*       DynModuleCtl_GetModuleInitFun(unsigned short usMID);
void*       DynModuleCtl_GetModuleDestroyFun(unsigned short usMID);
int         DynModuleCtl_SetModuleEnable(unsigned short usMID, int iNewEnableState);
int         DynModuleCtl_TimerScanCountIncrease(unsigned short usMID);
int         DynModuleCtl_TimerScanCountInit(unsigned short usMID);
int         DynModuleCtl_GetTimerScanCount(unsigned short usMID);
int         DynModuleCtl_GetModuleEnableState(unsigned short usMID);
void        DynModuleCtl_DealModuleDynCtrl(void);

int         DynModuleCtl_ModuleInit(void);
int         DynModuleCtl_ModuleDestroy(void);
void        DynModuleCtl_MSGHandle(ST_MSG * pstMSG);


/******************************************************************************
*                                 END                                        *
******************************************************************************/

#endif

