/*=========================================================================
Copyright (c), 1991-2007, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
�ļ����� : flash_layout.h
�ļ����� : flash image���ֵĶ���


�޶���¼ :
         1 ���� : ������
           ���� : 2008-9-12
           ���� :

         2 �޶�: ������
            ����: 2009-3-16
            ����: �Ż���Ŀ�Ĳ���

         3 �޶�: �ĳ���
         	 ����: 2011-08-08
         	 ����: ���¶��ļ�β������,�Լ���������


=========================================================================*/
#ifndef _FLASH_LAYOUT_H_
#define _FLASH_LAYOUT_H_

/*=========================================================================
	�ײ�������Ϣ�в�Ʒ���ơ��汾��Ϣ��
	image�������к��ַ������ȵĶ���
=========================================================================*/

#define PRODUCT_NAME_LEN			32
#define TBS_VERSION_NAME_LEN		32
#define IMAGE_TYPES_NAME_LEN		8
#define BOARD_ID_NAME_LEN			16
#define ITEM_DATA_LEN               255

/* Bootloader�̶�ռ��64KB��С,u-boot.bin */
#define	BOOTCODE_LENGTH		( 0x10000 )

#define SYSTEM_CONFIG_VERSION		"TBSCFG01"
#define IMAGE_TAIL_VERSION			"TBSTLV01"
#define	VERSION_LEN					12 
/*====================================================================
	ϵͳ������Ϣ���ݽṹ���������ڴ��ʱ��ʼ��
====================================================================*/
struct sys_cfg
{
    unsigned char cfg_version[VERSION_LEN];	/*������Ϣ�汾*/
    unsigned int first_kernel_offset;
    unsigned int first_kernel_len;
    unsigned int first_rootfs_offset;
    unsigned int first_rootfs_len;
    unsigned long first_image_checksum;
    unsigned int second_kernel_offset;
    unsigned int second_kernel_len;
    unsigned int second_rootfs_offset;
    unsigned int second_rootfs_len;
    unsigned long second_image_checksum;
    unsigned int ip;
    unsigned char mac[6];
    unsigned char endian;
    unsigned char boot_img_flag;		/*0: ϵͳ����image1��1: ϵͳ����image 2*/
    unsigned char board_id[BOARD_ID_NAME_LEN];			/*�ͻ�����ǩ��*/
    unsigned char product[PRODUCT_NAME_LEN];				/*������ʶ��*/
    unsigned char version[TBS_VERSION_NAME_LEN];			/*�汾��Ϣ*/
    unsigned char tag[4];						/*  �ײ����ÿ��ʶ��'sysc'  */
    unsigned char sn[20];
};

/*====================================================================
	wan��������Ϣ���ݽṹ��������������ʱ��ʼ��
====================================================================*/
typedef struct
{
    unsigned int version;
    unsigned int connection_type; //WAN�������ͣ�DHCP, PPPOE
    unsigned int ip;
    unsigned int subnet_mask;
    unsigned int gateway_ip;
    unsigned int vlan;
    unsigned int dns;
    unsigned int dns2;
    char pppoe_account[64];  //PPPOE�û���
    char pppoe_password[64]; //PPPOE����
    unsigned int pppoe_auth_method;  //PPPOE��֤����
}
wan_eth_ipv4_config_t;

/*====================================================================
	����������Ϣ���ݽṹ��������������ʱ��ʼ��
====================================================================*/
typedef struct
{
    char url[512];       //����URL
    char account[64];    //�û���
    char password[64];   //����
}
upg_config_t;


/*====================================================================
	imageβ����Ϣ���ݽṹ���������ڴ��ʱ��ʼ��, ����ԭ��ͷ��
====================================================================*/
struct image_tail
{
    unsigned int bootloader_len;							/*bootloader����*/
    unsigned int config_block_offset;					/*������ƫ�Ƶ�ַ*/
    unsigned int config_block_len;						/*����������*/
    unsigned int first_kernel_offset;					/*Сimage�ں�ƫ�Ƶ�ַ*/
    unsigned int first_kernel_len;						/*Сimage�ں˳���*/
    unsigned int first_rootfs_offset;					/*Сimage��������ַ*/
    unsigned int first_rootfs_len;						/*Сimage����������*/
    unsigned long first_image_checksum;					/*СimageCRCֵ*/
    unsigned int second_kernel_offset;					/*��image�ں˵�ַ*/
    unsigned int second_kernel_len;						/*��image�ں˳���*/
    unsigned int second_rootfs_offset;					/*��image��������ַ*/
    unsigned int second_rootfs_len;						/*��image����������*/
    unsigned long second_image_checksum;					/*��imageCRCֵ*/
    unsigned int boot_img_flag;		/*0: ϵͳ����image1��1: ϵͳ����image 2*/
    unsigned char board_id[BOARD_ID_NAME_LEN];			/*�ͻ�����ǩ��*/
    unsigned char version[TBS_VERSION_NAME_LEN];			/*�汾��Ϣ*/
    unsigned char product[PRODUCT_NAME_LEN];				/*������ʶ��*/
    unsigned int image_len;								/*�ļ�����*/
    unsigned int img_type; 								/*�ļ����ͱ�ʶ��*/
    unsigned char tail_version[VERSION_LEN];				/*β����Ϣ�汾*/
    unsigned long image_checksum;						    /*image�ļ�У���*/
};

struct update_parameters {
	struct image_tail tail;
	unsigned int cfg_sector;
	unsigned int f_s_sector;
	unsigned int f_e_sector;
	unsigned int s_s_sector;
	unsigned int s_e_sector;
	unsigned long dest_addr;
	int          size;
	int syscfg_version;
};	


/*====================================================================
	�ļ����ͱ�ʾ�����壬�����жϽ��յ��ļ����͡�
====================================================================*/

typedef enum {
    TYPE_UNKNOWN = 0,      /* δ֪�ļ����� */
    TYPE_SINGLE_IMG = 1,      /* ��image��IMG��ʽ�ļ� */
    TYPE_SINGLE_BIN = 2,      /* ��image��BIN��ʽ�ļ� */
    TYPE_DUAL_IMG = 3,      /* ˫image��IMG��ʽ�ļ� */
    TYPE_DUAL_BIN = 4,      /* ˫image��BIN��ʽ�ļ� */
    TYPE_BACKUP_IMG = 5,      /* ��Сimage��IMG��ʽ�ļ� */
    TYPE_BACKUP_BIN = 6,      /* ��Сimage��BIN��ʽ�ļ� */
    TYPE_BACKUP_FIMG = 7,     /* ��Сimage�Ĵ�IMG��ʽ�ļ� */
    TYPE_BACKUP_BIMG = 8,     /* ��Сimage��СIMG��ʽ�ļ� */
    TYPE_BOOTLOADER = 9,      /* Bootloader */
    TYPE_VIRTUAL_DIMG = 10     /* �����°汾˫IMAGE�ļ� */
}image_type;


/*=========================================================================
	������Ϣ��Ŀ���ݽṹ
=========================================================================*/

typedef struct
{
    unsigned short	crc;
    unsigned short	len;
    unsigned char	avail;
}
__attribute__ ((packed)) item_hdr_t;

typedef struct
{
    item_hdr_t	hdr;
    unsigned char	data[];				/* ����*/
}
item_t;

#define		ITEM_HEAD_LEN				(sizeof( item_hdr_t))
#define		ITEM_SIZE(data_len)			((ITEM_HEAD_LEN + (data_len)) + ((ITEM_HEAD_LEN + (data_len)) % 2))
#define		CONFIG_MARK		            "OK"
#define		CONFIG_MARK_LEN	            (strlen(CONFIG_MARK))
#define		ITEM_UNAVAIL	            0x00
#define		ITEM_AVAIL		            0x11
#define		ITEM_BAD		            0x22
#define		ITEM_NULL		            0xFF


/*=========================================================================
	�����ݽṹ����Ӧ��ӿ���ioctlʱ��ײ�������ݽ���
=========================================================================*/

typedef struct
{
    unsigned char *name;
    unsigned char item_error;
    unsigned long len;
    unsigned char *data;				/* ����*/
}
item_app_t;

/*=========================================================================
	ϵͳ��ʹ�õ���Ŀ���ƣ���Ŀ��������ú궨���������
	���ڹ����ά��
=========================================================================*/

#define		LLCONFIG_NAME			"llconfig"
#define		WLAN_NAME				"wlan_cfg"
#define		TBS_APP_CFG			    "tbs_app_cfg"
#define		TR069_EVENT			    "tr069_event"
#define		PPPOE_SSID			    "pppoe_ssid"
#define     BOOT_TIMES              "boot_times"
#define     TBS_BIT_FLAG            "tbs_bit_flag"
#define     TBS_DEFCFG_PATH         "def_cfg_path"
#define     TBS_USERCFG_ITEM        "user_cfg_item"
#define     TBS_USERCFG_PREFIX		"tbs_app_cfg_"
#define		BACKUP_NET_CFG			"backup_net_cfg"
#define		TR069_UPG   			"tr069_upg"
#define     TBS_SNMP_EVENT		    "tbs_snmp"
#define     WLAN_PASSWORD           "wlan_password"
#define     TELNET_PASSWORD         "telnet_password"
#define     CUR_TELNET_PASS         "cur_telnet_pass"
#define     WLAN_SSID               "wlan_ssid"

/*=========================================================================
	�ڶ���Ŀ����ʱ���صĴ�����
=========================================================================*/

enum item_error
{
    ERROR_ITEM_OK = 0,
    ERROR_ITEM_MTD,
    ERROR_ITEM_IOCTRL,
    ERROR_ITEM_MALLOC,
    ERROR_ITEM_NOT_FIND,
    ERROR_ITEM_CRC,
    ERROR_ITEM_BIG,
    ERROR_ITEM_REPEAT_OK,
    ERROR_ITEM_REPEAT_FAIL,
    ERROR_CONFIG_LOST,
    ERROR_FLASH_BUSY
};

/*=========================================================================
	�ڶ��ļ���������ʱ���صĴ�����
=========================================================================*/
enum upate_erros
{
	ERROR_OK ,
	ERROR_CRC ,
	ERROR_IMG_SIZE ,
	ERROR_PRODUCT ,
	ERROR_IMG_TYPE,
	ERROR_NO_MEM
};
/*=========================================================================
	Ӧ�ò��ȡ�ͱ�����Ŀ�Ľӿ�
=========================================================================*/

int app_item_get( void *data , char *item_name , unsigned short *len );		/* ��ȡ��Ӧ��Ŀ��Ϣ*/
int app_item_save( void *data , char *item_name , unsigned short len );		/* ��������Ŀ*/

/*=========================================================================
	�ײ��ȡ�ͱ�����Ŀ�Ľӿ�
=========================================================================*/
int item_get( void *data , char *item_name , unsigned short *len );		/* ��ȡ��Ӧ��Ŀ��Ϣ*/
int item_save( void *data , char *item_name , unsigned short len );		/* ��������Ŀ*/


/*=========================================================================
	Ӧ�ò��ȡMACַַ�ӿ�
=========================================================================*/

typedef struct
{
    int id;
    int	offset;
    unsigned char mac[6];
}
mac_t;

enum device_mac
{
    LAN_MAC = 0,
    WAN_MAC,
    WLAN_MAC,
    USB_MAC
};

int tbs_read_mac(int id, int offset, unsigned char *mac);
int app_tbs_read_mac(int id, int offset, unsigned char *mac);
int file_check(void *data, unsigned int len, int force_flag, void *ptr);
int flash_update(void *data, unsigned int len, int flag, void *tail);
int	is_sysdata(void *sys_data);

/*=========================================================================
	Ӧ�ò����DUAL IMAGEͬ���ӿ�
=========================================================================*/
int app_dual_image_sync(void);
#endif  /*_FLASH_LAYOUT_H_*/

