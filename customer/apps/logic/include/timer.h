/**********************************************************************
 Copyright (c), 1991-2009, T&W ELECTRONICS(SHENTHEN) Co., Ltd.

 �ļ����� : timer.h
 �ļ����� : timer ģ��Ĺ���ͷ�ļ�

 �޶���¼ :
          1 ���� : ղ��
            ���� : 2008-3-17
            ���� :
            
          2 �޸� : ������
            ���� : 2009-7-6
            ���� : ��������ע��
**********************************************************************/

#ifndef  __TIMER_H__
#define  __TIMER_H__

#include <time.h>

/******************************************************************************
*                                 MACRO                                      *
******************************************************************************/
#define TMR_EXPRESSION 0
#define TMR_INTERVAL   1

#define TIMER_NAME_LENGTH_MAX 16
#define TIMER_BASE_YEAR    1900

#define TIMER_TIMEOUT_SEND_NEED_NOT 0
#define TIMER_TIMEOUT_SEND_NEED 1

#define TIMER_DELETE_NEED_NOT 0
#define TIMER_DELETE_NEED 1

#define TIMER_RELATION_ARRAY_LEN 6

#define TIMER_MID_ALL 0xffff
#define TIMER_ID_ALL 0xffffffff

/* 365 ���е�����, 365 x 24 x 60 x 60, ����34,164,000 �� */
#define TIMER_YEAR_SECOND 0x02094D20L 

/******************************************************************************
 *                                  STRUCT                                    *
 ******************************************************************************/
typedef struct tag_ST_TIMER_EXPRESSION
{
    unsigned char ucYear;  /* �� 1900 �꿪ʼ�����, ����ͨ�� 0xff */
    unsigned char ucMonth; /* �£�1 ~ 12, ����ͨ�� 0xff, 
                            * ע�⣺
                            *   ���� Linux ��ʵ�֣��·����� 0 ��ʼ�ģ�
                            *   �ο� struct tm �ṹ��Ա tm_mon ��˵���� 
                            *   tm_mon: The number of months since January, in the range 0 to 11.
                            *   ����������·� = Linux ϵͳʱ���е��·� + 1 */
    unsigned char ucDay;    /* �գ�1 ~ 31, ����ͨ�� 0xff�����µ�����ɵ������Լ����� */
    unsigned char ucHour;   /* ʱ��0 ~ 23, ����ͨ�� 0xff */
    unsigned char ucMinute; /* �֣�0 ~ 59, ����ͨ�� 0xff */
    unsigned char ucSecond; /* �룬0 ~ 59, ����ͨ�� 0xff */
    /* ���������ͬʱΪͨ������� */

    unsigned char ucLastSend;   /* ��һ���Ƿ������ʱ֪ͨ */
    unsigned char aucPad;
} ST_TIMER_EXPRESSION;

typedef struct tag_ST_TIMER_INTERVAL
{
    time_t shot;                /* ��һ�γ�ʱʱ�� */
    unsigned long ulInterval;   /* ��ʱ�����ʱ��,����Ϊ0, ���ֵΪ365���е�����,
                                   TIMER_YEAR_SECOND(0x02094D20L=34,164,000=365 x 24 x 60 x 60)*/
} ST_TIMER_INTERVAL;

/*������ʱ���Ľṹ*/ 
typedef struct tag_ST_TIMER_ENTRY
{    
    char szTimerName[TIMER_NAME_LENGTH_MAX]; /*��ʱ������,����Ϊ���ַ���*/
    unsigned long ulTimerID;  /*��ʼ��ʱΪ0, ע���Ϊ1~0xfffffffe,0xffffffff����Ϊ��ʾ���ж�ʱ��*/  
    int iFlags; /*��ʾ��ʱ������TMR_EXPRESSION �� TMR_INTERVAL*/
    union
    {
        ST_TIMER_EXPRESSION stTmrExp;
        ST_TIMER_INTERVAL stTmrInter;
    } unTmrInfo;

    unsigned short usMID; /*ģ�� ID, ����Ϊ 0x0��0xffff*/
    char acPad[2];
    struct tag_ST_TIMER_ENTRY* pstPre;  /*��ʼ��Ϊ NULL*/
    struct tag_ST_TIMER_ENTRY* pstNext; /*��ʼ��Ϊ NULL*/
} ST_TIMER_ENTRY;

/*****************************************************************************
 *                              Function Prototype                           *
 *****************************************************************************/
/**************************************************************************
����: ���һ����ʱ��
����: pstTimer  ��ʱ���ṹָ��,�ɵ����߸�������ڴ�ͳ�ʼ��
����: �ɹ� -- TBS_SUCCESS(��ע��ɹ�, �޸� pstTimer->ulTimerID ��ֵ)
      ʧ�� -- ����������
��ע: 1.���� ST_TIMER_ENTRY *pstTimer ��ʾ��ʱ����������������:
        ST_TIMER_EXPRESSION �� ST_TIMER_INTERVAL;
      2.��� ST_TIMER_EXPRESSION ���͵Ķ�ʱ��ʱ��ע��:
        �˽ṹ���·����ϰ�߱�﷨,�� 1 ��ʼ������, ������Linux �б�ʾʱ��
        ���·��Ǵ� 0 ��ʼ��, �������ȡLinuxϵͳʱ��������ST_TIMER_EXPRESSION
        �ṹ��Ҫ��tm_mon + 1����ϰ����˵����;
      3. ��APIֻ����logic�ڵ�ģ�����
***************************************************************************/
int TIMER_AddTimer(ST_TIMER_ENTRY *pstTimer);

/**************************************************************************
����: 1. ���һ��INTERVAL��ʱ��
      2. �޸��Ѿ�ע����Ķ�ʱ��
         (����usMID, pulTimerID, pcTimerNameʶ����ע����Ķ�ʱ��)
����: usMID       ģ�� MID
      pulTimerID  ע��ɹ���TimerIDָ��,ֵΪNULLʱ����¼ TimerID
      pcTimerName ��ʱ������, ����Ϊ���ַ���
      ulInterval  ��ʱ�����ʱ��, ����Ϊ0, ���ֵΪ365���е�����(34164000)
      iFlag       �Ƿ��ظ���־, 0-���ظ�����ֻ����һ��, ��0-�ظ�����     
����: �ɹ� -- TBS_SUCCESS
      ʧ�� -- ����������
��ע: ��APIֻ����logic�ڵ�ģ�����
***************************************************************************/
int TIMER_QuickAddTimer(unsigned short usMID, unsigned long *pulTimerID, 
                        const char *pcTimerName, unsigned long ulInterval, int iFlag);

/**************************************************************************
����: ɾ��һ��ģ��ע��Ķ�ʱ��      
����: usMID       ģ�� MID
      ulTimerID   ��ע��Ķ�ʱ��ID, ��ʱ��IDΪTIMER_ID_ALL(0xffffffff) 
                  ʱɾ����ģ��ע������ж�ʱ��      
����: �ɹ� -- TBS_SUCCESS
      ʧ�� -- ����������
��ע: ��APIֻ����logic�ڵ�ģ�����
***************************************************************************/
int TIMER_DelTimer(unsigned short usMID, unsigned long ulTimerID);

/******************************************************************************
 *                                      END                                   *
 ******************************************************************************/
#endif /*__TIMER_H__*/

