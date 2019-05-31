/**********************************************************************
 Copyright (c), 1991-2007, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 �ļ����� : web_cmd.h
 �ļ����� : WEB ģ��˽�������������ͷ�ļ�


 �����б� :


 �޶���¼ :
          1 ���� : ����Ⱥ / 2007-8-21
            ���� :
            ���� :

**********************************************************************/
#ifndef _WEB_CMD_H_
#define _WEB_CMD_H_

#include "web_pub.h"

/******************************************************************************
 *                                 MACRO                                      *
 ******************************************************************************/


#define WEBCMD_START_TOKEN  "<?"
#define WEBCMD_END_TOKEN    "?>"
#define WEBCMD_VAR_TOKEN    '$'
#define WEBCMD_MODULE_TOKEN ':'
#define WEBCMD_QUOTE_BACK   '`'
#define WEBCMD_QUOTE_SINGLE '\''
#define WEBCMD_QUOTE_DOUBLE '\"'

#define WEBCMD_UNFIND_VAL   "-"
#define WEBCMD_VAL_TOKEN    "var:"
#define WEBCMD_LEAF_START   ".n("       // ��һ��object.name�Ľṹ
#define WEBCMD_LEAF_END     ')'         // ��һ��object.name�Ľṹ
#define WEBCMD_LEAF_NULL    ".n()"      // ��һ��object.name�Ŀսṹ

/* �����ֵĺ궨�� */
#define WEB_CMD_COMMENT         "--"
#define WEB_CMD_ECHO            "echo"
#define WEB_CMD_SET_VAR         "setvar"
#define WEB_CMD_SET_VAR_ONCE    "setvaronce"
#define WEB_CMD_QUERY           "query"
#define WEB_CMD_INCLUDE         "include"
#define WEB_CMD_IF              "if"
#define WEB_CMD_ERROR           "error"
#define WEB_CMD_MULTI_QUERY     "multiquery"
#define WEB_CMD_MULTI_QUERY_N   "multiquerycount"
#define WEB_CMD_OBJ_QUERY       "objget"
#define WEB_CMD_GET             "get"
#define WEB_CMD_MULTI_GET       "mget"


#define WEBCMD_TOKEN_LEN    2   /* strlen(WEBCMD_START_TOKEN),*/
                                /* ��ʼ���仯, �ö���ҲҪ�仯*/

#define WEB_IS_SEPARATOR(c)    (c == 0x20 || c == '\r' || c == '\n' || c == '\t')

/******************************************************************************
 *                                 ENUM                                       *
 ******************************************************************************/



enum en_QUOTE_TYPE
{
    EN_QUOTE_NONE,      /* ������*/
    EN_QUOTE_SINGLE,    /* ������*/
    EN_QUOTE_DOUBLE,    /* ˫����*/
    EN_QUOTE_BACK,      /* ������*/

    EN_QUOTE_BUTT
};


/******************************************************************************
 *                                STRUCT                                      *
 ******************************************************************************/


/* ����ṹ��*/
typedef struct tag_ST_WEBCMD_INFO
{
    struct tag_ST_WEBCMD_INFO  *pParent;            /* ������*/
    int                         nDepth;             /* �����Ƕ�����*/
    ST_WEB_VALUE               *pstValCur;
    int                         nValIndex;

    int                         nParam;
    ST_OM_BLOCK                *apParam[WEB_NUM_PARAM];/*ST_OM_BLOCK����Ϊ�����ȵĻ���*/
}ST_WEBCMD_INFO;




/******************************************************************************
 *                               FUNCTION                                     *
 ******************************************************************************/

int WEB_GetObjReplace(char *pBuffer, char szValMap[WEB_MAX_VAL_PER_OBJ][WEB_MAX_OBJ_LEN]);

/* �������˷ָ������������ṹ */
ST_WEBCMD_INFO *WEB_FindBackeLvelCmdInfo(int nBackLevel,
            ST_WEBCMD_INFO *pstCmdInfo);

/* �������Ĵ��еı��������滻 */
int    WEB_VarReplace(ST_OM_BLOCK *pBlockBuff, ST_WEBCMD_INFO *pstCmdInfo);

/* ��ȡ�����е����в�������������ṹ�Ĳ����б� */
void WEB_GetArgMap(char *szStart, int size, ST_WEBCMD_INFO *pstCmdInfo);

/* echo����������� */
void    WEB_CmdEcho(ST_WEBCMD_INFO *pstCmdInfo, ST_OM_BLOCK *pstOutBuff);

/* setvar����������� */
void    WEB_CmdSetVar(ST_WEBCMD_INFO *pstCmdInfo, BOOL bReset);

/* include������� */
void    WEB_CmdInclude(ST_WEBCMD_INFO *pstCmdInfo);

/* �����ѯ����Ľ��� */
void    WEB_CmdQuery(ST_WEBCMD_INFO *pstCmdInfo, BOOL bSingle,
                           BOOL bGetcount,  ST_OM_BLOCK *pstOutBuff);

/* �����ڵ�ֵ��ѯ����Ľ��� */
void    WEB_CmdValueGet(ST_WEBCMD_INFO *pstCmdInfo, ST_OM_BLOCK *pstOutBuff);
/* ����ڵ�ֵ��ѯ����Ľ��� */
void    WEB_CmdMultiGet(ST_WEBCMD_INFO *pstCmdInfo, ST_OM_BLOCK *pstOutBuff);
/* ����ڵ��ѯ����Ľ��� */
void    WEB_CmdObjGet(ST_WEBCMD_INFO *pstCmdInfo, ST_OM_BLOCK *pstOutBuff);

/* if��������Ľ��� */
void    WEB_CmdIf(ST_WEBCMD_INFO *pstCmdInfo, ST_OM_BLOCK *pstOutBuff);

/* error������� */
void    WEB_CmdError(ST_WEBCMD_INFO *pstCmdInfo, ST_OM_BLOCK *pstOutBuff);

/* �ı�������Ľ���������� */
int WEB_ParseCmd(char *szStart, int nSize,
                        ST_WEBCMD_INFO *pParent, ST_OM_BLOCK *pstOutBuff);

/* �ı�������ʵ�ֺ������ */
int    WEB_ParseString(char *szStart, int nSize, ST_WEBCMD_INFO *pCmdParent,
                       ST_OM_BLOCK *pstOutBuff);
/* �ı��������� */
int WEB_ParseText (char *szText, int nSize);


/******************************************************************************
 *                                 END                                        *
 ******************************************************************************/


#endif /* __WEB_CMD_H__ */


