/**********************************************************************
 Copyright (c), 1991-2009, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 �ļ�����: tbsutil.c
 �ļ�����: ���������ķ�װ���ṩ����ģ��ʹ��

 �޶���¼:
        1. ����: all
           ����: 2008-08-07
           ����: �����ļ�

**********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <syslog.h>
#include <asm/types.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <linux/sockios.h>
//#include <linux/ethtool.h>
#include <regex.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <resolv.h>
#include <netdb.h>
#include <fcntl.h>
#include <signal.h>

#include "tbsutil.h"
#include "tbserror.h"
#include "common.h"
#include "autoconf.h"

/*=========================================================================*/
/*  ��������: tbsMatch                                                     */
/*  ��������: �ַ���ƥ�亯��                                               */
/*  ��  ��  : pszString Ҫƥ����ַ���                                     */
/*            pszPattern   ƥ�����                                        */
/*  ��  ��  : BOOL   TRUE:ƥ��ɹ���FALSE����ƥ��                          */
/*  ��  ��  : tbs / 2008-04-19                                             */
/*=========================================================================*/
BOOL tbsMatch(const char *pszString, const char *pszPattern)
{
    int status;
    regex_t re;

    if (NULL == pszString || NULL == pszPattern)
    {
        return FALSE;
    }

    if (regcomp(&re, pszPattern, REG_EXTENDED|REG_NOSUB) != 0)
    {
        return FALSE; /* report error */
    }

    status = regexec(&re, pszString, (size_t) 0, NULL, 0);
    regfree(&re);

    if (status != 0)
    {
        return FALSE; /* report error */
    }

    return TRUE; /* match */
}

#ifdef _TBS_ENV_PC
#define execv(sh, cmd) \
{ \
    if (!printFlag) \
    { \
        printf("execv: %s\n", cmd[2]); \
    } \
    if (0 == strncmp(cmd[2], "kill ", 5) \
        || 0 == strncmp(cmd[2], "killall ", 8)) \
    { \
        system(cmd[2]); \
    } \
    else \
    { \
        execl("execl_null", cmd[2], NULL); \
    } \
}
#endif
/*=========================================================================*/
/*  ��������: tbsSystem                                                    */
/*  ��������: ϵͳ����system�����                                         */
/*  ��  ��  : char* ���������                                             */
/*            int   �����ӡ��־��0������ӡ����������ӡ                    */
/*  ��  ��  : int   0:ִ�гɹ�������������                                 */
/*  ��  ��  : tbs / 2007-12-14                                             */
/*=========================================================================*/
int tbsSystem(char *command, int printFlag)
{
	int pid = 0, status = 0;

    if( !command )
    {
        printf("tbsSystem: Null Command, Error!");
        return -1;
    }

	pid = fork();
  	if ( pid == -1 )
  	{
		return -1;
	}

  	if ( pid == 0 )
  	{
        char *argv[4];
    	argv[0] = "sh";
    	argv[1] = "-c";
    	argv[2] = command;
    	argv[3] = 0;
    	if (printFlag)
    	{
	        printf("[system]: %s\r\n", command);
            syslog(LOG_DEBUG, command);
        }
    	execv("/bin/sh", argv);
    	exit(127);
	}

        errno = 0;
  	/* wait for child process return */
  	do
  	{
	  	if ( waitpid(pid, &status, 0) == -1 )
    	{
	    	if ( errno != EINTR )
    		{
            	return -1;
      	    }
	    }
    	else
    	{
	    	return status;
		}
	} while ( 1 );

	return status;
}
/**************************************************************************
����: ͨ���ӿ�����ȡ�ӿڶ�Ӧ��MAC��ַ
����: pszIfName  ��Žӿ����Ļ�����ָ��(����)
       pszValue   ���MAC��ַ�Ļ�����ָ��(���)
����: �ɹ� -- TBS_SUCCESS
       ʧ�� -- ����������
��ע: ���øú����л���������ķ���,������Ӧ��ΪpszValueָ��Ļ���������
       �㹻�Ŀռ������MAC��ַ
***************************************************************************/
int tbsGetMacAddr(const char *pszIfName, char *pszMac) 
{
     struct ifreq stIfr;
     int iSockfd;

     strcpy(stIfr.ifr_name, pszIfName);

     /* �����׽���ʧ��*/
     if ((iSockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
     {
         return TR069_ERRNO_CPE_ERR;
     }

     if (ioctl(iSockfd, SIOCGIFHWADDR, &stIfr) < 0)
     {
         close(iSockfd);
         return TR069_ERRNO_CPE_ERR;
     }

     /* ��ȡMAC��ַ*/
     sprintf(pszMac, "%02x:%02x:%02x:%02x:%02x:%02x",
                         (unsigned char)stIfr.ifr_hwaddr.sa_data[0],
                         (unsigned char)stIfr.ifr_hwaddr.sa_data[1],
                         (unsigned char)stIfr.ifr_hwaddr.sa_data[2],
                         (unsigned char)stIfr.ifr_hwaddr.sa_data[3],
                         (unsigned char)stIfr.ifr_hwaddr.sa_data[4],
                         (unsigned char)stIfr.ifr_hwaddr.sa_data[5]);
     close(iSockfd);

     return TBS_SUCCESS;
}

/**************************************************************************
����: ��IP�ַ����н���ip
����: const char *pcIp,                 IP�ַ���;
����: ip��ַ
��ע:
***************************************************************************/
unsigned long tbsCnvtIpFromStr(const char *pcIp)
{
    unsigned long ip[4];
    unsigned long retIp;
    int ret;
#ifndef _TBS_ENV_PC
    unsigned char pos[] = {24, 16, 8, 0};
#else
    unsigned char pos[] = {0, 8, 16, 24};
#endif

    ret = sscanf(pcIp, "%lu.%lu.%lu.%lu",
           &ip[0], &ip[1], &ip[2], &ip[3]);
    if(ret == 0 || ret < 4)
    {
    		return 0;
    }
    retIp = (ip[0] << pos[0]) | (ip[1] << pos[1]) | (ip[2] << pos[2]) | (ip[3] << pos[3]);

    return retIp;
}

/**************************************************************************
����: ��ipתΪ�ַ���
����:    unsigned long ulIp      IP��ַ
                char *pszIp                 IP�ַ���;
����: ��
��ע:
***************************************************************************/
void tbsCnvtIpToStr(unsigned long ulIp, char *pszIp)
{
	char szSectIp[4][4];
	int  iSectIp, i;
	unsigned long ulTempIp = ulIp;

	for(i=0; i<4; i++)
	{
		iSectIp = ulTempIp%256;
		snprintf(szSectIp[i], 4, "%d", iSectIp);

		ulTempIp -= iSectIp;
		ulTempIp /= 256;
	}

	snprintf(pszIp, 20, "%s.%s.%s.%s", szSectIp[3], szSectIp[2], szSectIp[1], szSectIp[0]);

}


/*=========================================================================*/
/*  ��������: tbsSplitString                                               */
/*  ��������: ����ָ���ַ��ָ��ַ���                                       */
/*  ��  ��  : str ��Ҫ�ָ���ַ���ָ��                                     */
/*            substr ���ָ��ָ�õ�Ԫ��ָ��                              */
/*            delimit �ָ���                                               */
/*            max_count ���Ԫ�ظ���                                       */
/*  ��  ��  : void  ��                                                     */
/*  ��  ��  : tbs / 2007-12-14                                             */
/*  ��  ��  : zoudeming /2007-12-27                                        */
/*            ��˫��ѭ����Ϊ��ѭ������������ĳЩ�β�                       */
/*            zoudeming /2008-1-3                                          */
/*            ����ع�                                                     */
/*=========================================================================*/
int tbsSplitString(char *str, char *substr[], const char delimit, const int max_count)
{
	int count = 0;

	if ( str == NULL || substr == NULL )
		return 0;

	if ( 0 < max_count )
			substr[0] = str;

	count = 1;
	while ( *str )
	{

		while ( *str != delimit && *str != '\0' )
			str++;

		if ( *str == delimit )
		{
			*str++ = '\0';
			if(count < max_count)
				substr[count] = str;
			count++;
		}
	}

	return count;
}


BOOL tbsStrToBool(const char *pcValue)
{
	if ( pcValue == NULL || strlen(pcValue) == 0)
	{
		return FALSE;
	}

	if ( strcasecmp(pcValue, "1") == 0 || strcasecmp(pcValue, "true") == 0 )
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


/**************************************************************************
����: ����������1�ĸ���,�Ƿ������򷵻ظ���
����:    const char *pcMask      �����ַ���
����: ������1�ĸ���
��ע:
***************************************************************************/
int tbsGetBitsByMask(const char *pcMask)
{
    int i, iBits = 32, iSet = 0;
    unsigned long ulMask;

    if (NULL == pcMask)
    {
        return TBS_PARAM_ERR;
    }

    if (!inet_aton(pcMask, (struct in_addr *)((void *)&ulMask)))
    {
        return TBS_PARAM_ERR;
    }

    ulMask = ntohl(ulMask);

    for (i = 0; i < 32; i++)
    {
        if (ulMask & (1 << i))
        {
            iSet = 1;
        }
        else
        {
            if (iSet)
            {
                return TBS_PARAM_ERR;;
            }

            iBits--;
        }
    }

    return iBits;
}




/****************************************************
*
*   ��麯��
*
****************************************************/

/* ���IP�Ϸ��� */
BOOL tbsCheckIp(const char *pcip)
{
	struct in_addr addr;

	if (!tbsMatch(pcip, "^([0-9]{1,3}\\.){3}[0-9]{1,3}$"))
        return FALSE;

	if(!inet_aton(pcip, &addr))
		return FALSE;

	return TRUE;
}

BOOL tbsCheckPort(const char *pszString)
{
    char *pszPattern = "^[1-9][0-9]\{0,4}$";

    if (TRUE != tbsMatch(pszString, pszPattern))
        return FALSE;

    if (atoi(pszString) < 1 || atoi(pszString) > 65535)
        return FALSE;

    return TRUE;
}

BOOL tbsCheckPriority(const char *pszString)
{
    char *pszPattern = "^[1-8]$";

    if (TRUE != tbsMatch(pszString, pszPattern))
		return FALSE;

	if (atoi(pszString) < 1 || atoi(pszString) > 8)
	{
        return FALSE;
	}

    return TRUE;
}


BOOL tbsCheckMask(const char *pcValue)
{
	unsigned long ulMask = 0;

	if ( pcValue == NULL || strlen(pcValue) == 0 )
	{
		return FALSE;
	}

	if(FALSE == tbsCheckIp(pcValue))
	{
		return FALSE;
	}

	ulMask = tbsCnvtIpFromStr(pcValue);
	if ( ulMask == 0 )
	{
		return FALSE;
	}

#ifdef _TBS_ENV_PC
    ulMask = htonl(ulMask);
#endif

	while ( ulMask & 0x80000000 )
	{
		ulMask <<= 1;
	}

	ulMask &= 0xffffffff;
	if ( ulMask == 0 )
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}



BOOL tbsCheckEnable(const char *pszValue)
{
    if ( NULL == pszValue )
    {
        return FALSE;
    }

    if ( 0 == strcasecmp(pszValue, "1") || 0 == strcasecmp(pszValue, "true") ||
          0 == strcasecmp(pszValue, "0") || 0 == strcasecmp(pszValue, "false") )
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

BOOL tbsCheckInt(const char *pszValue)
{
    int iValue = 0;

    if ( NULL == pszValue )
    {
        return FALSE;
    }

    return (1 == sscanf(pszValue, "%d", &iValue));
}

BOOL tbsCheckUInt(const char *pszValue)
{
    unsigned int nValue = 0;

    if ( NULL == pszValue )
    {
        return FALSE;
    }

    return (1 == sscanf(pszValue, "%u", &nValue));
}

BOOL tbsCheckMac(const char *pcMac)
{
  int arr[6], index;
  const char *pattern = "^[0-9a-fA-F]{1,2}:[0-9a-fA-F]{1,2}:[0-9a-fA-F]{1,2}:[0-9a-fA-F]{1,2}:[0-9a-fA-F]{1,2}:[0-9a-fA-F]{1,2}$";

  if(pcMac == NULL)
    return FALSE;

  if(FALSE == tbsMatch(pcMac, pattern))
  {
  	return FALSE;
  }


  if(sscanf(pcMac,"%x:%x:%x:%x:%x:%x",\
           &arr[0],&arr[1],&arr[2],&arr[3],&arr[4],&arr[5]) != 6 )
    return FALSE;

#if 0
  /*���MAC��ַ�Ƿ�Ϊ�ಥMAC��ַ*/
  if( arr[0]%2 > 0 )
    return FALSE;
#endif

  /*���MAC��ַ�Ƿ�Ϸ�*/
  for(index = 1; index < 6; index ++)
  {
  	if(arr[index] > 0xff)
  	{
  		return FALSE;
  	}
  }
  return TRUE;
}




BOOL tbsCheckIpList(const char *pszIpList, int iMaxCount)
{
    return RET_SUCCEED(tbsCheckIpListEx(pszIpList, iMaxCount));
}

BOOL tbsCheckMacList(const char *pszMacList, int iMaxCount)
{
    return RET_SUCCEED(tbsCheckMacListEx(pszMacList, iMaxCount));
}


/*
����: tbsCheckRegex
����: ʹ�ô����������ʽƥ��Ҫ����ֵ,��ƥ���򷵻�ָ���Ĵ�����
����: pszValue      Ҫ����ֵ
      pszPattern    ������ʽ
      iErrNo        ������
����: �ɹ�,TBS_SUCCESS
      ʧ��,������
*/
int tbsCheckRegex(const char *pszValue, const char *pszPattern, int iErrNo)
{
    if ( !tbsMatch(pszValue, pszPattern) )
        return iErrNo;
    return TBS_SUCCESS;
}

/*
���IP�Ϸ��ԣ�������������ip
*/
int tbsCheckIpEx(const char *pszValue)
{
    if ( NULL == pszValue || 0 == strlen(pszValue))
    {
        return ERR_CAN_NOT_EMPTY;
    }

    if ( !tbsCheckIp(pszValue) )
    {
        printf("Invalid ip");
        return ERR_IP_INVALID_FORMAT;
    }

    return TBS_SUCCESS;
}


/*
���IP�Ϸ��ԣ�������Ϊ�鲥�㲥�������ַ
*/
int tbsCheckHostIpEx(const char *pszValue)
{
    unsigned long ulIp = 0;

    if ( NULL == pszValue || 0 == strlen(pszValue))
    {
        return ERR_CAN_NOT_EMPTY;
    }

    if ( !tbsCheckIp(pszValue) )
    {
        printf("Invalid ip\n");
        return ERR_IP_INVALID_FORMAT;
    }

    ulIp = tbsCnvtIpFromStr(pszValue);
    /*
      IP��Χ: 1.*.*.*   -  127.*.*.*,
              128.*.*.* -  224.*.*.*
    */
    if ((ulIp > 0x01000000 && ulIp < 0x7f000000)
        || (ulIp > 0x80000000 && ulIp < 0xe0000000) )
    {
        /* ����Ϊ*.*.*.0 ���� *.*.*.255 */
        if ( 0 != (ulIp&0x000000ff)
            && 0xff != (ulIp&0x000000ff) )
            return TBS_SUCCESS;
        else
            return ERR_IP_IS_CAST_OR_NET_ADDR;
    }
    else
        return ERR_IP_IS_WRONG_NET_SECTION;
}

/*
���DNS IP�Ϸ��ԣ�������Ϊ�鲥�㲥�������ַ,����0.0.0.0��IP����
*/
int tbsCheckDNSHostIpEx(const char *pszValue)
{
    unsigned long ulIp = 0;

    if ( NULL == pszValue || 0 == strlen(pszValue))
    {
        return ERR_CAN_NOT_EMPTY;
    }

    if (!safe_strcmp(pszValue, "0.0.0.0"))
    {
        return TBS_SUCCESS;
    }

    if ( !tbsCheckIp(pszValue) )
    {
        printf("Invalid ip");
        return ERR_IP_INVALID_FORMAT;
    }

    ulIp = tbsCnvtIpFromStr(pszValue);
    /*
      IP��Χ: 1.*.*.*   -  127.*.*.*,
              128.*.*.* -  224.*.*.*
    */
    if ((ulIp > 0x01000000 && ulIp < 0x7f000000)
        || (ulIp > 0x80000000 && ulIp < 0xe0000000) )
    {
        /* ����Ϊ*.*.*.0 ���� *.*.*.255 */
        if ( 0 != (ulIp&0x000000ff)
            && 0xff != (ulIp&0x000000ff) )
            return TBS_SUCCESS;
        else
            return ERR_IP_IS_CAST_OR_NET_ADDR;
    }
    else
        return ERR_IP_IS_WRONG_NET_SECTION;
}


int tbsCheckPortEx(const char *pszString)
{
    char *pszPattern = "^[1-9][0-9]\{0,4}$";

    if (TRUE != tbsMatch(pszString, pszPattern))
        return ERR_PORT_INVALID_FORMAT;

    if (atoi(pszString) < 1)
        return ERR_PORT_TOO_SMALL;

    if( atoi(pszString) > 65535)
        return ERR_PORT_TOO_BIG;

    return TBS_SUCCESS;
}

int  tbsCheckPriorityEx(const char *pszString)
{
    char *pszPattern = "^[1-8]$";

    if (TRUE != tbsMatch(pszString, pszPattern))
		return ERR_PRIORITY_INVALID_VALUE;

    if (atoi(pszString) < 1 )
        return ERR_PRIORITY_TOO_SMALL;

    if(atoi(pszString) > 8)
        return ERR_PRIORITY_TOO_BIG;

    return TBS_SUCCESS;
}


int tbsCheckMaskEx(const char *pcValue)
{
	unsigned long ulMask = 0;

	if ( pcValue == NULL || strlen(pcValue) == 0 )
	{
		return ERR_CAN_NOT_EMPTY;
	}

	if(FALSE == tbsCheckIp(pcValue))
	{
		return ERR_MASK_INVALID_FORMAT;
	}

	ulMask = tbsCnvtIpFromStr(pcValue);
	if ( ulMask == 0 )
	{
		return ERR_MAKS_ALL_ZERO;
	}

#ifdef _TBS_ENV_PC
    ulMask = htonl(ulMask);
#endif

	while ( ulMask & 0x80000000 )
	{
		ulMask <<= 1;
	}

	ulMask &= 0xffffffff;
	if ( ulMask == 0 )
	{
		return TBS_SUCCESS;
	}
	else
	{
		return ERR_MASK_INVALID_VALUE;
	}
}



int tbsCheckEnableEx(const char *pszValue)
{
 
    if ( NULL == pszValue )
    {
        return ERR_ENABLE_EMPTY;
    }

    if ( 0 == strcasecmp(pszValue, "1") || 0 == strcasecmp(pszValue, "true") ||
          0 == strcasecmp(pszValue, "0") || 0 == strcasecmp(pszValue, "false") )
    {
        return TBS_SUCCESS;
    }
    else
    {
        return ERR_ENABLE_INVALID_VALUE;
    }
}



/*********************************************************************
*  ����: ���Э�������Ƿ�Ϸ�
*  ����: pszValue, Э���ַ���
*  ����: const char *
*  ��ע: �Ϸ�ֵΪIPv4, IPv6, IPv4&6
*********************************************************************/
 int tbsCheckProtoType(const char * pszValue)
{
    if (NULL == pszValue)
    {
        return TBS_NULL_PTR;
    }

    if (!strcasecmp(pszValue, STR_PROTO_TYPE_IPV4) ||
        !strcasecmp(pszValue, STR_PROTO_TYPE_IPV6) ||
        !strcasecmp(pszValue, STR_PROTO_TYPE_IPV4_6))
    {
        return TBS_SUCCESS;
    }

    return ERR_INVALID_PROTO_TYPE;
}

int tbsCheckIntEx(const char *pszValue)
{
    int iValue = 0;

    if ( NULL == pszValue )
    {
        return ERR_INT_INVALID_VALUE;
    }

    if(1 == sscanf(pszValue, "%d", &iValue))
        return TBS_SUCCESS;
    else
        return ERR_INT_INVALID_VALUE;
}


int tbsCheckUIntEx(const char *pszValue)
{
    unsigned int nValue = 0;

    if ( NULL == pszValue )
    {
        return ERR_UINT_INVALID_VALUE;
    }

    if(1 == sscanf(pszValue, "%u", &nValue))
        return TBS_SUCCESS;
    else
        return ERR_UINT_INVALID_VALUE;
}


int tbsCheckIntRangeEx(const char *pszValue, int nMin, int nMax)
{
    unsigned int iValue = 0;

    if ( RET_FAILED(tbsCheckIntEx(pszValue)) )
        return ERR_INT_INVALID_VALUE;

    sscanf(pszValue, "%d", &iValue);
    if ( iValue > nMax || iValue <nMin )
        return ERR_OUT_OF_RANGE;
    else
        return TBS_SUCCESS;
}


int  tbsCheckMacEx(const char *pcMac)
{
    int arr[6], index;
    const char *pattern = "^[0-9a-fA-F]{1,2}:[0-9a-fA-F]{1,2}:[0-9a-fA-F]{1,2}:[0-9a-fA-F]{1,2}:[0-9a-fA-F]{1,2}:[0-9a-fA-F]{1,2}$";

    if(pcMac == NULL)
    return ERR_MAC_INVALID_VALUE;

    if(FALSE == tbsMatch(pcMac, pattern))
    {
    	return ERR_MAC_INVALID_VALUE;
    }

    if(sscanf(pcMac,"%x:%x:%x:%x:%x:%x",\
           &arr[0],&arr[1],&arr[2],&arr[3],&arr[4],&arr[5]) != 6 )
    return ERR_MAC_INVALID_VALUE;
    
#if 0
    /*���MAC��ַ�Ƿ�Ϊ�ಥMAC��ַ*/
    if( arr[0]%2 > 0 )
    return FALSE;
#endif

    /*���MAC��ַ�Ƿ�Ϸ�*/
    for(index = 1; index < 6; index ++)
    {
    	if(arr[index] > 0xff)
    	{
    		return ERR_MAC_INVALID_VALUE;
    	}
    }
    /* ���MAC��ַ�Ƿ���ȫ0 */
    if ((0==arr[0]) && (0==arr[1]) && (0==arr[2]) && (0==arr[3]) && (0==arr[4]) && (0==arr[5]))
    {
        return ERR_MAC_INVALID_VALUE;
    }
    return TBS_SUCCESS;
}


/*
 ip�б��ʽ:ip1,ip2
 ip�б��в������ظ�ip
*/
int tbsCheckIpListEx(const char *pszIpList, int iMaxCount)
{
    char szIPAddrs[MAX_IP_LIST_LEN] = {0};
    char *pszIP[MAX_IP_COUNT] = {NULL};
    int iCount = 0;
    int i = 0;
    int j = 0;

    if ( NULL == pszIpList || 0 == strlen(pszIpList))
    {
        return ERR_CAN_NOT_EMPTY;
    }

    safe_strncpy(szIPAddrs, pszIpList, MAX_IP_LIST_LEN);
    iCount = tbsSplitString(szIPAddrs, pszIP, ',', iMaxCount);
    if ( iCount > iMaxCount )
    {
        printf("Error: ip_addr count error\n");
        return ERR_IPLIST_TOO_MANY;
    }

    for (i = 0; i < iCount; i++)
    {
        if ( !pszIP[i] || !tbsCheckIp(pszIP[i]) )
        {
            return ERR_IPLIST_INVALID_FORMAT;
        }

        /* ����ظ� */
        for ( j = i+1; j < iCount; j++ )
        {
            if ( strcmp(pszIP[i], pszIP[j]) == 0 )
                return ERR_LIST_HAS_REPEAT;
        }
    }

    return TBS_SUCCESS;
}

/*
 ip�б��ʽ:ip1,ip2
 ip�б��е�ip������hostip(����Ϊ�㲥��ַ,�鲥��ַ�������Ƿ�ip)
 ip�б��в������ظ�ip
*/

int tbsCheckHostIpListEx(const char *pszIpList, int iMaxCount)
{
    char szIPAddrs[MAX_IP_LIST_LEN] = {0};
    char *pszIP[MAX_IP_COUNT] = {NULL};
    int iCount = 0;
    int i = 0;
    int j = 0;

    if ( NULL == pszIpList || 0 == strlen(pszIpList))
    {
        return ERR_CAN_NOT_EMPTY;
    }

    safe_strncpy(szIPAddrs, pszIpList, MAX_IP_LIST_LEN);
    iCount = tbsSplitString(szIPAddrs, pszIP, ',', iMaxCount);
    if ( iCount > iMaxCount )
    {
        printf("Error: ip_addr count error\n");
        return ERR_IPLIST_TOO_MANY;
    }

    for (i = 0; i < iCount; i++)
    {
        if ( !pszIP[i]
            || RET_FAILED(tbsCheckHostIpEx(pszIP[i])) )
        {
            return ERR_IPLIST_INVALID_FORMAT;
        }

        /* ����ظ� */
        for ( j = i+1; j < iCount; j++ )
        {
            if ( strcmp(pszIP[i], pszIP[j]) == 0 )
                return ERR_LIST_HAS_REPEAT;
        }
    }

    return TBS_SUCCESS;
}

/*
 ip�б��ʽ:ip1,ip2
 ip�б��е�ip������hostip(����Ϊ�㲥��ַ,�鲥��ַ�������Ƿ�ip)
 ip�б��в������ظ�ip,�����ж�0.0.0.0��IP,��������IP����
*/
int tbsCheckDNSHostIpListEx(const char *pszIpList, int iMaxCount)
{
    char szIPAddrs[MAX_IP_LIST_LEN] = {0};
    char *pszIP[MAX_IP_COUNT] = {NULL};
    int iCount = 0;
    int i = 0;
    int j = 0;

    if ( NULL == pszIpList || 0 == strlen(pszIpList))
    {
        return ERR_CAN_NOT_EMPTY;
    }

    safe_strncpy(szIPAddrs, pszIpList, MAX_IP_LIST_LEN);
    iCount = tbsSplitString(szIPAddrs, pszIP, ',', iMaxCount);
    if ( iCount > iMaxCount )
    {
        printf("Error: ip_addr count error\n");
        return ERR_IPLIST_TOO_MANY;
    }

    for (i = 0; i < iCount; i++)
    {
        if ( !pszIP[i]
            || RET_FAILED(tbsCheckDNSHostIpEx(pszIP[i])) )
        {
            return ERR_IPLIST_INVALID_FORMAT;
        }

        /* ����ظ� */
        for ( j = i+1; j < iCount; j++ )
        {
            if ( strcmp(pszIP[i], pszIP[j]) == 0 )
                return ERR_LIST_HAS_REPEAT;
        }
    }

    return TBS_SUCCESS;
}


int tbsCheckMacListEx(const char *pszMacList, int iMaxCount)
{
    char szMacList[MAX_MAC_LIST_LEN] = {0};
    char *apszMac[MAX_MAC_COUNT] = {NULL};
    int iCount = 0;
    int i = 0;
    int j = 0;

    if ( NULL == pszMacList )
    {
        return ERR_MACLIST_INVALID_FORMAT;
    }

    if ( 0 == strlen(pszMacList) )
    {
        return ERR_MACLIST_INVALID_FORMAT;
    }

    safe_strncpy(szMacList, pszMacList, MAX_MAC_LIST_LEN);
    iCount = tbsSplitString(szMacList, apszMac, ',', iMaxCount);
    if ( iCount > iMaxCount )
    {
        printf("Error: ip_addr count error\n");
        return ERR_MACLIST_TOO_MANY;
    }

    for (i = 0; i < iCount; i++)
    {
        if ( !apszMac[i] || !tbsCheckMac(apszMac[i]) )
        {
            return ERR_MACLIST_INVALID_FORMAT;
        }

        /* ����ظ� */
        for ( j = i+1; j < iCount; j++ )
        {
            if ( strcmp(apszMac[i], apszMac[j]) == 0 )
                return ERR_LIST_HAS_REPEAT;
        }
    }

    return TBS_SUCCESS;
}

/* ����û���/���� */
int tbsCheckUserEx(const char *pszValue)
{
    char pattern[MAX_USER_LEN] = { 0 };
    //sprintf(pattern, "^[0-9a-zA-Z\\.@_$-]{1,%d}$", MAX_USER_LEN);
    sprintf(pattern, "^[0-9a-zA-Z\\.@_#/!$%%^&*()+ =<>?-]{1,%d}$", MAX_USER_LEN);

    if (pszValue == NULL)
    {
        return ERR_USERNAME;
    }

    if (FALSE == tbsMatch(pszValue, pattern))
    {
        return ERR_USERNAME;
    }

    return TBS_SUCCESS;
}

int tbsCheckPasswdEx(const char *pszValue)
{
    if (pszValue == NULL)
    {
        return ERR_PASSWORD;
    }

    if (tbsCheckUserEx(pszValue) != TBS_SUCCESS)
    {
        return ERR_PASSWORD;
    }

    return TBS_SUCCESS;
}

int tbsCheckDomainName(const char *pszValue)
{
    if (pszValue == NULL)
    {
        return TBS_NULL_PTR;
    }

    if (!tbsMatch(pszValue, "^[[:alnum:]]([[:alnum:]-]+[.]?){1,5}[[:alpha:]]+$")
        || strlen(pszValue) > 67)
    {
        return ERR_INVALID_DOMAIN_NAME;
    }

    return TBS_SUCCESS;
}

/* �����������IP�ĺϷ��� */
int tbsCheckHost(const char *pszValue)
{
    BOOL bIsIP = TRUE;

    if (pszValue == NULL)
    {
        return TBS_NULL_PTR;
    }

    /*
      ���ܽ��Կ�ͷ�Ƿ����������ж��Ƿ�ΪIP����Ӧ
	 ��ȫ�������ֺ�'.'��ɣ���Ϊ��IP��ַ����ֹ����
	 163.com֮���������������Ϊ��IP��ַ��
	*/
    if (!tbsMatch(pszValue, "^([0-9]{1,3}\\.){3}[0-9]{1,3}$"))
        bIsIP = FALSE;

    /* ��ip��ַ */
	if ( bIsIP )
    {
		return tbsCheckHostIpEx(pszValue);
	}
    else
    {
		return tbsCheckDomainName(pszValue);
	}
}

void TW_printf(const void *buf, int len)
{
    const int LINE_WIDTH = 16;
    int i = 0, j = 0;
    int tail = 0;
    const unsigned char *pucBuf = NULL;

    if (len == 0 || NULL == buf)
    {
        return;
    }

    pucBuf = buf;

    for(; i<len; i++)
    {
        if(i>0 && i%LINE_WIDTH == 0)
        {
            j = i - LINE_WIDTH;
            printf("; ");
            for(; j<i; j++)
            {
                if(pucBuf[j] < 16)
                    printf("%c", '.');
                else
                    printf("%c", pucBuf[j]);
            }

            printf("\x0a\x0d");
        }

        printf("%02X ", pucBuf[i]);
    }

    tail = len%LINE_WIDTH == 0 ? len-LINE_WIDTH:(len/LINE_WIDTH)*LINE_WIDTH;
    if(tail != len-LINE_WIDTH)
    {
        for(i=0; i<48-(len-tail)*3; i++)
        {
            printf("%c", ' ');
        }
    }

    printf("; ");

    for(i=tail; i<len; i++)
    {
        if(pucBuf[i] < 16)
            printf("%c", '.');
        else
            printf("%c", pucBuf[i]);
    }

    printf("\x0a\x0d");
}


/*=========================================================================
*
*    Mem Pool function
*
*=========================================================================*/
/* �ڴ�����¼ */
typedef struct __ST_Mem_Rec
{
    struct list_head head;
    void *pAddr;
    unsigned int nSize;
    const char *szFile;
    const char *szFunc;
    unsigned long ulLine;
    unsigned int bPrinted;
}ST_Mem_Rec;

/* ȡ���ڴ�⺯�����ض��� */
#ifdef malloc
#undef malloc
#endif

#ifdef free
#undef free
#endif

#ifdef calloc
#undef calloc
#endif

#ifdef realloc
#undef realloc
#endif


#ifdef strdup
#undef strdup
#endif



/* ��ȫ�ͷ��ڴ�*/
#define safe_mem_free(pointer) \
    { \
        if ( pointer ) \
        { \
            free(pointer); \
            pointer = NULL;\
        } \
    }

static void MemList_AddTail(struct list_head *head, ST_Mem_Rec *pstMem)
{
    ST_Mem_Rec *l = (ST_Mem_Rec*)malloc(sizeof(ST_Mem_Rec));
	if(l == NULL)
	{
	    printf("MemList_AddTail: out of memory\n");
		return;
	}
    memcpy(l, pstMem, sizeof(ST_Mem_Rec));

    list_add_tail(&l->head, head);
}


void MemList_SetEntry(struct list_head *head, void *pAddr, ST_Mem_Rec *pstMem)
{
    ST_Mem_Rec *l = NULL;
    struct list_head *ptr;

    list_for_each(ptr, head)
    {
        l = list_entry(ptr, ST_Mem_Rec, head);
        if ( l->pAddr == pAddr )
        {
            l->pAddr = pstMem->pAddr;
            l->nSize = pstMem->nSize;
            l->szFile = pstMem->szFile;
            l->szFunc = pstMem->szFunc;
            l->ulLine = pstMem->ulLine;
            l->bPrinted= FALSE;
            return;
        }
    }

    MemList_AddTail(head, pstMem);
}


#define MEM_MALLOC_MAGIC    0x30    /* 0 */
#define MEM_FREE_MAGIC      0x39    /* 9 */
#define MEM_EXCCED_MAGIC    0x7a    /* z */
#define MEM_EXCCED_LEN      16


#define MEM_EXCCED_CHECK(l) \
{ \
    int i = 0; \
    for (i = 0; i < MEM_EXCCED_LEN; i++) \
    { \
        if (MEM_EXCCED_MAGIC != *((unsigned char *)(l->pAddr + l->nSize + i))) \
        { \
            printf("\n_   _ \n" \
                     "@   @ \n" \
                     "  )   \n" \
                     "  o  !!!!!!==== Mem Exceed at %s %lu ====!!!!!!\n", \
                     l->szFile, l->ulLine); \
            exit(-1); \
        } \
    } \
}

static BOOL MemList_DeleteEntry(struct list_head *head, void *pAddr)
{
    ST_Mem_Rec *l = NULL;
    struct list_head *ptr;

    list_for_each(ptr, head)
    {
        l = list_entry(ptr, ST_Mem_Rec, head);
        if ( l->pAddr == pAddr )
        {
            MEM_EXCCED_CHECK(l);
            memset(l->pAddr, MEM_FREE_MAGIC, l->nSize);
            safe_mem_free(l->pAddr);
            list_del(&l->head);
            free(l);
            return TRUE;
        }
    }
    return FALSE;
}

static void MemList_DropAll(struct list_head *head)
{
    ST_Mem_Rec  *l = NULL;

    while (!list_empty(head))
    {
        l = list_entry(head->next, ST_Mem_Rec , head);
        safe_mem_free(l->pAddr);
        list_del(&l->head);
        free(l);
    }
}

static void MemList_PrintAll(struct list_head *head, BOOL bOnlyPrintNew)
{
    struct list_head *ptr;
    ST_Mem_Rec *l;
    int total_mem_count = 0;
    int total_size = 0;

    printf("===========================================================================\n");
    printf("Memory Pool Status\n");
    printf("\n");

    list_for_each(ptr, head)
    {
        l = list_entry(ptr, ST_Mem_Rec, head);
        if(l)
        {
            total_mem_count++;
            total_size += l->nSize;

            MEM_EXCCED_CHECK(l);

            /* �Ƿ�ֻ��ӡ�·�������� */
            if ( bOnlyPrintNew )
            {
                if ( !(l->bPrinted) )
                {
                    printf("Mem Rec %4d/%p: Address=%p, Size=%u, Malloc at %s:%s:%4lu  \n",
                        total_mem_count, l, l->pAddr, l->nSize, l->szFile, l->szFunc, l->ulLine);
                    l->bPrinted = TRUE;
                }
            }
            else
                printf("Mem Rec %4d/%p:: Address=%p, Size=%u, Malloc at %s:%s:%4lu  \n",
                    total_mem_count, l, l->pAddr, l->nSize, l->szFile, l->szFunc, l->ulLine);
        }
    }
    printf("===========================================================================\n");
    printf("Total memory blocks = %d, Total memory size = %d bytes. \n", total_mem_count, total_size);
}


/* �ڴ������ͷ */
static struct list_head g_stMemPool = LIST_HEAD_INIT(g_stMemPool);


/*********************************************************************
*  ����: Mem_Malloc
*  ����: �����ڴ�,����¼
*  ����: nSize          : Ҫ�����ڴ�Ĵ�С
*        szFile         : �ļ���
*        szFunc         : ������
*        szLine         : �к�
*  ����: �ɹ�,������ڴ��ָ��
*        ʧ��,NULL
*  ����: ������ / 2008-10-15
*********************************************************************/
void *Mem_Malloc(size_t nSize, const char *szFile, const char *szFunc, unsigned long ulLine)
{
    void *pAddr = NULL;
    ST_Mem_Rec stMem;

    pAddr = malloc(nSize + MEM_EXCCED_LEN);
    if ( NULL == pAddr )
    {
        Mem_PrintStatus();
        return NULL;
    }

    /* ����ħ������ */
    memset(pAddr, MEM_MALLOC_MAGIC, nSize);
    memset(pAddr + nSize, MEM_EXCCED_MAGIC, MEM_EXCCED_LEN);

    memset(&stMem, 0, sizeof(ST_Mem_Rec));
    stMem.pAddr = pAddr;
    stMem.nSize = nSize;
    stMem.szFile = szFile;
    stMem.szFunc = szFunc;
    stMem.ulLine = ulLine;
    stMem.bPrinted= FALSE;

    MemList_AddTail(&g_stMemPool, &stMem);

    return pAddr;
}

char *Str_Dup(const char *pcStr, const char *szFile, const char *szFunc, unsigned long ulLine)
{
    size_t nSize = 0;
    char *pcNew = NULL;

    if (NULL == pcStr)
    {
        return NULL;
    }

    nSize = strlen(pcStr) + 1;
    pcNew = Mem_Malloc(nSize, szFile, szFunc, ulLine);
    if (NULL != pcNew)
    {
        strcpy(pcNew, pcStr);
    }

    return pcNew;
}


/*********************************************************************
*  ����: Mem_Calloc
*  ����: Ϊ��������ڴ�(���������ڴ���0),����¼
*  ����: nMemb          : Ҫ�����ڴ�Ŀ���
*        nSize          : Ҫ�����ÿ���ڴ�Ĵ�С
*        szFile         : �ļ���
*        szFunc         : ������
*        szLine         : �к�
*  ����: �ɹ�,������ڴ��ָ��
*        ʧ��,NULL
*  ����: ������ / 2008-10-15
*********************************************************************/
void *Mem_Calloc(size_t nMemb, size_t nSize, const char *szFile, const char *szFunc, unsigned long ulLine)
{
    void *pAddr = NULL;
    ST_Mem_Rec stMem;
    unsigned long ulLen = nMemb * nSize;

    pAddr = malloc(ulLen + MEM_EXCCED_LEN);
    if ( NULL == pAddr )
    {
        Mem_PrintStatus();
        return NULL;
    }
    memset(pAddr, 0, ulLen);

    /* ����ħ������ */
    memset(pAddr + ulLen, MEM_EXCCED_MAGIC, MEM_EXCCED_LEN);

    memset(&stMem, 0, sizeof(ST_Mem_Rec));
    stMem.pAddr = pAddr;
    stMem.nSize = nMemb * nSize;
    stMem.szFile = szFile;
    stMem.szFunc = szFunc;
    stMem.ulLine = ulLine;

    MemList_AddTail(&g_stMemPool, &stMem);

    return pAddr;
}


/*********************************************************************
*  ����: Mem_Realloc
*  ����: ���·����ڴ�( �ͷ�/��չԭ��������ڴ�),����¼
*  ����: pAddr          : ԭ�����ڴ�ָ��
*        nSize          : �·�����ڴ�Ĵ�С
*        szFile         : �ļ���
*        szFunc         : ������
*        szLine         : �к�
*  ����: �ɹ�,������ڴ��ָ��
*        ʧ��,NULL
*  ����: ������ / 2008-10-15
*********************************************************************/
void *Mem_Realloc(void *pAddr, size_t nSize, const char *szFile, const char *szFunc, unsigned long ulLine)
{
    void *pNewAddr = NULL;
    ST_Mem_Rec stMem;

    if (pAddr == NULL)
    {
        pNewAddr = Mem_Malloc(nSize, szFile, szFunc, ulLine);
        return pNewAddr;
    }

    if ( nSize == 0 )
    {
        Mem_Free(pAddr, szFile, szFunc, ulLine);
        return NULL;
    }

    pNewAddr = realloc(pAddr, nSize + MEM_EXCCED_LEN);
    if ( NULL == pNewAddr )
    {
        Mem_PrintStatus();
        return NULL;
    }

    /* ����ħ������ */
    //memset(pNewAddr, MEM_MALLOC_MAGIC, nSize);
    memset(pNewAddr + nSize, MEM_EXCCED_MAGIC, MEM_EXCCED_LEN);

    memset(&stMem, 0, sizeof(ST_Mem_Rec));
    stMem.pAddr = pNewAddr;
    stMem.nSize = nSize;
    stMem.szFile = szFile;
    stMem.szFunc = szFunc;
    stMem.ulLine = ulLine;
    MemList_SetEntry(&g_stMemPool, pAddr, &stMem);

    return pNewAddr;
}

/*********************************************************************
*  ����: Mem_Free
*  ����: �ͷ��ڴ�,��ɾ����¼
*  ����: pAddr          : Ҫ�ͷŵ��ڴ�ָ��
*        szFile         : �ļ���
*        szFunc         : ������
*        szLine         : �к�
*  ����: void
*  ����: ������ / 2008-10-15
*********************************************************************/
void Mem_Free(void *pAddr, const char *szFile, const char *szFunc, unsigned long ulLine)
{
    if ( NULL != pAddr)
    {
        if( !MemList_DeleteEntry(&g_stMemPool, pAddr) )
        {
            printf("[MEM]Free wrong memory: Address=%p, Free at %s:%s:%4lu \n",
                pAddr, szFile, szFunc, ulLine);
            free(pAddr);
        }
    }
}

/*********************************************************************
*  ����: Mem_FreeAll
*  ����: �ͷ������ڴ�, ��ɾ�����м�¼
*  ����: void
*  ����: void
*  ����: ������ / 2008-10-15
*********************************************************************/
void Mem_FreeAll(void)
{
    MemList_PrintAll(&g_stMemPool, FALSE );
    MemList_DropAll(&g_stMemPool );
}

/*********************************************************************
*  ����: Mem_PrintStatus
*  ����: ��ʾ����֮����δ�ͷŵ��ڴ����
*  ����: void
*  ����: void
*  ����: ������ / 2008-10-15
*********************************************************************/
void Mem_PrintStatus(void)
{
#ifdef CONFIG_TBS_APPS_DEBUG_MALLOC
    MemList_PrintAll(&g_stMemPool, FALSE);
#endif
}

/*********************************************************************
*  ����: Mem_PrintStatusOnce
*  ����: ��ʾ����֮����δ�ͷŵ��ڴ����(�Ѿ����ô˺�����ʾ�����ڴ�鲻����ʾ)
*  ����: void
*  ����: void
*  ����: ������ / 2008-10-15
*********************************************************************/
void Mem_PrintStatusOnce(void)
{
#ifdef CONFIG_TBS_APPS_DEBUG_MALLOC
    MemList_PrintAll(&g_stMemPool, TRUE );
#endif
}

/*********************************************************************
*  ����: tbsAddMac

*  ����: ����mac��ַ��֧��LAN��WAN�豸���޸�

*  ����: pIfNameΪ�豸��,��eth0
		 iInterfaceTypeΪMAC_LAN_DEVICE����MAC_WAN_DEVICE
		 szMacVal  ���ظ������ߵ�mac��ַ, �ڴ��ɵ���������

*  ����: TBS_SUCCESS/TBS_FAILED

*  ����: zhujianwen / 2008-12-25
*********************************************************************/
int tbsAddMac(const char *pIfName, int iInterfaceType, char *szMacVal)
{
    char szMacLow[MAX_MAC_ADDR_LEN] = {0}, szMacHigh[MAX_MAC_ADDR_LEN] = {0};
    char szCmd[SYS_CMD_LEN] = {0};
    unsigned long ulMacLow;
    int iMacOffSet;
    FILE *fp;
    int i;

    strcpy(szMacLow, "0x");
    /* ��ȡflash��mac��ַ */
    if((fp = fopen(PATH_MAC_ADDRESS, "r")) == NULL)
    {
        printf("Error: Fail to open %s \r\n", PATH_MAC_ADDRESS);
        return TBS_FAILED;
    }
    /* ƫ��2����Ϊ����0x */
    if(fgets(szMacHigh, MAX_MAC_ADDR_LEN, fp) == 0)
    {
        printf("Error: Fail to get data from FILE \r\n");
        fclose(fp);
        return TBS_FAILED;
    }
    fclose(fp);

    /* ���ݽӿ�����ѡ��ͬ��ƫ���� */
    if(iInterfaceType == MAC_LAN_DEVICE)
    {
        iMacOffSet = CONFIG_LAN_MAC_ADDRESS_OFFSET;
    }
    else if(iInterfaceType == MAC_WAN_DEVICE)
    {
        iMacOffSet = CONFIG_WAN_MAC_ADDRESS_OFFSET;
    }
    else
    {
        printf("Error: parament pass wrong: iInterfaceType\r\n");
        return TBS_FAILED;
    }

    /* ����mac��ַ��Ӧ��ʮ������������,�ֿ��ߵ������� */
    strcpy(szMacLow+2, szMacHigh+6);
    memset(szMacHigh+6, 0, MAX_MAC_ADDR_LEN-6);

    /* �ֱ�ȡ�ߵ�������,ת��Ϊʮ�������� */
    ulMacLow = strtol(szMacLow, NULL, 16);

    /* �������ƫ����������Χ, ��λ���� */
    if((ulMacLow+iMacOffSet) > MAX_VALUE_OF_HEX)
    {
            ulMacLow = ulMacLow + iMacOffSet - CYCLE_VALUE_OF_HEX;
    }
    else
    {
            ulMacLow += iMacOffSet;
    }

    /* ��' '��Ϊ0 */
    memset(szMacLow, 0, MAX_MAC_ADDR_LEN);
    sprintf(szMacLow, "%6lx", ulMacLow);
    for(i=0; i<6; i++)
    {
        if(szMacLow[i] == ' ')
            szMacLow[i] = '0';
    }

	memset(szMacVal, 0, MAX_MAC_ADDR_LEN);
	snprintf(szMacVal, MAX_MAC_ADDR_LEN, "%s%s", szMacHigh, szMacLow);
    /* дmac��ַ�������� */
    snprintf(szCmd, SYS_CMD_LEN,
             "ifconfig %s down;ifconfig %s hw ether %s%s;ifconfig %s up",
             pIfName, pIfName, szMacHigh, szMacLow, pIfName);

    return tbsSystem(szCmd, 0);
}

/* DNS������ص�API���ṹ��,��Ķ��� */
#define REG_MATCH_LEN  5

enum
{
	SPLIT_ERROR		= -1,		/* ����ʧ�� */
	SPLIT_SUCCESS	= 0,		/* �����ɹ� */
	SPLIT_IP		= 1			/* �����������IP��ַ */
};

/*********************************************************************
 *  ����: SplitUrl

 *  ����: �������������, �����������Ҫ�����Ĳ����Լ��˿ں�, �����
 IP��ַ��ֱ�ӷ���

 *  ����:
pHostName:	��Ҫ����������
pPort:		�˿ں�

 *  ����:
SPLIT_SUCCESS: �����ɹ�, ��ʱpHostName��ŵ��ǽ������������ַ
pPort��ŵ��Ƕ˿ں�
SPLIT_ERROR:	����ʧ��
SPLIT_IP:		�����URL��IP��ַ, ֱ�ӷ���

 *  ����: lichuang (2009-04-01)
 *********************************************************************/
/* �˿ںŵ���󳤶� */
#define PORT_LEN 6
static int SplitUrl(char *pHostName, char *pPort)
{
	size_t          nLen;
	regex_t         stRegex;             /* �洢����õ�������ʽ��������ʽ��ʹ��֮ǰҪ�������� */
	regex_t         stRegex1;
	regmatch_t      szRegMatch [REG_MATCH_LEN];     /* �洢ƥ�䵽���ַ���λ�� */
	int             i, nResult;

	char            szUrlPattern   [] = "^((http|https)://)?([^:/]*)(:[0-9]+[^/])*";    /* szUrlPattern�ַ��� */
	char            szIpPattern   [] = "^([0-9]{1,3}\\.){3,3}[0-9]{1,3}$";

	/* ����������ʽ */
	if (0 != regcomp(&stRegex, szUrlPattern, REG_EXTENDED))
	{
		return SPLIT_ERROR;
	}

	/* ִ��ģʽƥ�� */
	if (0 != regexec(&stRegex, pHostName, (size_t) REG_MATCH_LEN, szRegMatch, 0))
	{
		regfree(&stRegex);
		return SPLIT_ERROR;
	}

	nResult = SPLIT_SUCCESS;

	/* ���ȵõ��˿ں�, ����szUrlPattern�еĵ������ӱ��ʽ, ���������4��ʼ */
	if (-1 != szRegMatch[4].rm_so)
	{
		i = 4;
		nLen = szRegMatch[i].rm_eo - szRegMatch[i].rm_so;
		if (PORT_LEN < nLen)
		{
			regfree(&stRegex);
			return SPLIT_ERROR;
		}
		//strncpy(*ppPort, pHostName + szRegMatch[i].rm_so + 1, nLen - 1);
		strncpy(pPort, pHostName + szRegMatch[i].rm_so + 1, nLen - 1);
	}

	/* ��εõ�����, ����szUrlPattern�еĵ������ӱ��ʽ, �������������ʼ */
	if (-1 != szRegMatch[3].rm_so)
	{
		i = 3;
		/* ��Ҫ�Ƚ����ǲ���IP��ַ��ʽ */
		if (0 != regcomp(&stRegex1, szIpPattern, REG_EXTENDED))
		{
			regfree(&stRegex);
			return SPLIT_ERROR;
		}

		nLen = szRegMatch[i].rm_eo - szRegMatch[i].rm_so;
		strncpy(pHostName, pHostName + szRegMatch[i].rm_so, nLen);
		pHostName[nLen] = '\0';
		if (0 == regexec(&stRegex1, pHostName, (size_t) REG_MATCH_LEN, szRegMatch, 0))
		{
			nResult = SPLIT_IP;
		}
		regfree(&stRegex1);
	}

	regfree(&stRegex);   /* �����˱������ͷ� */

	return nResult;
}

/*********************************************************************
 *  ����: SetDefaultDns

 *  ����: ����DNS����֮ǰָ�������õ�DNS��������ַ,��������øú���,
 ��ʹ��ϵͳĬ�ϵ�DNS������

 *  ����:
pServer: DNS��������ַ

 *  ����:

 *  ����: lichuang (2009-04-01)
 *********************************************************************/
static int SetDefaultDns(const char *pszServer)
{
	struct in_addr stAddr;

    char szCmd[256] = {0};
    system("cp /var/resolv.conf /var/resolv.conf_old");
    snprintf(szCmd, sizeof(szCmd), "echo nameserver %s>/etc/resolv.conf", pszServer);
    system(szCmd);
    
	if (inet_pton(AF_INET, pszServer, &stAddr) > 0)
    {
		_res.nscount = 1;
		_res.nsaddr_list[0].sin_addr = stAddr;
        
        return AF_INET;
	}
    
#ifdef CONFIG_TBS_SUPPORT_IPV6
    struct in6_addr stAddr6;
    if (inet_pton(AF_INET6, pszServer, &stAddr6) > 0)
    {
        struct sockaddr_in6 *sin6;

        sin6 = malloc(sizeof(struct sockaddr_in6));
		if(sin6 == NULL)
		{
		    printf("SetDefaultDns: out of memory\n");
			return -1;
		}
        memset(sin6, 0, sizeof(struct sockaddr_in6));
        sin6->sin6_family = AF_INET6;
        sin6->sin6_port = htons(53);
        sin6->sin6_flowinfo = 0;
        sin6->sin6_addr = stAddr6;

        //_res.options |= RES_USE_INET6;
        _res._u._ext.nscount = 1;
        _res._u._ext.nsaddrs[0] = sin6;

        return AF_INET6;
    }
#endif

    return AF_UNSPEC;
}


static void RestoreOldDns(void)
{
    system("cp /var/resolv.conf_old /var/resolv.conf");
}

/*********************************************************************
 *  ����: __ResolveIpByName

 *  ����: �Դ�����������н���, �����������ST_DNS_INFO������

 *  ����:
pHostName: ��Ҫ����������

 *  ����: 0/-1

 *  ����: lichuang (2009-04-01)
 *********************************************************************/
static int __ResolveIpByName(const char *pszHostName, int iFamily)
{
	struct addrinfo *pResult = NULL;
	int nResult;
	struct addrinfo stAddr;
	char szPort[PORT_LEN] = {'\0'};

	if (SPLIT_ERROR == (nResult = SplitUrl(pszHostName, szPort)))
	{
		return -1;
	}
	else if (SPLIT_IP == nResult)
	{
		char szTemp[20];
		snprintf(szTemp, sizeof(szTemp), "/var/%d.cache", getpid());

		FILE *pFile = fopen(szTemp, "w");
		if (!pFile)
		{
			return -1;
		}

		fprintf(pFile, "%s\n", szPort);
		fprintf(pFile, "%s\n", pszHostName);
		fclose(pFile);
		return 0;
	}
    
	memset(&stAddr, 0 , sizeof(stAddr));
	stAddr.ai_socktype = SOCK_STREAM;
	stAddr.ai_family = iFamily;
	//stAddr.ai_flags = AI_CANONNAME;

	nResult = getaddrinfo(pszHostName, NULL, &stAddr, &pResult);

	if (!nResult)
	{
		char szTemp[20];
		snprintf(szTemp, sizeof(szTemp), "/var/%d.cache", getpid());

		FILE *pFile = fopen(szTemp, "w");
		if (!pFile)
		{
			return -1;
		}

		fprintf(pFile, "%s\n", szPort);

        char szAddr[MAX_IP6_LEN];
		struct addrinfo *pCurr = pResult;
		for (; pCurr; pCurr = pCurr->ai_next)
		{
			if(pCurr->ai_family == AF_INET)
			{
                inet_ntop(pCurr->ai_family, &(((struct sockaddr_in *)(pCurr->ai_addr))->sin_addr), szAddr, MAX_IP_LEN);
			}
			else if(pCurr->ai_family == AF_INET6)
			{
				inet_ntop(pCurr->ai_family, &(((struct sockaddr_in6 *)(pCurr->ai_addr))->sin6_addr), szAddr, MAX_IP6_LEN);
			}
            fprintf(pFile, "%s\n", szAddr);
		}
		fclose(pFile);
	}

	freeaddrinfo(pResult);

	return (nResult != 0);
}

/*********************************************************************
*  ����: tbsResolveIpByName

*  ����: �Դ�����������н���, �����������ST_DNS_INFO������

*  ����:
		pHostName: ��Ҫ����������
		ppDnsInfo: ��Ž������������Ԫ��ָ��, ʹ�����֮��ʹ����Ҫ
                   ʹ��tbsFreeDnsInfo�����ͷ�
		ppPort:    ��Ž���֮��Ķ˿ں�, ��tbsResolveIpByName�����ڲ�����
                    ��ʹ�����֮����Ҫ�������Լ��ͷ��ڴ�
		nMsecond:	��ʱ�õĺ�����
		pServer:    ����DNS��server��ַ, ����Ϊ��, Ϊ����ʹ��ϵͳ�ĵ�ַ

*  ����: TBS_SUCCESS/TBS_FAILED

*  ����: lichuang (2009-04-01)
*  ��ע:
         1) ����ú������óɹ�, ��Ҫ�������Լ�ȥ�ͷ�ppDnsInfo��ppPort���ڴ�
            �����������ʧ��, ����Ҫ����Щ,�ñ�mallocʧ��֮����ڴ治��Ҫfreeһ��

         2) ���URL��û�ж˿ں�,��ôppPort����һ������Ϊ0���ַ���,������Ȼ
         ��Ҫ������ȥ�ͷ�����ڴ�

*********************************************************************/
int tbsResolveIpByName(char *pszHostName, ST_DNS_INFO **ppDnsInfo,
					char** ppPort, int nMsecond, const char* pServer)
{
    *ppDnsInfo = NULL;
    *ppPort = NULL;
    int iRet = 0;

	pid_t nPid = fork();

	if (0 > nPid)
	{
		return TBS_FAILED;
	}
	else if (0 == nPid)
	{
		// child
        int af_family = AF_UNSPEC;
        
		if (pServer)
		{
			af_family = SetDefaultDns(pServer);
		}

        res_init();

        iRet = __ResolveIpByName(pszHostName, af_family);
        
        if (pServer)
		{
			RestoreOldDns();
		}
        
		if (iRet < 0)
		{
			exit(-1);
		}
		else
		{
			exit(0);
		}
	}
	else
	{
		// parent
		int nStatus = 0, nRet = 0;

		nMsecond *= 1000;

		/* ÿ��ѭ��usleep100΢�� */
		while (nMsecond > 0)
		{
			nMsecond -= 10000;
			nRet = waitpid(nPid, &nStatus, WNOHANG);
			if (nRet == nPid)
			{
				break;
			}
			usleep(10000);
		}

		if (!nRet)
		{
			char szTemp[32];
			snprintf(szTemp, sizeof(szTemp), "kill -9 %d", nPid);
			system(szTemp);
            do {
                nRet = waitpid(nPid, &nStatus, WNOHANG);
            } while (nRet != nPid);
			return TBS_FAILED;
		}
		else
		{
			if (-1 == nStatus)
			{
				return TBS_FAILED;
			}
			else if (0 == nStatus)
			{
				char szTemp[50];
				int nFlag = 0, nLen;

				*ppPort = (char*)malloc(sizeof(char) * PORT_LEN);
				if (!(*ppPort))
				{
				    snprintf(szTemp, sizeof(szTemp), "rm /var/%d.cache", nPid);
				    system(szTemp);

					return TBS_FAILED;
				}

				snprintf(szTemp, sizeof(szTemp), "/var/%d.cache", nPid);

				FILE *pFile = fopen(szTemp, "r");
				if (!pFile)
				{
					free(*ppPort);
                    *ppPort = NULL;
                    snprintf(szTemp, sizeof(szTemp), "rm /var/%d.cache", nPid);
            		system(szTemp);

					return TBS_FAILED;
				}

				ST_DNS_INFO *pDnsInfo;
				memset(szTemp, 0, sizeof(szTemp));

				while (fgets(szTemp, 50, pFile))
				{
					nLen = strlen(szTemp);
					if (!nLen)
						break;
					szTemp[nLen - 1] = '\0';

					if (!nFlag)
					{
						nFlag = 1;
						strcpy(*ppPort, szTemp);
					}
					else
					{
						pDnsInfo = (ST_DNS_INFO*)malloc(sizeof(ST_DNS_INFO));
						if (!pDnsInfo)
						{
							tbsFreeDnsInfo(*ppDnsInfo);
                            *ppDnsInfo = NULL;
							free(*ppPort);
                            *ppPort = NULL;
							fclose(pFile);
                            snprintf(szTemp, sizeof(szTemp), "rm /var/%d.cache", nPid);
            				system(szTemp);

							return -1;
						}
						if (!*ppDnsInfo)
						{
							*ppDnsInfo = pDnsInfo;
							pDnsInfo->pNext = NULL;
						}
						else
						{
							pDnsInfo->pNext = *ppDnsInfo;
							*ppDnsInfo = pDnsInfo;
						}
						strcpy(pDnsInfo->szIp, szTemp);
					}
				}

				fclose(pFile);
				snprintf(szTemp, sizeof(szTemp), "rm /var/%d.cache", nPid);
				system(szTemp);

				return TBS_SUCCESS;
			}
		}
	}

	return TBS_FAILED;
}

/*********************************************************************
 *  ����: tbsFreeDnsInfo

 *  ����: ����ST_DNS_INFO����, �ͷ����������Ԫ��

 *  ����:
pDnsInfo:  ��Ҫ�ͷŵ�����Ԫ��ָ��

 *  ����:

 *  ����: lichuang (2009-04-01)
 *********************************************************************/
void	tbsFreeDnsInfo(ST_DNS_INFO* pDnsInfo)
{
	ST_DNS_INFO* pTemp;
	while (pDnsInfo)
	{
		pTemp = pDnsInfo->pNext;
		free(pDnsInfo);
		pDnsInfo = pTemp;
	}
}

/*********************************************************************
*  ����: Strtok_r

*  ����: �̰߳�ȫ��strtok������ʵ��, ��������net-snmp-5.4.1��Ŀ

*  ����:
	pString: ��Ҫ�������ַ���, ��һ�ε��õ�ʱ����Ҫ������Ҫ�������ַ���
			 ��������NULL����
	pDelim:  �ָ��ַ���
	ppLast:  ������һ�ν��������ָ��

*  ����:
			�����NULL���Ҳ������ָ����ַ�����, �������Ѿ��ָ��õ��ַ���

*  ����: lichuang (2009-04-01)
*********************************************************************/
char* Strtok_r(char *pString, const char *pDelim, char **pLast)
{
	const char *pSpanp;
	int c, sc;
	char *pTok;

	if (pString == NULL && (pString = *pLast) == NULL)
		return (NULL);

	/*
	 * ��pString�в��ҵ�һ���Ƿָ�����Ԫ�ص�λ��
	*/
cont:
	c = *pString++;
	for (pSpanp = pDelim; (sc = *pSpanp++) != 0;)
	{
		if (c == sc)
			goto cont;
	}

	/*
	 * ���û�ҵ�, ֱ�ӷ���
	*/
	if (c == 0)
	{
		*pLast = NULL;
		return (NULL);
	}

	/*
	 * ����һ��λ��, ��λ���Ǻ���Ҫ���ص�ָ��
	*/
	pTok = pString - 1;

	/*
	 * �ӵ�ǰλ�ÿ�ʼ���ҵ�һ���ָ�����Ԫ�ص�λ��, �����λ����0Ȼ�󷵻�
	*/
	while (1)
	{
		c = *pString++;
		pSpanp = pDelim;
		do
		{
			if ((sc = *pSpanp++) == c)
			{
				if (c == 0)
					pString = NULL;
				else
					pString[-1] = 0;
				*pLast = pString;
				return (pTok);
			}
		} while (sc != 0);
	}
}

#if 0
/**************************************************************************
��  ��: ����ethernet WAN�ӿ����ƻ�ȡ�ӿڵ�Э������(��λMbps)

��  ��: pszDevName -- ethernet WAN�ӿ�����(��eth0)

����ֵ: ethernet WAN�ӿڵ�Э������

��  ע: created by XuXiaodong 2009-3-18
***************************************************************************/
int tbsGetEthWanIfLinkSpeed(const char *pszDevName)
{
    struct ifreq ifr;
	struct ethtool_cmd ecmd;
	int fd, err;

	if (pszDevName == NULL)
	{
		return TBS_FAILED;
	}

	/* Setup control structures. */
	memset(&ecmd, 0, sizeof(ecmd));
	memset(&ifr, 0, sizeof(ifr));
	strcpy(ifr.ifr_name, pszDevName);

	/* Open control socket. */
	fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (fd < 0) {
		perror("Cannot get control socket");
		return TBS_FAILED;
	}

	ecmd.cmd = ETHTOOL_GSET;
	ifr.ifr_data = (caddr_t)&ecmd;
	err = ioctl(fd, SIOCETHTOOL, &ifr);
	if (err == 0)
	{
		close(fd);
		return (int)ecmd.speed;
	}
	else
	{
		//printf("Call function \"ioctl\" failed\n");
		close(fd);
		return TBS_FAILED;
	}
}
#endif
int tbsCheckUserExWithLength(const char *pszValue, const int iLen)
{
    char pattern[MAX_USER_LEN] = { 0 };
    sprintf(pattern, "^[0-9a-zA-Z\\.@_-]{1,%d}$", iLen);

    if (pszValue == NULL)
    {
        return ERR_USERNAME;
    }

    if (FALSE == tbsMatch(pszValue, pattern))
    {
        return ERR_USERNAME;
    }

    return TBS_SUCCESS;
}


int tbsCheckPasswdExWithLength(const char *pszValue, const int iLen)
{
    if (pszValue == NULL)
    {
        return ERR_PASSWORD;
    }

    if (tbsCheckUserExWithLength(pszValue, iLen) != TBS_SUCCESS)
    {
        return ERR_PASSWORD;
    }

    return TBS_SUCCESS;
}

#ifdef CONFIG_VX180
typedef struct tag_ST_APFEATURE_MEMBER_LIST {
    struct tag_ST_APFEATURE_MEMBER_LIST *pstNext;
    unsigned short ucMID;
} ST_APFEATURE_MEMBER_LIST;

int tbsAddApfeaturMember(ST_APFEATURE_MEMBER_LIST **ppstApMemberList, unsigned short ucMID)
{
    ST_APFEATURE_MEMBER_LIST *pstNewMember, *pstMember;

    pstMember = *ppstApMemberList;  /* ppstApMemberList ����Ϊ NULL */
    while(pstMember)
    {
        if (pstMember->ucMID == ucMID)
            break;
        pstMember = pstMember->pstNext;
    }

    /* �Ѿ���¼��ģ����� apfeature ���� */
    if (pstMember)
        return 0;

    /* ��¼ģ����� apfeature ���� */
    pstNewMember = (ST_APFEATURE_MEMBER_LIST *)malloc(sizeof(ST_APFEATURE_MEMBER_LIST));
    if (NULL == pstNewMember)
    {
        fprintf(stderr, "ERROR: failed to record apfeature member, Out of memory\n");
        return -1;
    }

    memset(pstNewMember, 0, sizeof(ST_APFEATURE_MEMBER_LIST));
    pstNewMember->ucMID = ucMID;

    /* �����б� */
    pstMember = *ppstApMemberList;
    if (!pstMember)
    {
        *ppstApMemberList = pstNewMember;
    }
    else
    {
        while(pstMember)
        {
            if (!pstMember->pstNext)
            {
                pstMember->pstNext = pstNewMember;
                break;
            }

            pstMember = pstMember->pstNext;
        }
    }

    return 0;
}

int tbsRemoveApfeaturMember(ST_APFEATURE_MEMBER_LIST **ppstApMemberList, unsigned short ucMID)
{
    ST_APFEATURE_MEMBER_LIST *pstMemberPrev = NULL, *pstMember;

    pstMember = *ppstApMemberList;  /* ppstApMemberList ����Ϊ NULL */
    while(pstMember)
    {
        if (pstMember->ucMID == ucMID)
            break;

        pstMemberPrev = pstMember;
        pstMember = pstMember->pstNext;
    }

    /* û��ģ����� apfeature ���ܵļ�¼ */
    if (!pstMember)
        return 0;

    /* ���б���ɾ�� */
    if (!pstMemberPrev)
    {
        *ppstApMemberList = pstMember->pstNext;
    }
    else
    {
        pstMemberPrev->pstNext = pstMember->pstNext;
    }

    free(pstMember);

    return 0;
}

int tbsCountApfeatureMembers(ST_APFEATURE_MEMBER_LIST **ppstApMemberList)
{
    int iCount = 0;
    ST_APFEATURE_MEMBER_LIST *pstMember;

    pstMember = *ppstApMemberList;  /* ppstApMemberList ����Ϊ NULL */
    while(pstMember)
    {
        iCount++;
        pstMember = pstMember->pstNext;
    }

    return iCount;
}

int tbsApfeatureControl(unsigned short ucMID, int iEnable)
{
    static ST_APFEATURE_MEMBER_LIST *pstApMemberList = NULL;
    int iModuleCount = 0; /* ���� AP ���ٹ���ģ��ļ��� */
    char szCommand[100];

    /* ����/ɾ�������� apfeature ���ܵ�ģ���¼ */
    if (iEnable)
        tbsRemoveApfeaturMember(&pstApMemberList, ucMID);
    else
    {
        if (0 != tbsAddApfeaturMember(&pstApMemberList, ucMID))
            return -1;
    }

    iModuleCount = tbsCountApfeatureMembers(&pstApMemberList);

    fprintf(stderr, "INFO: MID: (%u), iEnable: (%d), apfeature %s\n",
            ucMID, iEnable, iModuleCount ? "disable" : "enable");

    sprintf(szCommand, "sysutil apfeature ALL 6 %s", iModuleCount ? "disable" : "enable");
    tbsSystem(szCommand, TBS_PRINT_CMD);

    sprintf(szCommand, "sysutil apfeature ALL 5 %s", iModuleCount ? "disable" : "enable");
    tbsSystem(szCommand, TBS_PRINT_CMD);

    sprintf(szCommand, "sysutil apfeature ALL VLANBRIDGE %s", iModuleCount ? "disable" : "enable");
    tbsSystem(szCommand, TBS_PRINT_CMD);

    return 0;
}

/* ���� IKANOS AP ���ٹ��� */
int tbsApfeatureEnable(unsigned short ucMID)
{
    return tbsApfeatureControl(ucMID, 1);
}

/* ���� IKANOS AP ���ٹ��� */
int tbsApfeatureDisable(unsigned short ucMID)
{
    return tbsApfeatureControl(ucMID, 0);
}
#endif

void tbsClearRamForUpgrade(int iUpgradeFlag)
{
	/* kill������Ҫ�Ľ��̣����﷢��16�źŸ�PC */
	tbsSystemMute("killall -16 pc");

    //sleep(1);
    tbsSystemMute("killall smbd");
    tbsSystemMute("killall nmbd");
    tbsSystemMute("killall ntfs-3g");


	/* ���iptables��ebtales�����й��� */
	tbsSystemMute("iptables -Z");
	tbsSystemMute("iptables -F");
	tbsSystemMute("iptables -X");
	tbsSystemMute("iptables -t nat -Z");
	tbsSystemMute("iptables -t nat -F");
	tbsSystemMute("iptables -t nat -X");
	tbsSystemMute("iptables -t mangle -Z");
	tbsSystemMute("iptables -t mangle -F");
	tbsSystemMute("iptables -t mangle -X");

	tbsSystemMute("ebtables -Z");
	tbsSystemMute("ebtables -F");
	tbsSystemMute("ebtables -X");
	tbsSystemMute("ebtables -t nat -Z");
	tbsSystemMute("ebtables -t nat -F");
	tbsSystemMute("ebtables -t nat -X");
	tbsSystemMute("ebtables -t broute -Z");
	tbsSystemMute("ebtables -t broute -F");
	tbsSystemMute("ebtables -t broute -X");


     //tbsSystemMute("flush_conntrack");
#if 0
	if(iUpgradeFlag == TR069_UPGRADE)
	{
		/* ��tr069������down��LAN�˽ӿ� */
		tbsSystemMute("ifconfig br1 down");
		tbsSystemMute("ifconfig eth0 down");
		tbsSystemMute("brctl delif br1 eth0");
		tbsSystemMute("brctl delbr br1");
		tbsSystemMute("ifconfig lo down");
	}
	else
	{
		tbsSystemMute("ifconfig lo down");
		tbsSystemMute("killall pc");
	    tbsSystemMute("killall dsl_cpe_control");
	}
#endif

    /* FIXME: only for LAN port upgrade */
    //tbsSystemMute("killall pc");
    //tbsSystemMute("killall dsl_cpe_control");
    //sleep(2);

    tbsSystemMute("echo 3 > proc/sys/vm/drop_caches");

    //sleep(1);
}

/*********************************************************************
*  ����: �ý��Ǳ�Ϊ�ػ�����
*  ����: closefd    �Ƿ�ر������ļ�������
*  ����: void
*  ����: ������ / 2009-7-22
*********************************************************************/
int tbsDaemonize(int closefd)
{
	int pid = 0;
    int i   = 0;

	switch(pid = fork())
	{
    	/* fork error */
    	case -1:
    		perror("fork()");
    		exit(1);

    	/* child process */
    	case 0:
    		/* obtain a new process group */
    		if( (pid = setsid()) < 0)
    		{
    			perror("setsid()");
    			exit(1);
    		}

            if (closefd)
            {
        		/* close all descriptors */
        		for (i=getdtablesize();i>=0;--i)
        		{
                    close(i);
        		}

                /* open stdin */
        		i = open("/dev/console",O_RDWR);

                /* stdout */
        		dup(i);

                /* stderr */
        		dup(i);
            }

    		umask(027);
    		chdir("/"); /* chdir to /tmp ? */

    		return pid;

    	/* parent process */
    	default:
    		exit(0);
	}

	return pid;
}

/*********************************************************************
*  ����: �ַ����滻���ܺ���
*  ����:
*  ����: const char *
*  ����: huangjidong
*  ��ע: ���ص��ַ�����Ҫ�û������ͷ�
*********************************************************************/
char* tbsStringReplaceWithMalloc(const char *pcSrc, const char *pcOldStr, const char *pcNewStr)
{
    char *pcRetBuf = NULL;
    char *pcBuf = NULL;
    char *pcTmp = NULL;
    char *pcStart = NULL;
    char *pcEnd = NULL;
    int iCount = 0;
    int iOldStrLen = 0;
    int iRetLen = 0;

    if(NULL == pcSrc || NULL == pcOldStr || NULL == pcNewStr)
        return NULL;

    pcTmp = pcBuf = strdup(pcSrc);
    /*if equal, return without replace*/
    if(0 == strcmp(pcOldStr, pcNewStr))
        return pcBuf;

    iOldStrLen = strlen(pcOldStr);
    while(NULL != (pcTmp = strstr(pcTmp, pcOldStr)))
    {
        iCount++;
        pcTmp += iOldStrLen;
    }

    /*calculate memory size add modify it*/
    iRetLen = strlen(pcSrc) + (strlen(pcNewStr)-strlen(pcOldStr))*iCount + 1;
    pcRetBuf = malloc(iRetLen);
    if(NULL == pcRetBuf)
    {
        free(pcBuf);
        return NULL;
    }
    memset(pcRetBuf, 0, iRetLen);

    pcStart = pcBuf;
    pcTmp = pcRetBuf;
    /*replace content in pcTmp to pcRetBuf*/
    while(NULL != (pcEnd = strstr(pcStart, pcOldStr)))
    {
        strncpy(pcTmp, pcStart, pcEnd-pcStart);
        strcat(pcTmp, pcNewStr);
        pcTmp+=strlen(pcTmp);
        pcStart = pcEnd + iOldStrLen;
    }
    strcat(pcTmp, pcStart);

    free(pcBuf);
    return pcRetBuf;
}


/******************************************************************
����: ����64λ���ܷ�ʽ���ַ������ܺ���
����: *value:�����ַ���
        vlen:���ܳ���
����: result: ���ܺ���ַ���
��ע: ���ô˺�����ע�����ⲿfree����ָ��
******************************************************************/
char *tbsBase64Encode(const unsigned char *value, int vlen)
{
	unsigned char oval = 0 ;
	char basis_64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    char *result = (char *)malloc((vlen * 4) / 3 + 5) ;
    char *out = result;
	if(result == NULL)
	{
	    printf("tbsBase64Encode: out of memory\n");
		return NULL;
	}
	
	while (vlen >= 3) {
        	*out++ = basis_64[value[0] >> 2];
        	*out++ = basis_64[((value[0] << 4) & 0x30) | (value[1] >> 4)];
        	*out++ = basis_64[((value[1] << 2) & 0x3C) | (value[2] >> 6)];
        	*out++ = basis_64[value[2] & 0x3F];
        	value += 3;
       		vlen -= 3;
    	}
	if (vlen > 0) {
	        *out++ = basis_64[value[0] >> 2];
	        oval = (value[0] << 4) & 0x30 ;
	        if (vlen > 1) oval |= value[1] >> 4;
	        *out++ = basis_64[oval];
	        *out++ = (vlen < 2) ? '=' : basis_64[(value[1] << 2) & 0x3C];
	        *out++ = '=';
	}
	*out = '\0';
    /* ��ע�⣬��ָ����Ҫ���ⲿfree */
	return result;
}

/******************************************************************
����: ����64λ���ܷ�ʽ���ַ������ܺ���
����: *value:�����ַ���
        vlen:���ܺ󳤶�
����: result: ���ܺ���ַ���
��ע: ���ô˺�����ע�����ⲿfree����ָ��
******************************************************************/
unsigned char *tbsBase64Decode(const char *value, int *rlen)
{
	int c1, c2, c3, c4;
	int vlen = strlen(value);
	unsigned char *result =(unsigned char *)malloc((vlen * 3) / 4 + 1);
	unsigned char *out = result;
	if(result == NULL)
	{
	    printf("tbsBase64Decode: out of memory\n");
		return NULL;
	}
	static signed char index_64[128] = {
	    -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
	    -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
	    -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,62, -1,-1,-1,63,
	    52,53,54,55, 56,57,58,59, 60,61,-1,-1, -1,-1,-1,-1,
	    -1, 0, 1, 2,  3, 4, 5, 6,  7, 8, 9,10, 11,12,13,14,
	    15,16,17,18, 19,20,21,22, 23,24,25,-1, -1,-1,-1,-1,
	    -1,26,27,28, 29,30,31,32, 33,34,35,36, 37,38,39,40,
	    41,42,43,44, 45,46,47,48, 49,50,51,-1, -1,-1,-1,-1
	} ;
#define CHAR64(c)  (((c) < 0 || (c) > 127) ? -1 : index_64[(c)])

	*rlen = 0;

	while (1) {
		if (value[0]==0) {
			*out = '\0' ;
			return result;
		}
		c1 = value[0];
		if (CHAR64(c1) == -1) goto base64_decode_error;
		c2 = value[1];
		if (CHAR64(c2) == -1) goto base64_decode_error;
		c3 = value[2];
		if ((c3 != '=') && (CHAR64(c3) == -1)) goto base64_decode_error;
		c4 = value[3];
		if ((c4 != '=') && (CHAR64(c4) == -1)) goto base64_decode_error;
		value += 4;
		*out++ = (CHAR64(c1) << 2) | (CHAR64(c2) >> 4);
		*rlen += 1;
		if (c3 != '=') {
			*out++ = ((CHAR64(c2) << 4) & 0xf0) | (CHAR64(c3) >> 2);
			*rlen += 1;
			if (c4 != '=') {
				*out++ = ((CHAR64(c3) << 6) & 0xc0) | CHAR64(c4);
				*rlen += 1;
			}
		}
	}
base64_decode_error:
	*result = 0;
	*rlen = 0;
	return result;
}


