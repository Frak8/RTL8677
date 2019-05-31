/**********************************************************************
 Copyright (c), 1991-2008, T&W ELECTRONICS(SHENTHEN) Co., Ltd.

 �ļ�����: ip_conntrack_devctl.c

 �ļ�����: ���ն����������ں�ģ��

 �����б�:


 �޶���¼:
           1 ���� : ������
             ���� : 2009-03-26
             ���� : ����

**********************************************************************/

#include <linux/types.h>
#include <linux/ctype.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/in.h>
#include <linux/inet.h>
#include <linux/socket.h>
#include <linux/ctype.h>
#include <linux/errno.h>
#include <linux/timer.h>
#include <linux/time.h>
#include <linux/config.h>
#include <linux/module.h>
#include <linux/netfilter.h>
#include <linux/ip.h>
#include <linux/udp.h>
#include <linux/icmp.h>
#include <linux/skbuff.h>
#include <linux/netlink.h>

#include <net/ip.h>
#include <net/sock.h>
#include <net/udp.h>
#include <net/tcp.h>
#include <net/icmp.h>
#include <net/route.h>

#include <net/netfilter/nf_conntrack_helper.h>
#include <net/netfilter/nf_conntrack_tuple.h>
#include <net/netfilter/nf_nat_protocol.h>
#include <net/netfilter/nf_nat_core.h>
#include <linux/netfilter_ipv4/ip_conntrack_devctl.h>
#include <linux/tbs_msg.h>



//#define DEVCTL_DEBUG 1
#ifdef DEVCTL_DEBUG
#define DEBUGP(fmt, args...) \
{\
    if(s_stConfig.print)\
    {\
        printk("%4d %20s: "fmt,  __LINE__, __FUNCTION__, ## args);\
    }\
}

#define TRACE(fmt, args...)  \
{\
    if(s_stConfig.print)\
    {\
        printk(fmt, ## args);\
    }\
}
#else
#define DEBUGP(fmt, args...)
#define TRACE(fmt, args...)
#endif

/* ������ppp�����������û� */
//#define CONFIG_DONT_CONTROL_PPP         1


/* �궨�� */
#define DC_SUCCESS                      0

#define TBSMSG_LEN(len)                 (sizeof(ST_MSG) + len)
#define SET_ULONG(p, value) \
    *( (unsigned long *)p ) = value; \
    p += sizeof(unsigned long);

/* ���������Ϣ */
typedef struct tagST_DC_SearchResultMsg
{
    ST_MSGHEAD msghead;
    ulong result;
    ST_DC_SearchResult data;
}ST_DC_SearchResultMsg;


/* hash�ڵ� */
typedef struct DC_HashNode
{
    struct DC_HashNode  *next;
    ST_DC_DeviceInfo    dev;
}ST_DC_HashNode;

/* hash�� */
typedef struct
{
    ST_DC_HashNode  **list;
    size_t          list_size;
    size_t          count;
    rwlock_t        lock;
}ST_DC_HashTab;


/******************************************************************************
*                   Function declare
******************************************************************************/

/* hash��غ��� */



/* ������Ϣ��� */


/* �豸��Ϣ ���/����/���� */
static int  dc_check_config(ST_DC_Config *config);

static ST_DC_DeviceInfo* dc_find_devinfo(EN_DC_FilterType type, char *value);
static int dc_copy_devinfo(EN_DC_FilterType type, char *value, int maxCnt, ST_DC_SearchResult *result);

static void dc_update_config(ST_DC_Config *pstConfig);
static inline void dc_update_devonline(ST_DC_DeviceInfo *pstOnline, ulong time);
static inline void dc_update_devinfo(ST_DC_DeviceInfo *pstDevInfo, ulong ipaddr, char *mac, EN_DC_DevType type);

/* NetLink��Ϣ�շ� */
static void dc_recv_handler(struct sock * sk, int len);
static int  dc_send_handler(struct sock * sk, struct nlmsghdr *nlh_recv, void *data, int data_len);
static void dc_response_simple_ack(struct sock * sk, struct nlmsghdr *nlh_recv, ushort msg_type, int msg_error);

/* ����������ݰ� */
static ST_DC_DeviceInfo *dc_get_devinfo(struct sk_buff **pskb);
static int dc_control_device_by_type(struct sk_buff **pskb, ST_DC_DeviceInfo *dev, int type);
static int dc_control_device(struct sk_buff **pskb);
static unsigned int dc_ip_devctl_out(unsigned int hooknum, struct sk_buff *skb,
	const struct net_device *in, const struct net_device *out,
	int (*okfn)(struct sk_buff *));


/* ģ�����/ע�� */
static void __init dc_init_config(void);
static int  __init dc_init(void);
static void __exit dc_fini(void);



/******************************************************************************
*                   Global vaiables
******************************************************************************/
static ST_DC_Config             s_stConfig;

static ST_DC_HashTab            *s_pstDeviceTable = NULL;
static ST_DC_DevTypeStatus      s_astDeviceTypeStatus[DC_DevType_Max];

static rwlock_t                 s_devinfo_lock;
static struct sock*             s_netlink_sock = NULL;


/******************************************************************************
*                   Function
******************************************************************************/


#define __HASH_SEC__


/*
����һ���µ����ݽڵ�, �����ƴ����ֵ
node->new
*/
static inline ST_DC_HashNode *dc_node_new(ulong ip, ST_DC_DeviceInfo *dev)
{
    ST_DC_HashNode *new_node = kmalloc(sizeof(ST_DC_HashNode),GFP_KERNEL);
    if (new_node == NULL)
    {
        return NULL;
    }

    new_node->next = NULL;
    memcpy(&(new_node->dev), dev, sizeof(ST_DC_DeviceInfo));

    return new_node;
}

/*
Ϊ�ڵ�������ֵ
node->free
*/
static inline void dc_node_update(ST_DC_HashNode *node, ST_DC_DeviceInfo *dev)
{
    /* ���︲��ԭ�������ݾͿ��� */
    memcpy(&(node->dev), dev, sizeof(ST_DC_DeviceInfo));
}


/*
�ͷ�ĳ�����ݽڵ���ڴ�,�������¸��ڵ��ָ��
node->free
*/
static inline ST_DC_HashNode *dc_node_free(ST_DC_HashNode *node)
{
    ST_DC_HashNode *next = node->next;

    kfree(node);
    node = NULL;

    return (next);
}


/*
�������ݽڵ������еĽڵ�
hashlist->find
*/
static inline ST_DC_HashNode *dc_list_find(ST_DC_HashNode **head, ulong ip)
{
    ST_DC_HashNode *pos = NULL;

    /* �����ڵ�����, ���αȽ� */
    for (pos = (*head); pos != NULL; pos = pos->next)
    {
        if (pos->dev.ipaddr == ip)
        {
            return pos;
        }
    }

    return NULL;
}


/*
�����ݽڵ���������ӽڵ�
hashlist->append
*/
static inline ST_DC_HashNode *dc_list_append(ST_DC_HashNode **head, ulong ip, ST_DC_DeviceInfo *dev)
{
    ST_DC_HashNode *n = NULL;
    ST_DC_HashNode *pos = NULL;

    /* �½�һ���ڵ� */
    n = dc_node_new(ip, dev);
    if (n == NULL)
    {
        return NULL;
    }

    /* ���ڵ㸽�ӵ��ڵ�������ȥ */
    if (*head == NULL)
    {
        *head = n;
    }
    else
    {
        for(pos = *head; pos->next != NULL; pos = pos->next)
        {
            ;
        }
        pos->next = n;
    }

    return n;
}


/*
�Ƴ����ݽڵ�������ĳ���ڵ�
hashlist->remove
*/
static inline int dc_list_remove(ST_DC_HashNode **head, ulong ip)
{
    ST_DC_HashNode *pos = NULL;
    ST_DC_HashNode *prev = NULL;

    /* �ڵ�����Ϊ��,���ش��� */
    if (NULL == *head)
        return -1;

    /* Ҫɾ���ľ���ͷ��� */
    if ((*head)->dev.ipaddr == ip)
    {
        *head = dc_node_free(*head);
        return 0;
    }

    /* �����������Ҫɾ���Ľڵ� */
    prev = *head;
    for (pos = prev->next; NULL != pos; pos = pos->next)
    {
        if (pos->dev.ipaddr == ip)
        {
            prev->next = dc_node_free(pos);
            return 0;
        }
        prev = pos;
    }

    return -1;
}


/*
����hash��
hashtab->create
*/
static ST_DC_HashTab *dc_hash_create(int size)
{
    int len = 0;

    /* ����hash��ṹ������ڴ� */
    ST_DC_HashTab *tab = kmalloc(sizeof(ST_DC_HashTab), GFP_KERNEL);
    if (tab != NULL)
    {
        /* hash������ */
        memset(tab, 0, sizeof(ST_DC_HashTab));

        /* ����hash���е�����������ڴ�, len = size * 4 */
        len = size * sizeof(ST_DC_HashNode *);
        tab->list = kmalloc(len, GFP_KERNEL);
        if (tab->list == NULL)
        {
    	    kfree(tab);
            return NULL;
        }

        /* ��ʼ��hash��ĳ�Ա */
        tab->list_size   = size;
        tab->count  = 0;
        rwlock_init(&(tab->lock));  //��ʼ����
        memset(tab->list, 0, len);
    }

    return tab;
}

/*
����hash��
hashtab->destory
*/
static void dc_hash_destory(ST_DC_HashTab *tab)
{
    int i;
    ST_DC_HashNode *pos = NULL;

    /* ����hash�����������, �����ͷ����ݽڵ������еĽڵ� */
    for (i = 0; i < tab->list_size; i++)
    {
        for (pos = tab->list[i]; pos != NULL; pos = dc_node_free(pos))
        {
            ;
        }
        tab->list[i] = NULL;
    }

    /* �ͷ��������� */
	kfree(tab->list);

    /* �ͷ����� */
	kfree(tab);

    tab =NULL;
}


/*
����hashֵ
hashtab->index
*/
static inline uint dc_hash_index(ST_DC_HashTab *tab, ulong ip)
{
    //return (HOST_ADDR(ip, (~0xff)) % (tab->list_size));
    /* ����ip��ַ�����һλ */
    return (HOST_ADDR(ip, (~0xff)));
}


/*
����hash���е�����
hashtab->get
*/
static inline ST_DC_DeviceInfo *dc_hash_get(ST_DC_HashTab *tab, ulong ip)
{
    ST_DC_HashNode *find = NULL;
    uint index = dc_hash_index(tab, ip);

    find = dc_list_find(&(tab->list[index]), ip);
    if (find != NULL)
    {
        return &find->dev;
    }

    return NULL;
}

/*
��hash������Ӽ�ֵ��(ȷ���˼�ֵ�Բ����ڵ�����µ���)
hashtab->add
*/
static inline ST_DC_DeviceInfo *dc_hash_add(ST_DC_HashTab *tab, ulong ip, ST_DC_DeviceInfo *dev)
{
    ST_DC_HashNode *node = NULL;
    uint index = dc_hash_index(tab, ip);

    /* hash�ڵ�������̫��, count < list_size*4 */
    if (tab->count >= (tab->list_size*4))
        return NULL;

    node = dc_list_append(&(tab->list[index]), ip, dev);
    if (node == NULL)
    {
        return NULL;
    }
    tab->count++;

    return &(node->dev);
}


/*
��hash�������ü�ֵ��(�������������,���������ֵ)
hashtab->set
*/
static inline ST_DC_DeviceInfo *dc_hash_set(ST_DC_HashTab *tab, ulong ip, ST_DC_DeviceInfo *dev)
{
    ST_DC_HashNode *find = NULL;
    uint index = dc_hash_index(tab, ip);

    /* ����hash�����Ƿ���ڴ˼�ֵ�� */
    find = dc_list_find(&(tab->list[index]), ip);

    /* ������������µļ�ֵ�� */
    if (find == NULL)
    {
        return dc_hash_add(tab, ip, dev);
    }
    /* �Ѵ�������¼�ֵ */
    else
    {
        dc_node_update(find, dev);
        return &find->dev;
    }
}

/*
��hash�������ü�ֵ��(�������������,���������ֵ)
hashtab->remove
*/
static inline void dc_hash_remove(ST_DC_HashTab *tab, ulong ip)
{
    uint index = dc_hash_index(tab, ip);
    if (0 == dc_list_remove(&(tab->list[index]), ip))
    {
        tab->count--;
    }
}

/*
��ȡhash���е�Ԫ�ظ���
hashtab->count
*/
static inline unsigned int dc_hash_count(ST_DC_HashTab *tab)
{
    return tab->count;
}

/*
����hash��
hashtab->foreach
*/
#define dc_hash_foreach(tab, pos, index)\
    for (index = 0; index < tab->list_size; index++)\
        for (pos = tab->list[index]; NULL != pos; pos = pos->next)

#define __DEBUG_SEC__

#ifdef DEVCTL_DEBUG


/*
��ӡ16��������
*/
static void dc_print_hex(const void *buf, int len)
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
            TRACE("; ");
            for(; j<i; j++)
            {
                if(pucBuf[j] < 16)
                {
                    TRACE("%c", '.');
                }
                else
                {
                    TRACE("%c", pucBuf[j]);
                }
            }

            TRACE("\n");
        }

        TRACE("%02X ", pucBuf[i]);
    }

    tail = len%LINE_WIDTH == 0 ? len-LINE_WIDTH:(len/LINE_WIDTH)*LINE_WIDTH;
    if(tail != len-LINE_WIDTH)
    {
        for(i=0; i<48-(len-tail)*3; i++)
        {
            TRACE("%c", ' ');
        }
    }

    TRACE("; ");

    for(i=tail; i<len; i++)
    {
        if(pucBuf[i] < 16)
        {
            TRACE("%c", '.');
        }
        else
        {
            TRACE("%c", pucBuf[i]);
        }
    }

    TRACE("\n");
}

/*
���Ը�������������ֵ��ʽMsgTypeת��Ϊ�ַ����ɶ���ʽ��
*/
const char* tbsGetMsgTypeName(unsigned short usMsgType)
{
	#define MSG_TYPE_NAME(name) case name: return #name

	switch (usMsgType)
    {
        MSG_TYPE_NAME(MSG_BLP_DEVCTL_ADD);
        MSG_TYPE_NAME(MSG_BLP_DEVCTL_ADD_ACK);
        MSG_TYPE_NAME(MSG_BLP_DEVCTL_DEL);
        MSG_TYPE_NAME(MSG_BLP_DEVCTL_DEL_ACK);
        MSG_TYPE_NAME(MSG_BLP_DEVCTL_QUERY);
        MSG_TYPE_NAME(MSG_BLP_DEVCTL_QUERY_ACK);
        MSG_TYPE_NAME(MSG_BLP_DEVCTL_CONFIG);
        MSG_TYPE_NAME(MSG_BLP_DEVCTL_CONFIG_ACK);

     	default:
		{
			static char s_szUnsupported[64];
			sprintf(s_szUnsupported, "UNKNOWN_MSGTYPE(%lu)", (unsigned long)usMsgType);
			return s_szUnsupported;
		}
	}
}


static inline void dc_print_nlmsghdr(struct nlmsghdr *nlh)
{
    TRACE("nlmsghdr:\n");
    TRACE("len:%d, type:0x%04x, flags:0x%04x, seq:%d, pid:%d\n",
        nlh->nlmsg_len, nlh->nlmsg_type,
        nlh->nlmsg_flags, nlh->nlmsg_seq, nlh->nlmsg_pid);
}

static inline void dc_print_nlmsg(struct nlmsghdr *nlh)
{
    dc_print_nlmsghdr(nlh);
    dc_print_hex(NLMSG_DATA(nlh), nlh->nlmsg_len-NLMSG_HDRLEN);
}

static inline void dc_print_tbsmsg(const ST_MSG *pstMsg)
{
    TRACE("MsgHead:\n");
    TRACE("SrcMID:0x%04x,DstMID:0x%04x, MsgID:0x%08lx, MsgType:%s, 0x%04x BodyLength:%lu\n",
           pstMsg->stMsgHead.usSrcMID,
           pstMsg->stMsgHead.usDstMID,
           pstMsg->stMsgHead.ulMsgID,
           tbsGetMsgTypeName(pstMsg->stMsgHead.usMsgType),
           pstMsg->stMsgHead.ulBodyLength);
    TRACE("MsgBody:\n");
    dc_print_hex(pstMsg->szMsgBody, pstMsg->stMsgHead.ulBodyLength);
}

static inline void dc_show_config(void)
{
    int i = 0;
    TRACE("\n==============================Config Info======================\n");
    TRACE("Kernel Module Config:\n");
    TRACE("mode=%d\n", s_stConfig.mode);
	TRACE("maxTotalNumber=%d\n", s_stConfig.maxTotalNumber);
    TRACE("lanIP="NIPQUAD_FMT"\n", NIPQUAD(s_stConfig.lanIP));
    TRACE("lanMask="NIPQUAD_FMT"\n", NIPQUAD(s_stConfig.lanMask));
	TRACE("pppBeginIP="NIPQUAD_FMT"\n", NIPQUAD(s_stConfig.pppBeginIP));
	TRACE("pppEndIP="NIPQUAD_FMT"\n", NIPQUAD(s_stConfig.pppEndIP));
	TRACE("timeout=%u\n", s_stConfig.timeout);
	TRACE("print=%u\n", s_stConfig.print);
    if (s_pstDeviceTable != NULL)
        TRACE("Total terminals count:%d\n", dc_hash_count(s_pstDeviceTable));
    for(i=0; i<DC_DevType_Max; i++)
    {
        TRACE("type=%d enable=%d maxNumber=%d currentNumber=%d\n",
            i,
    	    s_stConfig.devTypeSettings[i].enable,
            s_stConfig.devTypeSettings[i].maxNumber,
            s_astDeviceTypeStatus[i].currentNumber);
    }
    TRACE("==============================End  Info========================\n");
}


static inline void dc_show_devinfo(void)
{
    int i = 0;
    ST_DC_HashNode *pos = NULL;

    TRACE("\n==============================Device Info======================\n");
    TRACE("Current register device list:\n");

    /* ���� */
    read_lock_bh(&s_devinfo_lock);
    TRACE("Total terminals count:%d\n", dc_hash_count(s_pstDeviceTable));
    dc_hash_foreach(s_pstDeviceTable, pos, i)
    {
        //if (pos->dev.time != 0)
        {
            TRACE("ip="NIPQUAD_FMT", mac="NMACQUAD_FMT", type=%d, time=%lu\n",
                NIPQUAD(pos->dev.ipaddr),
                NMACQUAD(pos->dev.mac),
                pos->dev.type, pos->dev.time);
        }
    }
    /* ���� */
    read_unlock_bh(&s_devinfo_lock);

    TRACE("==============================End Info=========================\n");
}


#else

#define dc_print_hex(buf, len)
#define dc_print_nlmsghdr(nlh)
#define dc_print_nlmsg(nlh)
#define dc_print_tbsmsg(nlh)
#define dc_show_config()
#define dc_show_onlineinfo()
#define dc_show_devinfo()


#endif



#define __DATA_SEC__

/* ������������ */
static int dc_check_mask(ulong mask)
{
    /* ����ȫΪ0 */
    if (mask == 0)
        return 0;

    while ( mask & 0x80000000 )
	{
		mask <<= 1;
        DEBUGP("mask continue:"NIPQUAD_FMT",%0lx\n", NIPQUAD(mask), mask);
	}

	mask &= 0xffffffff;

	if ( mask == 0 )
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


/* У��������Ϣ */
static int dc_check_config(ST_DC_Config *config)
{
    int iRet = DC_SUCCESS;
    int i = 0;

    /* �������ģʽ */
    if (config->mode != 0
        && config->mode != 1
        && config->mode != 2
    )
    {
        iRet = ERR_DEVCTL_INVALID_CONFIG;
        DEBUGP("mode is invalid.\n");
        goto out;
    }

    /* ����������� */
    if ( config->mode > 0
        && (config->maxTotalNumber < 0 || config->maxTotalNumber > 254)
    )
    {
        iRet = ERR_DEVCTL_INVALID_CONFIG;
        DEBUGP("maxTotalNumber(%d) is invalid.\n", config->maxTotalNumber);
        goto out;
    }

    /* ���lanMask */
    if (!dc_check_mask(ntohl(config->lanMask)))
    {
        iRet = ERR_DEVCTL_INVALID_CONFIG;
        DEBUGP("lanmask("NIPQUAD_FMT") is invalid.\n",
                NIPQUAD(config->lanMask));
        goto out;
    }

    /* ���lanIP,����Ϊ�㲥��ַ���������ַ */
    if (IS_NET_ADDR(config->lanIP, config->lanMask)
        || IS_BROAD_ADDR(config->lanIP, config->lanMask)
    )
    {
        iRet = ERR_DEVCTL_INVALID_CONFIG;
        DEBUGP("lanIP("NIPQUAD_FMT")/lanmask("NIPQUAD_FMT") is invalid.\n",
                NIPQUAD(config->lanIP), NIPQUAD(config->lanMask));
        goto out;
    }

    /* ���pppoe��ַ��, Ҫ��lan������ */
    if ( !IN_SAME_SUBNET(config->lanIP, config->pppBeginIP, config->lanMask)
        || !IN_SAME_SUBNET(config->lanIP, config->pppEndIP, config->lanMask)
        || (config->pppBeginIP >= config->pppEndIP)
    )
    {
        iRet = ERR_DEVCTL_INVALID_CONFIG;
        DEBUGP("pppBeginIP("NIPQUAD_FMT")/pppEndIP("NIPQUAD_FMT") is invalid.\n",
            NIPQUAD(config->pppBeginIP), NIPQUAD(config->pppEndIP));
        goto out;
    }

    /* ��鳬ʱ */
    if (config->timeout <= 0)
    {
        iRet = ERR_DEVCTL_INVALID_CONFIG;
        DEBUGP("timeout is invalid.\n");
        goto out;
    }

    /* ���������������� */
    for (i=1; i<DC_DevType_Max; i++)
    {
        if( config->devTypeSettings[i].enable
            && (config->devTypeSettings[i].maxNumber < 0 || config->devTypeSettings[i].maxNumber > 254)
        )
        {
            iRet = ERR_DEVCTL_INVALID_CONFIG;
            DEBUGP("specify devtype maxNumber is invalid.\n");
            break;
        }
    }

out:
    return iRet;
}


/*
�����豸��Ϣ
*/
static ST_DC_DeviceInfo* dc_find_devinfo(EN_DC_FilterType type, char *value)
{
    ST_DC_DeviceInfo *devInfo = NULL;
    ST_DC_HashNode *pos = NULL;
    int i = 0;

    /* ����IP���� */
    if (type == DC_FT_ByIP)
    {
        devInfo = dc_hash_get(s_pstDeviceTable, *(ulong *)value);
    }
    /* ����MAC���� */
    else if (type == DC_FT_ByMac)
    {
        dc_hash_foreach(s_pstDeviceTable, pos, i)
        {
            if (memcmp(pos->dev.mac, value, DEV_MAC_LEN) == 0)
        	{
                devInfo = &(pos->dev);
                break;
        	}
        }
    }
    /* �����豸���Ͳ��� */

    return devInfo;
}



/*
�ӿ�: ����mac��ַ����ip��ַ����ȡ��Ӧ�ն˵��豸����
*/
EN_DC_DevType dc_get_devtype(EN_DC_FilterType type, char *value)
{
    ST_DC_DeviceInfo *devInfo = NULL;

    /* ���� */
    read_lock_bh(&s_devinfo_lock);

    /* ���� */
    devInfo = dc_find_devinfo(type, value);

    /* ���� */
    read_unlock_bh(&s_devinfo_lock);

    return (devInfo == NULL)? DC_DevType_None : devInfo->type;
}
EXPORT_SYMBOL_GPL(dc_get_devtype);


/*
���Ҳ������豸��Ϣ
*/
static int dc_copy_devinfo(EN_DC_FilterType type, char *value, int maxCnt,
    ST_DC_SearchResult *result)
{
    ST_DC_DeviceInfo *devInfo = NULL;
    ST_DC_HashNode *pos = NULL;
    int i = 0;

    /* ��ʼ�� */
    result->count = 0;

    /* ����IP���� */
    if (type == DC_FT_ByIP)
    {
        devInfo = dc_hash_get(s_pstDeviceTable, *(ulong *)value);
        if (devInfo != NULL)
        {
            result->count = 1;
            memcpy(result->devlist, devInfo, sizeof(ST_DC_DeviceInfo));
            return 1;
        }
    }
    /* ����MAC���� */
    else if (type == DC_FT_ByMac)
    {
        dc_hash_foreach(s_pstDeviceTable, pos, i)
        {
            if (memcmp(pos->dev.mac, value, DEV_MAC_LEN) == 0)
        	{
                devInfo = &(pos->dev);

                memcpy(&result->devlist[result->count], devInfo, sizeof(ST_DC_DeviceInfo));
                result->count = 1;
                return 1;
        	}
        }
    }
    /* �����豸���Ͳ��� */
    else
    {
        dc_hash_foreach(s_pstDeviceTable, pos, i)
        {
            if (pos->dev.type == *((EN_DC_DevType *)value))
        	{
                devInfo = &(pos->dev);

                /* �������maxCnt����� */
                if (result->count >= maxCnt)
                    break;

                memcpy(&result->devlist[result->count], devInfo, sizeof(ST_DC_DeviceInfo));
                result->count++;
        	}
        }
    }

    return result->count;
}

/*
����ģ��������Ϣ
*/
static inline void dc_update_config(ST_DC_Config *pstConfig)
{
    /* ����������Ϣ */
    memcpy(&s_stConfig, pstConfig, sizeof(ST_DC_Config));
}


/*
�����ն�������Ϣ
*/
static inline void dc_update_devonline(ST_DC_DeviceInfo *pstOnline, ulong time)
{
    pstOnline->time = time;
}

/*
�����ն�������Ϣ
*/
static inline void dc_update_devinfo(ST_DC_DeviceInfo *pstDevInfo,
    ulong ipaddr, char *mac, EN_DC_DevType type)
{
    pstDevInfo->ipaddr = ipaddr;
    memcpy(pstDevInfo->mac, mac, DEV_MAC_LEN);
    pstDevInfo->type = type;
}


#define __API_SEC__

/* �����豸��Ϣ */
static void on_msg_dev_add(struct sock *sk, struct nlmsghdr *nlh_recv, ST_MSG *pstMsg)
{
    int iRet = DC_SUCCESS;
    ST_DC_DeviceInfo *devInfo = (ST_DC_DeviceInfo *)pstMsg->szMsgBody;
    ST_DC_DeviceInfo *oldDevInfo = NULL;

    TRACE("\n============================Add Device=========================\n");
    TRACE("ip="NIPQUAD_FMT", mac="NMACQUAD_FMT", type=%d\n",
        NIPQUAD(devInfo->ipaddr), NMACQUAD(devInfo->mac), devInfo->type);
    TRACE("\n============================End Add============================\n");

    /* �����Ϣ�в����Ϸ��� */
    if (!IN_SAME_SUBNET(devInfo->ipaddr, s_stConfig.lanIP, s_stConfig.lanMask))
    {
        iRet = ERR_DEVCTL_INVALID_IP;
        DEBUGP("the ip("NIPQUAD_FMT") is not in address pool\n",
            NIPQUAD(devInfo->ipaddr));
        goto out;
    }
    if (IS_NET_ADDR(devInfo->ipaddr, s_stConfig.lanMask)
        || IS_BROAD_ADDR(devInfo->ipaddr, s_stConfig.lanMask))
    {
        iRet = ERR_DEVCTL_INVALID_IP;
        DEBUGP("the ip("NIPQUAD_FMT") is net address or broadcast address\n",
            NIPQUAD(devInfo->ipaddr));
        goto out;
    }
    if (devInfo->type <= DC_DevType_None || devInfo->type >= DC_DevType_Max)
    {
        iRet = ERR_DEVCTL_INVALID_DEVTYPE;
        DEBUGP("the devtype is invalid.\n");
        goto out;
    }

    /* ���� */
    write_lock_bh(&s_devinfo_lock);

    /* ��ѯ���ն���Ϣ�Ƿ�ע�� */
    oldDevInfo = dc_hash_get(s_pstDeviceTable, devInfo->ipaddr);

    /* �����ǰ�ն��Ѿ�ע���˲������ߣ�����ն���Ϣ�Ƿ��б仯����������ն���Ϣ,�����ն����� */
    if (oldDevInfo != NULL)
    {
        /* �ն���Ϣ�б仯����Ҫ�����ն���Ϣ */
        if (oldDevInfo->type != devInfo->type || memcpy(oldDevInfo->mac ,devInfo->mac, DEV_MAC_LEN) != 0)
        {
            /* �����ն���Ϣ */
            dc_update_devinfo(oldDevInfo, devInfo->ipaddr, devInfo->mac, devInfo->type);

            /* ����ն�����, ��Ҫ���ն��������� */
            if (oldDevInfo->time != 0)
            {
                /* �ն�����һ */
                (s_astDeviceTypeStatus[(s_stConfig.mode == 1) ? 0 : oldDevInfo->type].currentNumber)--;

                /* ���ն����� */
                dc_update_devonline(oldDevInfo, 0);
            }
        }
        /* �ն���Ϣû�б仯�����κβ�������ֹ�ն���Լʱ����ռ����Ȩ�� */
        else
        {
            //nothing to do
        }
    }
    /* �ն���Ϣ��δע��, �����ն���Ϣ��ӵ�����, Ĭ������Ϊ���� */
    else
    {
        oldDevInfo = dc_hash_add(s_pstDeviceTable, devInfo->ipaddr, devInfo);
        if (oldDevInfo == NULL)
        {
            /* ���� */
            write_unlock_bh(&s_devinfo_lock);

            iRet = ERR_DEVCTL_LIST_FULL;
            DEBUGP("the device list is full, can not store more device\n");
            goto out;
        }
        dc_update_devonline(oldDevInfo, 0);
    }

    /* ���� */
    write_unlock_bh(&s_devinfo_lock);

    //dc_show_devinfo();
out:
    dc_response_simple_ack(sk, nlh_recv,
        TBS_MSG_ACK_TYPE(pstMsg->stMsgHead.usMsgType), iRet);
}


/* ɾ���豸��Ϣ */
static void on_msg_dev_del(struct sock * sk, struct nlmsghdr *nlh_recv, ST_MSG *pstMsg)
{
    int iRet = DC_SUCCESS;
    ST_DC_SearchFilter *searchFilter = (ST_DC_SearchFilter *)pstMsg->szMsgBody;
    ST_DC_DeviceInfo *oldDevInfo = NULL;

    /* �����Ϣ�в����Ϸ��� */
    if (searchFilter->type != DC_FT_ByIP && searchFilter->type != DC_FT_ByMac)
    {
        iRet = ERR_DEVCTL_INVALID_PARA;
        DEBUGP("the search type is not support.\n");
        goto out;
    }
    if (searchFilter->type == DC_FT_ByIP
        && !IN_SAME_SUBNET(*(ulong *)(searchFilter->value), s_stConfig.lanIP, s_stConfig.lanMask))
    {
        iRet = ERR_DEVCTL_INVALID_IP;
        DEBUGP("the delete ip is not in address pool.\n");
        goto out;
    }

    /* ���� */
    write_lock_bh(&s_devinfo_lock);

    /* ���Ҷ�Ӧ���豸��Ϣ�ṹ */
    oldDevInfo = dc_find_devinfo(searchFilter->type, searchFilter->value);
    if (oldDevInfo == NULL)
    {
        /* ���� */
        write_unlock_bh(&s_devinfo_lock);

        iRet = ERR_DEVCTL_NOT_FOUND;
        DEBUGP("found devce failed.\n");
        goto out;
    }
    /* �ҵ��ն���Ϣ, �ӱ���ɾ�� */
    else
    {
        /* �ն����� */
        if (oldDevInfo->time != 0)
        {
            /* �ն�����һ */
            (s_astDeviceTypeStatus[(s_stConfig.mode == 1) ? 0 : oldDevInfo->type].currentNumber)--;
        }

        /* ɾ���ն���Ϣ */
        dc_hash_remove(s_pstDeviceTable, oldDevInfo->ipaddr);
    }

    /* ���� */
    write_unlock_bh(&s_devinfo_lock);

    //dc_show_devinfo();
out:
    dc_response_simple_ack(sk, nlh_recv,
        TBS_MSG_ACK_TYPE(pstMsg->stMsgHead.usMsgType), iRet);
}


/* ��ѯ�豸��Ϣ */
static void on_msg_dev_query(struct sock * sk, struct nlmsghdr *nlh_recv, ST_MSG *pstMsg)
{
    int iRet = DC_SUCCESS;
    ST_DC_SearchFilter *searchFilter = (ST_DC_SearchFilter *)pstMsg->szMsgBody;

    ST_DC_SearchResultMsg *sendMsg = NULL;
    uint maxCount = 0;
    uint cnt = 0;

    /* �����Ϣ�в����Ϸ��� */
    if (searchFilter->type != DC_FT_ByIP
        && searchFilter->type != DC_FT_ByMac
        && searchFilter->type != DC_FT_ByDevType)
    {
        iRet = ERR_DEVCTL_INVALID_PARA;
        DEBUGP("the search type is not support.\n");
        goto out;
    }
    if (searchFilter->type == DC_FT_ByIP
        && !IN_SAME_SUBNET(*(ulong *)(searchFilter->value), s_stConfig.lanIP, s_stConfig.lanMask))
    {
        iRet = ERR_DEVCTL_INVALID_IP;
        DEBUGP("the query ip is not in address pool.\n");
        goto out;
    }
    if (searchFilter->type == DC_FT_ByDevType
        && ( *(EN_DC_DevType*)(searchFilter->value) <= DC_DevType_None
            || *(EN_DC_DevType*)(searchFilter->value) >= DC_DevType_Max)
    )
    {
        iRet = ERR_DEVCTL_INVALID_DEVTYPE;
        DEBUGP("the query devtype is invalid.\n");
        goto out;
    }

    //dc_show_config();
    //dc_show_devinfo();

    /* �����ڴ� */
    maxCount = dc_hash_count(s_pstDeviceTable);
    sendMsg = kmalloc(
        sizeof(ST_DC_SearchResultMsg) + sizeof(ST_DC_DeviceInfo) * maxCount,
        GFP_KERNEL);
    if (sendMsg == NULL)
    {
        iRet = ERR_DEVCTL_MEM_OUT;
        DEBUGP("kmalloc failed.\n");
        goto out;
    }
    memset(sendMsg, 0,
        sizeof(ST_DC_SearchResultMsg) + sizeof(ST_DC_DeviceInfo) * maxCount);

     /* ���� */
    read_lock_bh(&s_devinfo_lock);

    /* ��ȡ�����������豸��Ϣ */
    cnt = dc_copy_devinfo(searchFilter->type, searchFilter->value,
        maxCount, &(sendMsg->data));

    /* ���� */
    read_unlock_bh(&s_devinfo_lock);

    sendMsg->msghead.usMsgType = TBS_MSG_ACK_TYPE(pstMsg->stMsgHead.usMsgType);
    sendMsg->msghead.ulBodyLength = sizeof(ST_DC_SearchResultMsg) + sizeof(ST_DC_DeviceInfo) * sendMsg->data.count;
    sendMsg->result = DC_SUCCESS;
out:
    if (iRet != DC_SUCCESS)
    {
        dc_response_simple_ack(sk, nlh_recv,
            TBS_MSG_ACK_TYPE(pstMsg->stMsgHead.usMsgType), iRet);
    }
    else
    {
        dc_send_handler(sk, nlh_recv, sendMsg, TBSMSG_LEN(sendMsg->msghead.ulBodyLength));
    }

    if (sendMsg != NULL)
    {
        kfree(sendMsg);
    }
}


/* ����ģ����� */
static void on_msg_dev_config(struct sock * sk, struct nlmsghdr *nlh_recv, ST_MSG *pstMsg)
{
    int iRet = DC_SUCCESS;
    int i = 0;
    ST_DC_Config *newConfig = (ST_DC_Config *)pstMsg->szMsgBody;
    ST_DC_HashNode *pos = NULL;

    /* �����Ϣ�в����Ϸ��� */
    if (pstMsg->stMsgHead.ulBodyLength != sizeof(ST_DC_Config))
    {
        iRet = ERR_DEVCTL_INVALID_CONFIG;
        DEBUGP("the config info is invalid in msg.\n");
        goto out;
    }

    iRet = dc_check_config(newConfig);
    if (iRet != DC_SUCCESS)
    {
        goto out;
    }

    /* ��ʾԭ����������Ϣ */
    dc_show_config();

    /* ���� */
    write_lock_bh(&s_devinfo_lock);

    /* ����������Ϣ */
    dc_update_config(newConfig);

    /* �������������Ϣ */
    dc_hash_foreach(s_pstDeviceTable, pos, i)
    {
        dc_update_devonline(&(pos->dev), 0);
    }

    /* ��ո������ն�ͳ����Ϣ */
    memset(&s_astDeviceTypeStatus, 0, sizeof(ST_DC_DevTypeStatus) * DC_DevType_Max);

    /* ���� */
    write_unlock_bh(&s_devinfo_lock);

    /* ��ʾ�µĵ�������Ϣ */
    dc_show_config();

out:
    dc_response_simple_ack(sk, nlh_recv,
        TBS_MSG_ACK_TYPE(pstMsg->stMsgHead.usMsgType), iRet);
}



#define __NETLINK_SEC__

/*��ʼ���ź���*/
DECLARE_MUTEX(s_recv_sem);

/* ����Ӧ�ò��ͨ��netlink���͵�������Ϣ */
static void dc_recv_handler(struct sock * sk, int len)
{
    struct sk_buff *skb;
    struct nlmsghdr *nlh = NULL;
    ST_MSG *pstMsg = NULL;
    ulong data_size = 0;

    /*��ȡ�ź���*/
    if(down_trylock(&s_recv_sem))
        return;

    while ((skb = skb_dequeue(&sk->sk_receive_queue))!= NULL)
    {
        do
        {
            /* ���netlink��Ϣ��ʽ */
            nlh = (struct nlmsghdr *)skb->data;
            if ( !NLMSG_OK(nlh, skb->len) )
            {
                DEBUGP("the length of netlink msg is wrong!\n");
                continue;
            }

            data_size = nlh->nlmsg_len - NLMSG_HDRLEN;
            pstMsg = (ST_MSG *)NLMSG_DATA(nlh);

            //TRACE("\n============================Netlink Msg========================\n");
            //dc_print_nlmsghdr(nlh);
            //dc_print_tbsmsg(pstMsg);
            //TRACE("\n============================End Message========================\n");

            /* ���tbs��Ϣ��ʽ */
            if (data_size < sizeof(ST_MSGHEAD)
                || pstMsg->stMsgHead.ulBodyLength > (data_size-sizeof(ST_MSGHEAD))
            )
            {
                DEBUGP("the length of tbs msg is too small.\n");
                continue;
            }

            /* ����tbs��Ϣ */
            switch( pstMsg->stMsgHead.usMsgType )
            {
                case MSG_BLP_DEVCTL_ADD:
                    on_msg_dev_add(sk, nlh, pstMsg);
                    break;
                case MSG_BLP_DEVCTL_DEL:
                    on_msg_dev_del(sk, nlh, pstMsg);
                    break;
                case MSG_BLP_DEVCTL_QUERY:
                    on_msg_dev_query(sk, nlh, pstMsg);
                    break;
                case MSG_BLP_DEVCTL_CONFIG:
                    on_msg_dev_config(sk, nlh, pstMsg);
                    break;
                default:
                    DEBUGP("invalid msgtype.\n");
                    break;
            }

        } while(0);

        /* �ͷ�skb */
        kfree_skb(skb);
    }

    /*�ͷ��ź���*/
    up(&s_recv_sem);
}


/* ͨ��netlink��ʽ������Ϣ��Ӧ�ò� */
static int dc_send_handler(struct sock * sk, struct nlmsghdr *nlh_recv,
    void *data, int data_len)
{
	int ret;
	unsigned char *old_tail = NULL;
	struct sk_buff *skb = NULL;
	struct nlmsghdr *nlh_send = NULL;

     /* ����һ���µ��׽��ֻ��� */
	skb = alloc_skb(NLMSG_SPACE(data_len), GFP_ATOMIC);
	old_tail = skb->tail;

	/* ��дnetlink��Ϣͷ */
	nlh_send = NLMSG_PUT(skb, 0,
	    nlh_recv->nlmsg_seq, nlh_recv->nlmsg_type, data_len);

    /* ��дnetlink��Ϣ�� */
	memcpy(NLMSG_DATA(nlh_send), data, data_len);

	/* ���㾭���ֽڶ���������ʵ�ʳ��� */
	nlh_send->nlmsg_len = skb->tail - old_tail;
    NETLINK_CB(skb).pid = 0;
    //NETLINK_CB(skb).dst_pid = nlh_recv->nlmsg_pid;
    NETLINK_CB(skb).dst_group = 0;

	/* �������� */
	ret = netlink_unicast(sk, skb, nlh_recv->nlmsg_pid, MSG_DONTWAIT);
    if (ret < 0)
    {
        DEBUGP("send msg to app falied, error=%d!\n", ret);
        goto nlmsg_failure;
    }

	//DEBUGP("message send! ret=%d,data_len=%d\n,",ret, data_len);
	return 0;

 nlmsg_failure:
 	DEBUGP("Fail to send netlink message.\n");
	if(skb != NULL)
		kfree_skb(skb);

	return -1;
}


/* �ظ�����Ϣ */
static void dc_response_simple_ack(struct sock * sk, struct nlmsghdr *nlh_recv,
    ushort msg_type, int msg_error)
{
    char buff[sizeof(ST_MSG)+sizeof(ulong)] = {0};
    ST_MSG *pstMsg = NULL;
    char *pcPos = NULL;

    pstMsg = (ST_MSG *)buff;
    pstMsg->stMsgHead.usMsgType = msg_type;
    pstMsg->stMsgHead.ulBodyLength = sizeof(ulong);

    pcPos = pstMsg->szMsgBody;
    SET_ULONG(pcPos, msg_error);

    dc_send_handler(sk, nlh_recv, pstMsg, TBSMSG_LEN(pstMsg->stMsgHead.ulBodyLength));
}


#define __NET_SEC__

/*
���Ӻ���
*/
static unsigned int dc_ip_devctl_out(unsigned int hooknum,
	struct sk_buff *skb,
	const struct net_device *in,
	const struct net_device *out,
	int (*okfn)(struct sk_buff *))
{
	int ret = NF_ACCEPT;

    //DEBUGP("mac: "NMACQUAD_FMT "->" NMACQUAD_FMT ", ip: " NIPQUAD_FMT "->" NIPQUAD_FMT "\n",
    //    NMACQUAD((*pskb)->mac.ethernet->h_source), NMACQUAD((*pskb)->mac.ethernet->h_dest),
    //    NIPQUAD((*pskb)->nh.iph->saddr), NIPQUAD((*pskb)->nh.iph->daddr));

    /* ģʽΪ0���򲻽����κ����� */
    if (s_stConfig.mode == 0)
    {
        //DEBUGP("Accept: mode equal 0, no limit.\n ");
        return NF_ACCEPT;
    }

    /* ����ģʽ����Ҫ�����ն��������� */
    else
    {
    	/* Դ��ַ��DHCP��ַ����,������LAN�˵�����,?��֧�ֶ��lan����?��ģʽ���� */
    	if (IN_SAME_SUBNET(ip_hdr(skb)->saddr, s_stConfig.lanIP, s_stConfig.lanMask))
    	{
            /* �����İ�����ͨ�� */
            if (ip_hdr(skb)->saddr == s_stConfig.lanIP)
            {
                return NF_ACCEPT;
            }

            /*
             ͨ��ppp�������������ն˲�����
            */
            #ifdef CONFIG_DONT_CONTROL_PPP
            /* Դ��ַ��DHCP��ַ���ڣ�����Ҳ��PPP�ĵ�ַ���� */
            if ( ntohl(ip_hdr(skb)->saddr) >= ntohl(s_stConfig.pppBeginIP)
                && ntohl(ip_hdr(skb)->saddr) <= ntohl(s_stConfig.pppEndIP) )
            {
                return NF_ACCEPT;
            }
            #endif

            /* Ŀ�ĵ�ַ��DHCP��ַ���ڣ����������� */
        	if (IN_SAME_SUBNET(ip_hdr(skb)->daddr, s_stConfig.lanIP, s_stConfig.lanMask))
        	{
                DEBUGP("Accept: destip is in address pool, " NIPQUAD_FMT "->" NIPQUAD_FMT "\n",
                    NIPQUAD(ip_hdr(skb)->saddr),
                    NIPQUAD(ip_hdr(skb)->daddr));
        		return NF_ACCEPT;
        	}
            /* Ŀ�ĵ�ַ��DHCP��ַ���⣬����������,��Ҫ�������� */
            else
            {
                /* �鲥��ַ/E���ַ/�㲥��ַ����ͨ��, >223.255.255.255 ?����������㲥��ַ��ô����*/
                if (ntohl(ip_hdr(skb)->daddr) > 0xdfffffff)
                {
                    DEBUGP("Accept: destip is in multicast or broadcast address, " NIPQUAD_FMT "->" NIPQUAD_FMT "\n",
                        NIPQUAD(ip_hdr(skb)->saddr),
                        NIPQUAD(ip_hdr(skb)->daddr));
                    return NF_ACCEPT;
                }

                write_lock_bh(&s_devinfo_lock);
        	    ret = dc_control_device(&skb);
                write_unlock_bh(&s_devinfo_lock);
            }
    	}
        /* WAN������/LAN���Ž�PPPoE����֮������, ֱ������ͨ�� */
        else
        {
            DEBUGP("Accept: srcip is out of address pool, " NIPQUAD_FMT "->" NIPQUAD_FMT "\n",
                NIPQUAD(ip_hdr(skb)->saddr),
                NIPQUAD(ip_hdr(skb)->daddr));
    		return NF_ACCEPT;
        }
    }

	return ret;
}

/*
��ȡ�ն�����
*/
static ST_DC_DeviceInfo *dc_get_devinfo(struct sk_buff **pskb)
{
    ST_DC_DeviceInfo *pstFindDev = dc_hash_get(s_pstDeviceTable, ip_hdr(*pskb)->saddr);

    if (pstFindDev != NULL)
    {
        /* ���ն���Ϣ�����ҵ����ն���Ϣ�������ն����ͣ�����¼IP��MAC��ַ */
        dc_update_devinfo(pstFindDev,
            ip_hdr(*pskb)->saddr, eth_hdr(*pskb)->h_source, pstFindDev->type);
    }
    else
    {
        /* ���ն���Ϣ�����Ҳ������ն���Ϣ����Ĭ��ΪComputer����,��¼IP��MAC��ַ */
        ST_DC_DeviceInfo stNewDev;
        dc_update_devonline(&stNewDev, 0);
        dc_update_devinfo(&stNewDev,
            ip_hdr(*pskb)->saddr, eth_hdr(*pskb)->h_source, DC_DevType_Computer);

        pstFindDev = dc_hash_add(s_pstDeviceTable, stNewDev.ipaddr, &stNewDev);
    }

    return pstFindDev;
}


static int dc_control_device(struct sk_buff **pskb)
{
    /* ģʽ0�������� */
	if(s_stConfig.mode == 0)
	{
        //DEBUGP("Accept: mode change to 0, no limit.\n ");
		return NF_ACCEPT;
	}
    else
    {
        /* ��ȡ���ݰ���Ӧ���ն����� */
        ST_DC_DeviceInfo *pstFindDev =  dc_get_devinfo(pskb);
        if (pstFindDev == NULL)
        {
            /* �ն����Ѿ������������,�����İ�ֱ�Ӿܾ� */
            return NF_DROP;
        }

        /* ģʽ1���������ն����� */
    	if(s_stConfig.mode == 1)
    	{
    		//DEBUGP("Limit all terminal:\n");
    		return dc_control_device_by_type(pskb, pstFindDev, DC_DevType_None);
    	}
        /* ģʽ2������ָ�������ն� */
    	else
    	{
    		//DEBUGP("Limit specify the type of terminal:\n");
    		return dc_control_device_by_type(pskb, pstFindDev, pstFindDev->type);
    	}
    }
}


/*
�����豸���Ϳ����ն˷�������
0:None
1:Computer
2:Camera
3:STB
4:Phone
*/
static int dc_control_device_by_type(struct sk_buff **pskb,
    ST_DC_DeviceInfo *dev, int type)
{
    int i = 0;
	int maxTypeNumber = 0;
	int *pTypeNumber;
	unsigned int bTypeEnable = 0;

    unsigned long srcIP = ip_hdr(*pskb)->saddr;
    unsigned long destIP = ip_hdr(*pskb)->daddr;

    if ( type == DC_DevType_None)
    {
        bTypeEnable = TRUE;
        maxTypeNumber = s_stConfig.maxTotalNumber;
        pTypeNumber = &(s_astDeviceTypeStatus[type].currentNumber);
    }
    else
    {
        bTypeEnable = s_stConfig.devTypeSettings[type].enable;
		maxTypeNumber = s_stConfig.devTypeSettings[type].maxNumber;
		pTypeNumber = &(s_astDeviceTypeStatus[type].currentNumber);
    }

    DEBUGP("TotalLimit: type=%d, typeEnable=%d, maxTypeNumber=%d, typeNumber=%d\n",
            type, bTypeEnable, maxTypeNumber, *pTypeNumber);

	/* ������ƿ��عر�, ������ͨ�� */
	if (bTypeEnable == FALSE)
	{
		DEBUGP("Accept: limit is disabled, " NIPQUAD_FMT "->" NIPQUAD_FMT "\n",
            NIPQUAD(srcIP), NIPQUAD(destIP));
		return NF_ACCEPT;
	}

    /* ��ǰ���ݰ���Ӧ���ն��Ѿ����ߣ�����·���ʱ�䣬������ͨ�� */
    if ( dev->time > 0 )
    {
        dc_update_devonline(dev, jiffies);

        DEBUGP("Accept: this terminal has online, " NIPQUAD_FMT "->" NIPQUAD_FMT "\n",
            NIPQUAD(srcIP), NIPQUAD(destIP));
        return NF_ACCEPT;
    }
    /* �ն˲����ߣ�����Ҫ�ж������ն����Ƿ�ﵽ���� */
    else
    {
        /* �����δ�ﵽ������������򽫵�ǰ���ݰ���Ӧ���ն�����Ϊ���ߣ�������ͨ�� */
        if (*pTypeNumber < maxTypeNumber)
        {
            dc_update_devonline(dev, jiffies);
            (*pTypeNumber)++;

            DEBUGP("Accept: this terminal set to online, " NIPQUAD_FMT "->" NIPQUAD_FMT "\n",
                NIPQUAD(srcIP), NIPQUAD(destIP));
            return NF_ACCEPT;
        }
        /* ����Ѿ��ﵽ������������򽫳�ʱ���ն�����Ϊ���� */
        else
        {
            /* �����ն��б�����ǰ�ն����͵ĳ�ʱ�ն�����Ϊ���� */
            ST_DC_HashNode *pos = NULL;
            dc_hash_foreach(s_pstDeviceTable, pos, i)
            {
                if (pos->dev.time != 0
                    && (pos->dev.type == type || type == DC_DevType_None)
                    && (jiffies - pos->dev.time) > s_stConfig.timeout * HZ )
            	{
                    dc_update_devonline(&(pos->dev), 0);
                    if ((*pTypeNumber) > 0)
                    {
                        (*pTypeNumber)--;
                    }


                    DEBUGP("this terminal(" NIPQUAD_FMT ") has timeout, set to offline\n",
                        NIPQUAD(pos->dev.ipaddr));
            	}
            }

            /*
             �ٴ��ж������ն����Ƿ�ﵽ���������
            */

            /* ���δ�ﵽ���ޣ��򽫵�ǰ���ݰ���Ӧ���ն�����Ϊ���ߣ�������ͨ�� */
            if (*pTypeNumber < maxTypeNumber)
            {
                dc_update_devonline(dev, jiffies);
                (*pTypeNumber)++;

                DEBUGP("Accept: this terminal set to online, " NIPQUAD_FMT "->" NIPQUAD_FMT "\n",
                    NIPQUAD(srcIP), NIPQUAD(destIP));
                return NF_ACCEPT;
            }
            /* ����Ѿ��ﵽ������������򽫵�ǰ���ݰ���Ӧ���ն�����Ϊ���ߣ����ܾ�ͨ�� */
            else
            {
                dc_update_devonline(dev, 0);

                DEBUGP("DROP: Terminals up to limit, so this terminal set to offline, " NIPQUAD_FMT "->" NIPQUAD_FMT "\n",
                    NIPQUAD(srcIP), NIPQUAD(destIP));

                return NF_DROP;
            }
        }
    }

    return NF_ACCEPT;
}


#define __INIT_SEC__

/*
����
������FORWARD��������ΪNAT��ı�ԴIP,�������ǹ���PREROUTING����,���ȼ���NAT��
*/
static struct nf_hook_ops ip_devicectl_out_ops = {
	.hook		= dc_ip_devctl_out,
	.owner		= THIS_MODULE,
	.pf			= PF_INET,
	.hooknum	= NF_INET_PRE_ROUTING,
	.priority   = NF_IP_PRI_FIRST,
};

/* ��ʼ������ */
static void __init dc_init_config(void)
{
    /* ��ʼ������ */
    memset(&s_stConfig, 0, sizeof(ST_DC_Config));
    /* ��ʼ��״̬���� */
    memset(&s_astDeviceTypeStatus, 0, sizeof(ST_DC_DevTypeStatus) * DC_DevType_Max);
    /* ��ʼ���� */
    rwlock_init(&s_devinfo_lock);

    dc_show_config();
}


/* ģ���ʼ�� */
static int __init dc_init(void)
{
	int ret = 0;

    DEBUGP("module load...\n");

    /* ��ʼ��������Ϣ */
	dc_init_config();
    s_stConfig.print = 1;

    /* Ϊ�豸��Ϣ������ڴ� */
    s_pstDeviceTable = dc_hash_create(MAX_DEVICE_COUNT);
    if (s_pstDeviceTable == NULL)
    {
        DEBUGP("ip_conntrack_devctl: can't get memory for devinfo.\n");
		goto out;
    }

	/* ע����PREROUTING��ע�ṳ�� */
	ret = nf_register_hook(&ip_devicectl_out_ops);
	if (ret < 0)
    {
		DEBUGP("ip_conntrack_devctl: can't register pre-routing hook.\n");
		goto out;
	}

    /* ����netlink�׽��ּ���Ӧ�ò�Ŀ������� */
    s_netlink_sock = netlink_kernel_create(&init_net, NETLINK_TBS_DEVCTL, 0,
        dc_recv_handler, NULL, THIS_MODULE);
	if (s_netlink_sock == NULL)
    {
        DEBUGP("Fail to create netlink socket.\n");
        ret = -EIO;
        goto out;
    }

	DEBUGP("module load...success\n");

out:
	if(ret != 0)
		dc_fini();

	return ret;
}

/* ģ��ע�� */
static void __exit dc_fini(void)
{
	DEBUGP("module unload...\n");

    /* �ر�netlink�׽��� */
    if (s_netlink_sock != NULL)
        sock_release(s_netlink_sock->sk_socket);

    /* ж�ع��� */
	nf_unregister_hook(&ip_devicectl_out_ops);

    /* �����豸��Ϣ�� */
    if (s_pstDeviceTable != NULL)
        dc_hash_destory(s_pstDeviceTable);

	DEBUGP("module unload...success\n");
}

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("kuangsuwen <kuagnsuwen@twsz.com>");
MODULE_DESCRIPTION("multi-terminals access internet control module");

module_init(dc_init);
module_exit(dc_fini);


