/**********************************************************************
 Copyright (c), 1991-2007, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 �ļ����� : om_lib.c
 �ļ����� : ����OM�����ṩ�Ŀ��ļ�


 �����б� :


 �޶���¼ :
          1 ���� : ����Ⱥ
            ���� : 2007-9-29
            ���� :

**********************************************************************/

#include "../include/om.h"

/******************************************************************************
 *                                GLOBAL                                      *
 ******************************************************************************/



/******************************************************************************
 *                               FUNCTION                                     *
 ******************************************************************************/


/*=========================================================================*/
/*  ��������: Web_MsgRequestParse                                          */
/*  ��������: ����webģ���������Ϣ�Ľ���                                  */
/*  �������: pstMsg: ���յ�����Ϣ                                         */
/*  �������: pszName��pszVal, �������name��val��ָ��                     */
/*  ��  ��  : ����Ⱥ / 2007-9-29                                           */
/*  ˵  ��  : ��Ϣ������"name=val, ���ӿڽ�name��val��ָ��ֱ���pszName��*/
/*            pszVal�� ����'='��Ϊ'\0'                                     */
/*=========================================================================*/
int WEB_MsgRequestParse(ST_MSG *pstMsg, char **pszName, char **pszVal)
{
    if (pstMsg == NULL || pstMsg->stMsgHead.ulBodyLength == 0)
        return TBS_FAILED;

    *pszName = pstMsg->szMsgBody;
    *pszVal  = strchr(pstMsg->szMsgBody, '=');
    if (*pszVal != NULL)
    {
        **pszVal = '\0';
        (*pszVal)++;
    }
    return TBS_SUCCESS;
}


/*=========================================================================*/
/*  ��������: WEB_MsgRespAddOne                                            */
/*  ��������: �ڷ���WEBģ�����Ӧ��Ϣ������һ��ֵ                          */
/*  �������: nObjNum/nVarNum: ��Ӧ��Ϣ�ж��������ֵ�ĸ���                */
/*            strVal         : ��Ӧ��Ϣ���¼����ֵ                        */
/*  �������: pstMsg         : ����ֵ�����Ϣ��                            */
/*  ����ֵ  : �ɹ�/ʧ��                                                    */
/*  ��  ��  : ����Ⱥ / 2007-9-29                                           */
/*=========================================================================*/
int WEB_MsgRespAddOne(ST_MSG *pstMsg, unsigned char nObjNum, unsigned char nVarNum, PCSTR strVal)
{
    if (pstMsg == NULL)
        return TBS_FAILED;

    ST_OM_VAL_RESP *pstResp = (ST_OM_VAL_RESP *)pstMsg->szMsgBody;

    // ���������һ�£� ��ʾ���ոտ�ʼ
    if (pstMsg->stMsgHead.ulBodyLength == 0
     || pstResp->ucObjNum != nObjNum
     || pstResp->ucValNum != nVarNum)
    {
        pstResp->ucObjNum = nObjNum;
        pstResp->ucValNum = nVarNum;
        pstMsg->stMsgHead.ulBodyLength = sizeof(ST_OM_VAL_RESP);
    }

    // ֱֵ�����������
    ST_OM_MSG_VAL *pstVal = (ST_OM_MSG_VAL *)(pstMsg->szMsgBody + pstMsg->stMsgHead.ulBodyLength);
    strcpy(pstVal->ucBuffer, strVal);
    pstVal->usLen = strlen(strVal) + 1;
    pstVal->usType = 0;
    pstMsg->stMsgHead.ulBodyLength += pstVal->usLen + sizeof(ST_OM_MSG_VAL);

    return TBS_SUCCESS;
}

/*=========================================================================*/
/*  ��������: WEB_MsgRespSetOne                                            */
/*  ��������: �ڷ���WEBģ�����Ӧ��Ϣ������һ����ֵ                        */
/*  �������: strVal         : ��Ӧ��Ϣ�����õ�ֵ                          */
/*  �������: pstMsg         : ����ֵ�����Ϣ��                            */
/*  ����ֵ  : �ɹ�/ʧ��                                                    */
/*  ��  ��  : ����Ⱥ / 2007-9-29                                           */
/*=========================================================================*/
int WEB_MsgRespSetOne(ST_MSG *pstMsg, PCSTR strVal)
{
    if (pstMsg == NULL)
        return TBS_FAILED;

    pstMsg->stMsgHead.ulBodyLength = 0;

    return WEB_MsgRespAddOne(pstMsg, 1, 1, strVal);
}

/*=========================================================================*/
/*  ��������: WEB_MsgGetNodeMap                                            */
/*  ��������: ����Ϣ�еĽڵ㴮���з���                                     */
/*  �������: szName : �ڵ㴮                                              */
/*            aszNode: �ڵ�����                                            */
/*  ����ֵ  : �����Ľڵ����                                             */
/*  ��  ��  : liuzequn / 2007-10-9                                         */
/*=========================================================================*/
int WEB_MsgGetNodeMap(char *szName, char aszNode[OM_NODE_MAX][OM_NODE_LEN])
{
    char *pPosDot = NULL;
    char *pPosStart = NULL;
    int  nNodeNum = 0;

    if (aszNode == NULL || szName == NULL)
        return TBS_PARAM_ERR;


    for (pPosDot = szName, pPosStart = szName; pPosStart != NULL && *pPosStart != '\0'; )
    {
        if ((pPosDot = strchr(pPosStart, OM_NODE_TOKEN)) != NULL)
        {
            *pPosDot = '\0';
            strcpy(aszNode[nNodeNum++], pPosStart);
            pPosStart = pPosDot + 1;
        }
        else
        {
            strcpy(aszNode[nNodeNum++], pPosStart);
            break;
        }
    }
    return nNodeNum;

}


/******************************************************************************
 *                                 END                                        *
 ******************************************************************************/


