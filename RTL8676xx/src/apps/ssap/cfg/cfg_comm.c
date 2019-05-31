/**********************************************************************
 Copyright (c), 1991-2007, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 �ļ����� : cfg_comm.c
 �ļ����� : cfgģ����һЩȫ��ͨ�õ��Ӻ����ͳ�ʼ����غ�����ʵ��
 �����б� :

 �޶���¼ :
          1 ���� : ��Ծ��
            ���� : 2007-10-15
            ���� :
          2 �޸� : ��Ծ��
            ���� : 2009-07-06
            ���� : ���Ӻ���CFG_StrChrTok, ����֮�滻 strtok

**********************************************************************/

#include <unistd.h>
#include <time.h>
#include "cfg_file.h"
#include "cfg_prv.h"
#include "autoconf.h"
#include "tbsutil.h"
#include "tbsmsg.h"
#include <led.h>
#include "flash_layout.h"

#ifdef _cplusplus
    #if _cplusplus
        extern "C" {
    #endif
#endif

//telefonica ���󣬻ָ���������ʱ�ƣ�ȫ����˸��
#define CONFIG_LED_FACTORT_FLASHING 


/* ������ */
scew_tree *g_pstCfgTree = NULL;

const char *g_pcCfgAccessor = NULL;

#ifndef CPE_PREFIX
#define CPE_PREFIX  "X_TWSZ-COM"
#define CPE_SECOND_PREFIX  "X_TELEFONICA-ES"
#endif

#ifdef CONFIG_APPS_CMM_NODE_FIX
//����������
scew_tree *g_pstDefaultTree = NULL;
#endif
#define MAX_PATH_LEN 257
/*************************************************************************
Function:      unsigned char CFG_IsAccessorIn(const char *pcAccessList,
                                       const char *pcAccessor)
Description:   ��ѯ�������Ƿ��ڷ������б���
Calls:         ��
Data Accessed:
Data Updated:
Input:         pcAccessList, �������б�
               pcAccessor, ������
Output:        ��
Return:        1, ����
               0, ������
Others:
*************************************************************************/
unsigned char CFG_IsAccessorIn(const char *pcAccessList, const char *pcAccessor)
{
    const char *pcBegin = pcAccessList;
    long ret = 0;
    unsigned char ucState = 0;

    if ('\0' == pcAccessor[0])  /* Ϊ���ַ���, ��ʾ������ΪTR069 */
    {
        return 1;
    }

    while (1)
    {
        if ((' ' == *pcAccessList) || ('\0' == *pcAccessList))
        {
            if (1 == ucState)
            {
                ucState = 0;
                ret = strncmp(pcBegin, pcAccessor,
                               (unsigned long)(pcAccessList - pcBegin));
                if (0 == ret)
                {
                    return 1;
                }
            }

            if ('\0' == *pcAccessList)
            {
                break;
            }

            pcBegin = pcAccessList + 1;
        }
        else
        {
            if (0 == ucState)
            {
                ucState = 1;
                pcBegin = pcAccessList;
            }
        }

        pcAccessList++;
    }

    return 0;
}



/*************************************************************************
Function:      char *cfg_strdup(const char *pcPath)
Description:   CFGģ����������·������,
Calls:         ��
Data Accessed:
Data Updated:
Input:         pcPath, Ҫ���Ƶ�·��
Output:        ��
Return:        ���ƺ��ַ����ĵ�ַ
Others:        �����������·������ڴ�, ʵ�����õ��Ǿ�̬����, Ŀ���Ǽ���malloc�Ĵ���
*************************************************************************/
char *cfg_strdup(const char *pcPath)
{
    static char acPath[CFG_MAX_PATH_LEN];

    if (strlen(pcPath) >= CFG_MAX_PATH_LEN)
    {
        CFG_ERR(ERR_CFG_BUF_NOT_ENOUGH);

        return NULL;
    }

    strcpy(acPath, pcPath);

    return acPath;
}




/*
ȡ�ýڵ�, �Ӻ���, pcPath ��д
*/


/*************************************************************************
Function:      void CFG_InitNodeInfo(ST_CFG_NODE_INFO *pstNodeInfo, unsigned long ulMask)
Description:  ��ʼ���ڵ���Ϣ
Calls:         ��
Data Accessed:
Data Updated:
Input:         ulMask, �ڵ���Ϣ�����־
Output:        pstNodeInfo, ��ʼ����Ľڵ���Ϣ�ṹ
Return:        ��
Others:
*************************************************************************/
void CFG_InitNodeInfo(ST_CFG_NODE_INFO *pstNodeInfo, unsigned long ulMask)
{
    if (ulMask & CFG_NODE_INFO_MID)
    {
        pstNodeInfo->usMID = 0;
    }
    if (ulMask & CFG_NODE_INFO_STANDARD)
    {
        pstNodeInfo->ucStandard = 1;
    }
    if (ulMask & CFG_NODE_INFO_NOTI)
    {
        pstNodeInfo->ucNoti = 0;
    }
    if (ulMask & CFG_NODE_INFO_ACCESSLIST)
    {
        pstNodeInfo->pcAccessList = "Subscriber";
    }
}

/*************************************************************************
Function:      void CFG_UpdateNodeInfo(const scew_element *pstNode,
                        ST_CFG_NODE_INFO *pstNodeInfo, unsigned long ulMask)
Description:  ���½ڵ���Ϣ�ṹ
Calls:         ��
Data Accessed:
Data Updated:
Input:         pstNode, �ڵ��ַ
               ulMask, �ڵ���Ϣ�����־
Output:        pstNodeInfo, ���º�Ľڵ���Ϣ�ṹ
Return:        ��
Others:
*************************************************************************/
void CFG_UpdateNodeInfo(const scew_element *pstNode,
       ST_CFG_NODE_INFO *pstNodeInfo, unsigned long ulMask)
{
    scew_attribute *pstAttr =NULL;
    const char *pcAttrVal = NULL;

    if (ulMask & CFG_NODE_INFO_MID)
    {
        pstAttr = scew_attribute_by_name(pstNode, ATTR_MID);
        if (NULL != pstAttr)
        {
            pcAttrVal = scew_attribute_value(pstAttr);
            pstNodeInfo->usMID = strtol(pcAttrVal, NULL, CFG_MID_BASE);
        }
    }
    if (ulMask & CFG_NODE_INFO_STANDARD)
    {
        if (0 != pstNodeInfo->ucStandard)
        {
            pstAttr = scew_attribute_by_name(pstNode, ATTR_STANDARD);
            if (NULL != pstAttr)
            {
                pcAttrVal = scew_attribute_value(pstAttr);
                pstNodeInfo->ucStandard = (unsigned char )(pcAttrVal[0] - '0');
            }
        }
    }
    if (ulMask & CFG_NODE_INFO_NOTI)
    {
        pstAttr = scew_attribute_by_name(pstNode, ATTR_NOTI);
        if (NULL != pstAttr)
        {
            pcAttrVal = scew_attribute_value(pstAttr);
            pstNodeInfo->ucNoti = (unsigned char )(pcAttrVal[0] - '0');
        }
    }
    if (ulMask & CFG_NODE_INFO_ACCESSLIST)
    {
        pstAttr = scew_attribute_by_name(pstNode, ATTR_ACCESS_LIST);
        if (NULL != pstAttr)
        {
            pcAttrVal = scew_attribute_value(pstAttr);
            pstNodeInfo->pcAccessList = pcAttrVal;
        }
    }
}


/*************************************************************************
Function:      long CFG_GetNode(const void *pstTree, char *pcPath, void **ppvNode)
Description:   ���ҽڵ�
Calls:
Data Accessed:
Data Updated:
Input:         pstTree, ����
               pcPath, ·��,
Output:        ppvNode, �ڵ�ָ��
Return:        0,�ɹ�;
               ����, ʧ��
Others:
*************************************************************************/
#ifdef CONFIG_APPS_CMM_NODE_FIX
char g_BootPeriod = 0;
#endif
CFG_RET CFG_GetNode(const void *pvTree, char *pcPath, void **ppvNode,
                    ST_CFG_NODE_INFO *pstNodeInfo, unsigned long ulMask)
{
    char *pszTmp = NULL;
    scew_element *pstNode = NULL;
    CFG_RET ret = CFG_OK;

#ifdef CONFIG_APPS_CMM_NODE_FIX
    char pszPath[MAX_PATH_LEN];
    if (g_BootPeriod)
        strcpy(pszPath, pcPath);
#endif

    if (NULL == pvTree)
    {
        pvTree = g_pstCfgTree;
    }

    pszTmp = CFG_StrChrTok(pcPath, DELIMIT_C, &pcPath);

    pstNode = scew_tree_root(pvTree);
    if (NULL != pszTmp)
    {
        /* ������ṩ�ڵ�·��, �����ڵ��Ƿ�ƥ�� */
        ret = strcmp(scew_element_name(pstNode), pszTmp);
        if (0 != ret)
        {
            /* �ڵ㲻���� */
            CFG_ERR(ERR_CFG_PATH_NOT_EXSITED, "%s", pszTmp);
            //return ERR_CFG_PATH_NOT_EXSITED;
            goto ERROR_CODE;
        }
        CFG_InitNodeInfo(pstNodeInfo, ulMask);
        CFG_UpdateNodeInfo(pstNode, pstNodeInfo, ulMask);

        while (pszTmp = CFG_StrChrTok(pcPath, DELIMIT_C, &pcPath),
               NULL != pszTmp)
        {
            /* ���� �� pszTmp ��ת�� */
            CFG_NUM_TO_NODE(pszTmp);

            pstNode = scew_element_by_name(pstNode, pszTmp);
            if (NULL == pstNode)
            {
                /* �ڵ㲻���� */
                CFG_ERR(ERR_CFG_PATH_NOT_EXSITED, "%s", pszTmp);
                //return ERR_CFG_PATH_NOT_EXSITED;
                ret = ERR_CFG_PATH_NOT_EXSITED;
                goto ERROR_CODE;
            }

            CFG_UpdateNodeInfo(pstNode, pstNodeInfo, ulMask);
        }
    }
    else
    {
        CFG_InitNodeInfo(pstNodeInfo, ulMask);
        CFG_UpdateNodeInfo(pstNode, pstNodeInfo, ulMask);
    }

    *ppvNode = pstNode;
    return CFG_OK;

ERROR_CODE:
#ifdef CONFIG_APPS_CMM_NODE_FIX
    if (g_BootPeriod)
    {
        //���������У������ȡָ���ڵ�ʧ�ܣ����ȴ�
        //���������лָ�����δ�TR069 Model�ϻָ�
        const char *pValue = NULL;
        CFG_TRACE("###pszPath = %s###\n", pszPath);
        ret = CFG_RecoverNodeValueFromDefault(pszPath, &pValue, ppvNode);
        if ((CFG_OK != ret) || (NULL == pValue))
        {
            ret = CFG_RecoverNodeValueFromMid(pszPath, &pValue, ppvNode);
        }
        CFG_TRACE("###pValue = %s\n###", pValue);
    }
#endif
    if (CFG_OK != ret)
        return ERR_CFG_PATH_NOT_EXSITED;
    return CFG_OK;
}


CFG_RET CFG_InitMergeKeyTree(void);
CFG_RET CFG_GetUsbInitPath(char *pcPath);

/***********************************************************
�ӿ�:   ����ָ���ӿڶ�ȡ��������Ȼ�������һ����
����:   ppstTree, ��������ɵ���
        pcPath, ��ȡ���ݵ�·��
        pfnFunc, ��ȡ����
����ֵ: 0:�ɹ�
        ����:ʧ��
***********************************************************/
CFG_RET CFG_InitTreeByFunc(scew_tree **ppstTree, const char *pcPath,
                           FUNC_CFG_READ_CFG pfnFunc)
{
    char *pcCfg = NULL;
    unsigned long ulLen = 0;
    CFG_RET ret = CFG_OK;
    char *pcFileBufTmp = NULL;

    ret = pfnFunc(pcPath, &pcCfg, 0, CFG_NUM_TRANS_MEAN_LEN, &ulLen);
    if (CFG_OK != ret)   /* ʧ�� */
    {
        CFG_ERR(ret);
        return ret;
    }
    pcCfg[ulLen] = '\0';

    /*�Ա��ļ�ͷ�������û��ָ��encoding������Ҫָ�������û��ָ����һЩ�����ַ����ܱ���Ϊ�Ƿ�*/
    if(strstr(pcCfg, XML_CTL_HEAD) == NULL)
    {
        pcFileBufTmp = malloc(ulLen +1+CFG_NUM_TRANS_MEAN_LEN+XML_CTL_HEAD_LEN);
        if(!pcFileBufTmp)
        {
            free(pcCfg);
             return TBS_OUT_OF_MEM;
        }
        memcpy(pcFileBufTmp, XML_CTL_HEAD, XML_CTL_HEAD_LEN);
        memcpy(pcFileBufTmp+XML_CTL_HEAD_LEN, pcCfg, ulLen+CFG_NUM_TRANS_MEAN_LEN+1);
        free(pcCfg);
        pcCfg = pcFileBufTmp;
        ulLen += XML_CTL_HEAD_LEN;
    }

    /* ���Ӷ����ֵ�ת�� */
    CFG_FileTransMean(pcCfg, ulLen);

    /* ���������� */
    ret = CFG_XmlInitPaser(ppstTree, (const char *)pcCfg);
    free(pcCfg);
    if (CFG_OK != ret)
    {
        CFG_ERR(ret);
        return ret;
    }

    /* ���ڵ�����ת���ȥ */
    ret = CFG_TreeTransBack(*ppstTree);
    if (CFG_OK != ret)
    {
        CFG_ERR(ret);
        return ret;
    }

    ret = CFG_FillDataTreeExAttrs();
    if (CFG_OK != ret)
    {
        CFG_ERR(ret);
        return ret;
    }

    ret = CFG_FillDataTreeExNodes();
    if (CFG_OK != ret)
    {
        CFG_ERR(ret);
        return ret;
    }

    /* ɾ����Ҫ����ɾ���Ľڵ� */
//    CFG_RmvDeletingNode(*ppstTree);

    return CFG_OK;
}

/***********************************************************
�ӿ�:   ��ȡĬ�����õĺ���
����:   pcFile, �ļ���
        ppcBuf, �����������ַ
        ulPrevSpace, Ҫ�󻺳���֮ǰ�ж��ٿռ�
        ulPostSpace, Ҫ�󻺳���֮���ж��ٿռ�
        pulLen, �����ȡ�����ļ�����
����ֵ: 0:�ɹ�
        ����:ʧ��
***********************************************************/
CFG_RET CFG_ReadDefaultCfgByPath(const char *pcFile, char **ppcBuf, unsigned long ulPrevSpace,
                     unsigned long ulPostSpace, unsigned long *pulLen)
{
    (void)pcFile;
    return CFG_ReadDefaultCfg(ppcBuf, ulPrevSpace, ulPostSpace, pulLen);
}


/* �����ӿ� */
/***********************************************************
�ӿ�:   ���ÿ��ʼ��
����:   ��
����ֵ: 0:�ɹ�
        ����:ʧ��
***********************************************************/
CFG_RET CFG_LibInit(void)
{
    CFG_RET ret = CFG_OK;

    /* ��ʼ�� mid�� */
    ret = CFG_MIDTreeInit();
    if (CFG_OK != ret)
    {
        CFG_ERR(ret);
        return ret;
    }

    /* �� ��������ó�ʼ���� */
    ret = CFG_InitTreeByFunc(&g_pstCfgTree, NULL, CFG_ReadCurCfg);
    if (CFG_OK != ret)   /* ʧ�� */
    {
        /* �� ȱʡ���ó�ʼ���� */
        ret = CFG_InitTreeByFunc(&g_pstCfgTree, NULL, CFG_ReadDefaultCfgByPath);
#ifdef CONFIG_APPS_SSAP_PROTEST

        #define MAX_WPAPSK_LEN       	133
        #define AUTH_LOGIN_NAME_LEN     64

        char PassWd[MAX_WPAPSK_LEN] = {0};
        char szValue[ITEM_DATA_LEN] = {0};
        char szPath[MAX_PATH_LEN] = {0};
        const char *pszBeaconType = NULL;
        const char *pszPassword = NULL;
        const char *pszPSKExpression = NULL;
        int iRet = 0;
        unsigned short len;
#ifdef CONFIG_APPS_CLI_SEPARATE_AUTH
        len = ITEM_DATA_LEN;
        iRet = app_item_get(szValue, TELNET_PASSWORD, &len);
        szValue[len] = '\0';
        if ((ERROR_ITEM_OK == iRet) && (strlen(szValue) > 0))
        {
            app_item_save(szValue, CUR_TELNET_PASS, strlen(szValue));
        }
        else
        {
            strcpy(szValue, "etisalat");
            app_item_save(szValue, CUR_TELNET_PASS, strlen(szValue));
        }
#endif
        /* ��FLASH��ȡ��һ��VAP�ļ���ģʽ����Կ */
        len = MAX_WPAPSK_LEN;
        iRet = app_item_get(PassWd , WLAN_PASSWORD ,&len);
        if((ERROR_ITEM_OK == iRet) && (strlen(PassWd) > 0))
        {
            sprintf(szPath, "%s.%s", WLAN_BASIC_PATH, NODE_WLAN_BEACON_TYPE);
            iRet = CFG_GetNodeValPtr(szPath, &pszBeaconType, NULL);

            if (iRet != CFG_OK)
            {
                CFG_TRACE("Error: Get %s value failed.\n", NODE_WLAN_BEACON_TYPE);
                return CFG_FAIL;
            }

            if (strcmp(pszBeaconType, BEACON_TYPE_BASIC) == 0)
            {
                if(strlen(PassWd) == 5 || strlen(PassWd) == 13 || strlen(PassWd) == 10 || strlen(PassWd) == 26)
                {
                    sprintf(szPath, "%s.WEPKey.1.%s", WLAN_BASIC_PATH, NODE_WLAN_WEPKEY);
                    CFG_SetNodeVal(szPath, PassWd, NULL);
                }
                else
                {
                    CFG_TRACE("Error: Set WEP password failed.\n");
                    //return CFG_FAIL;
                }
            }
            else if ((strcmp(pszBeaconType, BEACON_TYPE_WPA) == 0) || (strcmp(pszBeaconType, BEACON_TYPE_11i) == 0) || (strcmp(pszBeaconType, BEACON_TYPE_WPA2_MIXED) == 0))
            {
                sprintf(szPath, "%s.%s", WLAN_BASIC_PATH, NODE_WLAN_PSK_EXPRESSION);
                iRet = CFG_GetNodeValPtr(szPath, &pszPSKExpression, NULL);
				
                if (iRet != CFG_OK)
                {
                    CFG_TRACE("Error: Get %s value failed.\n", NODE_WLAN_PSK_EXPRESSION);
                    return CFG_FAIL;
                }

                /* ��PSKExpressionΪKeyPassphrase��ʽ*/
                if (strcmp(pszPSKExpression, NODE_WLAN_WPA_KEY_PASSPHRASE) == 0)
				{	
                    if(strlen(PassWd) <= 63 && strlen(PassWd) >= 8)
                    {
                        sprintf(szPath, "%s.PreSharedKey.1.%s", WLAN_BASIC_PATH, NODE_WLAN_WPA_KEY_PASSPHRASE);
                        CFG_SetNodeVal(szPath, PassWd, NULL);
                        /*2nd SSID*/
                        #ifdef CONFIG_APPS_LOGIC_WLAN_2NDSSID
                        CFG_SetNodeVal("InternetGatewayDevice.LANDevice.1.WLANConfiguration.2.PreSharedKey.1.KeyPassphrase",
                                        PassWd, NULL);
                        #endif
                    }
                    else
                    {
                        CFG_TRACE("Error: Set WPA password failed.\n");
                        //return CFG_FAIL;
                    }
                }
                /* ��PSKExpressionΪPreSharedKey��ʽ*/
                else if (strcmp(pszPSKExpression, NODE_WLAN_WPA_PSK) == 0)
                {	
                    if(strlen(PassWd) == 64)
                    {
                        sprintf(szPath, "%s.PreSharedKey.1.%s", WLAN_BASIC_PATH, NODE_WLAN_WPA_PSK);
                        CFG_SetNodeVal(szPath, PassWd, NULL);
                        /*2nd SSID*/
                        #ifdef CONFIG_APPS_LOGIC_WLAN_2NDSSID
                        CFG_SetNodeVal("InternetGatewayDevice.LANDevice.1.WLANConfiguration.2.PreSharedKey.1.PreSharedKey",
                                        PassWd, NULL);
                        #endif
                    }
                    else
                    {
                        CFG_TRACE("Error: Set WPA password failed.\n");
                        //return CFG_FAIL;
                	}
                }
            }
	    }

        /* ��FLASH��ȡ��һ��SSID�����Ʋ�д������ */
        len = ITEM_DATA_LEN;
        iRet = app_item_get(szValue , WLAN_SSID ,&len);
        szValue[len] = '\0';
        if((ERROR_ITEM_OK == iRet) && (strlen(szValue) > 0))
        {
            sprintf(szPath, "%s.%s", WLAN_BASIC_PATH, NODE_WLAN_SSID);
            CFG_SetNodeVal(szPath, szValue, NULL);
        }

#endif
		
    }
    else
    {
#if 0 //#ifdef CONFIG_APPS_LOGIC_USB_MASS
        char acPath[CFG_MAX_FILE_LEN] = {0};

        /* ���ô�usb��ʼ���ĺ��� */
        ret = CFG_GetUsbInitPath(acPath);
        if (CFG_OK == ret && '\0' != acPath[0])
        {
            scew_tree *pstTreeSave = g_pstCfgTree;

            ret = CFG_InitTreeByFunc(&g_pstCfgTree, acPath, CFG_ReadCurCfg);
            if (CFG_OK == ret)
            {
                scew_tree_free(pstTreeSave);
                ret = CFG_SaveCfg();
            }
            ret = CFG_OK;
        }
#endif
    }
    if (CFG_OK != ret)
    {
        CFG_ERR(ret);
        return ret;
    }

#ifdef CONFIG_APPS_CMM_NODE_FIX
    ret = CFG_InitTreeByFunc(&g_pstDefaultTree, NULL, CFG_ReadDefaultCfgByPath);
    if (ret != CFG_OK)
    {
        CFG_TRACE("######Failed to init Default Tree######\n");
        return ret;
    }
    ret = CFG_SearchAndRecoverNodeValueFromMid();
    if (CFG_OK != ret)
    {
        CFG_TRACE("######Failed to fix the current config######\n");
        return ret;
    }
#endif

#ifdef CONFIG_APPS_SSAP_HOLD_KEY_PARA
    ret = CFG_InitMergeKeyTree();  /* ���� ��ʼ���ϲ��ؼ����Ĳ��� */
    if (CFG_OK != ret)
    {
        CFG_ERR(ret);
        return ret;
    }
#endif

#ifdef _CFG_DEBUG
    (void)CFG_ListNoTypeNode();
//    (void)CFG_PrintToExcel(); exit(0);
#endif

    return CFG_OK;
}


/* ���ÿ�ȥ��ʼ�� */
/***********************************************************
�ӿ�:   ���ÿ����ÿ�ȥ��ʼ��
����:   ��
����ֵ: 0:�ɹ�
        ����:ʧ��
***********************************************************/
CFG_RET CFG_LibFinal(void)
{
    if (NULL != g_pstCfgTree)
    {
        scew_tree_free(g_pstCfgTree);
        g_pstCfgTree = NULL;
    }
    (void)CFG_MIDTreeFinal();
    return CFG_OK;
}




/******************************************************************************
  ��������: XML_InitPaser
  ����˵��: ʹ��ָ��XML���ݳ�ʼ��XML������
  �������: pszXMLContent  XML��������, ֻ��
  �������: ��
  �� �� ֵ: CFG_NOERROR��ʾ�ɹ�,
            CFG_ERR_PASER_ERROR�����������ͳ���, һ����XML��ʽ����
  �޶���¼:
         1. ����: zhaihaichen
            ����: 2007-9-4
            ����: �����ļ�
 ******************************************************************************/
CFG_RET CFG_XmlInitPaser(scew_tree **ppstTree, const char *pszXMLContent)
{
    scew_parser* pParser = NULL;
    pParser = scew_parser_create();

    scew_parser_ignore_whitespaces(pParser, 1);

    /* Loads an XML file */
    if (!scew_parser_load_buffer(pParser, pszXMLContent, strlen(pszXMLContent)))
    {
        scew_error code = scew_error_code();
        CFG_ERR(ERR_CFG_INTERNAL_ERR, "Unable to load file (error #%d: %s)\n",
                 code, scew_error_string(code));
        if (code == scew_error_expat)
        {
            #ifdef _CFG_DEBUG
            enum XML_Error expat_code = scew_error_expat_code(pParser);
            #endif
            CFG_ERR(ERR_CFG_INTERNAL_ERR, "Expat error #%d (line %d, column %d): %s\n",
                     expat_code,
                     scew_error_expat_line(pParser),
                     scew_error_expat_column(pParser),
                     scew_error_expat_string(expat_code));
        }

        /* ˵��, ����������֤, ����� scew_parser_free �������ͷ����е��ڴ�.
           ��Щ�ڴ涼����scew_parser_load_buffer�б������.
           �����Ԫ�����ߵ������֧, �ɲ��ؼ���ڴ�й¶����.
        */

        /* Frees the SCEW parser */
        scew_parser_free(pParser);

        return ERR_CFG_INTERNAL_ERR;
    }

    /* ����XML�� */
    *ppstTree = scew_parser_tree(pParser);

    /* Frees the SCEW parser */
    scew_parser_free(pParser);

    return CFG_OK;
}


/*************************************************************************
Function:      void CFG_FileTransMean(char *pcBuf, unsigned long ulLen)
Description:   �ļ�����ת��, ��Ҫ�ǰ����ֽڵ���תΪ�����ֽڵ���
Calls:
Data Accessed:
Data Updated:
Input:         pcBuf, �ļ�����
               ulLen, ���ݳ���
Output:        pcBuf, �����������
Return:        0,�ɹ�;
               ����, ʧ��
Others:
*************************************************************************/
void CFG_FileTransMean(char *pcBuf, unsigned long ulLen)
{
    char *pcBufWork = pcBuf;
    unsigned long ulLenWork = ulLen;
    unsigned char ucState = 0;

    while ('\0' != *pcBufWork && ulLenWork > 0)
    {
        switch (*pcBufWork)
        {
            case '<':
                ucState = 1;
                break;
            case ' ':
            case '\t':
            case '/':
                break;
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                if (1 == ucState)
                {
                    MEM_BACKCPY(pcBufWork, pcBufWork + 1, ulLenWork);
                    *pcBufWork = '_';
                    pcBufWork++;
                    pcBufWork[ulLenWork] = '\0';

                    ucState = 0;
                }
                break;
            default:
                ucState = 0;
                break;
        }

        pcBufWork++;
        ulLenWork--;
    }
}

/***********************************************************
�ӿ�:   ���������ļ�
����:   pcFileContent: ������
        pulLen: ���뻺��������, �������������ʵ���Ƕ��ĳ���
����ֵ: 0:�ɹ�
        ����:ʧ��
***********************************************************/
CFG_RET CFG_GetCfgFile(char *pcFile, unsigned long *pulLen,
                       unsigned long ulCompress, unsigned long ulOverWrite,
                       const char *pcAccessor)
{
    scew_element *pElement = NULL;
    ST_CFG_DUMP_BUF stDumpBuf = {NULL, 0, 0, 0};
    char *pcFileBuf = NULL;
    CFG_RET ret = CFG_OK;
#ifdef CONFIG_REPLACE_NODE_PREFIX
    char *pcRet = NULL;
#endif
#ifdef CONFIG_REPLACE_NODE_SECOND_PREFIX
    char *pcSecRet = NULL;
#endif

    if (NULL == pcFile || NULL == pulLen)
    {
        CFG_ERR(ERR_CFG_PARA_INVALID);
        return ERR_CFG_PARA_INVALID;
    }

    //if (1 == ulCompress)  /* ѹ����ʽ */
    {
         /* �ļ��Ѿ����� */
        ret = access(pcFile, F_OK);
        if (CFG_OK == ret)
        {
            if (0 == ulOverWrite) /* �޸��Ǳ�־ */
            {
                /* ���� ����: �ļ��Ѿ����� */
                CFG_ERR(ERR_FILE_OPEN_EXSITED);
                return ERR_FILE_OPEN_EXSITED;
            }
            else
            {
                /* ɾ���ļ� */
                ret = remove(pcFile);
                if (CFG_OK != ret)
                {
                    int err = errno;
                    if (EACCES == err)
                    {
                        ret = ERR_FILE_NOT_ALLOWED;
                    }
                    else
                    {
                        ret = ERR_FILE_RM_UNKOWN;
                    }
                    CFG_ERR(ret);
                    return ret;
                }
            }
        }
    }

    pcFileBuf = malloc(CFG_MAX_FILE_LEN);
    if (NULL == pcFileBuf)
    {
        CFG_ERR(ERR_CFG_MALLOC_FAIL);
        return ERR_CFG_MALLOC_FAIL;
    }

    stDumpBuf.pcBuf = pcFileBuf;
    stDumpBuf.ulLen = CFG_MAX_FILE_LEN;
    pElement = scew_tree_root(g_pstCfgTree);

    /* dump �� */
    g_pcCfgAccessor = pcAccessor;
    (void)CFG_TreeDumpAccess(pElement, &stDumpBuf, 0);
    if (stDumpBuf.ulInfactLen != stDumpBuf.ulPos)
    {
        free(stDumpBuf.pcBuf);
        CFG_REDUMP_FILE(stDumpBuf, pElement);
    }

#ifdef CONFIG_REPLACE_NODE_PREFIX
    /*Ϊ�����ļ���ӽ�����*/
    stDumpBuf.pcBuf = realloc(stDumpBuf.pcBuf, stDumpBuf.ulInfactLen+1);
    if(!stDumpBuf.pcBuf)
    {
        return TBS_OUT_OF_MEM;
    }
    stDumpBuf.pcBuf[stDumpBuf.ulInfactLen] = '\0';
    pcRet = tbsStringReplaceWithMalloc(stDumpBuf.pcBuf, CPE_PREFIX, CONFIG_PRODUCT_PREFIX);
    free(stDumpBuf.pcBuf);
    stDumpBuf.pcBuf = pcRet;
    stDumpBuf.ulInfactLen = strlen(stDumpBuf.pcBuf);
#endif

#ifdef CONFIG_REPLACE_NODE_SECOND_PREFIX
    /*Ϊ�����ļ���ӽ�����*/
    stDumpBuf.pcBuf = realloc(stDumpBuf.pcBuf, stDumpBuf.ulInfactLen+1);
    if(!stDumpBuf.pcBuf)
    {
        return TBS_OUT_OF_MEM;
    }
    stDumpBuf.pcBuf[stDumpBuf.ulInfactLen] = '\0';
    pcSecRet = tbsStringReplaceWithMalloc(stDumpBuf.pcBuf, CPE_SECOND_PREFIX, CONFIG_PRODUCT_SECOND_PREFIX);
    free(stDumpBuf.pcBuf);
    stDumpBuf.pcBuf = pcSecRet;
    stDumpBuf.ulInfactLen = strlen(stDumpBuf.pcBuf);
#endif

    if (1 == ulCompress)  /* ѹ����ʽ */
    {
        ret = CFG_SaveCurCfg(pcFile, stDumpBuf.pcBuf, stDumpBuf.ulInfactLen);
    }
    else
    {
        ret = CFG_WriteFile(pcFile, stDumpBuf.pcBuf, stDumpBuf.ulInfactLen);
    }
    if (CFG_OK != ret)
    {
        CFG_ERR(ret);
    }

    pcFile = pcFile;
    *pulLen = *pulLen;

    free(stDumpBuf.pcBuf);
    return ret;
}


/***********************************************************
�ӿ�:   ���������ļ�
����:   pcFileContent: �����ļ�����
        ulLen: ���ݳ���
����ֵ: 0:�ɹ�
        ����:ʧ��
***********************************************************/
CFG_RET CFG_SetCfgFile(const char *pcFile, unsigned long ulLen)
{
    unsigned long ulNewLen = 0;
    scew_tree *pstNewTree = NULL;
    long ret = CFG_OK;
    char *pcFileBuf = NULL;
    char *pcFileBufTmp = NULL;
    
#ifdef CONFIG_REPLACE_NODE_PREFIX
    char *pcRet = NULL;
#endif
#ifdef CONFIG_REPLACE_NODE_SECOND_PREFIX
    char *pcSecRet = NULL;
#endif
    (void)ulLen;
    if (NULL == pcFile)
    {
        CFG_ERR(ERR_CFG_PARA_INVALID);
        return ERR_CFG_PARA_INVALID;
    }
#ifdef CONFIG_TBS_BACKUP_ENCRYPTION
	ret =  CFG_ReadCfgfile_EX(pcFile, &pcFileBuf, 0,CFG_NUM_TRANS_MEAN_LEN,&ulNewLen);
#else
    ret = CFG_ReadFile(pcFile, &pcFileBuf, 0, CFG_NUM_TRANS_MEAN_LEN, &ulNewLen);
#endif
	if (CFG_OK != ret)
    {
        CFG_ERR(ret);
        return ret;
    }

#ifdef CONFIG_REPLACE_NODE_PREFIX
    pcFileBuf = realloc(pcFileBuf, ulNewLen+1);
    if(!pcFileBuf)
    {
        return TBS_OUT_OF_MEM;
    }
    pcFileBuf[ulNewLen] = '\0';
    pcRet = tbsStringReplaceWithMalloc(pcFileBuf, CONFIG_PRODUCT_PREFIX, CPE_PREFIX);
    free(pcFileBuf);
    pcFileBuf = pcRet;
    ulNewLen = strlen(pcFileBuf);
    /*���·����ڴ�ʱһ��Ҫ��2K����ת��*/
    pcFileBuf = realloc(pcFileBuf, ulNewLen+1+CFG_NUM_TRANS_MEAN_LEN);
#endif

#ifdef CONFIG_REPLACE_NODE_SECOND_PREFIX
    pcFileBuf = realloc(pcFileBuf, ulNewLen+1);
    if(!pcFileBuf)
    {
        return TBS_OUT_OF_MEM;
    }
    pcFileBuf[ulNewLen] = '\0';
    pcSecRet = tbsStringReplaceWithMalloc(pcFileBuf, CONFIG_PRODUCT_SECOND_PREFIX, CPE_SECOND_PREFIX);
    free(pcFileBuf);
    pcFileBuf = pcSecRet;
    ulNewLen = strlen(pcFileBuf);
    /*���·����ڴ�ʱһ��Ҫ��2K����ת��*/
    pcFileBuf = realloc(pcFileBuf, ulNewLen+1+CFG_NUM_TRANS_MEAN_LEN);
#endif

    /*�Ա��ļ�ͷ�������û��ָ��encoding������Ҫָ�������û��ָ����һЩ�����ַ����ܱ���Ϊ�Ƿ�*/
    if(strstr(pcFileBuf, XML_CTL_HEAD) == NULL)
    {
        pcFileBufTmp = malloc(ulNewLen +1+CFG_NUM_TRANS_MEAN_LEN+XML_CTL_HEAD_LEN);
        if(!pcFileBufTmp)
        {
            free(pcFileBuf);
             return TBS_OUT_OF_MEM;
        }
        memcpy(pcFileBufTmp, XML_CTL_HEAD, XML_CTL_HEAD_LEN);
        memcpy(pcFileBufTmp+XML_CTL_HEAD_LEN, pcFileBuf, ulNewLen+CFG_NUM_TRANS_MEAN_LEN+1);
        free(pcFileBuf);
        pcFileBuf = pcFileBufTmp;
        ulNewLen += XML_CTL_HEAD_LEN;
     }
    
    /* ���Ӷ����ֵ�ת�� */
    CFG_FileTransMean(pcFileBuf, ulNewLen);

    /* �������� */
    ret = CFG_XmlInitPaser(&pstNewTree, (const char *)pcFileBuf);
    if (CFG_OK != ret)
    {
        free(pcFileBuf);

        CFG_ERR(ret);
        return ret;
    }

    free(pcFileBuf);

    /* �ͷž��� */
    scew_tree_free(g_pstCfgTree);

    /* ��ȫ����ָ��ָ������ */
    g_pstCfgTree = pstNewTree;

    ret = CFG_TreeTransBack(g_pstCfgTree);
    if (CFG_OK != ret)
    {
        CFG_ERR(ret);
        return ret;
    }
    /*д�ļ� */
    ret = CFG_SaveCfg();
    if (CFG_OK != ret)
    {
        CFG_ERR(ret);
    }

    return ret;
}




/***********************************************************
�ӿ�:   д����
����:   ��
����ֵ: 0:�ɹ�
        ����:ʧ��
***********************************************************/
CFG_RET CFG_SaveCfg(void)
{
    unsigned long ulLen = CFG_MAX_FILE_LEN;
    ST_CFG_DUMP_BUF stDumpBuf = {NULL, 0, 0, 0};
    scew_element *pstNode = NULL;
    long ret = CFG_OK;
    
#ifdef CONFIG_APPS_LOGIC_WANSELECT
    //LAN�����л��󣬲�Ҫ�������ã��������LAN��������
    if(access("/var/wanModeChangedFlag", F_OK) == 0)
    {
        return CFG_OK;
    }
#endif    

    g_pcCfgAccessor = NULL;
    pstNode = scew_tree_root(g_pstCfgTree);

    stDumpBuf.pcBuf = malloc(ulLen);
    if (NULL == stDumpBuf.pcBuf)
    {
        CFG_ERR(ERR_CFG_MALLOC_FAIL);
        return ERR_CFG_MALLOC_FAIL;
    }
    stDumpBuf.ulLen = ulLen;

    /* ���ڵ���dump ����, */
    (void)CFG_TreeDumpAccess(pstNode, &stDumpBuf, 0);
    if (stDumpBuf.ulInfactLen != stDumpBuf.ulPos)
    {
        free(stDumpBuf.pcBuf);
        CFG_REDUMP_FILE(stDumpBuf, pstNode);
    }

    /* ����file�ӿڱ��� */
    ret = CFG_SaveCurCfg(NULL, stDumpBuf.pcBuf, stDumpBuf.ulInfactLen);
    if (CFG_OK != ret)
    {
        CFG_ERR(ret);
    }

    free(stDumpBuf.pcBuf);

    /* Added by yjs 20100115 config change so notify snmpd */
    FILE *fp;
    fp = fopen( "/var/tmp/cfgchg.smp", "w" );
	if( fp != NULL )
	{
		fprintf( fp, "%d", 1 );
		fclose( fp );
	}
    
    if( ( fp = fopen( "/var/tmp/tr064_change", "w" ) ) != NULL )
    {
        if( fp != NULL )
		{
			fprintf( fp, "%d", 1 );
			fclose( fp );
		}
    }
    
    return ret;
}
#ifdef CONFIG_LED_FACTORT_FLASHING 
/*************************************************************************
����: Telefonica������������ʱ�����е�led����2HZ��˸��
����: ��
����: ��
��ע:
*************************************************************************/
void Restore_Factory_Led_Flashing()
{
	char cmd[128] = {0};
	char temp[64] = {0};
	int i =0;

	sprintf(cmd, "for i in ");
	for(i=0;i<led_end;i++)
		{
		sprintf(temp, "%d ", i);
		strcat(cmd, temp);
		}
	sprintf(temp, "; do echo $i %d > /proc/led; done", led_flash_2hz_trig);
	/* ͨ��дproc�ļ�ϵͳ������LED */
	strcat(cmd, temp);
	tbsSystem(cmd, 0);
}
#endif
/* �ָ��������� */
/***********************************************************
�ӿ�:   �ָ���������
����:   ��
����ֵ: 0:�ɹ�
        ����:ʧ��
***********************************************************/
CFG_RET CFG_RecoverFactSet(const char *pcAccessor)
{
    long ret = CFG_OK;

#ifdef CONFIG_LED_FACTORT_FLASHING      
    Restore_Factory_Led_Flashing();
#endif

    /* ��������������Ϊ��ǰ���� */
    ret = CFG_RecoverFactCfg(pcAccessor);
    if (CFG_OK != ret)
    {
        CFG_ERR(ret);
    }

    return ret;
}


/* �����ǰ���� */
/***********************************************************
�ӿ�:   �����ǰ����
����:   ��
����ֵ: 0:�ɹ�
        ����:ʧ��
***********************************************************/
CFG_RET CFG_ClearCfg(void)
{
    CFG_RET ret = CFG_OK;

    ret = CFG_ClearCurCfg();
    if (CFG_OK != ret)
    {
        CFG_ERR(ret);
    }
    
    /*
     ����Reset flag,��������д����
     ֻ�������������� add by pengyao 20121015
    */
    g_bResetedFlag = 1;

    return ret;
}

#ifdef CONFIG_APPS_LOGIC_WANSELECT

/* ����������� */
/***********************************************************
�ӿ�:   �����������
����:   ��
����ֵ: 0:�ɹ�
        ����:ʧ��
***********************************************************/
CFG_RET CFG_ClearAllCfg(void)
{
    CFG_RET ret = CFG_OK;

    ret = CFG_ClearAllTheCfg();
    if (CFG_OK != ret)
    {
        CFG_ERR(ret);
    }

    return ret;
}
#endif

/***********************************************************
�ӿ�:   �жϽڵ��Ƿ�Ϊ���ֽڵ�
����:   pcNode, �ڵ���
����ֵ: 0:����
        ����:��
***********************************************************/
unsigned char CFG_IsNodeNum(const char *pcNode)
{
    while ('\0' != *pcNode)
    {
        if (*pcNode > '9' || *pcNode < '0')
        {
            return 0;
        }
        pcNode++;
    }

    return 1;
}

/***********************************************************
�ӿ�:   ������strtok�Ĺ���, ר����cfgģ�������'.'�ָ���·��
����:   pcStr, ·��
        cKey, �ָ���
        ppcNext, ��һ��·��
����ֵ: �������Ľڵ���
***********************************************************/
char *CFG_StrChrTok(char *pcStr, char cKey, char **ppcNext)
{
    char *pcPos = NULL;

    if (NULL == pcStr || '\0' == pcStr[0] || cKey == pcStr[0])
    {
        return NULL;
    }

    pcPos = strchr(pcStr, cKey);
    if (NULL != pcPos)
    {
        *pcPos = '\0';
        *ppcNext = pcPos + 1;
    }
    else
    {
        *ppcNext = NULL;
    }

    return pcStr;
}

/*************************************************************************
Function:      scew_element *scew_element_insert_elem(scew_element* element,
                                 scew_element *left, scew_element* new_elem)
Description:   ���ڵ㵽��һ�ڵ�ָ���ӽڵ�ĺ���
Calls:         ��
Data Accessed:
Data Updated:
Input:         element, �����ڵ��ַ
               left, �����λ��
               new_elem, �²���Ľڵ��ַ
Output:        ��
Return:        new_elem
Others:
*************************************************************************/
scew_element *scew_element_insert_elem(scew_element* element,
                           scew_element *left, scew_element* new_elem)
{
    scew_element* current = NULL;

    element->n_children++;

    new_elem->parent = element;
    if (element->last_child == NULL)
    {
        element->last_child = new_elem;
    }
    else
    {
        if (NULL != left)
        {
            if (NULL != left->right)
            {
                left->right->left = new_elem;
                new_elem->right = left->right;
            }
            else
            {
                element->last_child = new_elem;
            }
            new_elem->left = left;
            left->right = new_elem;
        }
        else
        {
            current = element->child;
            current->left = new_elem;
            new_elem->right = current;
        }
    }
    if (NULL == left || NULL == element->child)
    {
        element->child = new_elem;
    }

    return new_elem;
}

#ifdef CONFIG_APPS_CMM_NODE_FIX
/*************************************************************************
Function: CFG_RET CFG_RecoverNodeValueFromDefault(const char *pszPath, const char **pValue, void **ppvNode)
Description: �������ǰ������ȱ���������ܵ����ýڵ㣬����ӳ����������ϻָ�
Calls:
Data Accessed:
Data Updated:
Input: pszPath ��ǰ�����Ϸ��ʲ����Ľڵ�·��
Output: pValue �ӳ��������ϻָ��Ľڵ��ֵ�����ò���
        ppvNode �ӳ��������ϻָ��Ľڵ�ĵ�ַ�����û�ã����Կ���ɾ����
Return: �ɹ�ΪCFG_OK
Others: ���������Ҫ���ڻָ���ʵ���ڵ�
*************************************************************************/
CFG_RET CFG_RecoverNodeValueFromDefault(const char *pszPath, const char **pValue, void **ppvNode)
{
    char szPathTmp[MAX_PATH_LEN] = {0};
    char *pszTmp = NULL;
    char *pcPathTmp = szPathTmp;
    scew_element *pstBig = NULL;
    scew_element *pstDef = NULL;
    scew_element *pstChild = NULL;
    long ret = CFG_OK;

    CFG_TRACE("%d %s Entry\n", __LINE__, __FUNCTION__);
    pstBig = scew_tree_root(g_pstCfgTree);
    pstDef = scew_tree_root(g_pstDefaultTree);

    strcpy(pcPathTmp, pszPath);
    pszTmp = CFG_StrChrTok(pcPathTmp, DELIMIT_C, &pcPathTmp);
    if (NULL != pszTmp)
    {
        ret = strcmp(scew_element_name(pstBig), pszTmp);
        if (0 != ret)
        {
            CFG_ERR(ERR_CFG_PATH_NOT_EXSITED, "%s", pszPath);
            return ERR_CFG_PATH_NOT_EXSITED;
        }
        
        //ͨ����ֽڵ�·���������ҿ��Դӳ��������лָ��Ľڵ�
        while (pszTmp = CFG_StrChrTok(pcPathTmp, DELIMIT_C, &pcPathTmp),
                   NULL != pszTmp)
        {
            //���������������Ƿ�������ڵ�
            pstDef = scew_element_by_name(pstDef, pszTmp);
            if (NULL != pstDef)
            {
                //��鵱ǰ���������Ƿ�������ڵ�
                pstChild = scew_element_by_name(pstBig, pszTmp);
                if (NULL == pstChild)
                {
                    //��ʼ�ӳ����������лָ��ڵ�
                    ret = CFG_DupAccess(pstDef, &pstChild);
                    if (CFG_OK != ret)
                    {
                        CFG_ERR(ret);
                        return ret;
                    }
                    (void)scew_element_add_elem(pstBig, pstChild);
                    if (CFG_IS_NODE_NUM(pszTmp))
                    {
                        CFG_UpdateMaxIdx(pstBig);
                        (void)CFG_UpdateObjCurInstCount(pstBig);
                    }
                    //�������ֱ��break��֮����ӽڵ��Ѿ�ͬʱ�����ˡ�
                }
                pstBig = pstChild;
            }
            else
            {
                return CFG_FAIL;
            }
        }
    }

    *pValue = scew_element_contents(pstChild);
    if (NULL != ppvNode)  //û�ÿ���ɾ��
        *ppvNode = pstChild;
    CFG_TRACE("%d %s Exit\n", __LINE__, __FUNCTION__);
    return CFG_OK;
}

/*************************************************************************
Function: CFG_RET CFG_RecoverNodeValueFromMid(const char *pszPath, const char **pValue, void **ppvNode)
Description: �������ǰ������ȱ���������ܵ����ýڵ㣬�����TR069 Model�ϻָ�
Calls:
Data Accessed:
Data Updated:
Input: pszPath ��ǰ�����Ϸ��ʲ����Ľڵ�·��
Output: pValue ��TR069 Model�ϻָ��Ľڵ��ֵ�����ò���
        ppvNode ��TR069 Model�ϻָ��Ľڵ�ĵ�ַ�����û�ã����Կ���ɾ����
Return: �ɹ�ΪCFG_OK
Others: ���������Ҫ���ڻָ�ʵ���ڵ�
*************************************************************************/
CFG_RET CFG_RecoverNodeValueFromMid(const char *pszPath, const char **pValue, void **ppvNode)
{
    char szPathTmp[MAX_PATH_LEN] = {0};
    char *pszTmp = NULL;
    char *pcPathTmp = szPathTmp;
    scew_element *pstBig = NULL;
    scew_element *pstMid = NULL;
    scew_element *pstChild = NULL;
    long ret = CFG_OK;

    CFG_TRACE("%d %s Entry\n", __LINE__, __FUNCTION__);
    pstBig = scew_tree_root(g_pstCfgTree);
    pstMid = scew_tree_root(CFG_GetMIDTree());

    strcpy(pcPathTmp, pszPath);
    pszTmp = CFG_StrChrTok(pcPathTmp, DELIMIT_C, &pcPathTmp);
    if (NULL != pszTmp)
    {
        ret = strcmp(scew_element_name(pstBig), pszTmp);
        if (0 != ret)
        {
            CFG_ERR(ERR_CFG_PATH_NOT_EXSITED, "%s", pszPath);
            return ERR_CFG_PATH_NOT_EXSITED;
        }

        while (pszTmp = CFG_StrChrTok(pcPathTmp, DELIMIT_C, &pcPathTmp),
                   NULL != pszTmp)
        {
            if (CFG_IS_NODE_NUM(pszTmp)) /* ��ǰ�ڵ������� */
            {
                pstMid = scew_element_by_name(pstMid, NODE_WILDCARD);
            }
            else
            {
                pstMid = scew_element_by_name(pstMid, pszTmp);
            }
            if (NULL != pstMid)
            {
                pstChild = scew_element_by_name(pstBig, pszTmp);
                if (NULL == pstChild)
                {
                    if (CFG_IS_NODE_NUM(pszTmp))
                    {
                        /* ���һ��ʵ������ */
                        ret = CFG_DupAccess(pstMid, &pstChild);
                        if (CFG_OK != ret)
                        {
                            CFG_ERR(ret);
                            return ret;
                        }
                        /* �޸Ľڵ��� */
                        (void)scew_element_set_name(pstChild, pszTmp);
                        (void)scew_element_add_elem(pstBig, pstChild);
                        CFG_UpdateMaxIdx(pstBig);
                        (void)CFG_UpdateObjCurInstCount(pstBig);
                        continue; /* ���ܻ���2�����ϵĽڵ� */
                    }
                    else
                    {
                        /* ���һ������ */
                        ret = CFG_DupAccess(pstMid, &pstChild);
                        if (CFG_OK != ret)
                        {
                            CFG_ERR(ret);
                            return ret;
                        }
                        (void)scew_element_add_elem(pstBig, pstChild);
                        break;
                    }
                }
                pstBig = pstChild;
            }
            else
            {
                return CFG_FAIL;
            }
        }
    }

    *pValue = scew_element_contents(pstChild);
    if (NULL != ppvNode)  //û�ÿ���ɾ��
        *ppvNode = pstChild;
    CFG_TRACE("%d %s Exit\n", __LINE__, __FUNCTION__);
    return CFG_OK;
}

/*************************************************************************
Function: CFG_RET CFG_CheckAndFixNode(scew_element *pstRoot, scew_element *pstMid, scew_element *pstCurrent)
Description: ����TR069 Model����Mid Tree���䵱ǰʵ����ȱ�ٵĽڵ�
Calls: CFG_SearchAndRecoverNodeValueFromMid
Data Accessed:
Data Updated:
Input: pstRoot ��ǰ�������ĸ��ڵ�ָ��
       pstMid TR069 Model�ĸ��ڵ�ָ��
       pstCurrent ��ǰ����������Ҫ��TR069 Model�ϻָ��ڵ��ʵ��ָ��
Output:
Return: �ɹ�ΪCFG_OK
Others: ���������Ҫ���ڻָ�ʵ���ڵ�
*************************************************************************/
CFG_RET CFG_CheckAndFixNode(scew_element *pstRoot, scew_element *pstMid, scew_element *pstCurrent)
{
    long ret = CFG_OK;
    char szPathData[16][256]; //�ö�ά���鱣��·��
    int iDepth = 0;
    scew_element *pstNode = NULL;
    scew_element *pstChild = NULL;
    scew_element *pstNew = NULL;

    memset(szPathData, 0, 16*256);
    CFG_TRACE("%d %s Entry\n", __LINE__, __FUNCTION__);

    //��pstCurrentһ�������ϻ�ȡ��·��
    pstNode = pstCurrent;
    while(pstNode != pstRoot)
    {
        strcpy(szPathData[iDepth], pstNode->name);
        iDepth++;
        pstNode = pstNode->parent;
    }
    //strcpy(szPathData[iDepth], "InternetGatewayDevice"); //����ͷ���(ͷ��㲻��Ҫ����)
    //iDepth++;
    pstNode = pstMid;
    while(iDepth > 0)
    {
        //����szPath����pstMid��ȡ��ָ���Ľڵ�
        if (strlen(szPathData[iDepth - 1]) > 0)
        {
            if (CFG_IS_NODE_NUM(szPathData[iDepth - 1])) /* ��ǰ�ڵ������� */
            {
                pstNode = scew_element_by_name(pstNode, NODE_WILDCARD);
            }
            else
            {
                pstNode = scew_element_by_name(pstNode, szPathData[iDepth - 1]);
            }
            if (NULL == pstNode)
            {
                 //���Mid treeû������ڵ㣬�����޸�
                 CFG_TRACE("CFG : Mid tree do not have node : %s\n", szPathData[iDepth - 1]);
                 return CFG_OK;
            }
        }
        else
        {
            CFG_TRACE("CFG : The path data has empty string.\n");
            ret = CFG_FAIL;
            break;
        }
        CFG_TRACE("%s.", szPathData[iDepth - 1]);
        iDepth--;
    }

    CFG_TRACE("\nCFG : Begin search and fix the node\n");
    //��pstMid��ָ���ڵ���ӽڵ��ϱ�������pstCurrent��������
    while(pstChild = scew_element_next(pstNode, pstChild),
              NULL != pstChild)
    {
        //��������ӽڵ㣬�򲻲���ڵ㣬�����ƽڵ����ѿ���
        if (pstChild->child == NULL)
        {
            if (NULL == scew_element_by_name(pstCurrent, pstChild->name))
            {
                //��ǰ��������û������ڵ㣬��Ҫ��pstMid�ϲ���
                ret = CFG_DupAccess(pstChild, &pstNew);
                if (CFG_OK != ret)
                {
                    CFG_ERR(ret);
                    return ret;
                }
                scew_element_add_elem(pstCurrent, pstNew);
            }
        }
    }

    CFG_TRACE("%d %s Exit\n", __LINE__, __FUNCTION__);
    return ret;
}

/*************************************************************************
Function: CFG_RET CFG_SearchAndRecoverNodeValueFromMid()
Description: ������ǰ������ʵ���ڵ㣬��鲢����ȱ�ٵĽڵ�
Calls:
Data Accessed:
Data Updated:
Input:
Output:
Return: �ɹ�ΪCFG_OK
Others:
    1)���ǲ���ʵ���е����ԣ������Ƿ�ʵ���ڵ㡣
    2)�������������ȫȡ��CFG_RecoverNodeValueFromMid()������
      ��Ȼ��������������ʵ���ڵ㣬������ACL�����
      �µķ��ʿ���ʵ��ʱ����������͸��ǲ����ˡ�
    3)������ڷ�ʵ���ڵ㣬Ҫ��CFG_RecoverNodeValueFromDefault()������
    4)�˽ڵ�ָ����ܣ����������������ִ��һ�μ��ɣ�
      ��Ӳ���������һ�������ʵ�֡�
*************************************************************************/
CFG_RET CFG_SearchAndRecoverNodeValueFromMid()
{
    scew_element *pstRoot = NULL;
    scew_element *pstParent = NULL;
    scew_element *pstMid = NULL;
    scew_element *pstChild = NULL;
    long ret = CFG_OK;

    CFG_TRACE("%d %s Entry\n", __LINE__, __FUNCTION__);
    pstRoot = scew_tree_root(g_pstCfgTree);
    pstMid = scew_tree_root(CFG_GetMIDTree());
    if ((NULL == pstRoot) || (NULL == pstMid))
    {
        CFG_TRACE("%s is NULL\n", (NULL == pstRoot)?"g_pstCfgTree":"s_pstCfgMIDTree");
        return CFG_FAIL;
    }
    pstParent = pstRoot;

    do
    {
        pstChild = scew_element_next(pstParent, pstChild);
        if (NULL != pstChild)
        {
            if (NULL != pstChild->child)
            {
                //�ӽڵ㲻Ϊ��
                pstParent = pstChild;
                pstChild = NULL;
                continue;
            }
        }
        else
        {
            //�ӽڵ������ϣ�������һ��
            pstChild = pstParent;
            pstParent = pstChild->parent;
            //������ڵ��Ѿ������һ���ڵ㣬�����ʵ���ڵ���ж�
            if (NULL != pstChild->right)
                continue;
        }

        if (NULL == pstChild->right)
        {
            //�ֵܽڵ�Ϊ�գ��Ѿ���ͬ������β��
            //�ж��Ƿ���ʵ��
            if (NULL == pstParent)  //���ڵ�Ϊ�գ��ӽڵ����pstRoot
                continue;
            if (CFG_IS_NODE_NUM(scew_element_name(pstParent)))
            {
                ret = CFG_CheckAndFixNode(pstRoot, pstMid, pstParent);
                if (CFG_OK != ret)
                {
                     CFG_TRACE("%s : Fix node failed.\n", __FUNCTION__);
                     break;
                }
            }

            //ȡ���ڵ���ҽڵ㣬���ֵܽڵ�
            //pstChild = pstParent;
            //pstParent = pstChild->parent;
        }
    }while(pstChild != pstRoot);

    CFG_TRACE("%d %s Exit\n", __LINE__, __FUNCTION__);
    return ret;
}
#endif

#ifdef _CFG_DEBUG

CFG_RET CFG_PrintNode(const void *pvTree, const char *pcPath,
                      unsigned long ulLen, void *pvFile)
{
    scew_element *pstNode = NULL;
    CFG_RET ret = CFG_OK;
    ST_CFG_DUMP_BUF stDumpBuf = {NULL, 0, 0, 0};
    char *pcPathTmp = NULL;

    if (NULL == pvFile)
    {
        pvFile = stdout;
    }

    CFG_DUP_PATH(pcPathTmp, pcPath);

    /* ���ҽڵ� */
    ret = CFG_GetNode(pvTree, pcPathTmp, (void **)(void *)&pstNode, NULL, 0);
    CFG_FREE_PATH(pcPathTmp);
    if (CFG_OK != ret || NULL == pstNode)  /* û���ҵ� */
    {
        printf("Node not failed\n");
        return  ret;
    }

    stDumpBuf.pcBuf = malloc(ulLen);
    if (NULL == stDumpBuf.pcBuf)
    {
        printf("Malloc buffer failed\n");
        return ERR_CFG_MALLOC_FAIL;
    }
    stDumpBuf.ulLen = ulLen;

    g_pcCfgAccessor = NULL;
    (void)CFG_TreePrintAccess(pstNode, &stDumpBuf, 0);
    if (stDumpBuf.ulPos == stDumpBuf.ulLen)
    {
        stDumpBuf.pcBuf[stDumpBuf.ulPos - 1] = '\0';
    }
    else
    {
        stDumpBuf.pcBuf[stDumpBuf.ulPos] = '\0';
    }
    fprintf(pvFile, "%s", stDumpBuf.pcBuf);
    fprintf(pvFile, "\n");

    free(stDumpBuf.pcBuf);
    return CFG_OK;
}

#endif




#ifdef _cplusplus
    #if _cplusplus
        }
    #endif
#endif


