/**********************************************************************
 Copyright (c), 1991-2007, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 �ļ����� : pc_mon.h
 �ļ����� : ���̼�ع���ʵ�ֵ���ض��������


 �����б� :


 �޶���¼ :
          1 ���� : ��Ծ��
            ���� : 2007-8-23
            ���� :

**********************************************************************/


#ifndef __PC_MON_H__
#define __PC_MON_H__


#include "pc_main.h"

#ifdef _cplusplus
    #if _cplusplus
        extern "C" {
    #endif
#endif



/******************************************************************************
 *                                 MACRO                                      *
******************************************************************************/

/* �����ļ�һ����󳤶ȶ */
#define PC_FILE_LINE_MAX_LEN   512

/* ��������������� */
#define PC_START_MAX_ARGC      20

/* ������������󳤶� */
#define PC_PROC_MAX_OUT_LEN    1024

/* ƥ�䵽����Ϣ����󳤶�( ������Ϣͷ) */
#define PC_MSG_MAX_OUT_LEN      5*1024




/******************************************************************************
 *                                 ENUM                                       *
******************************************************************************/



/******************************************************************************
 *                                STRUCT                                      *
******************************************************************************/



/******************************************************************************
 *                               GLOBAL VAR                                   *
******************************************************************************/




/******************************************************************************
 *                               FUNCTION                                     *
******************************************************************************/



/*************************************************************************
 Function:      PC_RET PC_MonInit(void)
 Description:   ��ع���ģ���ʼ������
 Calls:         PC_StartKeyProc
 Data Accessed:
 Data Updated:
 Input:         ��
 Output:        ��
 Return:        0,�ɹ�;
                ����, ʧ��
 Others:
 *************************************************************************/
PC_RET PC_MonInit(const char *pcCfgFile);

/*************************************************************************
 Function:      PC_RET PC_StartKeyProc(char *pcCfgFile)
 Description:   ����ϵͳ�ؼ�����
 Calls:         PC_AllocPcb,
                PC_GetNextWord,
                PC_GetStartInfo
 Data Accessed:
 Data Updated:
 Input:         pcCfgFile, �����ļ���, ��·��
 Output:        ��
 Return:        0,�ɹ�;
                ����, ʧ��
 Others:
 *************************************************************************/
PC_RET PC_StartKeyProc(const char *pcCfgFile);


/*************************************************************************
 Function:      PC_RET PC_OutMonProc(void)
 Description:   �����Ϣ��ش�����
 Calls:         PC_GetNextPcb
                MSG_SendMessage
 Data Accessed:
 Data Updated:
 Input:         ��
 Output:        ��
 Return:        0,�ɹ�;
                ����, ʧ��
 Others:
 *************************************************************************/
PC_RET PC_OutMonProc(void);


/*************************************************************************
 Function:      PC_RET PC_StateMonProc(void)
 Description:   ����״̬��ش�����
 Calls:         PC_GetPcbByPid
                MSG_SendMessage
                forkpty
 Data Accessed:
 Data Updated:
 Input:         pcCfgFile, ϵͳ�����ļ�
 Output:        ��
 Return:        0,�ɹ�;
                ����, ʧ��
 Others:
 *************************************************************************/
PC_RET PC_StateMonProc(void);


PC_RET PC_MsgFunc(ST_PC_PROC_CTRL_INFO *pstPcMsg, unsigned short usComID,
                  unsigned char *pucProcID);


/******************************************************************************
 *                                 END                                        *
******************************************************************************/

#ifdef _PC_VER_R1

#include "pc_pcb.h"

PC_RET PC_StartProcOld(
    unsigned short           usComID,
    unsigned char            ucNeedSteady,
    char            *apcArgVals[],
    unsigned char            ucMonCount,
    char            *apcMonVals[],
    unsigned char           *pucProcID);


PC_RET PC_SendProcSignalOld(
    unsigned short           usComID,
    unsigned char            ucProcID,
    unsigned char            ucSignal,
    unsigned char            ucNeedRestart,
    unsigned char            ucNeedSteady,
    char                    *apcArgVals[],
    unsigned char            ucMonCount,
    char                    *apcMonVals[]);

PC_RET PC_StateMonProcOld(ST_PC_PCB *pstPcb, unsigned char ucProcID);


PC_RET PC_OutMonProcOld(const ST_PC_PCB *pstPcb, unsigned char ucProcID);

#endif



#ifdef _cplusplus
    #if _cplusplus
        }
    #endif
#endif


#endif /* __PC_MON_H__ */



