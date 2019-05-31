/**********************************************************************
  Copyright (c), 1991-2008, T&W ELECTRONICS(SHENTHEN) Co., Ltd.

  �ļ�����: diag_common.h
  �ļ�����: ��ϳ��򹫹�ͷ�ļ�

  �޶���¼:
          1 ���� : kuangsuwen
            ���� : 2009-7-30
            ���� : ����
 **********************************************************************/
#ifndef __DIAG_COMMON_H__
#define __DIAG_COMMON_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <linux/sockios.h>
#include <sys/file.h>
#include <sys/time.h>
#include <sys/signal.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <sys/uio.h>
#include <sys/poll.h>
#include <ctype.h>
#include <errno.h>
#include <string.h>
#include <getopt.h>
#include <netdb.h>

#include <netinet/in.h>
#include <arpa/inet.h>

/******************************************************************************
*                                 MACRO                                      *
******************************************************************************/
/* ���Ժ� */
//#define DG_DEBUG 1
#ifdef DG_DEBUG
#define diag_trace(fmt, args...)    printf("[%s:%s:%4d]: "fmt, __FILE__,__FUNCTION__, __LINE__, ##args)
#else
#define diag_trace(fmt, args...)
#endif

/* �����Ϣ */
#define diag_out                    printf

/* �����жϷ���ֵ�Ƿ�����*/
#define TBS_SUCCESS      0
#define TBS_FAILED       -1


#define RET_SUCCEED(ret) (ret == TBS_SUCCESS)
#define RET_FAILED(ret)  (ret != TBS_SUCCESS)


/******************************************************************************
*                                 ENUM                                       *
******************************************************************************/


/******************************************************************************
*                                STRUCT                                      *
******************************************************************************/

struct diag_ext
{
    struct diag_ext *next;
    char *name;

    size_t size;

    /* Function which parses command options; returns zero if it ate an option */
	int (*parse)(int c, void *args);

    /* Function diag */
    int (*diag)(void *args);

    /* Final check; return zero if ok. */
    int (*check)(void *args);

    /* Pointer to list of extra command-line options */
    char *extra_opts;

};


 /******************************************************************************
*                               GLOBAL VAR                                   *
******************************************************************************/


/******************************************************************************
*                               FUNCTION                                     *
******************************************************************************/
void diag_register_ext(struct diag_ext *me);

/******************************************************************************
*                                 END                                        *
******************************************************************************/


#endif //__DIAG_COMMON_H__
