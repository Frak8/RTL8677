#include <stdio.h>
#include <stdlib.h>
#include "tbserror.h"

typedef struct __error_info
{
  int errnum;
  const char *const msg;
}error_info;


/*
����Ҫ��:
1. ��Ӧ��TR069
2. Ӣ�Ľ���
3. ���Ľ���

*/


static const error_info error_table[] =
{
    {TR069_ERRNO_UNSUP_FUNC,      "Method not supported"},
    {TR069_ERRNO_REJECT,          "Request denied"},
    {TR069_ERRNO_CPE_ERR,         "Internal error"},
    {TR069_ERRNO_INVALID_PARAM,   "Invalid arguments"},
    {TR069_ERRNO_NO_RESOURCE,     "Resources exceeded"},
    {TR069_ERRNO_INVALID_NAME,    "Invalid parameter name"},
    {TR069_ERRNO_INVALID_TYPE,    "Invalid parameter type"},
    {TR069_ERRNO_INVALID_VAL,     "Invalid parameter value"},
    {TR069_ERRNO_WRITE_FORBID,    "Attempt to set a non-writable parameter"},
    {TR069_ERRNO_NOTI_REQ_REJ,    "Notification request rejected"},
    {TR069_ERRNO_DOWNLOAD_FAIL,   "unable to connect to the server"},
    {TR069_ERRNO_UPLOAD_FAIL,     "unable to connect to the server"},
    {TR069_ERRNO_FS_AUTHFAIL,     "File transfer server authentication failure"},
    {TR069_ERRNO_FS_NOTSUPP,      "Unsupported protocol for file transfer"},
    {TR069_ERRNO_INVALID_IGMP,      "Download failure: unable to join multicast group"},
    {TR069_ERRNO_INVALID_URL,     "Download failure: unable to contact file server"},
    
    {TBS_FAILED,                  "failure"},
    {TBS_MSG_SEND_ERR,            "message sending failed"},
    {TBS_OUT_OF_MEM,              "memory not enough"},
    {TBS_PARAM_ERR,               "Invalid parameter"},
    {TBS_OUT_OF_SRC,              "Resources not enough"},
    {TBS_TIME_OUT,                "timeout"},
    {TBS_COLLISION,               "collision"},
    {TBS_NULL_PTR,                "NULL pointer"},
    {TBS_LIST_NOT_EMPTY,          "List is not empty"},
    {TBS_LIST_EMPTY,              "List is empty"},
    {TBS_INVALID_INDEX,           "Invalid index"},
    {TBS_INDEX_OVERFLOW,          "Index overflow"},
    {TBS_CORRUPT_STRUCT,          "Corrupt struct"},
    {TBS_NO_INSTANCE,             "Do not found the instance"},
    {TBS_MODULE_BUSY,             "Setting up. Wait a moument"},

};

#define num_error_names  (sizeof(error_table)/sizeof(error_info))

/*=========================================================================*/
/*  ��������: tbsstrerr                                                    */
/*  ��������: ���Ҵ������Ӧ���ַ���������                                 */
/*  ��  ��  : errnum: �����룬ӦΪ����                                     */
/*  ��  ��  : huangce / 2008-03-19                                             */
/*=========================================================================*/

const char *tbsstrerr (int errnum)
{
    int i;
    const char *msg = "";
    static char buf[32];

    for(i = 0; i < num_error_names; i ++)
    {
        if(error_table[i].errnum == errnum)
        {
            msg = error_table[i].msg;
            break;
        }
    }
    if(i == num_error_names)
    {
        sprintf (buf, "Error %d", errnum);
        msg = buf;
    }
    return msg;
}


int tbserr2tr069(int tbserrno)
{
    int i;
    for(i = 0; i < num_error_names; i ++)
    {
        //TBS�д������Ǹ���
        if((error_table[i].errnum == tbserrno) && (error_table[i].errnum < 0))
        {
            //TR069���Զ���Ĵ����뷶ΧΪ9800 - 9899
            return 9900 + error_table[i].errnum;
            break;
        }
    }
    return tbserrno;
}



const ST_TBS_ERR_TABLE g_astTbsErrTable[] =
{
    {ERR_NOT_SUPPORT_FUNCTION,    "Not support this function,now", "��ʱ��֧�����ַ���"},
    {ERR_IP_INVALID_FORMAT,       "Invalid IP format", "IP��ʽ����"},
    {ERR_PORT_INVALID_FORMAT,     "Invalid port format", "�˿ڸ�ʽ����"},
    {ERR_PORT_TOO_SMALL,          "Port cant be zero or negative,should be in 1-65535",
                                  "�˿�ֵ����Ϊ0����, Ӧ��Ϊ1-65535"},
    {ERR_PORT_TOO_BIG,            "Port should be in 1-65535",  "�˿�ֵ��Ч��Ӧ����1-65535"},
    {ERR_PRIORITY_INVALID_VALUE,  "Invalid priority value",  "���ȼ�������1-8֮��"},
    {ERR_PRIORITY_TOO_SMALL,      "Priority too small,should be in 1-8",  "���ȼ�������1-8֮��"},
    {ERR_PRIORITY_TOO_BIG,        "Priority too big,should be in 1-8",  "���ȼ�������1-8֮��"},
    {ERR_MASK_INVALID_FORMAT,     "Net mask invalid format",  "���������ʽ����"},
    {ERR_MASK_EMPTY,              "net mask can not be emty",  "�������벻��Ϊ��"},
    {ERR_MAKS_ALL_ZERO,           "net mask can not be zero",  "�������벻��Ϊȫ0"},
    {ERR_MASK_INVALID_VALUE,      "the left part of net mask must be continuous binary 1",  "�����������Ӧ����������1"},
    {ERR_ENABLE_EMPTY,            "enable should not be zero", "enable ����Ϊ��"},
    {ERR_ENABLE_INVALID_VALUE,    "bool val must be \"0\" or \"1\"", "����Ϊ\"0\" ����\"1\""},
    {ERR_INT_INVALID_VALUE,       "must be int value",  "����������"},
    {ERR_UINT_INVALID_VALUE,      "must be unsigned int", "������������"},
    {ERR_MAC_INVALID_VALUE,       "mac must be \"xx:xx:xx:xx:xx:xx\" format and consist of hexadecimal number without all 0",  "mac��ַӦ���ǲ�ȫΪ0��xx:xx:xx:xx:xx:xx��ʽ��ʮ����������"},
    {ERR_IPLIST_TOO_MANY,         "ip number more than max limit",  "IP��������"},
    {ERR_IPLIST_INVALID_FORMAT,   "valid ip list format",  "IP �б��ʽ����"},
    {ERR_MACLIST_INVALID_FORMAT,  "valid mac list format",  "MAC �б��ʽ����"},
    {ERR_MACLIST_TOO_MANY,        "too many mac",  "mac ��������"},
    {ERR_INSTANCE_TOO_MANY,       "too many instance,can not add new one",  "�Ѵ�ʵ�����ޣ����������"},
    {ERR_INSTANCE_IMMUTABLE,      "this instance can not be delete",  "�����ò�����ɾ��"},
    {ERR_INNER_MSG_INVALID,       "inner msg format error",  "�ڲ���Ϣ��������ϵ������"},
    {ERR_INNER_MSG_REPEATED_NAME, "inner msg has repeated name",  "�ڲ���Ϣ����ͬ�����ͣ�����ϵ������"},
    {ERR_INNER_CACHE_INVALID,     "inner cache error",  "�ڲ��ṹ��������ϵ������"},
    {ERR_IP_MASK_NOT_MATCH,       "IP address does not match with the subnet mask ",  "IP��ַ���������벻ƥ��"},
    {ERR_IP_IS_HOST_ADDR,         "IP address can not be host address ",  "����Ϊ���ص�ַ"},
    {ERR_IP_IS_CAST_ADDR,         "IP address can not be broadcast address ",  "����Ϊ�㲥��ַ"},
    {ERR_IP_ADDR_START_MINOR_END, "IP end address minor start address ",  "������ַ����С�ڿ�ʼ��ַ"},
    {ERR_IP_ADDR_POOL_OVERLAP,    "IP address pool overlaps ",  "IP��ַ�س�ͻ"},
    {ERR_STR_TOO_LONG,            "String is too long ",  "�ַ�������"},
    {ERR_CAN_NOT_EMPTY,           "can not be empty ",  "����Ϊ��"},
    {ERR_GET_NODE_VALUE_FAIL,     "get node value failed ",  "��ȡ�ڵ�ֵʧ��"},
    {ERR_SET_NODE_VALUE_FAIL,     "Set node value failed ",  "���ýڵ�ֵʧ��"},
    {ERR_INVALID_DOMAIN_NAME,     "Invalid domain name ",  "������ʽ����"},
    {ERR_OUT_OF_RANGE,            "the value out of range", "ֵ����ָ���ķ�Χ"},
    {ERR_IPLIST_HAS_REPEAT,       "IP list has to repeat", "�б������ظ�IP��ַ"},
    {ERR_IP_IS_WRONG_NET_SECTION,         "wrong net section", "IP��ַ���δ���"},
    {ERR_IP_IS_CAST_OR_NET_ADDR,         "IP address can not be broadcast address or net address",  "����Ϊ�㲥��ַ�������ַ"},
    {ERR_ENABLE_DHCPS_CONFLICT_DHCPR, "DHCP couldnt perform as server and relay simultaneously", "DHCP����ͬʱ������Server��Relay"},
    {ERR_PORT_RANGE_INVALID,      "invalid portrange (min > max)", "��Ч�Ķ˿ڷ�Χ(�����˿�>��ʼ�˿�)"},
    {ERR_INVALID_VAL,           "invalid value", "��Чֵ"},
    {ERR_LIST_HAS_REPEAT,       "The list are duplicates", "�б������ظ���"},
    {ERR_PACKET_RANGE_INVALID,      "invalid packetlength (min > max)", "��Ч�İ�����Χ(��ʼ����>��������)"},
		

    {ERR_MSG_MID_INVALID,       "ERR_MSG_MID_INVALID","ERR_MSG_MID_INVALID"},
    {ERR_MSG_SOCKET,            "ERR_MSG_SOCKET","ERR_MSG_SOCKET"},
    {ERR_MSG_PROC_NOT_FOUND,    "ERR_MSG_PROC_NOT_FOUND","ERR_MSG_PROC_NOT_FOUND"},
    {ERR_MSG_SEND_FAIL,         "ERR_MSG_SEND_FAIL","ERR_MSG_SEND_FAIL"},
    {ERR_MSG_DSTMID_UNREGED,    "ERR_MSG_DSTMID_UNREGED","ERR_MSG_DSTMID_UNREGED"},
    {ERR_MSG_NOT_FULL,          "ERR_MSG_NOT_FULL","ERR_MSG_NOT_FULL"},
    {ERR_MSG_PART_NOEFFECT,     "ERR_MSG_PART_NOEFFECT","ERR_MSG_PART_NOEFFECT"},
    {ERR_MSG_PART_INVALID,      "ERR_MSG_PART_INVALID","ERR_MSG_PART_INVALID"},
    {ERR_MSG_TIMEOUT,           "ERR_MSG_TIMEOUT","ERR_MSG_TIMEOUT"},
    {ERR_MSG_PART_LIST_FULL,    "ERR_MSG_PART_LIST_FULL","ERR_MSG_PART_LIST_FULL"},

    {ERR_MALLOC_FAILED,         "ERR_MALLOC_FAILED","ERR_MALLOC_FAILED"},
    {ERR_BUF_NOT_ENOUGH,        "ERR_BUF_NOT_ENOUGH","ERR_BUF_NOT_ENOUGH"},
    {ERR_INTERNAL_ERR,          "ERR_INTERNAL_ERR","ERR_INTERNAL_ERR"},
    {ERR_PARA_INVALID,          "ERR_PARA_INVALID","ERR_PARA_INVALID"},
    {ERR_FILE_OPEN,             "ERR_FILE_OPEN","ERR_FILE_OPEN"},
    {ERR_FILE_READ,             "ERR_FILE_READ","ERR_FILE_READ"},
    {ERR_FILE_WRITE,            "ERR_FILE_WRITE","ERR_FILE_WRITE"},
    {ERR_FILE_CHKSUM,           "ERR_FILE_CHKSUM","ERR_FILE_CHKSUM"},
    {ERR_BUSY,                  "ERR_BUSY","ERR_BUSY"},
    {ERR_CREATE_MSG_FAIL,       "ERR_CREATE_MSG_FAIL","ERR_CREATE_MSG_FAIL"},
    {ERR_MSG_EXT_MID_LIST_FULL, "ERR_MSG_EXT_MID_LIST_FULL","ERR_MSG_EXT_MID_LIST_FULL"},

    {ERR_CFG_ATTR_STAT,         "Stat data not support set noti",             "ͳ�����ݲ�֧������noti����"},
    {ERR_CFG_PATH_NOT_EXSITED,  "Path not exist",                           "·��������"},
    {ERR_CFG_INVALID_OBJ_VAL,   "Attemp to set val to obj",                   "��ͼ�Զ���·������ֵ"},
    {ERR_CFG_INVALID_STRING,    "Invalid string val",                         "�Ƿ��ַ���"},
    {ERR_CFG_INVALID_INT_0,     "0 must expressed by \"0\", \"+0\"\"-0\"\"00\" are all invalid", "0ֻ����\"0\"��ʾ, \"+0\"\"-0\"\"00\"����Ϊ�Ƿ�"},
    {ERR_CFG_INVALID_INT_01,    "non-zero int must begin with \"+\" or non-zero number", "��0�����������Է��Ż�������ֵ��ͷ"},
    {ERR_CFG_INVALID_INT_A,     "include invalid char",                       "���зǷ��ַ�"},
    {ERR_CFG_INVALID_UINT_0,    "0 must expressed by \"0\", \"+0\"\"-0\"\"00\" are all invalid", "0ֻ����\"0\"��ʾ, \"+0\"\"-0\"\"00\"����Ϊ�Ƿ�"},
    {ERR_CFG_INVALID_UINT_01,   "non-zero unit must begin non-zero number",   "��0�޷������������Է��Ż�������ֵ��ͷ"},
    {ERR_CFG_INVALID_UINT_A,    "include invalid char",                       "���зǷ��ַ�"},
    {ERR_CFG_INVALID_BOOL,      "bool val must be \"0\" or \"1\"",                "����Ϊ\"0\" ����\"1\""},
    {ERR_CFG_INVALID_TIME,      "dateTime val must be like \"0000-00-02T03:04:05\"", "�������� \"0000-00-02T03:04:05\""},
    {ERR_CFG_INVALID_HEX,       "Hex val must be number or A-F a-f",          "ʮ�����������������ֻ���a-f A-F"},
    {ERR_CFG_NOT_WRITABLE,      "The path not writable",                      "��·������д"},
    {ERR_CFG_NOT_IN_ACCESSLIST, "Not in accesslist",                          "���ڿɷ����б���"},
    {ERR_CFG_PATH_NOT_INST,     "The path is not a valid inst",               "��·������һ���Ϸ��Ķ���ʵ��"},
    {ERR_CFG_NOT_STANDARD,      "The path is not a standard path",            "��·������һ����׼��·��"},
    {ERR_CFG_INVALID_TYPE,      "The node type is invalid",                   "�ڵ����ͷǷ�"},
    {ERR_CFG_ITEM_OPT_FAIL,     "App config flash opt failed",                "Flash�������ݲ���ʧ��"},
    {ERR_CFG_WILDCARD_QUE_FULL, "Wildcard queue has been full",               "ͨ��·����������"},
    {ERR_CFG_INVALID_PATH,      "Invalid path",                               "�Ƿ�·����"},
    {ERR_CFG_OBJ_INST_FULL,     "The object instance count has reached the max allowed count",    "����ʵ�������Ѿ��ﵽ������������"},
    {ERR_CFG_REJECT_ATTR_SET,     "This node reject to set of active notication",    "�˽ڵ㲻������Ϊ�����ϱ�"},

    {ERR_CMM_NOMID,            "Not support the opt to this path",           "��ǰ��֧�ָ�·�����в���"},

    {ERR_DGN_INVALID_STATE,    "invalid state (must be \"Requested\")",        "�Ƿ�״ֵ̬(������\"Requested\")"},
    {ERR_DGN_INVALID_INTF,     "invalid intf (not exsited)",                 "�Ƿ��ӿ�(������)"},
    {ERR_DGN_INVALID_HOST_IP_PART, "invalid host (not support partly ip)",  "�Ƿ�����(��ǰ��֧�ֲ���ip��ַ)"},
    {ERR_DGN_INVALID_HOST_IP_OCT,  "invalid host (not support oct format ip)", "�Ƿ�����(��ǰ��֧�ְ˽��Ƶ�ip��ַ)"},
    {ERR_DGN_INVALID_HOST_BEGIN,   "invalid host (must begin with number,char or \"_\")", "�Ƿ�����(������������������ĸ����\"_\"��ʼ)"},
    {ERR_DGN_INVALID_TIMES,    "invalid Repeate times",                      "�Ƿ��ظ�����"},
    {ERR_DGN_INVALID_TO,       "invalid timeout",                            "�Ƿ���ʱʱ��"},
    {ERR_DGN_INVALID_SIZE,     "invalid data size",                          "�Ƿ����ݰ���С"},
    {ERR_DGN_INVALID_DSCP,     "invalid DSCP",                               "�Ƿ�DSCPֵ"},
    {ERR_DGN_NOHOST,          "not set host",                               "��ǰ��δ��������"},
    {ERR_DGN_INVALID_HOST_A,   "invalid host (must not contain char except \".\", \"_\", \"-\", \"@\", a-z A-Z 0-9)",    "�Ƿ�����(���������ܰ���\".\", \"_\", \"-\", \"@\", ��ĸ������֮����ַ�)"},
    {ERR_DGN_INVALID_HOST_LEN, "invalid host (too long, exceeding 256)",    "������̫��(����256���ַ�)"},

    /* AUTH */
    {ERR_AUTH_CANNOT_CHANGE_USER,   "cannot modify user name","�������޸��û���","No puede modificar nombre del usuario"},
    {ERR_AUTH_CANNOT_CHANGE_LEVEL,  "cannot modify user name","�������޸��û�����","No puede modificar el nivel de usuario"},
    {ERR_AUTH_CANNOT_REMOVE_ADMIN,  "cannot remove admin","������ɾ��admin�û�","No puede quitar el admin"},
    {ERR_AUTH_HAVE_SAME_USER,       "have same user","������ͬ�û���","Tenga mismo usuario"},
    {ERR_AUTH_PASSWORD_TOO_LONG,    "password length must less than 64","���볤�ȱ���С��64", "Longitud de la contrasena debe menos de 64"},
    {ERR_AUTH_USERNAME_TOO_LONG,    "user name length must less than 32","�û������ȱ���С��32","La longitud del username debe menos de 32"},
    {ERR_AUTH_SESSION_FULL,         "Too many users log on, Please wait a minute","��¼�û�����","Demasiados usuarios abren una sesion, satisfacen espera al minuto"},
    {ERR_AUTH_WRONG_USER,           "Username wrong","������û���","Mal del username"},
    {ERR_AUTH_WRONG_PASSWORD,       "Password wrong","���������","Contrasena incorrecta"},
    {ERR_AUTH_WRONG_USER_OR_PWD,    "Username or Password wrong","������û���������","Contrasena incorrecta"},
    {ERR_AUTH_SESSION_TIMEOUT,      "Session is timeout","�Ự��ʱ","Tiempo de inactividad excedido"},
    {ERR_AUTH_SESSION_ILLEGAL,      "Inadequate access","����Ȩ�޲���","Acceso inadecuado"},
    {ERR_AUTH_LOGIN_FREQUENT,       "Please login in one minute","��һ���Ӻ��ٵ�¼","Por favor conexion en un minuto"},
    {ERR_AUTH_LOGOUT_SUCCESS,       "Logout success","ע���ɹ�","Se ha desconectado del equipo"},
    {ERR_AUTH_OLDPASSWORD_INVALID,"Old password is wrong!","���������","La vieja contrasena es incorrecta"},
#ifdef CONFIG_APPS_LOGIC_FIRSTLOGIN_CHECK
    {ERR_AUTH_LOGIN_FIRSTUSE,    "first login!","��һ�ε�¼","Primera conexion"},
    {ERR_AUTH_WRONG_PASSWD,    "Please input a new password different from the default one","�����벻����Ĭ��������ͬ"},
#endif
    /* WAN */
    {ERR_WAN_INVALID_INNER_MSG, "inner msg error",                            "�ڲ���Ϣ����"},
    {ERR_WAN_INVALID_VAL,       "get invalid value",                          "��ȡֵ��Ч"},
    {ERR_WAN_INVALID_SVC_LIST,   "Invalid Service List",                          "�Ƿ��ķ�������"},
    {ERR_WAN_INVALID_CONN_TYPE,  "Invalid Connection Type",                       "�Ƿ�����������"},
    {ERR_WAN_INVALID_LAN_INTF,   "Invalid Lan Intf",                              "�Ƿ���lan�ڰ�"},
    {ERR_WAN_LAN_INTF_REPEAT,     "The lan intf mutil bind",                       "lan���ظ���"},
    {ERR_WAN_TOO_MORE_BRIDGE,    "Not allowed to Create more than one bridge on the same vlan", "һ��wan vlan�ϲ����������������"},
    {ERR_WAN_BRIDGE_ROUTE_MIX,   "Not allowed to mix bridge and route on the same vlan",        "һ��wan vlan�ϲ��������ź�·�����ӹ�������ӵ����"},
    {ERR_WAN_TR069_BRIDGE,       "The TR069 service type is not allowed to config on a bridge conn",  "TR069�������͵�wan���Ӳ��ܲ���������"},
    {ERR_WAN_TR069_LAN,          "Not allowed to bind lan to a TR069 wan conn",   "TR069�������͵�wan���Ӳ������lan��"},
    {ERR_WAN_NOT_EXIST,          "please select a wan connection",   "��ѡ��һ��wan����"},
    {ERR_WAN_INVALID_VLAN_ID,    "Invalid Vlan ID", ""},
    {ERR_WAN_NOREOA_LINK_EXIST,  "For one PVC, only one IPOA or PPPOA can be configured", ""},
    {ERR_WAN_EOA_LINK_EXIST,  "For one PVC, MER, PPPOE and Bridging are exclusive with IPOA and PPPOA", ""},
    {ERR_WAN_TOO_MANY_PVC,    "CPE can support up to 8 PVCs", ""},
    {ERR_WAN_CANNOT_CHNG_LINK_TYPE, "For one PVC, more than one MER, PPPOE or Bridging connetion exist, can not change protocol to IPOA or PPPOA", ""},
    {ERR_WAN_ETH_LINK_CONFLICT, "For one VLAN,only 1 IPOE, 1 Bridging and 4 PPPOE connections can be configured at the same time", "һ��VLAN��ֻ��ͬʱ����һ���ź�һ��MER����"},
    {ERR_WAN_DSL_LINK_CONFLICT, "For one (PVC,VLAN), only 1 MER, 1 Bridging and 4 PPPOE connections can be configured at the same time", "һ����PVC,VLAN����ֻ��ͬʱ����һ���ź�һ��MER����"},
    {ERR_WAN_TOO_MANY_ENABLED_DSL_PPP, "CPE can support up to 4 enabled PPPOA and PPPOE connections", ""},
    {ERR_WAN_TOO_MANY_ENABLED_ETH_PPP, "CPE can support up to 4 enabled PPPOE connections", ""},
    {ERR_DSL_TOO_MANY_IP_INST, "For one (PVC,VLAN), only 1 MER, 1 Bridging connections can be configured at the same time", "һ����PVC,VLAN����ֻ��ͬʱ����һ���ź�һ��MER����"},
    {ERR_ETH_TOO_MANY_IP_INST, "For one VLAN, only 1 IPOE, 1 Bridging connections can be configured at the same time", "һ��VLAN��ֻ��ͬʱ����һ���ź�һ��MER����"},
    
    /* LAN */
    {ERR_LAN_INVALID_IP_FORMAT, "Invalid IP format",             "�Ƿ�IP��ַ��ʽ"},
    {ERR_LAN_INVALID_LAN_IP, "Invalid LAN IP",             "�Ƿ���LAN IP��ַ"},
    {ERR_LAN_MACLIST_TOO_LONG, "Mac addreess list too long",             "mac��ַ�б�������������"},
    {ERR_LAN_INVALID_MACLIST, "Invalid Mac address",             "�����mac��ַ"},
    {ERR_LAN_IP_MASK_CONFLICT, "Net Mask confilct with IP",             "IP��ַ�����������ͻ"},
    {ERR_LAN_IP_CONFLICT, "Repeated IP",             "IP��ַ������IP�����ظ�"},
    {ERR_LAN_LAST_INSTANCE, "This is first IP config,cannot delete",             "��һ��IP���ò�����ɾ��"},
    {ERR_LAN_TOO_MANY_INSTANCE, "Too many LAN IP config",             "LAN IP�����Ѿ��дﵽ���ޣ��޷������"},
    {ERR_LAN_SAME_REGION_CONFLICT, "same net region with other lan device,will bring route error",             "LAN IP����������LAN����������ͬ���ᵼ��VLAN����"},
    {ERR_LAN_NOT_EXIST, "please select a lan interface",             "��ѡ��һ��lan����"},
    {ERR_LAN_INVALID_IPV6_FORMAT, "Invalid IPv6 address format", "�Ƿ�IPv6��ַ��ʽ"},

    /* DNS */
    {ERR_DNS_VALID_DNS_ADDR,        "invalid DNS address",            "DNS��ַ��ʽ����"},
    {ERR_DNS_HAS_REPEAT,            "DNS address repeat",             "DNS��ַ���ظ�"},
    {ERR_DNS_CUSTOM_BE_EMPTY,       "When allowed to custom DNS, DNS addresses can not be empty",
                                    "�������Զ���DNSʱ, DNS��ַ����Ϊ��"},

    /* ETHLAN & ETHWAN */
    {ERR_ETH_INVALID_BITRATE,    "invalid max bit rate",     "�Ƿ�������"},
    {ERR_ETH_INVALID_DUPLEXMODE, "invalid duplex mode",     "�Ƿ���˫��ģʽ"},

    /* LSVLAN */
    {ERR_LSVLAN_INVALID_BRIDGENAME,  "invalid bridge name",  "�Ƿ�������"},
    {ERR_LSVLAN_INVALID_VLANID,      "invalid Vlan ID",      "�Ƿ�VLAN ID"},
    {ERR_LSVLAN_INVALID_FILTERBRREF, "invalid Filter Bridge Reference",  "�Ƿ��������ù��˹���"},
    {ERR_LSVLAN_INVALID_FILTERIF,    "invalid Filter Interface",  "�Ƿ��ӿ����ù��˹���"},
    {ERR_LSVLAN_DEL_FIRST_BRIDGE,    "the first bridge should not be deleted",  "��һ���Ų���ɾ��"},
    {ERR_LSVLAN_INSTANCE_IN_USE,     "the instance which is in use should not be deleted", "����ʹ��ʵ������ɾ��"},
    {ERR_LSVLAN_BRNAME_CONFLICT,     "Bridge name conflict", "�����Ƴ�ͻ"},
    {ERR_LSVLAN_BRVLAN_CONFLICT,     "Vlan id conflict", "VLAN ID��ͻ"},
    {ERR_LSVLAN_IF_CONFLICT,         "One interface must belong to only one LAN Group", "һ���ӿ�ֻ������һ��LAN Group"},
    {ERR_LSVLAN_WLAN_IF_TAGGED,      "VAP interface should not be tagged mode", "VAP�ӿڲ�����taggedģʽ"},

    /*LPVLAN*/
    {ERR_LPVLAN_INVALID_VLAN_ID,     "invalid Vlan ID", "�Ƿ�VLAN ID"},
    {ERR_LPVLAN_INVALID_VLAN_ID_LIST,"invalid Vlan ID list", "�Ƿ�VLAN ID�б�"},
    {ERR_LPVLAN_TOO_MANY_VLAN,       "too many Vlan ID in list", "�б���VLAN ID����"},
    

    /* IPCONN */
    {ERR_IPCONN_NAME_TOO_LONG,      "the length of connection name can not over 256",
                                    "�������Ƴ��Ȳ��ܳ���256"},
    {ERR_IPCONN_NAME_INVALID,       "invalid connection name (only contains the following characters: [-_.@0-9a-zA-Z] ) ",
                                    "��Ч��������(ֻ���������ַ�: [-_.@0-9a-zA-Z])"},
    {ERR_IPCONN_ADDR_TYPE_INVALID,  "invalid address type value",
                                    "��Ч�ĵ�ַ����"},
    {ERR_IPCONN_ACTIVE_CONN_OVER,   "the number of active connections to the ceiling",
                                    "��ǰ����Ӹ����ѵ�����"},
    {ERR_IPCONN_WAN_CONN_OVER,      "the number of WAN connections to the ceiling",
                                    "��ǰWAN���Ӹ����ѵ�����"},
    {ERR_IPCONN_WAN_IPCONN_OVER,    "Each WAN Device allow to create just a IP Connection",
                                    "һ��WAN�豸ֻ������һ��IP����"},
    {ERR_IPCONN_IP_GATEWAY_SAME,    "Static IP address and default gateway address can not be the same",
                                    "��̬IP��ַ��Ĭ�����ص�ַ����һ��"},
    {ERR_IPCONN_NAME_EXISTS,        "Connection Name has exists",
                                    "�����������ظ�"},
    {ERR_IPCONN_CONNTYPE_INVALID,   "invalid connection type",
                                    "��Ч����������"},
    {ERR_PPPOE_ACNAME_INVALID,   "invalid AC name",
                                    "��Ч��AC��"},
    {ERR_PPPOE_SERVICENAME_INVALID,   "invalid service name",
                                    "��Ч�ķ�����"},
    {ERR_IPCONN_IP_GATEWAY_NOT_MATCH,    "Static IP address and default gateway address is not in the same network",
                                    "��̬IP��ַ��Ĭ�����ص�ַ����ͬһ����"},

    /* DHCPS */
    {ERR_DHCPS_INVALID_CLASS_ID, "Unknown class id ", "δ֪�ͻ�����"},
    {ERR_DHCPS_EXIST_CLASS_ID, "Already exist class id ", "�Ѵ��ڵĿͻ�����"},
    {ERR_DHCPS_IFIP_ADDRPOOL_UNMATCH, "Interface address and dhcp address pool not match", "�ӿڵ�ַ���ַ�ز�ƥ��"},
    {ERR_DHCPS_LEASE_TIME, "Lease time must be more than or equal to -1", "��Լʱ�������ڻ����-1"},
    {ERR_DHCPS_CPEIP_CONFLICT_DHCP_POLL_IPADDR, "CPE IP conflicts with DHCP pool IP addr", "����IP��DHCP��ַ�س�ͻ"},

    /* PPPOE */
    {ERR_PPPOE_NO_RESOURSE, "no resourse",  "û����Դ����"},
    {ERR_PPPOE_INVALID_VAL,  "invalid value",  "��Чֵ"},
    {ERR_PPPOE_INVALID_PATH,	"invalid path", "��Ч·��"},
    {ERR_PPPOE_EXCEED_CONNECTION_NUM,	"exceed connection number", "����������"},
    {ERR_PPPOE_INVALID_INSTANCE,	"invalid instance", "��Чʵ��"},
    {ERR_PPPOE_ENABLE_INVALID,	"invalid enable", "��Чenable"},
    {ERR_PPPOE_USERNAME_INVALID,	"invalid  username", "��Ч�û���"},
    {ERR_PPPOE_USERNAME_OVERLENTH,	"username  over  length", "�û�������"},
    {ERR_PPPOE_USERNAME_EMPTY,	"username empty", "�û���Ϊ��"},
    {ERR_PPPOE_PASSWD_INVALID,	"invalid passwd", "��Ч����"},
    {ERR_PPPOE_PASSWD_OVERLENTH,	"passwd over length", "���볬��"},
    {ERR_PPPOE_PASSWD_EMPTY,	"passwd empty", "����Ϊ��"},
    {ERR_PPPOE_CONNETNAME_INVALID,	"connected name invalid", "��Ч����������"},
    {ERR_PPPOE_CONNETNAME_OVERLENTH,	"connected name over length", "�������Ƴ���"},
    {ERR_PPPOE_AUTH_INVALID,	"invalid auth", "��Ч�ļ�Ȩ��ʽ"},
    {ERR_PPPOE_MRU_INVALID,	"MRU invalid", "��Ч��MRU"},
    {ERR_PPPOE_TRIGGER_INVALID,	"invalid trigger", "��Ч�Ĵ�����ʽ"},
    {ERR_PPPOE_CONNACTION_INVALID,	"connect button invalid", "�ֶ�������ʽ�İ�ť��Ч"},
    {ERR_PPPOE_LCPECHO_INVALID,	"LCPECHO invalid, should be 5 ~ 65535", "��Ч��LCPECHO��Ӧ��Ϊ 5 ~ 65535"},
    {ERR_PPPOE_LCPECHO_RETRY_INVALID,	"LCPECHO retry invalid, should be 0 ~ 65535", "��Ч��LCPECHO���Դ�����Ӧ��Ϊ 0 ~ 65535"},
    {ERR_PPPOE_ACTIVE_CONN_OVER,	"exceed active connect", "�������õ�������"},
    {ERR_PPPOE_INVALID_INNER_MSG,	"inner message invalid", "�ڲ���Ϣ����"},
    {ERR_PPPOE_INVALID_MATCH_INDEX,	"the index of message invalid", "��Ϣ��������"},
    {ERR_PPPOE_NO_PPPSESSION, "no pppsession in flash", "û��pppsession ����flash��"},
    {ERR_PPPOE_NO_PPPSESSION_IDX, "invalid pppsession idx", "pppsession idx ����"},
    {ERR_PPPOE_ERROR_PPPSESSION_VAL, "pppsession  value error", "pppsession ����ֵ"},
    {ERR_PPPOE_NAME_REPEATED,  "WAN connected name repeated",  "WAN���������ظ�"},
    {ERR_PPPOE_IDLEDISCONNTIME_INVALID,  "invalid idle disconnect time",  "��Ч�����ó�ʱʱ��"},
    {ERR_PPPOE_APN_INVALID,                 "invalid apn",  "��Ч��APN"},
    {ERR_PPPOE_APN_EMPTY,	                "apn empty",  "APNΪ��"},
    {ERR_PPPOE_APN_OVERLENTH,	            "apn over  length",  "APN����"},
    {ERR_PPPOE_DIALNUMBER_INVALID,          "invalid dialnumber",  "��Ч�Ĳ��ź���"},
    {ERR_PPPOE_DIALNUMBER_EMPTY,            "dialnumber empty",  "���ź���Ϊ��"},
    {ERR_PPPOE_DIALNUMBER_OVERLENTH,	    "dialnumber over length",  "���ź��볬��"},
    {ERR_PPPOE_MTU_INVALID,	                "MTU invalid", "��Ч��MTU"},
    {ERR_PPPOE_TOO_MANY_PPPOU_INST,         "CPE can support up to 1 PPPoU connections", "CPEֻ����һ��3G����"},
    {ERR_PPPOE_IDLEDELAYTIME_INVALID,	    "Backup delay time invalid, should be 0~600", "��Ч�ı����ӳ�ʱ�䣬Ӧ��Ϊ0~600������Ҫ�ܱ�15����"},
    {ERR_PPPOE_IDLEDELAYTIME_EMPTY,	        "Backup delay time empty", "�����ӳ�ʱ��Ϊ��"},
    {ERR_PPPOE_IDLEDELAYTIME_DIVIDE,	    "Backup delay time  must be divisible by 15", "�����ӳ�ʱ������ܱ�15����"},

    /* QOS */
#ifdef CONFIG_APPS_LOGIC_QOS_IMQ    
    {ERR_QOS_INVALID_COMMITTED_RATE,    "Invalid Bandwidth value, should be 100-102400Kbps",
                                        "��Ч�Ĵ���ֵ,Ӧ��Ϊ100-102400Kbps"},
    {ERR_QOS_INVALID_PEAK_RATE,         "Invalid peak rate value, should be 100-102400Kbps",
                                        "��Ч�ķ�ֵ����ֵ,Ӧ��Ϊ100-102400Kbps"},
    {ERR_QOS_INVALID_BURST_SIZE,        "Invalid burst size, should be 5-100bytes", "��Ч��ͻ��������,Ӧ��Ϊ5-100bytes"},
    {ERR_QOS_INVALID_QUE_LEN,           "Queue length should be 10-150packets or 15000-225000bytes",
                                        "���г���Ӧ��Ϊ10-150packets��15000-225000bytes"},
    {ERR_QOS_INVALID_QUE_WEIGHT,        "Invalid queue weight value,should be 0-100", "��Ч�Ķ���Ȩ��ֵ,Ӧ��Ϊ0-100"},
    {ERR_QOS_INVALID_QUE_CAR,           "Invalid queue bandwidth value, should be 0-102400Kbps", "��Ч�Ķ��д���ֵ,Ӧ��Ϊ0-102400Kbps"},
    {ERR_QOS_INVALID_CLASS_IF,          "Invalid ingress interface", "��Ч������ӿ�"},
    {ERR_QOS_INVALID_VLAN_ID,           "Invalid vlan id, shoud be 1-4094", "��Ч��VLAN ID,Ӧ��Ϊ1-4094"},
    {ERR_QOS_INVALID_IP_LEN,            "Invalid ip length value,should be 20-1500", "��Ч��IP���ݳ���,Ӧ��Ϊ20-1500"},
    {ERR_QOS_INVALID_CLASS_QUE,         "Invalid index of queue instance", "��Ч�Ķ���ʵ������"},
    {ERR_QOS_QUEUE_NOT_EXIST,           "The queue instance does not exist", "����ʵ��������"},
    {ERR_QOS_QUEUE_CAR_CONFLICT,        "The bandwidth sum of all enabled queues shouled be less than or equal to the global bandwidth value",
                                        "���������ö��еĴ���ֵ֮��Ӧ��С�ڻ�����ܴ���ֵ"},
    {ERR_QOS_SHAPE_RATE_CONFLICT,       "Committed rate should be less than or equal to peak rate", "��ŵ����Ӧ��С�ڵ��ڷ�ֵ����"},
    {ERR_QOS_QUEUE_CAR_NOLIMIT_CONFLICT,"The bandwidth sum of all enabled queues shouled be less than or equal to peak rate",
                                        "���������ö��еĴ���ֵ֮��Ӧ��С�ڻ���ڷ�ֵ����"},
    {ERR_QOS_QUEUE_LEN_CONFLICT,        "Queue length should be 20-150packets or 30000-225000bytes",
                                        "���г���Ӧ��Ϊ20-150packets��30000-225000bytes"},
    {ERR_QOS_QUEUE_WEIGHT_CONFLICT,     "The weight sum of all enabled queues should be less than or equal to 100%",
                                        "���������ö��е�Ȩ��֮��Ӧ��С�ڻ����100%"},
    {ERR_QOS_QUEUE_PRIO_CONFLICT,       "Two enabled queues binding the same SP interface should not have the same priority",
                                        "�����󶨵�ͬһSP�ӿڵ������ö��е����ȼ�������ͬ"},
    {ERR_QOS_IP_LENGTH_CONFLICT,        "The minimum ip length should be less than or equal to the maximum ip length",
                                        "IP������СֵӦ��С�ڻ����IP�������ֵ"},
    {ERR_QOS_PORT_CONFLICT,             "The minimum port value should be less than or equal to the maximum port value",
                                        "�˿���СֵӦ��С�ڻ���ڶ˿����ֵ"},
    {ERR_QOS_APP_NAME_CONFLICT,         "The configuration with same app name has existed", "������ͬҵ�����Ƶ������Ѿ�������"},
    {ERR_QOS_INVALID_IP_ADDR,           "", "IP��ַ��ʽ����ȷ"},
    {ERR_QOS_INVALID_PREFIX_LEN,        "", "��Ч��IPv6��ַǰ׺����,Ӧ��Ϊ0-128"},
    {ERR_QOS_SRCIP_CONFLICT,            "", "ԴIP��ַ��ʽ����ȷ"},
    {ERR_QOS_DSTIP_CONFLICT,            "", "Ŀ��IP��ַ��ʽ����ȷ"},
    {ERR_QOS_INVALID_FLOW_LABEL,        "", "��Ч��Flow Labelֵ,Ӧ��Ϊ0-1048575"},
    {ERR_QOS_INVALID_TRAFFIC_CLASS,     "", "��Ч��Traffic Classֵ,Ӧ��Ϊ0-255"},
#else
    {ERR_QOS_INVALID_IF_PATH,           "Invalid egress interface" , "��Ч������ӿ�"},
#if 0
    {ERR_QOS_INVALID_COMMITTED_RATE,    "To ethernet interface,rate value should be 100-100000Kbps and to VAP interface it should be 100-30000Kbps",
                                        "������̫���ӿ�,����ֵӦ��Ϊ100-100000Kbps,����VAP�ӿ�Ӧ��Ϊ100-30000Kbps"},
    {ERR_QOS_INVALID_PEAK_RATE,         "To ethernet interface,rate value should be 100-100000Kbps and to VAP interface it should be 100-30000Kbps",
                                        "������̫���ӿ�,����ֵӦ��Ϊ100-100000Kbps,����VAP�ӿ�Ӧ��Ϊ100-30000Kbps"},
    {ERR_QOS_INVALID_COMMITTED_RATE,    "Invalid committed rate value, should be 100-100000Kbps",
                                        "��Ч�ĳ�ŵ����ֵ��Ӧ��Ϊ100-100000Kbps"},
    {ERR_QOS_INVALID_PEAK_RATE,         "Invalid peak rate value, should be 100-100000Kbps",
                                        "��Ч�ķ�ֵ����ֵ��Ӧ��Ϊ100-100000Kbps"},
    {ERR_QOS_INVALID_BURST_SIZE,        "Invalid burst size, should be 2-100", "��Ч��ͻ��������,Ӧ��Ϊ2-100"},
#endif    
    {ERR_QOS_INVALID_QUE_SCH_ALGORITHM, "Invalid queue scheduler Algorithm, should be DWRR or SP",
                                        "��Ч�Ķ��е��ȷ�ʽ��Ӧ��ΪDWRR��SP"},
    {ERR_QOS_INVALID_QUE_IF,            "Invalid index of interface instance", "��Ч�Ľӿ�ʵ������"},
    {ERR_QOS_QUE_IF_NOT_EXIST,          "The interface instance does not exist", "�ӿ�ʵ��������"},
    {ERR_QOS_INVALID_QUE_LEN,           "Queue length should be 20-150packets or 30000-225000bytes",
                                        "���г���Ӧ��Ϊ20-150packets��30000-225000bytes"},
    {ERR_QOS_INVALID_QUE_LEN_TYPE,      "Invalid queue length type, should be 0(packets) or 1(bytes)",
                                        "��Ч�Ķ��г������ͣ�Ӧ��Ϊ0(packets)��1(bytes)"},
    {ERR_QOS_INVALID_QUE_WEIGHT,        "Invalid queue weight value,should be 1-99", "��Ч�Ķ���Ȩ��ֵ,Ӧ��Ϊ1-99"},
    {ERR_QOS_INVALID_DROP_ALGORITHM,    "Invalid drop algorithm, should be DT(drop tail)", "��Ч�Ķ����㷨,Ӧ��ΪDT(β����)"},
    {ERR_QOS_INVALID_CLASS_TYPE,        "Invalid classify type, should be 0(L2) or 1(L3)", "��Ч�ķ�������,Ӧ��Ϊ0(L2)��1(L3)"},
    {ERR_QOS_INVALID_CLASS_IF,          "Invalid ingress interface", "��Ч������ӿ�"},
    {ERR_QOS_INVALID_ETH_TYPE,          "Invalid ethertype field valid", "��Ч����̫�������ֶ�ֵ"},
    {ERR_QOS_INVALID_ETH_PRIO,          "Invalid ethernet priority value, shoud be 0-7", "��Ч����̫�����ȼ�,Ӧ��Ϊ0-7"},
    {ERR_QOS_INVALID_VLAN_ID,           "Invalid vlan id, shoud be 1-4094", "��Ч��VLAN ID,Ӧ��Ϊ1-4094"},
    {ERR_QOS_INVALID_IP_LEN,            "Invalid ip length value,should be 20-1500", "��Ч��IP���ݳ���,Ӧ��Ϊ20-1500"},
    {ERR_QOS_INVALID_DSCP,              "Invalid DSCP value, should be PHB format", "��Ч��DSCPֵ,Ӧ��ΪPHB��ʽ"},
    {ERR_QOS_INVALID_PROTOCOL,          "Invalid protocol value, should be 0(tcp&udp),1(icmp),6(tcp),17(udp)",
                                        "��Ч��Э��ֵ��Ӧ��Ϊ0(tcp&udp), 1(icmp), 6(tcp), 17(udp)"},
    {ERR_QOS_INVALID_CLASS_QUE,         "Invalid index of queue instance", "��Ч�Ķ���ʵ������"},
    {ERR_QOS_CLASS_QUE_NOT_EXIST,       "The queue instance does not exist", "����ʵ��������"},
    {ERR_QOS_SHAPE_RATE_CONFLICT,       "Committed rate should be less than or equal to peak rate", "��ŵ����Ӧ��С�ڵ��ڷ�ֵ����"},
    {ERR_QOS_QUEUE_LEN_CONFLICT,        "Queue length should be 20-150packets or 30000-225000bytes",
                                        "���г���Ӧ��Ϊ20-150packets��30000-225000bytes"},
    {ERR_QOS_QUEUE_WEIGHT_CONFLICT,     "The weight sum of all enabled queue binding the same DWRR interface should be less than or equal to 100%",
                                        "�󶨵�ͬһ��DWRR�ӿڵ����������ö��е�Ȩ�غ�Ӧ��С�ڻ����100%"},
    {ERR_QOS_QUEUE_PRIO_CONFLICT,       "Two enabled queues binding the same SP interface should not have the same priority",
                                        "�����󶨵�ͬһSP�ӿڵ������ö��е����ȼ�������ͬ"},
    {ERR_QOS_IP_LENGTH_CONFLICT,        "The minimum ip length should be less than or equal to the maximum ip length",
                                        "IP������СֵӦ��С�ڻ����IP�������ֵ"},
    {ERR_QOS_PORT_CONFLICT,             "The minimum port value should be less than or equal to the maximum port value",
                                        "�˿���СֵӦ��С�ڻ���ڶ˿����ֵ"},
#endif
#if 0
    /* DYNAMIC ROUTE */
    {ERR_DYNAMIC_INVALID_VAL, "value invalid",  "ֵ��Ч"},
    {ERR_DYNAMIC_INVALID_CONFIG_FILE, "invalid configurate file",  "�����ļ���Ч"},
    {ERR_DYNAMIC_VERSION_INVALID, "RIP version invalid",  "��Ч��rip�汾"},
    {ERR_DYNAMIC_INVALID_PC_MSG, "PC inner message invalid",  "��Ч��PC�ڲ���Ϣ"},
    {ERR_DYNAMIC_INVALID_WAN, "WAN device invalid",  "WAN�豸��Ч"},
    {ERR_DYNAMIC_INVALID_INNER_PARA,  "invalid inner parament ",  "��Ч���ڲ�����"},
#endif

    /* DYNAMIC ROUTE */
    {ERR_DROUTE_INVALID_VAL, "value invalid",  "ֵ��Ч"},
    {ERR_DROUTE_INVALID_CONFIG_FILE, "invalid configurate file",  "�����ļ���Ч"},
    {ERR_DROUTE_VERSION_INVALID, "RIP version invalid",  "��Ч��rip�汾"},
    {ERR_DROUTE_INVALID_PC_MSG, "PC inner message invalid",  "��Ч��PC�ڲ���Ϣ"},
    {ERR_DROUTE_INVALID_WAN, "WAN device invalid",  "WAN�豸��Ч"},
    {ERR_DROUTE_INVALID_INNER_PARA,  "invalid inner parament ",  "��Ч���ڲ�����"},

    {ERR_STATIC_INVALID_INSTANCE      , "invalid instance",  "��Чʵ��"},
    {ERR_STATIC_INTERFACE             , "invalid interface ",  "��Ч�������豸"},
    {ERR_STATIC_NO_RESOURSE           , "has no resourse",  "û����Դ"},
    {ERR_STATIC_INVALID_VAL           , "value invalid",  "ֵ��Ч"},
    {ERR_STATIC_ENABLE_INVALID        , "enable invalid",  "ʹ�ܲ�����Ч"},
    {ERR_STATIC_INVALID_PATH          , "invalid path",  "��Ч·��"},
    {ERR_STATIC_INVALID_DEFAULT_PARA  , "invalid default route parament",  "��ЧĬ��·�ɲ���"},
    {ERR_STATIC_INVALID_ROUTE_PARA    , "invalid route para",  "��Ч·�ɲ���"},
    {ERR_STATIC_INVALID_PARA          , "invalid parament",  "��Ч����"},
    {ERR_STATIC_DEL_INSTANCE          , "del instance error",  "ɾ��ʵ������"},
    {ERR_STATIC_INVALID_METRIC        , "invalid metric",  "��Чmetric"},
    {ERR_STATIC_INVALID_IP            , "invalid IP",  "��ЧIP"},
    {ERR_STATIC_IP_EMPTY              , "IP empty",  "IP ��ַ��"},
    {ERR_STATIC_INVALID_MASK          , "invalid netmask",  "��Ч������"},
    {ERR_STATIC_MASK_EMPTY            , "netmask empty",  "�������"},
    {ERR_STATIC_INVALID_GATEWAY       , "invalid gateway",  "��Ч����"},
    {ERR_STATIC_METRIC_OUT_LIMITED    , "metric is out of limited number 0~128",  "metric����������Χ0~128"},
    {ERR_STATIC_INVALID_LOGFILE       , "invalid log file",  "��Ч��log�ļ�"},

   /* V6 Static routing */
//    {ERR_V6STATIC_ENABLE_INVALID        , "enable invalid",  "ʹ�ܲ�����Ч"},
//    {ERR_V6STATIC_BAD_PREFIX        , "Invalid Prefix! Valid range is 3-128",  "Prefix����ȷ����ȷ��ȡֵ��Χ��3-128"},    
//    {ERR_V6STATIC_BAD_METRIC        , "Invalid Metric!",  "��Ч��metric����"},  

    /* SNMPC */
    {ERR_SNMPC_INVALID_VERSION,         "invalid SNMP version", "��Ч��SNMP�汾��"},

    /* Firewall */
    {ERR_FW_RULES_OVER,                 "The number of rules can not be over 20",
                                        "������ӵĹ����ܳ���20��"},
    {ERR_FW_RULE_EXISTS,                "this rule has exists in filter table",
                                        "�����ظ��Ĺ������"},
    {ERR_FW_SRC_IP_EMPTY,                "Source IP Address can not be empty !",
                                        "ԴIP��ַ�ز���Ϊ��"},
    {ERR_FW_DEST_IP_EMPTY,                "Destination IP Address can not be empty !",
                                        "Ŀ��IP��ַ�ز���Ϊ��"},

    /* SNTP */
    {ERR_SNTP_INVALID_UP_INTERVAL,      "Invalid update interval value, shoud be 1-24(hours)",
                                        "��Ч�ĸ���ʱ����ֵ,Ӧ��Ϊ1-24(Сʱ)"},
    {ERR_SNTP_INVALID_RETRY_INTERVAL,   "Invalid retry interval value, should be 1-60(minutes)",
                                        "��Ч������ʱ����,Ӧ��Ϊ1-60(����)"},
    {ERR_SNTP_INVALID_NTP_SERVER,       "The Ip address or domain name of NTP server is invalid",
                                        "NTP��������IP��ַ����������Ч��"},
    {ERR_SNTP_INVALID_DATA_TIME,        "Invalid time format",   "ʱ���ʽ���Ϸ�"},
    {ERR_SNTP_NULL_NTPSERVER_STR,       "Two NTP servers should not be null at the same time",
                                        "����NTP����������ͬʱΪ��"},
    {ERR_SNTP_NTPSERVERS_CONFLICT,      "Two NTP servers should not be same value",
                                        "����NTP������������ͬ"},
    {ERR_SNTP_DST_CONFLICT,             "The conflict between the start and the end of daylight saving time exist",
                                        "����ʱ��ʼ�ͽ���ʱ��֮����ڳ�ͻ"},

    /*tr069be*/
    {ERR_TR069BE_INVALID_URL,  "invalid URL", "�Ƿ���URL"},
    {ERR_TR069BE_INVALID_UPLOAD_URL, "unable to connect to the server", "���ӷ�����ʧ��"},
    {ERR_TR069BE_INVALID_DOWNLOAD_URL, "unable to connect to the server", "���ӷ�����ʧ��"},
    {ERR_USERNAME,  "invalid username", "�Ƿ����û���"},
    {ERR_PASSWORD,  "invalid password", "�Ƿ�������"},
    {ERROR_TR069_INVALID_CAFILE, "Invalid CAfile", "�Ƿ���֤��"},

    {ERR_URLFILTER_INVALID_INNER_PARA  ,  "inner parament error",   "�ڲ��������ݴ���"},
    {ERR_URLFILTER_INVALID_VAL         ,  "invalid value",   "ֵ��Ч"},
    {ERR_URLFILTER_ENABLE_EMPTY        ,  "value enable empty",   "ֵenableΪ��"},
    {ERR_URLFILTER_ENABLE_INVALID      ,  "value enable wrong",   "ֵenable����"},
    {ERR_URLFILTER_INVALID_CONFIG_FILE ,  "invalid config file",   "��Ч�����ļ�"},
    {ERR_URLFILTER_MODE_EMPTY          ,  "value mode empty",   "ֵģʽΪ��"},
    {ERR_URLFILTER_MODE_INVALID        ,  "value mode wrong",   "ֵģʽ����"},
    {ERR_URLFILTER_URL_CONFLICT        ,  "URL conflict",   "URL ��ͻ"},
    {ERR_URLFILTER_URLNUMBER_OVER      ,  "URL Number is over", "������url��Ŀ������Χ"},
    {ERR_URLFILTER_URL_LENGTH_OVER     ,  "URL length is over", "URL����"},
    {ERR_URLFILTER_URL_ERROR           ,  "URL value is error",  "URLֵ����"},
    {ERR_URLFILTER_INVALID_FILTERMODE           ,  "Invalid filter mode",  "��Ч��URL����ģʽ"},
    {ERR_URLFILTER_INVALID_TIME           ,  "Invalid time",  "��Чʱ��"},
    {ERR_URLFILTER_INVALID_DAY           ,  "Invalid day",  "��Ч������"},
    {ERR_URLFILTER_TIME_CONFLICT           ,  "Start time is larger then end time",  "��ʼʱ����ڽ���ʱ��"},
    {ERR_URLFILTER_MAC_CONFLICT           ,  "MAC is exist already",  "MAC��ַ�Ѿ�����"},

    /* IPT */
    {ERR_IPT_CONFLICT_DMZ,            "DMZ host ip should not be null", "DMZ������ַ������Ϊ��"},

    /* WLAN */
    {ERR_WLAN_NO_RESOURSE,            "no resourse", "��Դȱ��"},
    {ERR_WLAN_INVALID_VAL,            "invalid value",   "ֵ��Ч"},
    {ERR_WLAN_INVALID_PATH,            "invalid path",  "��Ч·��"},
    {ERR_WLAN_VAL_EMPTY,            "value empty",   "ֵΪ��"},
    {ERR_WLAN_EXCEED_VAP_NUM,             "The number of Vap can not exceed 5",
                                                                "������ӵ�VAP���ܳ���5��"},
    {ERR_WLAN_INVALID_INSTANCE,            "invalid instance", "��Чʵ��"},
    {ERR_WLAN_INVALID_VAPIDX,            "invalid Vap index", "��Ч��Vap���"},
    {ERR_WLAN_NAME_REPEATED,            "name repeated", "���ظ�"},
    {ERR_WLAN_INVALID_SESSION,   "wps session has already built", "�Ѿ�������WPSЭ��"},
	{ERR_WLAN_IN_OOB,   "It is already in OOB status", "�Ѿ�����OOB״̬"},
	{ERR_WLAN_11N_TIKP_ERROR,   "802.11n mode and TIKP encryption can not be configurated together!", "11n ģʽ���ܺ�TKIP����ģʽ����!"},

    /* PORTMAPPING */
    {ERR_PTMP_INVALID_DESCRIPTION,    "Invalid characters inclued in description", "������Ϣ�к��зǷ��ַ�"},
    {ERR_PTMP_CONFLICT_PORTRANGE,     "The minimum port value should be less than or equal to the maximum port value",
                                      "�˿���СֵӦ��С�ڻ���ڶ˿����ֵ"},
    {ERR_PTMP_CONFLICT_TUPLE,         "Two instances conflict for the conflicting tuple composed of RemoteHost, ExternalPort and Protocol ",
                                      "��������ʵ������Ԫ��(Զ������, �ⲿ�˿ں�Э��)������ͻ"},
	{ERR_PTMP_CONFLICT_PORTRANGE_GENERAL,"Port have been used by ACL,Please change other port",
	                                  "�˿��Ѿ�����������б�ռ��,��ʹ�������˿�!"},
	{ERR_PTMP_HOST_NAME_NOT_EXIST,    "The Host Name does not exist, please check the DHCP client table!",
	                                  "�����������ڣ�����DHCP�ͻ����б�!"},


	/* USB3G */
	{ERR_USB3G_INVALID_PIN_LENGTH,                "The length of PIN code must be 4",
                                        "PIN��ĳ��ȱ���Ϊ4"},
    {ERR_USB3G_INVALID_PUK_LENGTH,                "The length of PUK code must be 8",
                                        "PUK��ĳ��ȱ���Ϊ8"},                                    
	{ERR_USB3G_INVALID_PINPUKCODE,            "The PIN/PUK code must be decimal",
                                        "PIN/PUK�������ʮ�����������"},

    /* ACL */
    {ERR_ACL_RULES_OVER,                "The number of rules can not be over 10",
                                        "������ӵķ����ܳ���10��"},
    {ERR_ACL_RULE_EXISTS,               "This open port has been occupied",
                                        "�˿��Ŷ˿��Ѿ���ռ��"},
    {ERR_ACL_PORT_BE_ZERO,              "When protocol is ICMP, the port value must be zero",
                                        "��Э��ΪICMPʱ,�˿�ֵ����Ϊ0"},
    {ERR_ACL_PORT_CANOT_ZERO,           "When protocol is not ICMP, The port value can not be zero",
                                        "��Э�鲻ΪICMPʱ,�˿�ֵ����Ϊ0"},
    {ERR_ACL_PORT_IN_USE,               "This port has been used",
                                        "�˶˿��Ѿ���ʹ��"},

    /* LOGGER */
    {ERR_LOGGER_UPLOAD_FAILED,          "Log file upload failed",  "��־�ļ��ϴ�ʧ��"},
    {ERR_LOGGER_INVALID_TFTP_SERVER,    "Invalid TFTP server",  "�Ƿ���TFTP������"},
    {ERR_LOGGER_CLEAR_FAILED,           "Log file clear failed",  "��־�ļ����ʧ��"},
    {ERR_LOGGER_EMPTY_TFTP_SERVER,      "Server address cannot be empty","��������ַ����Ϊ��"},
    {ERR_LOGGER_INVALID_SERVER,         "Invalid server","�Ƿ��ķ�����"},

    /* ATM */
    {ERR_ATM_PVC,                       "VPI/VCI is invalid", "PVC���ò��Ϸ�������VPI/VCI�ĸ�ʽ��д"},
    {ERR_ATM_VPI,                       "Invalid VPI value (0~255)", "VPIֵ�Ƿ�(0~255)"},
    {ERR_ATM_VCI,                       "Invalid VCI value (32~65535)", "VCIֵ�Ƿ�(32~65535)"},
    {ERR_ATM_UNKNOWN_ENCAP,             "Unkown encapsulation", "δ֪��װ����"},
    {ERR_ATM_UNKNOWN_QOS_TYPE,          "Unkown Qos type", "δ֪Qos����"},
    {ERR_ATM_PCR,                       "Invalid PCR value (0~7100)", "��ֵ��Ԫ����/��Ԫ�ӳٱ仯�ʷǷ�(0~7100)"},
    {ERR_ATM_MBS,                       "Invalid MBR value (0~1000000)", "���ͻ���ߴ�Ƿ�(0~1000000)"},
    {ERR_ATM_SCR,                       "Invalid SCR value (0~7099)", "ƽ����Ԫ���ʷǷ�"},
    {ERR_ATM_MIN_PCR,                   "Wrong MCR value (0~pcr)", "��С��Ԫ���ʷǷ�"},
    {ERR_ATM_VLAN_ID,                   "Wrong vlan id (0~4095)", "VLAN ID�Ƿ�(0~4095)"},
    {ERR_ATM_VLAN_PRIORITY,             "Wrong vlan priority(0~7)", "VLAN���ȼ��Ƿ�(0~7)"},
    {ERR_ATM_VLAN_ID_CONFLICT,          "Same vlan id", "VLAN ID�����ظ�"},

    /* UPNP */
    {ERR_UPNP_SRCIP_EXIST,              "This IP address is already in the list","��IP��ַ�����б���"},

    /* DDNS */
    {ERR_DDNS_INVALID_DDNSPROVIDER,     "Invalid DDNS provider","��Ч�Ķ�̬DNS������"},
    {ERR_DDNS_INVALID_HOSTNAME,         "Invalid hostname","��Ч��ע������"},
    {ERR_DDNS_HOSTNAME_LENGTH_OVER,     "Hostname length is over","ע����������"},
    {ERR_DDNS_INVALID_WAN_CONN_SID,     "Invalid WAN connection ID","��Ч��WAN����"},
    {ERR_DDNS_INVALID_USERNAME,         "Invalid username","��Ч���û���"},
    {ERR_DDNS_USERNAME_LENGTH_OVER,     "Username length is over","�û�������"},
    {ERR_DDNS_INVALID_PASSWORD,         "Invalid password","��Ч������"},
    {ERR_DDNS_PASSWORD_LENGTH_OVER,     "Password length is over","���볬��"},
    {ERR_DDNS_INVALID_EMAIL,            "Invalid email address","��Ч�������ַ"},
    {ERR_DDNS_EMAIL_LENGTH_OVER,        "Email address length is over","�����ַ����"},
    {ERR_DDNS_INVALID_KEY,              "Invalid key","��Ч����Կ"},
    {ERR_DDNS_KEY_LENGTH_OVER,          "Key length is over","��Կ����"},
    {ERR_DDNS_HOSTNAME_CONFLICT,        "Hostname exist already","ע�������Ѿ�����"},
    {ERR_DDNS_INVALID_DDNS_HOST,         "Invalid DDNS host","��Ч��DDNS ������"},
    {ERR_DDNS_DDNS_HOST_LENGTH_OVER,     "DDNS host length is over","DDNS������������ʶ����"},
    {ERR_DDNS_PRIVODER_CONFLICT,        "www.oray.cn exist already","www.oray.cn�Ѿ�����"},

    /* SUPP */
    {ERR_SUPP_USERNAME_OR_PASSWORD_BE_EMPTY, "username or password empty", "�û���������Ϊ��"},

    /* MACFILTER */
    {ERR_MACFILTER_INVALID_USERNAME,  "Invalid username","��Ч�û���"},
    {ERR_MACFILTER_INVALID_FILTERMODE,  "Invalid filter mode","��Ч�Ĺ���ģʽ"},
    {ERR_MACFILTER_INVALID_TIME,        "Invalid time","��Ч��ʱ��"},
    {ERR_MACFILTER_INVALID_DAY,         "Invalid day","��Ч������"},
    {ERR_MACFILTER_TIME_CONFLICT,       "Start time is larger then end time","��ʼʱ����ڽ���ʱ��"},
    {ERR_MACFILTER_MAC_CONFLICT,        "MAC is exist already","MAC��ַ�Ѿ�����"},
    {ERR_MACFILTER_INVALID_INNER_PARA,  "Invalid inner parameter","��Ч�ڲ�����"},

    /*RADVD*/
    {ERR_HEX_FORMAT_WRONG,          "invalid hex number","������Ч��ʮ��������"},
    {ERR_GLOBALID_OUT_OF_RANGE, "invalid global id,must be 40bit hex number","global id������ʮ��������"},
    {ERR_CONFIG_TYPE_INVALID_VALUE, "invalid config type,must be Static or Delegated","�������ͱ����� Static��Delegated"},
    {ERR_ROUTE_LIFETIME_OUT_OF_RANGE, "lifetime range is 0~65535","��������Ϊ0~65535"},
    {ERR_ROUTE_PREFERENCE_INVALID_VALUE, "route preference must be low,medium or high","���ȼ�����������Ϊlow,medium or high"},
    {ERR_SUBNET_ID_OUT_OF_RANGE, "SubnetID range is 0~255","������ΧΪ0~255"},
    {ERR_INVALID_PREFIX_LEN, "Invalid prefix length, it should be 0~128","��Ч�ĵ�ַǰ׺���ȣ���ΧΪ0~128"},        
    {ERR_INVALID_PREFIX, "Invalid prefix","��Ч�ĵ�ַǰ׺"},   


    /* TRACEROUT */
    {ERR_TRACERT_NOSTART,               "Traceroute is not started","����δ����"},
    {ERR_TRACERT_BUSY,                  "Traceroute is running","������������"},

    /* URLFILTER */
    {ERR_URLFILTER_INVALID_FILTERMODE,  "Invalid filter mode","��Ч�Ĺ���ģʽ"},
    {ERR_URLFILTER_INVALID_TIME,        "Invalid time","��Ч��ʱ��"},
    {ERR_URLFILTER_INVALID_DAY,         "Invalid day","��Ч������"},
    {ERR_URLFILTER_TIME_CONFLICT,       "Start time is larger than end time","��ʼʱ����ڽ���ʱ��"},
    {ERR_URLFILTER_INVALID_INNER_PARA,  "Invalid inner parameter","��Ч�ڲ�����"},

    /* SCHEDULE */
    {ERR_SCHEDULE_INVALID_NAME,             "Name is invalid.", "���ֲ���Ϊ���Ҳ��ܰ������ļ������ַ�"},
    {ERR_SCHEDULE_INVALID_ENABLEALLDAYS,    "Day select is invalid.","����ѡ�񲻺Ϸ�"},
    {ERR_SCHEDULE_INVALID_STARTTIME,         "Invalid time,must be 00:00-23:59","ʱ�䲻�Ϸ���������00:00-23:59"},
    {ERR_SCHEDULE_INVALID_ENDTIME,          "End time is invalid.","����ʱ�䲻�Ϸ�"},
    {ERR_SCHEDULE_INVALID_SPICIFIEDDAY,     "No days are specified.","δָ������"},
    {ERR_SCHEDULE_INVALID_SCHEDULEDAYS,     "Day select is invalid.","����ѡ�񲻺Ϸ�"},
    {ERR_SCHEDULE_INVALID_ENABLEALLTIME,    "Start time is invalid.","��ʼʱ�䲻�Ϸ�"},
    {ERR_SCHEDULE_TIME_CONFLICT,    "Start time is larger than End time.","��ʼʱ����ڽ���ʱ��"},
	{ERR_SCHEDULE_TIME_DISABLED,    "Before you create rules, please enable NTP service", "��������֮ǰ����������NTP����"},

    /* IGMP */
    {ERR_IGMP_INVALID_VERSION,              "IGMP version is invalid.","IGMP�汾���Ϸ�"},

    /* Diagnostics */
    {ERR_DG_INVALID_STATE,              "Diagnostics state is invalid.",    "���״ֵ̬���Ϸ�"},
    {ERR_DG_INVALID_INTERFACE,          "Interface is invalid.",            "�ӿڲ��Ϸ�"},
    {ERR_DG_PVC_TOO_MUCH,               "Too much PVC pairs in search list.", "�����б���PVC����̫��"},


    //default gateway
    {ERR_DEGW_EMPTY_IP,              "IP address is empty","IP��ַΪ��"},
    {ERR_DEGW_INVALID_IP,              "IP address is invalid","IP��ַ��Ч"},
    {ERR_DEGW_EMPTY_USERINIF,              "User interface is empty","�û��ӿ�Ϊ��"},
    {ERR_DEGW_INVALID_USERINIF,              "User interface is invalid","�û��ӿ���Ч"},
    {ERR_DEGW_EMPTY_SELECT,              "Mode is empty","ģʽΪ��"},
    {ERR_DEGW_INVALID_SELECT,              "Mode must be one of AutoDeftGW,GWIP andUserInIf","ģʽ����ΪAutoDeftGW,GWIP��UserInIf"},

    //IP ACL
    {ERR_IP_ACL_EMPTY_IP,              "IP address is empty","IP��ַΪ��"},
    {ERR_IP_ACL_INVALID_IP,              "IP address is invalid","IP��ַ��Ч"},
    {ERR_IP_ACL_IP_ZERO,              "You can use this function unless there is a IP at least","������һ����¼����ʹ���������"},

    /* IPTUNNEL */
    {ERR_IPTUNNEL_NO_RESOURSE,                "no resourse.", "û����Դ����"},
    {ERR_IPTUNNEL_VAL_EMPTY,                  "value empty.", "ֵΪ��"},
    {ERR_IPTUNNEL_NAME_OVERLENGTH,            "name over maxlength.", "ֵ������󳤶�"},
    {ERR_IPTUNNEL_VALID_STRING,               "name is valid string.", "ֵ����Ч�ַ���"},
    {ERR_IPTUNNEL_EXIST_NAME,                 "Already exist tunnel name.", "�Ѵ��ڵ�����"},
    {ERR_IPTUNNEL_EXIST_WAN_IF,               "Wan connection has been used.", "��ʹ�õ�Wan����"},

    /* GRE */
    {ERR_GRE_NO_RESOURSE,                "no resourse.", "û����Դ����"},
    {ERR_GRE_INVALID_MASK_LENGTH,        "Pritvate Tunnel Mask Len should be 1~32", "ͨ�������С��Ϊ1��32"},
    {ERR_GRE_INVALID_IPV4_ADDRESS,            "invalid ipv4 address.", "�Ƿ�ipv4��ַ"},
    {ERR_GRE_VALID_STRING,               "name is valid string.", "ֵ����Ч�ַ���"},
    {ERR_GRE_EXIST_NAME,                 "Already exist tunnel name.", "�Ѵ��ڵ�����"},
    {ERR_GRE_VAL_EMPTY,                  "value empty.", "ֵΪ��"},
    {ERR_GRE_NAME_OVERLENGTH,            "name over maxlength.", "ֵ������󳤶�"},
	{ERR_GRE_WAN_IP_CONFLICT,            "Tunnel already exists with the same remote IP and WAN interface", "�Ѵ����������ͬ��Զ��IP��WAN�ӿ�"},

    /* v6sroute */
    {ERR_V6SROUTE_BAD_ADDR,                   "Invalid IP ! Please input a right IPv6 address!", "v6��ַ������������ȷ��v6��ַ"},
    {ERR_V6SROUTE_REP_ADDR,                   "Ipv6 address conflicts.", "v6��ַ��ͻ"},
    
	/* DHCPv6 */
	{ERR_DHCP6S_RANGE_ERROR, "IPv6 Interface ID range error", "IPv6�ӿ�ID��Χ����"},

    /* IPSEC*/
    {ERR_IPSEC_INVALID_NAME,                   "Name is invalid.", "���ֲ��Ϸ�"},
    {ERR_IPSEC_INVALID_TUNNELMODE,             "Tunnel Mode is invalid.", "ͨ��ģʽ���Ϸ�"},
    {ERR_IPSEC_INVALID_IP,                     "IP is invalid.", "IP��ַ���Ϸ�"},
    {ERR_IPSEC_IP_EMPTY,                       "IP is empty.", "IP��ַΪ��"},
    {ERR_IPSEC_INVALID_IPMASK,                     "IP mask is invalid.", "�������벻�Ϸ�"},
    {ERR_IPSEC_IPMASK_EMPTY,                       "IP mask is empty.", "��������Ϊ��"},
    {ERR_IPSEC_IP_MASK_CONFLICT,               "IP address and netmask conflict",  "IP��ַ�����������ͻ"},
    {ERR_IPSEC_INVALID_KEYEXCHMETHOD,          "Key Exchange Method is invalid.", "��Կ��չģʽ���Ϸ�"},
    {ERR_IPSEC_INVALID_SHARDKEY,               "Shard key is invalid.", "������Կ���Ϸ�"},
    {ERR_IPSEC_INVALID_MODE,             "Mode is invalid.", "ģʽ���Ϸ�"},
    {ERR_IPSEC_INVALID_ENCRYALG,         "Encryption Algorithm is invalid.", "�����㷨���Ϸ�"},
    {ERR_IPSEC_INVALID_INTEGALG,         "Integrity Alogorithm is invalid.", "��֤�㷨���Ϸ�"},
    {ERR_IPSEC_INVALID_DHGROUP,         "Diffie-Hellman Group Key Exchange is invalid.", "Diffie-Hellman Group Key���Ϸ�"},
    {ERR_IPSEC_INVALID_KEYLIFETIME,         "Key Life Time is invalid.", "��Կ��Чʱ�䲻�Ϸ�"},
    {ERR_IPSEC_INVALID_ENCRYKEY,         "Encryption Key is invalid.", "��Կ���Ϸ�"},
    {ERR_IPSEC_ENCRY_CONFLICT,           "Encryption Algorithm and Encryption Key conflict",  "�����㷨�������Կ��ƥ��"},
    {ERR_IPSEC_INVALID_INTERFACE,         "WAN Interface is invalid.", "WAN�ӿ�·�����Ϸ�"},
    {ERR_IPSEC_INVALID_SPI,              "SPI is invalid.", "SPI���Ϸ�"},
    {0, NULL, NULL}
};



/*============================================================================
                      Debug Utils
=============================================================================*/
/*
*	��������: 	tbsGetMsgTypeName
*	��������: 	���Ը�������������ֵ��ʽMsgTypeת��Ϊ�ַ����ɶ���ʽ��
*	�������: 	usMsgType - MsgType��ֵ
*	����ֵ  :	��Ӧ��MsgType�ַ������֡�������ں���ʶ��Χ�У�����
*				UNKNOWN_MSGTYPE(ddd)��ʽ��
*	����    : 	������ / 2008-11-10
*/
const char* tbsGetMsgTypeName(unsigned short usMsgType)
{
	#define MSG_TYPE_NAME(name) case name: return #name

	switch (usMsgType)
    {

        MSG_TYPE_NAME(MSG_POLL_EXIT);
        MSG_TYPE_NAME(MSG_POLL_EXIT_ACK);

        MSG_TYPE_NAME(MSG_PC_START_PROC);
        MSG_TYPE_NAME(MSG_PC_START_PROC_ACK);
        MSG_TYPE_NAME(MSG_PC_SEND_SIGNAL);
        MSG_TYPE_NAME(MSG_PC_SEND_SIGNAL_ACK);
        MSG_TYPE_NAME(MSG_PC_PROC_STATE);
        MSG_TYPE_NAME(MSG_PC_PROC_STATE_ACK);
        MSG_TYPE_NAME(MSG_PC_PROC_OUTPUT);
        MSG_TYPE_NAME(MSG_PC_PROC_OUTPUT_ACK);
        MSG_TYPE_NAME(MSG_PC_EXIT);
        MSG_TYPE_NAME(MSG_PC_EXIT_ACK);

        MSG_TYPE_NAME(MSG_CMM_GET_VAL);
        MSG_TYPE_NAME(MSG_CMM_GET_VAL_ACK);
        MSG_TYPE_NAME(MSG_CMM_SET_VAL);
        MSG_TYPE_NAME(MSG_CMM_SET_VAL_ACK);
        MSG_TYPE_NAME(MSG_CMM_ADD_NODE);
        MSG_TYPE_NAME(MSG_CMM_ADD_NODE_ACK);
        MSG_TYPE_NAME(MSG_CMM_DEL_NODE);
        MSG_TYPE_NAME(MSG_CMM_DEL_NODE_ACK);
        MSG_TYPE_NAME(MSG_CMM_GET_NAME);
        MSG_TYPE_NAME(MSG_CMM_GET_NAME_ACK);
        MSG_TYPE_NAME(MSG_CMM_GET_ATTR);
        MSG_TYPE_NAME(MSG_CMM_GET_ATTR_ACK);
        MSG_TYPE_NAME(MSG_CMM_SET_ATTR);
        MSG_TYPE_NAME(MSG_CMM_SET_ATTR_ACK);
        MSG_TYPE_NAME(MSG_CMM_INFORM_NOTI);
        MSG_TYPE_NAME(MSG_CMM_INFORM_NOTI_ACK);
        MSG_TYPE_NAME(MSG_CMM_GET_NOTI);
        MSG_TYPE_NAME(MSG_CMM_GET_NOTI_ACK);
        MSG_TYPE_NAME(MSG_CMM_CLEAR_NOTI);
        MSG_TYPE_NAME(MSG_CMM_CLEAR_NOTI_ACK);
        MSG_TYPE_NAME(MSG_CMM_COMMIT);
        MSG_TYPE_NAME(MSG_CMM_COMMIT_ACK);
        MSG_TYPE_NAME(MSG_CMM_CANCEL);
        MSG_TYPE_NAME(MSG_CMM_CANCEL_ACK);
        MSG_TYPE_NAME(MSG_CMM_UPDATE);
        MSG_TYPE_NAME(MSG_CMM_UPDATE_ACK);
        MSG_TYPE_NAME(MSG_CMM_SAVE_CFG);
        MSG_TYPE_NAME(MSG_CMM_SAVE_CFG_ACK);
        MSG_TYPE_NAME(MSG_CMM_RECOVER_CFG);
        MSG_TYPE_NAME(MSG_CMM_RECOVER_CFG_ACK);
        MSG_TYPE_NAME(MSG_CMM_UPGRADE);
        MSG_TYPE_NAME(MSG_CMM_UPGRADE_ACK);
        MSG_TYPE_NAME(MSG_CMM_BACKUP);
        MSG_TYPE_NAME(MSG_CMM_BACKUP_ACK);
        MSG_TYPE_NAME(MSG_CMM_REBOOT);
        MSG_TYPE_NAME(MSG_CMM_REBOOT_ACK);

        MSG_TYPE_NAME(MSG_TIMER_REGSTER);
        MSG_TYPE_NAME(MSG_TIMER_REGSTER_ACK);
        MSG_TYPE_NAME(MSG_TIMER_UNREGSTER);
        MSG_TYPE_NAME(MSG_TIMER_UNREGSTER_ACK);
        MSG_TYPE_NAME(MSG_TIMER_TIMEOUT);
        MSG_TYPE_NAME(MSG_TIMER_TIMEOUT_ACK);

        MSG_TYPE_NAME(MSG_WAN_INTERFACE_STATE);
        MSG_TYPE_NAME(MSG_WAN_INTERFACE_STATE_ACK);
        MSG_TYPE_NAME(MSG_WAN_CONN_EST);
        MSG_TYPE_NAME(MSG_WAN_CONN_EST_ACK);
        MSG_TYPE_NAME(MSG_WAN_CONN_FIN);
        MSG_TYPE_NAME(MSG_WAN_CONN_FIN_ACK);
        MSG_TYPE_NAME(MSG_WAN_LINK_CHG);
        MSG_TYPE_NAME(MSG_WAN_LINK_CHG_ACK);

        MSG_TYPE_NAME(MSG_LAN_IP_UPDATE);
        MSG_TYPE_NAME(MSG_LAN_IP_UPDATE_ACK);
        MSG_TYPE_NAME(MSG_LAN_MACLIST_UPDATE);
        MSG_TYPE_NAME(MSG_LAN_MACLIST_UPDATE_ACK);
        MSG_TYPE_NAME(MSG_LAN_DOMAIN_NAME_UPDATE);
        MSG_TYPE_NAME(MSG_LAN_DOMAIN_NAME_UPDATE_ACK);

        MSG_TYPE_NAME(MSG_GET_DHOST_LIST);
        MSG_TYPE_NAME(MSG_GET_DHOST_LIST_ACK);
        MSG_TYPE_NAME(MSG_DNS_CHANGE);
        MSG_TYPE_NAME(MSG_DNS_CHANGE_ACK);
        MSG_TYPE_NAME(MSG_PROUTE_SET_DNS);
        MSG_TYPE_NAME(MSG_PROUTE_SET_DNS_ACK);
        MSG_TYPE_NAME(MSG_WLAN_ADD_DEVICE);
        MSG_TYPE_NAME(MSG_WLAN_ADD_DEVICE_ACK);
        MSG_TYPE_NAME(MSG_WLAN_DEL_DEVICE);
        MSG_TYPE_NAME(MSG_WLAN_DEL_DEVICE_ACK);

        MSG_TYPE_NAME(MSG_TR069_SET_PARKEY);
        MSG_TYPE_NAME(MSG_TR069_SET_PARKEY_ACK);
        MSG_TYPE_NAME(MSG_TR069_GET_EVENTS);
        MSG_TYPE_NAME(MSG_TR069_GET_EVENTS_ACK);
        MSG_TYPE_NAME(MSG_TR069_CLEAR_EVENTS);
        MSG_TYPE_NAME(MSG_TR069_CLEAR_EVENTS_ACK);
        MSG_TYPE_NAME(MSG_TR069_DOWNLOAD);
        MSG_TYPE_NAME(MSG_TR069_DOWNLOAD_ACK);
        MSG_TYPE_NAME(MSG_TR069_UPLOADLOAD);
        MSG_TYPE_NAME(MSG_TR069_UPLOADLOAD_ACK);
        MSG_TYPE_NAME(MSG_TR069_REBOOT);
        MSG_TYPE_NAME(MSG_TR069_REBOOT_ACK);
        MSG_TYPE_NAME(MSG_TR069_FACTORYRESET);
        MSG_TYPE_NAME(MSG_TR069_FACTORYRESET_ACK);
        MSG_TYPE_NAME(MSG_TR069_SCHEDULEINFORM);
        MSG_TYPE_NAME(MSG_TR069_SCHEDULEINFORM_ACK);
        MSG_TYPE_NAME(MSG_TR069_GET_TRANSFERCOMPLETEINFO);
        MSG_TYPE_NAME(MSG_TR069_GET_TRANSFERCOMPLETEINFO_ACK);

        MSG_TYPE_NAME(MSG_LBT_SET_STATE);
        MSG_TYPE_NAME(MSG_LBT_SET_STATE_ACK);
        MSG_TYPE_NAME(MSG_LBT_GET);
        MSG_TYPE_NAME(MSG_LBT_GET_ACK);

        MSG_TYPE_NAME(MSG_TR069_HTTPD_CONNECT);
        MSG_TYPE_NAME(MSG_TR069_HTTPD_CONNECT_ACK);
        MSG_TYPE_NAME(MSG_TR069_HTTPD_CREATE_UPFILE);
        MSG_TYPE_NAME(MSG_TR069_HTTPD_CREATE_UPFILE_ACK);

        MSG_TYPE_NAME(MSG_ETHNET_LINK_STATE);
        MSG_TYPE_NAME(MSG_ETHNET_LINK_STATE_ACK);
        MSG_TYPE_NAME(MSG_ETHWAN_INTF_NAME);
        MSG_TYPE_NAME(MSG_ETHWAN_INTF_NAME_ACK);

        MSG_TYPE_NAME(MSG_MON_INTF_REGISTER);
        MSG_TYPE_NAME(MSG_MON_INTF_REGISTER_ACK);
        MSG_TYPE_NAME(MSG_MON_INTF_UNREGISTER);
        MSG_TYPE_NAME(MSG_MON_INTF_UNREGISTER_ACK);
        MSG_TYPE_NAME(MSG_MON_INTF_STATUS_QUERY);
        MSG_TYPE_NAME(MSG_MON_INTF_STATUS_QUERY_ACK);
        MSG_TYPE_NAME(MSG_MON_INTF_STATUS_INFORM);
        MSG_TYPE_NAME(MSG_MON_INTF_STATUS_INFORM_ACK);

        MSG_TYPE_NAME(MSG_AUTH);
        MSG_TYPE_NAME(MSG_AUTH_ACK);

        MSG_TYPE_NAME(MSG_WAN_DEL_INST);
        MSG_TYPE_NAME(MSG_WAN_DEL_INST_ACK);
        MSG_TYPE_NAME(MSG_WAN_ADD_INST);
        MSG_TYPE_NAME(MSG_WAN_ADD_INST_ACK);
        MSG_TYPE_NAME(MSG_LAN_DEL_INST);
        MSG_TYPE_NAME(MSG_LAN_DEL_INST_ACK);
        MSG_TYPE_NAME(MSG_LAN_ADD_INST);
        MSG_TYPE_NAME(MSG_LAN_ADD_INST_ACK);
        MSG_TYPE_NAME(MSG_WLAN_DEL_INST);
        MSG_TYPE_NAME(MSG_WLAN_DEL_INST_ACK);
        MSG_TYPE_NAME(MSG_WLAN_ADD_INST);
        MSG_TYPE_NAME(MSG_WLAN_ADD_INST_ACK);

        MSG_TYPE_NAME(MSG_WLAN_ENABLE_CHANGE);
        MSG_TYPE_NAME(MSG_WLAN_ENABLE_CHANGE_ACK);
        MSG_TYPE_NAME(MSG_CMD);
        MSG_TYPE_NAME(MSG_CMD_ACK);
        MSG_TYPE_NAME(MSG_RESP);
        MSG_TYPE_NAME(MSG_RESP_ACK);
        MSG_TYPE_NAME(MSG_RESP_FRAGMENT);
        MSG_TYPE_NAME(MSG_RESP_FRAGMENT_ACK);
        MSG_TYPE_NAME(MSG_RESP_ERR);
        MSG_TYPE_NAME(MSG_RESP_ERR_ACK);

        MSG_TYPE_NAME(MSG_CMM_CLEAR_CFG);
        MSG_TYPE_NAME(MSG_CMM_CLEAR_CFG_ACK);

        MSG_TYPE_NAME(MSG_BUTTON);
        MSG_TYPE_NAME(MSG_BUTTON_ACK);

        MSG_TYPE_NAME(MSG_SNTP_UPDATE_TIME);
        MSG_TYPE_NAME(MSG_SNTP_UPDATE_TIME_ACK);
        MSG_TYPE_NAME(MSG_SNTP_TIME_CHANGED);
        MSG_TYPE_NAME(MSG_SNTP_TIME_CHANGED_ACK);
        MSG_TYPE_NAME(MSG_NTPS_CHANGED);
        MSG_TYPE_NAME(MSG_NTPS_CHANGED_ACK);

        MSG_TYPE_NAME(MSG_TR111_DEVICE_ADD);
        MSG_TYPE_NAME(MSG_TR111_DEVICE_ADD_ACK);
        MSG_TYPE_NAME(MSG_TR111_DEVICE_DEL);
        MSG_TYPE_NAME(MSG_TR111_DEVICE_DEL_ACK);

        MSG_TYPE_NAME(MSG_DIAGNOSTIC_COMPLETE);
        MSG_TYPE_NAME(MSG_DIAGNOSTIC_COMPLETE_ACK);

        MSG_TYPE_NAME(MSG_MSG4UDP_REGISTER);
        MSG_TYPE_NAME(MSG_MSG4UDP_REGISTER_ACK);
        MSG_TYPE_NAME(MSG_MSG4UDP_UNREGISTER);
        MSG_TYPE_NAME(MSG_MSG4UDP_UNREGISTER_ACK);
        MSG_TYPE_NAME(MSG_MSG4UDP_MESSAGE);
        MSG_TYPE_NAME(MSG_MSG4UDP_MESSAGE_ACK);

        MSG_TYPE_NAME(MSG_WAN_CONN_ENABLE_UPDATE);
        MSG_TYPE_NAME(MSG_WAN_CONN_ENABLE_UPDATE_ACK);

        MSG_TYPE_NAME(MSG_VLAN_ADD_BRIDGE);
        MSG_TYPE_NAME(MSG_VLAN_ADD_BRIDGE_ACK);
        MSG_TYPE_NAME(MSG_VLAN_DEL_BRIDGE);
        MSG_TYPE_NAME(MSG_VLAN_DEL_BRIDGE_ACK);
        MSG_TYPE_NAME(MSG_VLAN_BIND_PORT);
        MSG_TYPE_NAME(MSG_VLAN_BIND_PORT_ACK);
        MSG_TYPE_NAME(MSG_VLAN_UNBIND_PORT);
        MSG_TYPE_NAME(MSG_VLAN_UNBIND_PORT_ACK);


        MSG_TYPE_NAME(MSG_VDSL_BRIEF_STATUS_OF_PORT);
        MSG_TYPE_NAME(MSG_VDSL_BRIEF_STATUS_OF_PORT_ACK);
        MSG_TYPE_NAME(MSG_VDSL_EXTENDED_STATUS_OF_PORT);
        MSG_TYPE_NAME(MSG_VDSL_EXTENDED_STATUS_OF_PORT_ACK);
        MSG_TYPE_NAME(MSG_VDSL_STATUS);
        MSG_TYPE_NAME(MSG_VDSL_STATUS_ACK);
        MSG_TYPE_NAME(MSG_VDSL_SNR_STATUS);
        MSG_TYPE_NAME(MSG_VDSL_SNR_STATUS_ACK);
        MSG_TYPE_NAME(MSG_VDSL_BME_FIRMWARE_VERSION);
        MSG_TYPE_NAME(MSG_VDSL_BME_FIRMWARE_VERSION_ACK);

        MSG_TYPE_NAME(MSG_LAN_ETH_DEL_INST);
        MSG_TYPE_NAME(MSG_LAN_ETH_ADD_INST);

        MSG_TYPE_NAME(MSG_PORTOFF_TRIGGER);

        MSG_TYPE_NAME(MSG_CMM_INST_ADDED);
        MSG_TYPE_NAME(MSG_CMM_INST_DELED);

     	default:
		{
			static char s_szUnsupported[64];
			sprintf(s_szUnsupported, "UNKNOWN_MSGTYPE(%04x)", usMsgType);
			return s_szUnsupported;
		}
	}
}

/*
*	��������: 	tbsGetMIDName
*	��������: 	���Ը�������������ֵ��ʽMIDת��Ϊ�ַ����ɶ���ʽ��
*	�������: 	usMID - MID��ֵ
*	����ֵ  :	��Ӧ��MID�ַ������֡�������ں�����ʶ��Χ�У�����
*				UNKNOWN_MID(ddd)��ʽ��
*	����    : 	������ / 2008-11-10
*/
const char* tbsGetMIDName(unsigned short usMID)
{
	#define MID_NAME(name) case name: return #name
	switch (usMID)
    {
        MID_NAME(MID_CCP);
        MID_NAME(MID_AUTH);
        MID_NAME(MID_ATM);
        MID_NAME(MID_IGMP);
        MID_NAME(MID_CMM);
        MID_NAME(MID_LAN);
        MID_NAME(MID_IPT);
        MID_NAME(MID_ETHLAN);
        MID_NAME(MID_ETHWAN);
        MID_NAME(MID_PPPOE);
        MID_NAME(MID_WLAN);
        MID_NAME(MID_TR069BE);
        MID_NAME(MID_DGN);
        MID_NAME(MID_DHCPR);
        MID_NAME(MID_DHCPS);
        MID_NAME(MID_TIMER);
        MID_NAME(MID_IPCONN);
        MID_NAME(MID_FIREWALL);
        MID_NAME(MID_SNMPC);
        MID_NAME(MID_QOS);
        MID_NAME(MID_STATIC_ROUTING);
        MID_NAME(MID_VDSL);
        MID_NAME(MID_DNS);
        MID_NAME(MID_ALG);
        MID_NAME(MID_WAN);
        MID_NAME(MID_DROUTE);
        MID_NAME(MID_SNTP);
        MID_NAME(MID_VLAN);
        MID_NAME(MID_USB_MASS);
        MID_NAME(MID_LOG);
        MID_NAME(MID_FTPD);
        MID_NAME(MID_NATPRIO);
        MID_NAME(MID_WPS);
        MID_NAME(MID_ACL);
        MID_NAME(MID_UPNP);
        MID_NAME(MID_LSVLAN);
        MID_NAME(MID_PORTOFF);
        MID_NAME(MID_ANTIATTACK);
	    MID_NAME(MID_SPT);
        MID_NAME(MID_SUPP);
        MID_NAME(MID_TRACERT);
        MID_NAME(MID_WEBP);
        MID_NAME(MID_DIAG);
	    MID_NAME(MID_DEVCONFIG);	
	    MID_NAME(MID_PORTMAPPING);	
        MID_NAME(MID_URLFILTER);	
        MID_NAME(MID_WEB);
        MID_NAME(MID_LNB);
        MID_NAME(MID_CLI);
        MID_NAME(MID_TR069FE);
        MID_NAME(MID_DDNS);	
        MID_NAME(MID_CFG);	
        MID_NAME(MID_MACFILTER);
        MID_NAME(MID_BRIDGE_FILTER);
        MID_NAME(MID_IP_ACL);
        MID_NAME(MID_DEFAULTGW);
        MID_NAME(MID_SAMBA);
        MID_NAME(MID_TF_PORTMAPPING);
        MID_NAME(MID_TF_FIREWALL);	
        MID_NAME(MID_OS_INFO);	
        MID_NAME(MID_IPMACFILTER);	
        MID_NAME(MID_TR069_HTTPD);
        MID_NAME(MID_SNMPA);
		MID_NAME(MID_TR064FE);

        MID_NAME(MID_PC);
        MID_NAME(MID_ELM);
        MID_NAME(MID_UPG);
        MID_NAME(MID_VDSLD);
        MID_NAME(MID_DSL);
        MID_NAME(MID_TM);
		MID_NAME(MID_IPTUNNEL);
		MID_NAME(MID_GRE);
		MID_NAME(MID_MLD);
		MID_NAME(MID_TF_GUI);
        //MID_NAME(MID_MON);
        //MID_NAME(MID_LBT);
        MID_NAME(MID_UPGCGI);
        MID_NAME(MID_FTPUPG);
        MID_NAME(MID_PTI);
        MID_NAME(MID_MSG4UDP);
        MID_NAME(MID_TFTPUPG);
        MID_NAME(MID_DEVINFO);

        MID_NAME(MID_GRP_CCP);

        MID_NAME(MID_GRP_BASE);

        MID_NAME(MID_GRP_MACLIST_UPDATE);
        MID_NAME(MID_GRP_MON_LINK_UPDATE);
        MID_NAME(MID_GRP_WAN_LINK_UPDATE);
        MID_NAME(MID_GRP_WAN_CONN_UPDATE);
        MID_NAME(MID_GRP_DNS_UPDATE);
        MID_NAME(MID_GRP_WAN_DEL_INST);
        MID_NAME(MID_GRP_WAN_ADD_INST);
        MID_NAME(MID_GRP_BUTTON);
        MID_NAME(MID_GRP_TIME_CHANGED);
        MID_NAME(MID_GRP_WAN_CONN_ENABLE_UPDATE);
        MID_NAME(MID_GRP_LAN_DEL_INST);
        MID_NAME(MID_GRP_LAN_ADD_INST);

        MID_NAME(MID_GRP_WLAN_DEL_INST);
        MID_NAME(MID_GRP_WLAN_ADD_INST);
        MID_NAME(MID_GRP_LAN_ETH_INST_CHG);
        MID_NAME(MID_TR069DIAG);
        

        MID_NAME(MID_NULL);

	 	default:
		{
			static char s_szUnsupported[64];
			sprintf(s_szUnsupported, "UNKNOWN_MID(%04x)", usMID);
			return s_szUnsupported;
		}
	}
}


