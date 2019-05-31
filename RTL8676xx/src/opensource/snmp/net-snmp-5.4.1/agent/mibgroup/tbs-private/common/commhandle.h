/* commhandle.h */

#ifndef __commhandle_h__
#define __commhandle_h__

#ifdef __cplusplus
extern          "C" {
#endif

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>

#include <sys/queue.h>         /*include it for LIST*/


#ifndef ROWSTATUS_ENUMS
#define ROWSTATUS_ENUMS

#define ROWSTATUS_ACTIVE  1
#define ROWSTATUS_NOTINSERVICE  2
#define ROWSTATUS_NOTREADY  3
#define ROWSTATUS_CREATEANDGO  4
#define ROWSTATUS_CREATEANDWAIT  5
#define ROWSTATUS_DESTROY  6

#endif                          /* ROWSTATUS_ENUMS */


/*
* ��ҵ��OID��igdMIB��OID
* ����Ҫ��ֲMIB�ҿ���ʱ�������޸Ĵ˴���ע���޸ĺ���Ҫ�������е�MIB֧��
* ���ļ���
*/
#ifndef IGD_ENTERPRISE_OID
#define IGD_ENTERPRISE_OID		33003
#endif
#ifndef IGDMIB_OID
#define IGDMIB_OID				99
#endif

/*
˵��:���滻�Զ����ɴ����OIDǰ׺Ϊ����ĺꡣ����һ����Ҫ�޸�OID�ҿ��㣬ֻ��Ҫ�޸�����һ�Ρ�
�Զ����ɵĴ��룬��Ҫ�޸�xxxTable_oids.h������Ǳ������޸�xxx.c��
*/
#define IGD_OID_PREFIX			1,3,6,1,4,1,IGD_ENTERPRISE_OID,IGDMIB_OID
#define IGD_BasicInfo_PREFIX    1,3,6,1,2,1,47,1,1,1,1
#define IGD_DHCP_PREFIX         1,3,6,1,4,1,33003,1
#define IGD_TW_PREFIX           1,3,6,1,4,1,33003
#define IGD_IPRoute_PREFIX      1,3,6,1,2,1,4,21,1

/* TruthValueȡֵö��ֵ */
#ifndef TRUTHVALUE_ENUMS
#define TRUTHVALUE_ENUMS

#define TRUTHVALUE_TRUE  1
#define TRUTHVALUE_FALSE  2

#endif                          /* TRUTHVALUE_ENUMS */

/* CACHE�������ڴ�פ��ʱ�䣬��λ:�� */
#define SNMP_CACHE_DEFAULT_TIMEOUT	5



/* ����TR069�ڵ����ƺ�·������ */
#define TR069_ROOT_LanDevice 	       "InternetGatewayDevice.LANDevice."
#define TR069_ROOT_LanHostMgmt 	TR069_ROOT_LanDevice"1.LANHostConfigManagement."
#define TR069_ROOT_VapTable 	       TR069_ROOT_LanDevice"1.WLANConfiguration."
#define TR069_ROOT_WanConnDev		"InternetGatewayDevice.WANDevice.1.WANConnectionDevice."
#define TR069_ROOT_WanEthIfConf	"InternetGatewayDevice.WANDevice.1.WANEthernetInterfaceConfig."
#define TR069_ROOT_FireWall 	       "InternetGatewayDevice.X_TWSZ-COM_Firewall."
#define TR069_ROOT_Vlan 	              "InternetGatewayDevice.X_TWSZ-COM_VLAN."
#define TR069_ROOT_VlanTable 	       TR069_ROOT_Vlan"Marking."
#define TR069_ROOT_SNMPAGENT 	       "InternetGatewayDevice.X_TWSZ-COM_SNMPAgent."

#define TR069_NODE_WanIpConnection	"WANIPConnection"
#define TR069_NODE_WanPppConnection	"WANPPPConnection"
#define TR069_NODE_lanIPInterface		"IPInterface."



/*
ϵͳ���ϵͳ�ļ����ֶ���
�����ļ�������Ŀ��������ϵͳ��ͬ������ͬ����������ֲʱ��
��Ҫ�������ӱ���ѡ�
*/
#define PROC_MEMINFO		"/proc/meminfo"
#define PROC_CPUINFO		"/proc/cpuinfo"
#define PROC_FLASHFREE		"/proc/llconfig/flash_free"

#define MEMTOTAL			"MemTotal"
#define MEMFREE				"MemFree"
#define CPUMOD				"cpu model"
#define CPUFRQ				"BogoMIPS"

#define IFCONFIG_IP			"inet addr"



#if 0
/*****************************************************************************
 * �����ڵ㴦����
 *	cNodeName - ȫ·���ڵ�����
 *	uMaxValueSize - �����ַ������͵Ľڵ㣬��Ҫָ������ַ������ȡ�
 *	ucIntType - �����������͵Ľڵ㣬��Ҫָ����ASN.1�����е����ͣ�
 *			��ASN_INTEGER, ASN_COUNTER�ȡ�
 *	reqinfo - ����SNMP��Ϣ������Ϣ��
 *	requests - ����SNMP�����󶨡�
 *
 *****************************************************************************/
int commhandle_ReadOnlyStr(const char* cNodeName, unsigned int uMaxValueSize,
	netsnmp_agent_request_info *reqinfo, netsnmp_request_info *requests);
int commhandle_ReadWriteStr(const char* cNodeName, unsigned int uMaxValueSize,
	netsnmp_agent_request_info *reqinfo, netsnmp_request_info *requests);
int commhandle_ReadOnlyInt(const char* cNodeName, u_char ucIntType/* ASN_INTEGER, ASN_COUNTER etc*/,
	netsnmp_agent_request_info *reqinfo, netsnmp_request_info *requests);
int commhandle_ReadWriteInt(const char* cNodeName, u_char ucIntType/* ASN_INTEGER, ASN_COUNTER etc*/,
	netsnmp_agent_request_info *reqinfo, netsnmp_request_info *requests);
int commhandle_ReadOnlyTruthValue(const char* cNodeName,
	netsnmp_agent_request_info *reqinfo, netsnmp_request_info *requests);
int commhandle_ReadWriteTruthValue(const char* cNodeName,
	netsnmp_agent_request_info *reqinfo, netsnmp_request_info *requests);
int commhandle_ReadOnlyLocalInt(u_long* pulValue, u_char ucIntType/* ASN_INTEGER, ASN_COUNTER etc*/,
	netsnmp_agent_request_info *reqinfo, netsnmp_request_info *requests);
int commhandle_ReadWriteLocalInt(u_long* pulValue, u_char ucIntType/* ASN_INTEGER, ASN_COUNTER etc*/,
	netsnmp_agent_request_info *reqinfo, netsnmp_request_info *requests);
int commhandle_ReadOnlyLocalStr(const char* cValue,
	netsnmp_agent_request_info *reqinfo, netsnmp_request_info *requests);



/*****************************************************************************
 * Slot������
 *
 * Slot����ʵ����SNMP��Ϊ�������У���Slot����GET��������Ϊ�����һ���µ�
 * �ؼ��֡�ÿ��Slot������������ɣ�xxxSlotCreate��xxxSlot������xxxSlotCreate
 * ΪTruthValue���ͣ����ڿ��Ƶ�xxxSlot���յ�GET��Ϣʱ�Ƿ񴴽����в������µ�
 * �ؼ��֡���xxxSlotCreateΪTRUTHVALUE_TRUEʱ����xxxSlot����GET������������
 * xxx����һ����ʵ����xxxSlotΪ������ʵ���š���xxxSlotCreateΪTRUTHVALUE_FALSE
 * ʱ��GET xxxSlot���ᴴ���¶��󣬽�����xxxSlot�ĵ�ǰֵ��
 *****************************************************************************/
int commhandle_SlotCreate(u_long* pulSlotCreationEnabled,
	netsnmp_agent_request_info *reqinfo, netsnmp_request_info *requests);
int commhandle_Slot(const char* cObjName, u_long* pulSlot, u_long* pulCreationEnabled,
	netsnmp_agent_request_info *reqinfo, netsnmp_request_info *requests);
#endif


/*****************************************************************************
 * ��ṹ�д���������
 *****************************************************************************/
int CMM_GetStrColumn(const char* cInstancePrefix, const char* cNode,
	char* cValue, unsigned int uSize, size_t* pulLen, char **pv, int pvLen);
int CMM_GetIpAddrColumn(const char* cInstancePrefix, const char* cNode,
	unsigned char* cValue, unsigned int uSize, size_t* pulLen, char **pv, int pvLen);
int CMM_SetStrColumn(const char* cInstancePrefix, const char* cNode,
	const char* cValue, size_t uSize, char **pv, int pvLen);
#if 0
int CMM_GetTruthColumn(const char* cInstancePrefix, const char* cNode,
	u_long* pulValue);
int CMM_SetTruthColumn(const char* cInstancePrefix, const char* cNode,
	u_long ulValue);
#endif
int CMM_GetIntColumn(const char* cInstancePrefix, const char* cNode,
	u_long* pulValue, char **pv, int pvLen);
int CMM_SetIntColumn(const char* cInstancePrefix, const char* cNode,
	u_long ulValue, char **pv, int pvLen);


#if 0
/*****************************************************************************
* ����ֵ�жϺ����ͺꡣ
*****************************************************************************/
#define STRENUMEND	NULL
#define INTENUMEND ((u_long)(-1))

int IsStrEnum(const char* szValue, u_long ulSize, ...);
int IsIntEnum(u_long ulValue, ...);

#define IS_VALID_TRUTHVALUE(value) \
	IsIntEnum((value), TRUTHVALUE_TRUE, TRUTHVALUE_FALSE, INTENUMEND)
#define IS_VALID_ROWSTATUS(value) \
	IsIntEnum((value), \
		ROWSTATUS_ACTIVE, \
		ROWSTATUS_NOTINSERVICE, \
		ROWSTATUS_NOTREADY, \
		ROWSTATUS_CREATEANDGO, \
		ROWSTATUS_CREATEANDWAIT, \
		ROWSTATUS_DESTROY, \
		INTENUMEND)
#endif
/*****************************************************************************
*
* ��������
*
*****************************************************************************/

char* GetFirstVapInstance();
int GetGlobalVapConfigName(char* cNodeName, const char* cName);

int GetFeature(const char* cFileName, const char* cFeature, char* cValue, size_t uSize);
int GetFeatureInt(const char* cFileName, const char* cFeature,
	u_long* pulValue, int base/* 10 - 10���ƣ�16 - 16���� */);
int GetProcInt(const char* path, u_long* pulValue, int base);
u_long FlashTotal();
int mib_get_cur_ticks(int *ticks);
int mib_get_cpu_occup(float *cpu_occup);
int GetVapTotalStats(u_long* pulTotal, const char* cNmae, time_t* ptLastUpdate);
int IfConfigRead(const char* ifname, const char *pszPrefix, const char* itemname, 
                              const char* seperator, char* value, size_t size);
int GetStp(u_long* pulEnabled);
int IsHexStr(char* value, u_long size);



/****************************************************************************
*                             Linked List Defines                           *
****************************************************************************/
// here are some Linked List MACROS I wanted to use,
// but curiously were not in /usr/includes/sys/queue.h

#ifndef LIST_EMPTY
  #define	LIST_EMPTY(head)	((head)->lh_first == NULL)
#endif

#ifndef LIST_NEXT
  #define	LIST_NEXT(elm, field)	((elm)->field.le_next)
#endif

#ifndef LIST_INSERT_BEFORE
  #define	LIST_INSERT_BEFORE(listelm, elm, field) do {	\
	  (elm)->field.le_prev = (listelm)->field.le_prev;		\
	  LIST_NEXT((elm), field) = (listelm);				\
	  *(listelm)->field.le_prev = (elm);				       \
	  (listelm)->field.le_prev = &LIST_NEXT((elm), field);	\
  } while (0)
#endif

#ifndef LIST_FIRST
  #define	LIST_FIRST(head)	((head)->lh_first)
#endif


/****************************************************************************
*                           Linked List Structure                           *
****************************************************************************/
/*static*/
struct lstNode {
  LIST_ENTRY ( lstNode ) nodes;
  char *data;                                 // pointer to data
};

typedef LIST_HEAD ( , lstNode ) lstList_t;

/*pointer to last,     pointer to newly allocated,   pointer to the node being used momentarily*/
struct lstNode *lstNode, *nwlNode, *hdlNode;

void
TW_addList (char *l, char *data, int len, int refBytes);

void 
TW_flushList ( char *l );

void
TW_copyList( char *d, char *s, int dataLen );


#define IS_CACH_TIMEOUT(lasttime) \
do{ \
    if((lasttime) == 0)  \
        (lasttime) = time(NULL) - SNMP_CACHE_DEFAULT_TIMEOUT; \
    if (time(NULL)-(lasttime) < SNMP_CACHE_DEFAULT_TIMEOUT) return 0; \
    else (lasttime) = time(NULL); \
}while(0);

#ifdef __cplusplus
}
#endif

#endif

