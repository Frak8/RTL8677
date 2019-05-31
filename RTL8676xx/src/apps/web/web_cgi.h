/**********************************************************************
 Copyright (c), 1991-2007, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 �ļ����� : web_cgi.h
 �ļ����� : WEB���������ֵ�Cgi�����ļ�


 �����б� :


 �޶���¼ :
          1 ���� : ����Ⱥ
            ���� : 2007-8-21
            ���� :

**********************************************************************/


#ifndef __WEB_CGI_H__
#define __WEB_CGI_H__

#include "web_pub.h"
#include "web_cmd.h"
#include "autoconf.h"


/******************************************************************************
 *                                 MACRO                                      *
 ******************************************************************************/

#define WEB_ESC_CHAR            '%'             /* ��������ת���*/
#define WEB_SPLIT_CHAR_SET      "&;"            /* �������ݷָ�������*/
#define WEB_SSID_TOKEN          "sessionid"
#define WEB_NEWOBJID_TOKEN      "newobjindex"   /* ���һ��AddObj�ɹ���Ľڵ���� */
#define WEB_LANG_TOKEN          "language"      /* ҳ������ */
#define WEB_NOREDIRECT_TOKEN    "noredirect"    /* POST����֮�󲻽����ض������ */

#define WEB_USERNAME_TOKEN      "sys_UserName"      /* �û���¼�� */
#define WEB_USERLEVEL_TOKEN     "sys_UserLevel"     /* �û��ȼ� */
#define WEB_USERINDEX_TOKEN     "sys_UserIndex"     /* �û�Id */

#define WEB_ACTION_TOKEN        "obj-action"
#define WEB_ACTION_SET          "set"
#define WEB_ACTION_ADD          "add"
#define WEB_ACTION_DEL          "del"
#define WEB_ACTION_AUTH         "auth"
#define WEB_ACTION_SAVE         "save"
#define WEB_ACTION_RECOVER      "recover"
#define WEB_ACTION_REBOOT       "reboot"
#define WEB_ADD_OBJ_NAME        "add-obj"
#define WEB_DEL_OBJ_NAME        "del-obj"
#define WEB_ADD_SET_NAME        "add-set"
#define WEB_ADD_SET_OBJ_NAME    "add-obj"

#define WEB_ACTION_DEL_ADD_SET  "del-add-set"

#define WEB_SSID_NAME           WEBCMD_VAL_TOKEN WEB_SSID_TOKEN
#define WEB_NEWOBJID_NAME       WEBCMD_VAL_TOKEN WEB_NEWOBJID_TOKEN
#define WEB_LANG_NAME           WEBCMD_VAL_TOKEN WEB_LANG_TOKEN
#define WEB_NOREDIRECT_NAME     WEBCMD_VAL_TOKEN WEB_NOREDIRECT_TOKEN

#define WEB_USERNAME_NAME       WEBCMD_VAL_TOKEN WEB_USERNAME_TOKEN
#define WEB_USERLEVEL_NAME      WEBCMD_VAL_TOKEN WEB_USERLEVEL_TOKEN
#define WEB_USERINDEX_NAME      WEBCMD_VAL_TOKEN WEB_USERINDEX_TOKEN

/**/
#define WEB_MID_TAG             "mid"

/* ģ���б��ļ�*/
#define WEB_PATH_MODLIST        "/etc/modlist.ini"
/*ҳ��Ȩ���б�*/
#define WEB_AUTH_LIST           "/usr/www/webAuth.xml"
/* ģ���ǰ׺*/
#define LOGIC_CONFIG_TOKEN      "CONFIG_APPS_LOGIC_"


/******************************************************************************
 *                                 ENUM                                       *
 ******************************************************************************/

/* CGIͨ�����ͺͷ���ֵ*/
enum en_CGI_GEN_RET
{
    CGI_NONE = 0,       /* ��*/
    CGI_GET,            /* GET����*/
    CGI_POST,           /* POST����*/
    CGI_ERROR           /* ����*/
};

/******************************************************************************
 *                                STRUCT                                      *
 ******************************************************************************/

/******************************************************************************
 *                               FUNCTION                                     *
 ******************************************************************************/

/* ��Url�е�ת���ȡ��ת�� */
void WEB_UrlUnEsc(char *szUrl);

/*  ����һ�������SesionID  */
unsigned long WEB_CreateSSID();

/* ��name/val�ķ�ʽ��ȡCGI���� */
int  WEB_GetCgiVars(ST_NAME_VAL **ppstCgiVal);

/* �����ȡ����CGI����������ȫ������ */
int  WEB_GetEnv(void);

/* ���ϵͳ֧�ֵ�ģ�鵽g_pstWebVars��*/
void WEB_AddSupportMods();

/* ���ϵͳ֧�ֵ����Ե� g_pstWebVars */
void WEB_AddSupportLangs();

/*=========================================================================*/
/*  ��������: OBJCET��POST�Ĵ�����                                       */
/*=========================================================================*/
int WEB_PostObjSet();   // ����
int WEB_PostObjAdd();   // ���
int WEB_PostObjDel();   // ɾ��
int CheckAuth(char *pszSessionID);
int WEB_PostObjAddSet();
int WEB_PostObjDelAddSet();

int WEB_PostSimpCmd(unsigned short usMsgType);


/* POST�Ĵ����� */
int  WEB_Post();
/*ҳ����֤������*/
int WEB_CheckPageLevel(char *pageName,int pageLevel);

#endif /* __WEB_CGI_H__ */


