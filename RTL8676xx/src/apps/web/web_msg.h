/**********************************************************************
 Copyright (c), 1991-2007, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 �ļ����� : web_msg.h
 �ļ����� : WEB ģ����Ϣ����ͷ�ļ�

 �����б� :


 �޶���¼ :
          1 ���� : ����Ⱥ
            ���� : 2007-8-21
            ���� :

**********************************************************************/
#ifndef __WEB_MSG_H__
#define __WEB_MSG_H__

#include "web_pub.h"

/******************************************************************************
 *                                 MACRO                                      *
 ******************************************************************************/
#ifdef WEBPROC_DEBUG
#define WEB_WAIT_MSG_TIME      60       // �ȴ���Ϣ��ʱ�� 3s
#else
#define WEB_WAIT_MSG_TIME      60       // �ȴ���Ϣ��ʱ�� 5s
#endif

/******************************************************************************
 *                                 ENUM                                       *
 ******************************************************************************/




/******************************************************************************
 *                                STRUCT                                      *
 ******************************************************************************/



/******************************************************************************
 *                               FUNCTION                                     *
 ******************************************************************************/

int WEB_SendCmd(char *szDstMid, unsigned short usType, PCSTR szName, PCSTR szValue, BOOL bSerialRand);
int WEB_SendPostMsg(ST_MSG* pstMsg);
int WEB_SendAgtPostMsg(ST_MSG* pstMsg);
int WEB_MsgObjNameGet(char *szName, char  szIndexMap[WEB_MAX_OBJ_NUM][12]);
//int WEB_MsgGetLeafVal(char *szPathName, char *szValue); wei changed it ====>
int WEB_MsgGetLeafVal(char *szPathName, char **p_szValue, unsigned short usDestModID);
int WEB_MsgGetObjVal(char *szPathName,  // ·������
                     char *szIndex,     // ����
                     char szValMap[WEB_MAX_VAL_PER_OBJ][WEB_MAX_OBJ_LEN],   // ����ȡ��ֵ����
                     int nValNum,
                     char szValRet[WEB_MAX_VAL_PER_OBJ][WEB_MAX_OBJ_LEN]   // ����ȡ��ֵ���
                     );

int WEB_GetResponse();
/*  ��������: ��ȡһ����Ϣ����, ��������ֹλ��                             */
ST_WEB_VALUE* WEB_MsgGetVal(PCSTR *pszBuffer, unsigned short usLen, int nCount);
/*  ��������: ����Ϣ���������Ϣֵ�б�                                     */
ST_WEB_VALUE* WEB_MsgParseVal(PCSTR pBuffer,  unsigned short usLen);
/*  ��������: ��ȡ���һ����Ϣ������                                       */
//unsigned short WEB_MsgGetRecvType(); wei changed it ====>
unsigned short WEB_MsgGetRecvType(unsigned short usOrigType);
/*  ��������: ��ȡ���һ����Ϣ����Ϣ��                                     */
PSTR WEB_MsgGetRecvBody();
/*  ��������: ��ȡ���һ����Ϣ����Ϣ�峤��                                 */
unsigned short WEB_MsgGetRecvLen();
/*  ��������: ���ٵ�����Ϣֵ                                               */
void WEB_MsgFreeVal(ST_WEB_VALUE *pstVal);
/*  ��������: ������Ϣֵ����                                               */
void WEB_MsgFreeVals(ST_WEB_VALUE *pstVal);



/******************************************************************************
 *                                 END                                        *
 ******************************************************************************/







/******************************************************************************
 *                                 END                                        *
 ******************************************************************************/


#endif /* __WEB_MSG_H__ */


