/**********************************************************************
 Copyright (c), 1991-2007, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 �ļ����� : pc_pcb.h
 �ļ����� : ���̿��ƿ�����������

 �޶���¼ :
          1 ���� : ��Ծ��
            ���� : 2007-8-23
            ���� :

**********************************************************************/

#ifndef __PC_PCB_H__
#define __PC_PCB_H__


#include "pc_main.h"
#include <regex.h>


#ifdef _cplusplus
    #if _cplusplus
        extern "C" {
    #endif
#endif


/******************************************************************************
 *                                 MACRO                                      *
******************************************************************************/


/* ���pcb��Ŀ���� */
#define PC_MAX_PCB_NUM  255

/* ��ָ�붨�� */
#define PC_LIST_NULL   0xff

/* ����ͷ���� */
#define PC_LIST_HEAD   0xff



/******************************************************************************
 *                                 ENUM                                       *
******************************************************************************/


#define PC_FREE_REG(pstRegData, count) \
{ \
    long ii = count; \
    while (--ii >= 0) \
    { \
        regfree(pstRegData + ii); \
    } \
    free(pstRegData); \
}


/******************************************************************************
 *                                STRUCT                                      *
******************************************************************************/


/* pcb �Ľṹ */
typedef struct
{
    char acName[PC_PROC_NAME_LEN];
    unsigned long ulInstNo;

    char   **ppcArg;                 /* ���������б� */
    char   **ppcMonCond;             /* ��������б� */
    regex_t *pstRegData;             /* ������������ʽ */
    unsigned char    ucMonCount;             /* ����������� */
    unsigned char    ucIsCComProc;           /* �Ƿ�������� */
    unsigned char    ucNeedSteady;           /* �Ƿ���Ҫ�־û����� */
    unsigned char    ucNeedMon;              /* �Ƿ���Ҫ��������Ϣ */
    long     lPid;                   /* ���̵�pid */
    long     lTty;                   /* �����Ϣͨ�ž�� */

    /* char *pcLastLine; */          /* ���һ��Ƭ��, ��ʱ���������ִ���,
                                        ��Ϊû�кõİ취�ж����һ���Ƿ����,
                                        �����û��з����ж�Ҳ���ܼ��� fflush(stdout)
                                        ����� */

    unsigned short   usComID;                /* �������������id */
    unsigned char    ucLastSignal : 7;       /* ���̸ô��˳�ǰ���յ����ź� */
    unsigned char    ucNeedRestart : 1;      /* �Ƿ���Ҫ�������� */
    unsigned char    ucPrev;                 /* ǰһ��pcb����� */

#ifdef _PC_VER_R1
    unsigned char    ucVer;                  /* �汾��Ϣ, Ϊ���ݾ���Ϣ��ʽ���� */
#endif

} ST_PC_PCB;


/******************************************************************************
 *                               GLOBAL VAR                                   *
******************************************************************************/



/******************************************************************************
 *                               FUNCTION                                     *
******************************************************************************/





/*************************************************************************
 Function:      PC_RET PC_PcbInit(void)
 Description:   Pcb����ģ���ʼ��
 Calls:         ��
 Data Accessed: g_ppstPcb,
                g_ucPcbTopFree,
                g_ucPcbTopUsed,
                g_pucPcbNextList
 Data Updated:
 Input:         ��
 Output:        ��
 Return:        0,�ɹ�;
                ����, ʧ��
 Others:
 *************************************************************************/
PC_RET PC_PcbInit(void);

/*************************************************************************
 Function:      PC_RET PC_AllocPcb(ST_PC_PCB **ppstPcb, unsigned char *pucID)
 Description:   ����һ��Pcb
 Calls:         ��
 Data Accessed:
 Data Updated:  g_ppstPcb,
                g_ucPcbTopFree,
                g_ucPcbTopUsed,
                g_pucPcbNextList
 Input:         ��
 Output:        ppstPcb, ���䵽��pcb�ĵ�ַ
                pucID, ���䵽��pcb�����
 Return:        0,�ɹ�;
                ����, ʧ��
 Others:
 *************************************************************************/
PC_RET PC_AllocPcb(ST_PC_PCB **ppstPcb, unsigned char *pucID);

/*************************************************************************
 Function:      PC_RET PC_GetPcbByProcID(unsigned char ucID, ST_PC_PCB **ppstPcb)
 Description:   ��ȡָ����ŵ�pcb
 Calls:         ��
 Data Accessed: g_ppstPcb
 Data Updated:
 Input:         ucID, ָ�������
 Output:        ppstPcb, ��ȡ����pcbָ��
 Return:        0,�ɹ�;
                ����, ʧ��
 Others:        �� ucID = PC_LIST_HEADʱ, ��ʾȡ�Ѿ�ʹ�õĵ�һ��
 *************************************************************************/
PC_RET PC_GetPcbByProcID(unsigned char ucID, ST_PC_PCB **ppstPcb);

/*************************************************************************
 Function:      PC_RET PC_FreePcb(unsigned char ucID)
 Description:   �ͷ�pcb
 Calls:         ��
 Data Accessed:
 Data Updated:  g_ppstPcb,
                g_ucPcbTopFree,
                g_ucPcbTopUsed,
                g_pucPcbNextList
 Input:         ucID, ָ�������
 Output:        ��
 Return:        0,�ɹ�;
                ����, ʧ��
 Others:
 *************************************************************************/
PC_RET PC_FreePcb(unsigned char ucID);

/*************************************************************************
 Function:      PC_RET PC_GetPcbByPid(long lPid, ST_PC_PCB **ppstPcb)
 Description:   ͨ��pid��ȡpcb
 Calls:         ��
 Data Accessed: g_ppstPcb,
                g_ucPcbTopFree,
                g_ucPcbTopUsed,
                g_pucPcbNextList
 Data Updated:
 Input:         lPid, ָ����pid
 Output:        ppstPcb, ��ȡ����pcbָ��
 Return:        0,�ɹ�;
                ����, ʧ��
 Others:
 *************************************************************************/
PC_RET PC_GetPcbByPid(long lPid, ST_PC_PCB **ppstPcb, unsigned char *pucID);

PC_RET PC_GetPcbByName(const char *pcName, unsigned long ulInstNo,
                       ST_PC_PCB **ppstPcb, unsigned char *pucID);


/*************************************************************************
 Function:      PC_RET PC_GetNextPcb(unsigned char ucCurID,
                           ST_PC_PCB **ppstPcb, unsigned char *pucID)
 Description:   ��ȡ��һ��pcb, ���Ƕ����ṩ�ı�����������ʹ�õ�pcb�Ľӿ�
 Calls:         ��
 Data Accessed: g_ppstPcb,
                g_ucPcbTopFree,
                g_ucPcbTopUsed,
                g_pucPcbNextList
 Data Updated:
 Input:         ucCurID, ��ǰ��pcb�����
 Output:        ppstPcb, ��ȡ������һ��pcbָ��
                pucID, ��ȡ������һ��pcb�����
 Return:        0,�ɹ�;
                ����, ʧ��
 Others:        ��ucCurID����PC_LIST_HEADʱ,��ʾ��ȡ��һ��
 *************************************************************************/
PC_RET PC_GetNextPcb(unsigned char ucCurID, ST_PC_PCB **ppstPcb,
                                          unsigned char *pucID);


/*************************************************************************
 Function:      PC_RET PC_PcbFinal(void)
 Description:   Pcb����ģ��ȥ��ʼ��
 Calls:         ��
 Data Accessed: g_ppstPcb,
                g_ucPcbTopFree,
                g_ucPcbTopUsed,
                g_pucPcbNextList
 Data Updated:
 Input:         ��
 Output:        ��
 Return:        0,�ɹ�;
                ����, ʧ��
 Others:
 *************************************************************************/
PC_RET PC_PcbFinal(void);


/*************************************************************************
 Function:      PC_RET PC_ChildFinal(void)
 Description:   Pcb�����ӽ����˳�, �ú�������PC�յ� TER �� INT �źź�Ĵ�����
 Calls:         ��
 Data Accessed: g_ppstPcb,
 Data Updated:
 Input:         ��
 Output:        ��
 Return:        0,�ɹ�;
                ����, ʧ��
 Others:
 *************************************************************************/
PC_RET PC_ChildFinal(int sig);


/******************************************************************************
 *                                 END                                        *
******************************************************************************/


#ifdef _cplusplus
    #if _cplusplus
        }
    #endif
#endif


#endif /* __PC_PCB_H__ */

