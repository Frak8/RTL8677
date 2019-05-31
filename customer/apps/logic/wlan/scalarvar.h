#ifndef _SCALARVAR_H_
#define _SCALARVAR_H_
#include "tbstype.h"

#define SYNED 0     /* �����Ѿ���������ͬ��*/
#define DIRTY 1      /* �����ѱ��޸ģ���δͬ�� */

typedef struct tag_ST_ScalarVal
{
    char *pcNodePath;                                                        /* �ڵ�ȫ·�� */
    char *pcValidateRegx;                                                  /* У��ֵ�Ϸ��Ե�������ʽ */
    void *pValue;                                                                /* ����ֵ�����λ�� */
    unsigned int  type;                                                        /* �ڵ����ͣ�boolean/int/string */
    BOOL (*pFuncScalarValueCheck)(struct tag_ST_ScalarVal *pstScalarVal, char *pszValue);    /* �ڵ�ֵ���,�������ͨ��������ʽ��飬����Ҫʵ�ִ˺������������ͨ��������ʽ��ɣ���ΪNULL */
    int (*pFuncWlanScalarValueSet)(struct tag_ST_ScalarVal *pstScalarVal, char *pszValue);		/* set�����еĹ��Ӻ��� */
    int (*pFuncWlanScalarValueCommit)(struct tag_ST_ScalarVal *pstScalarVal);	          /* ��Ч���� */
    int iSetFlag;                                                                  /* set��Ǳ�ʾ��ǰֵ�Ƿ�set����ʼΪ0��set��Ϊ1����Ч�ָ�Ϊ0 */
}ST_ScalarVal;

int SetScalarValHook();
int CommitScalarValHook();
int CancelScalarValHook();
int InitScalarValHook();
void UninitScalarVarHook(void);
ST_ScalarVal* GetScalarByNodePath(char *pcNodePath);

/* commit call back flags */    
#define COMMIT_F_RESTART 0x1

/* added by heym, 20080817 */
int SV_SaveVarByNodePath(char *pcNodePath, int Value);


#endif
