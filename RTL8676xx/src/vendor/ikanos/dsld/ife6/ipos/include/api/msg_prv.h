
#ifndef __MSG_PRV_H__
#define __MSG_PRV_H__

#include "msg.h"
#include "warnlog.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <asm/ioctls.h>
#include <sys/ioctl.h>


#ifdef _cplusplus
    #if _cplusplus
        extern "C" {
    #endif
#endif


#ifdef _MSG_DEBUG

/*
#define MSG_ERR_HND

#define MSG_ERR(ErrCode) \
    printf("\n[MSG]: %s %d errno: %04x %08x %s.\n", __FILE__, __LINE__, s_usMsgCurMID, (unsigned int)ErrCode, strerror(errno))

#define MSG_ERR_P(ErrCode, ParaList) \
    printf("\n[MSG]: %s %d errno: %04x %08x %s. Para: ", __FILE__, __LINE__, s_usMsgCurMID, (unsigned int)ErrCode, strerror(errno)); \
    printf ParaList; \
    printf("\n")

#define MSG_LOG_HND
#define MSG_LOG(info) \
    printf info; \
    printf("\n")



#define MSG_ERR_HND MID_CCP,

#define MSG_ERR(ErrCode) \
    TBS_TRACE(MID_CCP, "errno: %04x %08x %s.", s_usMsgCurMID, (unsigned int)ErrCode, strerror(errno))

#define MSG_ERR_P(ErrCode, ParaList) \
    TBS_TRACE(MID_CCP, "errno: %04x %08x %s. Para: ", s_usMsgCurMID, (unsigned int)ErrCode, strerror(errno)); \
    TBS_TRACE ParaList


#define MSG_LOG_HND MID_CCP,
#define MSG_LOG(info)  \
    if (s_ucMsgPid != PID_TM) TBS_TRACE info
*/


#define MSG_ERR_HND LOG_INFO,

#define MSG_ERR(ErrCode) \
    syslog(LOG_INFO, "[MSG]: %s %d errno: %04x %08x %s.", __FILE__, __LINE__, s_usMsgCurMID, (unsigned int)ErrCode, strerror(errno))

#define MSG_ERR_P(ErrCode, ParaList) \
    syslog(LOG_INFO, "[MSG]: %s %d errno: %04x %08x %s. Para: ", __FILE__, __LINE__, s_usMsgCurMID, (unsigned int)ErrCode, strerror(errno)); \
    syslog ParaList


#define MSG_LOG_HND LOG_INFO,
#define MSG_LOG(info)  \
    if (s_ucMsgPid != PID_TM) syslog info


#else

#define MSG_ERR(ErrCode) (void)0
#define MSG_ERR_P(ErrCode, ParaList) (void)0

#define MSG_LOG(info) (void)0

#endif




#define STATIC  static

#ifdef _MSG_UT

#undef STATIC
#define STATIC


int msg_socket(int domain, int type, int protocol);
int msg_bind(int sockfd, struct sockaddr *my_addr, socklen_t addrlen);
int msg_connect(int  sockfd,  const  struct sockaddr *serv_addr, socklen_t addrlen);
int msg_unlink(const char *pathname);
int msg_sendto(int s, const void *msg, size_t len, int flags, const struct sockaddr *to, socklen_t tolen);
int msg_select(int n, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);
int msg_ioctl(int d, int request, unsigned long *pulLen);
int msg_recvfrom(int  s, void *buf, size_t len, int flags, struct sockaddr *from, socklen_t *fromlen);

void *msg_malloc(unsigned long ulLen);



#define socket    msg_socket
#define bind      msg_bind
#define connect   msg_connect
#define unlink    msg_unlink
#define sendto    msg_sendto
#define select    msg_select
#define ioctl     msg_ioctl
#define recvfrom  msg_recvfrom

#define malloc    msg_malloc

#endif


/* ����������MID���� */
#define  MSG_MAX_MID_NUM      0xff

/* ������Ϣ������, ����Ϣͷ */
#define  MSG_MAX_LEN          (0xffff >> 1)

/* ������Ϣ������, ������Ϣͷ */
#define MSG_BODY_MAX_LEN  (MSG_MAX_LEN - MSG_SEND_EXT_LEN)


/* ���ɴ�������Ϣ���� */
#define MSG_CREATE_MAX_LEN  (MSG_BODY_MAX_LEN * 16)


/* ���ս�������Ϣ�ǵ������ʱ */
#define MSG_POLL_ONCE_TIME      5

/* �����׽����ļ��ĸ�ʽ */
#define MSG_SOCKET_FORMAT     "/var/pid/0x%02x"

/* �׽����ļ�����󳤶� */
#define MSG_SOCKET_LEN        32





/* ����MID�Ĵ��������� */
typedef struct tag_ST_MSG_PROC_NODE
{
    struct tag_ST_MSG_PROC_NODE *pstNext;
    FUN_RECEIVE                  pfnFunc;
} ST_MSG_PROC_NODE;


/* ��Ϣ�����ƽṹ */
typedef struct tag_ST_MSG_NODE
{
    struct tag_ST_MSG_NODE *pstNext;    /* ��Ϣ����ָ����һ����Ϣ */
    char           cRefCount;           /* ���ü��� */
    unsigned char  ucPartFlag;          /* ��Ϣ��Ƭ��־ */
    unsigned short usMsgLen;            /* ��Ϣ����, ��ָstMsg�ĳ��� */
    ST_MSG         stMsg;               /* �û��ܿ�������Ϣ�ṹ */
} ST_MSG_NODE;


/* ������Ϣʱ������ϢƬ�εĽṹ */
typedef struct
{
    char *pcCurPart;
    unsigned long ulRestLen;
    ST_MSG_NODE *pstMsgNode;

} ST_MSG_PART;


/* ��ͬʱ���յ������Ϣ��Ƭ��Ϣ���� */
#define MSG_MAX_PART_GROUP   8



/* ͨ��mid��ȡ�±� */
#define MID2INDEX(mid) ((mid) & 0x00ff)

/* ȡ����Ϣ���������ݲ��ֵĳ��� */
#define MSG_CTRL_LEN  (unsigned long)(&(((ST_MSG_NODE *)0)->stMsg))

/* ��Ϣ����ʱ���⸽���ĳ��� */
#define MSG_SEND_EXT_LEN    (sizeof(ST_MSGHEAD) + MSG_CTRL_LEN)

/* ���û���Ϣָ��ȡ�ÿ�����Ϣָ�� */
#define MSG_GET_NODE_ADDR(pstMsg) \
    (ST_MSG_NODE *)((unsigned char *)(pstMsg) - MSG_CTRL_LEN)

/* �Ƿ�ಥ��Ϣ */
#define MSG_IS_GRP_MID(mid)   (MID2INDEX(mid) > MID_GRP_BASE)

#define MSG_PART_FLAG   1




#ifdef _cplusplus
    #if _cplusplus
        }
    #endif
#endif


#endif /* __MSG_PRV_H__ */




