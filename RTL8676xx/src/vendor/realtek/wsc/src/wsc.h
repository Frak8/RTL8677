/*
 *  Header file for WiFi Simple-Config
 *
 *	Copyright (C)2006, Realtek Semiconductor Corp. All rights reserved.
 *
 *	$Id: wsc.h,v 1.15 2010/02/05 13:43:01 cathy Exp $
 */
/*================================================================*/

#ifndef INCLUDE_WSC_H
#define INCLUDE_WSC_H

/*================================================================*/
/* Define Flags */
#define DEBUG	
#define PRINT_ERR
#define CONFIG_RTL867x
//cathy
#define WPA2_MIXED_2MODE_ONLY	//wpa+tkip or wp2+aes

//#define TEST 
//#define DEBUG_TRACE
#define SUPPORT_ENROLLEE
#define SUPPORT_REGISTRAR
#define SUPPORT_UPNP
#define WIFI_SIMPLE_CONFIG		// must define
#define CLIENT_MODE
#define MUL_PBC_DETECTTION
//#define TEST_FOR_MULTIPLE_CREDENTIAL
#define PREVENT_PROBE_DEADLOCK
#define BLOCKED_ROGUE_STA
#define USE_MUTEX

#define WINDOW7

extern int gMIB_WSC_CONFIGURED        ;
extern int gMIB_WLAN_SSID             ;
extern int gMIB_WSC_SSID              ;
extern int gMIB_WLAN_AUTH_TYPE        ;
extern int gMIB_WLAN_ENCRYPT          ;
extern int gMIB_WSC_AUTH              ;
extern int gMIB_WLAN_WPA_AUTH         ;
extern int gMIB_WLAN_WPA_PSK          ;
extern int gMIB_WLAN_WPA_PSK_FORMAT   ;
extern int gMIB_WSC_PSK               ;
extern int gMIB_WLAN_WPA_CIPHER_SUITE ;
extern int gMIB_WLAN_WPA2_CIPHER_SUITE;
extern int gMIB_WLAN_WEP              ;
extern int gMIB_WLAN_WEP64_KEY1       ;
extern int gMIB_WLAN_WEP64_KEY2       ;
extern int gMIB_WLAN_WEP64_KEY3       ;
extern int gMIB_WLAN_WEP64_KEY4       ;
extern int gMIB_WLAN_WEP128_KEY1      ;
extern int gMIB_WLAN_WEP128_KEY2      ;
extern int gMIB_WLAN_WEP128_KEY3      ;
extern int gMIB_WLAN_WEP128_KEY4      ;
extern int gMIB_WLAN_WEP_DEFAULT_KEY  ;
extern int gMIB_WLAN_WEP_KEY_TYPE     ;
extern int gMIB_WSC_ENC               ;
extern int gMIB_WSC_CONFIG_BY_EXT_REG ;
extern int gMIB_WSC_PIN;
extern int gMIB_ELAN_MAC_ADDR;
extern int gMIB_WLAN_MODE;
extern int gMIB_WSC_REGISTRAR_ENABLED;
extern int gMIB_WLAN_CHAN_NUM;
extern int gMIB_WSC_UPNP_ENABLED;
extern int gMIB_WSC_METHOD;
extern int gMIB_WSC_MANUAL_ENABLED;
extern int gMIB_SNMP_SYS_NAME;
extern int gMIB_WLAN_NETWORK_TYPE;

#ifdef USE_MINI_UPNP
	#undef PREVENT_PROBE_DEADLOCK
	#undef USE_MUTEX
#endif

/*================================================================*/
/* Package dependent define */
#ifdef CONFIG_RTL8186_KB
	#define NO_IWCONTROL
	#define WSC_1SEC_TIMER
	//#ifdef MUL_PBC_DETECTTION
		//#undef MUL_PBC_DETECTTION
	//#endif
	#ifdef CLIENT_MODE
		#undef CLIENT_MODE
	#endif		
#endif

/*================================================================*/
//#define CONFIG_RTL8186_KLD_REPEATER

/*================================================================*/

/* Include Files */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/time.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <linux/if_packet.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
//#include <linux/wireless.h>
#include <sys/sysinfo.h>
#ifdef USE_MUTEX
	#include <pthread.h> 
#endif

#include <openssl/crypto.h>
#include <openssl/bn.h>
#include <openssl/dh.h>
#include <openssl/rand.h>

#include "built_time"
#include "iwlib.h"

#ifdef SUPPORT_UPNP
	#include "simplecfg_upnp.h"
	#ifdef USE_MINI_UPNP
		#include <syslog.h>
		#include "mini_upnp.h"
		#include "upnphttp.h"
		#include "upnpsoap.h"
		#include "upnpreplyparse.h"
		#include "minixml.h"
	#endif
#endif

//#include "../../boa/src-project/LINUX/options.h"
//xl_yue:
#ifdef MIB_WLAN_RESTART_SUPPORT
extern int gMIB_WLAN_RESTART;
#endif

/*================================================================*/
/* Macro Definitions */

#define DISPLAY_BANNER \
	printf("\nWiFi Simple Config %s (%s).\n\n", VERSION_STR, BUILT_TIME)

#define IS_PIN_METHOD(mode)	((mode & CONFIG_METHOD_PIN) ? 1 : 0)
#define IS_PBC_METHOD(mode)	((mode & CONFIG_METHOD_PBC) ? 1 : 0)

#ifdef DEBUG_TRACE
#define DBFENTER		printf("----->%s\n", __FUNCTION__)
#define DBFEXIT			printf("%s----->\n", __FUNCTION__)
#else
#define DBFENTER
#define DBFEXIT
#endif

#ifdef DEBUG
#define DEBUG_PRINT(fmt, args...); \
	if (pCtx->debug) printf(fmt, ## args);

#define _DEBUG_PRINT(fmt, args...) printf(fmt, ## args)
#else

#define DEBUG_PRINT(fmt, args...);
#define _DEBUG_PRINT(fmt, args...)
#define debug_out(fmt, args...);
#endif // DEBUG

#ifdef PRINT_ERR
#define DEBUG_ERR(fmt, args...) printf(fmt, ## args)
#else

#define DEBUG_ERR(fmt, args...)
#endif

#ifdef USE_MUTEX
#define WSC_pthread_mutex_init(x, y); 	pthread_mutex_init(x, y);
#define WSC_pthread_mutex_destroy(x); 	pthread_mutex_destroy(x);
#define WSC_pthread_mutex_lock(x);		pthread_mutex_lock(x);
#define WSC_pthread_mutex_unlock(x);	pthread_mutex_unlock(x);
#define WSC_pthread_mutex_t			pthread_mutex_t
#else
#define WSC_pthread_mutex_init(x, y);
#define WSC_pthread_mutex_destroy(x);
#define WSC_pthread_mutex_lock(x);
#define WSC_pthread_mutex_unlock(x);
#define WSC_pthread_mutex_t			unsigned char
#endif

#ifdef CONFIG_RTL8186_TR
#define SET_LED_ON_FOR_10S() { \
	char tmpbuf[100]; \
	wlioctl_set_led(pCtx->wlan_interface_name, LED_WSC_OK); \
	sprintf(tmpbuf, "echo 1 > %s", LED_ON_10S_FILE); \
	system(tmpbuf); \
}
#endif

/*================================================================*/
/* Constant Definitions */

#define PROGRAM_NAME				"wscd"
#define VERSION_STR					"v1.5"
#define DEFAULT_CONFIG_FILENAME		("/var/"PROGRAM_NAME".conf")
#define DEFAULT_PID_FILENAME		("/var/run/"PROGRAM_NAME)
#define DEFAULT_LAN_INTERFACE		("br0")
#define REINIT_WEB_FILE				("/tmp/reinit_web")
#define REINIT_WSCD_FILE			("/tmp/reinit_wscd")
#ifdef CONFIG_RTL8186_TR
#define LED_ON_10S_FILE				("/tmp/wps_led")
#endif
#define WSCD_BYEBYE_FILE			("/tmp/wscd_byebye")
#define WSCD_CONFIG_FILE			("/tmp/wscd_config")
#define WSCD_CONFIG_STATUS		("/tmp/wscd_status")
#define WSCD_CANCEL_PROTOCOL		("/tmp/wscd_cancel")

#define PIN_LEN						8
#define ETHER_ADDRLEN				6
#define ETHER_HDRLEN				14
#define TX_BUFFER_SIZE				1512
#define RX_BUFFER_SIZE				1600
#define MAX_MSG_SIZE				1600	/* byte number of FIFO event */

#define UUID_LEN					16
#define NONCE_LEN					16
#define PUBLIC_KEY_LEN				192
#define MAX_MANUFACT_LEN			64
#define MAX_MODEL_NAME_LEN			32
#define MAX_MODEL_NUM_LEN			32
#define MAX_SERIAL_NUM_LEN			32
#define MAX_DEVICE_NAME_LEN			32
#define MAX_SSID_LEN				32
#define MAX_NETWORK_KEY_LEN			64
#define MAX_WEP_KEY_LEN			26
#define OUI_LEN						4
#define BYTE_LEN_64B				(64/8)
#define BYTE_LEN_128B				(128/8)
#define BYTE_LEN_256B				(256/8)
#define BYTE_LEN_640B				(640/8)

#define IV_LEN						16
#define MAX_WSC_IE_LEN				256
#define MAX_BSS_DESC				64
#define PROBEIELEN					260

#define EAPOL_HDRLEN				4
#define EAP_HDRLEN					4

#define ETHER_EAPOL_TYPE			0x888e
#define EAPOL_VER					1
#define EAPOL_EAPPKT				0
#define EAPOL_START					1
#define EAP_REQUEST					1
#define EAP_RESPONSE				2
#define EAP_SUCCESS					3
#define EAP_FAIL					4

#define EAP_TYPE_IDENTITY			1
#define EAP_TYPE_EXPANDED			254

#define FIFO_HEADER_LEN				5
#define MAX_STA_NUM					10
#define MAX_EXTERNAL_REGISTRAR_NUM	3
#define MAX_BLOCKED_STA_NUM			10
#ifdef CONFIG_BOARD_003v6
#define BUTTON_HOLD_TIME			5
#else
#define BUTTON_HOLD_TIME			3
#endif
#define PBC_WALK_TIME				120 // in sec
#define PIN_WALK_TIME				180 // in sec
#define SETSELREG_WALK_TIME		120 // in sec
#define SESSION_OVERLAP_TIME		(pCtx->PBC_overlapping_LED_time_out)
#define WAIT_REBOOT_TIME			3
#ifdef SUPPORT_UPNP
#define IS_UPNP_CONTROL_POINT		0x8000000
#ifdef PREVENT_PROBE_DEADLOCK
#define MAX_WSC_PROBE_STA			10
#define PROBE_EXPIRED				10
#endif
#define MAX_SUBSCRIPTION_TIMEOUT  	180
#define UPNP_EXTERNAL_REG_EXPIRED	 (MAX_SUBSCRIPTION_TIMEOUT + 3)
#define MAX_SUBSCRIPTION_NUM		10
#endif
#define BASIC_TIMER_UNIT 			1000000

#ifdef BLOCKED_ROGUE_STA
#define BLOCKED_ENTRY_NOT_FOUND -1
#define DEFAULT_BLOCK_TIME			3600
#endif

#define EAP_ID_ENROLLEE				("WFA-SimpleConfig-Enrollee-1-0")
#define EAP_ID_REGISTRAR			("WFA-SimpleConfig-Registrar-1-0")
#define KDF_STRING					("Wi-Fi Easy and Secure Key Derivation")

#define WSC_IE_ID					221
#define WSC_VENDOR_TYPE				1

#define WSC_OP_START				1
#define WSC_OP_ACK					2
#define WSC_OP_NACK					3
#define WSC_OP_MSG					4
#define WSC_OP_DONE					5

#define TAG_AP_CHAN					0x1001
#define TAG_ASSOC_STATE				0x1002
#define TAG_AUTH_TYPE				0x1003
#define TAG_AUTH_TYPE_FLAGS			0x1004
#define TAG_AUTHENTICATOR			0x1005
#define TAG_CONFIG_METHODS 			0x1008
#define TAG_CONFIG_ERR				0x1009
#define TAG_CONFIG_URL4				0x100A
#define TAG_CONFIG_URL6				0x100B
#define TAG_CONNECT_TYPE			0x100C
#define TAG_CONNECT_TYPE_FLAGS		0x100D
#define TAG_CREDENTIAL				0x100E
#define TAG_DEVICE_NAME				0x1011
#define TAG_DEVICE_PASSWORD_ID		0x1012
#define TAG_E_HASH1					0x1014
#define TAG_E_HASH2					0x1015
#define TAG_E_SNONCE1				0x1016
#define TAG_E_SNONCE2				0x1017
#define TAG_ENCRYPT_SETTINGS		0x1018
#define TAG_ENCRYPT_TYPE			0x100F
#define TAG_ENCRYPT_TYPE_FLAGS		0x1010
#define TAG_EROLLEE_NONCE			0x101A
#define TAG_FEATURE_ID				0x101B
#define TAG_IDENTITY				0x101C
#define TAG_IDENTITY_PROOF			0x101D

#define TAG_INIT_VECTOR				0x1060
#define TAG_KEY_WRAP_AUTH			0x101E
#define TAG_KEY_IDENTIFIER			0x101F
#define TAG_MAC_ADDRESS				0x1020
#define TAG_MANUFACTURER			0x1021
#define TAG_MSG_TYPE				0x1022
#define TAG_MODEL_NAME				0x1023
#define TAG_MODEL_NUMBER			0x1024
#define TAG_NETWORK_INDEX			0x1026
#define TAG_NETWORK_KEY				0x1027
#define TAG_NETWORK_KEY_INDEX		0x1028
#define TAG_NEW_DEVICE_NAME			0x1029
#define TAG_NEW_PASSWORD			0x102A
#define TAG_OOB_DEVICE_PASSWORD		0x102C
#define TAG_OS_VERSION				0x102D
#define TAG_POWER_LEVEL				0x102F

#define TAG_PSK_CURRENT				0x1030
#define TAG_PSK_MAX					0x1031
#define TAG_PUB_KEY					0x1032
#define TAG_RADIO_ENABLED			0x1033
#define TAG_REBOOT					0x1034
#define TAG_REGISTRAR_CURRENT		0x1035
#define TAG_REGISTRAR_ESTAB			0x1036
#define TAG_REGISTRAR_LIST			0x1037
#define TAG_REGISTRAR_MAX			0x1038
#define TAG_REGISTRAR_NONCE			0x1039
#define TAG_REQUEST_TYPE			0x103A
#define TAG_RESPONSE_TYPE			0x103B
#define TAG_RF_BAND					0x103C
#define TAG_R_HASH1					0x103D
#define TAG_R_HASH2					0x103E
#define TAG_R_SNONCE1				0x103F

#define TAG_R_SNONCE2				0x1040
#define TAG_SELECTED_REGITRAR		0x1041
#define TAG_SERIAL_NUM				0x1042
#define TAG_SIMPLE_CONFIG_STATE		0x1044
#define TAG_SSID					0x1045
#define TAG_TOTAL_NETWORK			0x1046
#define TAG_UUID_E					0x1047
#define TAG_UUID_R					0x1048
#define TAG_VENDOR_EXT				0x1049
#define TAG_VERSION					0x104A
#define TAG_X509_CERTIFICATE_REQ	0x104B
#define TAG_X509_CERTIFICATE		0x104C
#define TAG_EAP_IDENTITY			0x104D
#define TAG_MSG_COUNTER				0x104E
#define TAG_PUB_KEY_HASH			0x104F

#define TAG_REKEY					0x1050
#define TAG_KEY_LIFETIME			0x1051
#define TAG_PERMITTED_CONFIG_METHODS	0x1052
#define TAG_SEL_REG_CONFIG_METHODS	0x1053
#define TAG_PRIMARY_DEVICE_TYPE		0x1054
#define TAG_SEC_DEVICE_TYPE_LIST	0x1055
#define TAG_PORTABLE_DEVICE			0x1056
#define TAG_AP_SECTUP_LOCKED		0x1057
#define TAG_APPLICATION_EXTENSION		0x1058
#define TAG_EAP_TYPE				0x1059
#define TAG_WEP_TRANSMIT_KEY		0x1064

#define WSC_VER						0x10

#define MSG_TYPE_M1					4
#define MSG_TYPE_M2					5
#define MSG_TYPE_M2D				6
#define MSG_TYPE_M3					7
#define MSG_TYPE_M4					8
#define MSG_TYPE_M5					9
#define MSG_TYPE_M6					10
#define MSG_TYPE_M7					11
#define MSG_TYPE_M8					12
#define MSG_TYPE_ACK				13
#define MSG_TYPE_NACK				14
#define MSG_TYPE_DONE				15

#define NOT_GREATER_THAN_MASK		0x80000000
#define SIOCGIWIND      			0x89ff
#define SIOCGIWRTLSCANREQ			0x8B33	// scan request
#define SIOCGIWRTLGETBSSDB			0x8B34	// get bss data base
#define SIOCGIWRTLGETMIB			0x89f2	// get mib (== RTL8190_IOCTL_GET_MIB)
#define SIOCGIWRTLJOINREQ			0x8B35	// join request
#define SIOCGIWRTLJOINREQSTATUS		0x8B36	// get status of join request

#define FIFO_MODE					(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
#define WRITE_FLASH_PROG			("flash")
#define PARAM_TEMP_FILE				("/tmp/flash_param")
#define WEB_PID_FILENAME			("/var/run/webs.pid")

#if defined(CONFIG_RTL8186_KB)
	/* --- Forrest added, 2007.10.31. */
	#define LED_WSC_START			-1
	#define LED_WSC_END				-2
	#define LED_PBC_OVERLAPPED		-3
	#define LED_WSC_ERROR			-4
	#define LED_WSC_SUCCESS			-5
	/* Forrest added, 2007.10.31. --- */
#elif defined(CONFIG_RTL8186_TR)
	#define LED_WSC_START			2
	#define LED_WSC_END				0
	#define LED_PBC_OVERLAPPED		6	
	#define LED_WSC_OK				1
	/* --- Forrest added, 2007.10.31. */
	#define LED_WSC_ERROR			TURNKEY_LED_WSC_NOP
	#define LED_WSC_SUCCESS			TURNKEY_LED_WSC_NOP
	/* Forrest added, 2007.10.31. --- */
#elif defined(CONFIG_RTL865X_SC)	
	#define LED_WSC_START			-1
	#define LED_WSC_END				-2
	#define LED_PBC_OVERLAPPED		-3
	/* --- Forrest added, 2007.10.31. */
	#define LED_WSC_ERROR			TURNKEY_LED_WSC_NOP
	#define LED_WSC_SUCCESS			TURNKEY_LED_WSC_NOP
	/* Forrest added, 2007.10.31. --- */
#elif defined(CONFIG_RTL867x)
	#define LED_WSC_START			-1
	#define LED_WSC_END			-2
	#define LED_PBC_OVERLAPPED		-3
	/* --- Forrest added, 2007.10.31. */
	#define LED_WSC_ERROR			-4
	#define LED_WSC_SUCCESS			-5
	/* Forrest added, 2007.10.31. --- */
#else
	#define LED_WSC_START			-1
	#define LED_WSC_END				-2
	#define LED_PBC_OVERLAPPED		-3
	/* --- Forrest added, 2007.10.31. */
	#define LED_WSC_ERROR			TURNKEY_LED_WSC_NOP
	#define LED_WSC_SUCCESS			TURNKEY_LED_WSC_NOP
	/* Forrest added, 2007.10.31. --- */
#endif
	#define TURNKEY_LED_WSC_START		-1
	#define TURNKEY_LED_WSC_END			-2
	#define TURNKEY_LED_PBC_OVERLAPPED	-3
	/* --- Forrest added, 2007.10.31. */
	#define TURNKEY_LED_WSC_ERROR		-4
	#define TURNKEY_LED_WSC_SUCCESS		-5
	#define TURNKEY_LED_WSC_NOP			-6
	/* Forrest added, 2007.10.31. --- */


enum { RSP_TYPE_ENR, RSP_TYPE_ENR_1X, RSP_TYPE_REG, RSP_TYPE_AP };
enum { REQ_TYPE_ENR, REQ_TYPE_ENR_1X, REQ_TYPE_REG, REQ_TYPE_MANAGER };

typedef enum{
        DOT11_EVENT_NO_EVENT = 1,
        DOT11_EVENT_REQUEST = 2,
        DOT11_EVENT_ASSOCIATION_IND = 3,
        DOT11_EVENT_ASSOCIATION_RSP = 4,
        DOT11_EVENT_AUTHENTICATION_IND = 5,
        DOT11_EVENT_REAUTHENTICATION_IND = 6,
        DOT11_EVENT_DEAUTHENTICATION_IND = 7,
        DOT11_EVENT_DISASSOCIATION_IND = 8,
        DOT11_EVENT_DISCONNECT_REQ = 9,
        DOT11_EVENT_SET_802DOT11 = 10,
        DOT11_EVENT_SET_KEY = 11,
        DOT11_EVENT_SET_PORT = 12,
        DOT11_EVENT_DELETE_KEY = 13,
        DOT11_EVENT_SET_RSNIE = 14,
        DOT11_EVENT_GKEY_TSC = 15,
        DOT11_EVENT_MIC_FAILURE = 16,
        DOT11_EVENT_ASSOCIATION_INFO = 17,
        DOT11_EVENT_INIT_QUEUE = 18,
        DOT11_EVENT_EAPOLSTART = 19,

        DOT11_EVENT_ACC_SET_EXPIREDTIME = 31,
        DOT11_EVENT_ACC_QUERY_STATS = 32,
        DOT11_EVENT_ACC_QUERY_STATS_ALL = 33,
        DOT11_EVENT_REASSOCIATION_IND = 34,
        DOT11_EVENT_REASSOCIATION_RSP = 35,
        DOT11_EVENT_STA_QUERY_BSSID = 36,
        DOT11_EVENT_STA_QUERY_SSID = 37,
        DOT11_EVENT_EAP_PACKET = 41,

#ifdef RTL_WPA2_PREAUTH
        DOT11_EVENT_EAPOLSTART_PREAUTH = 45,
        DOT11_EVENT_EAP_PACKET_PREAUTH = 46,
#endif

#ifdef RTL_WPA2_CLIENT
	DOT11_EVENT_WPA2_MULTICAST_CIPHER = 47,
#endif

	DOT11_EVENT_WPA_MULTICAST_CIPHER = 48,

#ifdef AUTO_CONFIG
		DOT11_EVENT_AUTOCONF_ASSOCIATION_IND = 50,
		DOT11_EVENT_AUTOCONF_ASSOCIATION_CONFIRM = 51,
		DOT11_EVENT_AUTOCONF_PACKET = 52,
		DOT11_EVENT_AUTOCONF_LINK_IND = 53,
#endif

#ifdef WIFI_SIMPLE_CONFIG
		DOT11_EVENT_WSC_SET_IE = 55,
		DOT11_EVENT_WSC_PROBE_REQ_IND = 56,
		DOT11_EVENT_WSC_PIN_IND = 57,
		DOT11_EVENT_WSC_ASSOC_REQ_IE_IND = 58,
#endif
} DOT11_EVENT;


/*================================================================*/
/* Type Declarations */

#define __PACK__	__attribute__ ((packed))

enum { PROXY=0, ENROLLEE=1, REGISTRAR=2 };
enum { METHOD_PIN=1, METHOD_PBC=2 };
enum { CONFIG_METHOD_ETH=0x2, CONFIG_METHOD_PIN=0x4, CONFIG_METHOD_PBC=0x80 };
enum { 
		MODE_AP_UNCONFIG=1, 			// AP unconfigured (enrollee)
		MODE_CLIENT_UNCONFIG=2, 		// client unconfigured (enrollee) 
		MODE_CLIENT_CONFIG=3,			// client configured (registrar) 
		MODE_AP_PROXY=4, 				// AP configured (proxy)
		MODE_AP_PROXY_REGISTRAR=5,		// AP configured (proxy and registrar)
		MODE_CLIENT_UNCONFIG_REGISTRAR=6		// client unconfigured (registrar)
};
enum { AUTH_OPEN=1, AUTH_WPAPSK=2, AUTH_SHARED=4, AUTH_WPA=8, AUTH_WPA2=0x10, AUTH_WPA2PSK=0x20, AUTH_WPA2PSKMIXED=0x22 };
enum { ENCRYPT_NONE=1, ENCRYPT_WEP=2, ENCRYPT_TKIP=4, ENCRYPT_AES=8, ENCRYPT_TKIPAES=12 };
enum { CONNECT_TYPE_BSS=1, CONNECT_TYPE_IBSS=2 };
enum { EV_START, EV_STOP, EV_EAP, EV_ASSOC_IND, EV_PIN_INPUT, EV_PB_PRESS, EV_PROBEREQ_IND };
enum { ASSOC_STATE_NOT_ASSOC, ASSOC_STATE_CONNECT_SUCCESS,
		ASSOC_STATE_CONFIG_FAIL, ASSOC_STATE_ASSOC_FAIL, ASSOC_STATE_IP_FAIL};
enum { CONFIG_STATE_UNCONFIGURED=1, CONFIG_STATE_CONFIGURED=2};
enum { CONFIG_BY_INTERNAL_REGISTRAR=1, CONFIG_BY_EXTERNAL_REGISTRAR=2, MANUAL_SETTING_TO_ENROLLEE=3};
enum { ST_ENROLLE=0x80000000, ST_INT_REG=0x40000000, ST_EXT_REG=0x20000000 };

enum { ST_IDLE, ST_WAIT_REQ_ID, ST_WAIT_RSP_ID, ST_WAIT_START, ST_WAIT_M1,
		ST_WAIT_M2, ST_WAIT_M3, ST_WAIT_M4, ST_WAIT_M5, ST_WAIT_M6,
		ST_WAIT_M7, ST_WAIT_M8, ST_WAIT_ACK, ST_WAIT_DONE, 
#ifdef SUPPORT_UPNP
		ST_UPNP_DONE, ST_UPNP_WAIT_M1, ST_UPNP_PROXY, ST_UPNP_WAIT_REBOOT, ST_UPNP_WAIT_DONE,
#endif
		ST_WAIT_EAP_FAIL, ST_WAIT_EAPOL_START};

enum { PASS_ID_DEFAULT, PASS_ID_USER, PASS_ID_MACHINE, PASS_ID_REKEY,
		PASS_ID_PB, PASS_ID_REG, PASS_ID_RESERVED };

enum {	REINIT_SYS=1, SYNC_FLASH_PARAMETER=2};
enum {TYPE_BYTE, TYPE_WORD, TYPE_DWORD, TYPE_STR, TYPE_BIN};

enum {SET_IE_FLAG_BEACON=1, SET_IE_FLAG_PROBE_RSP=2, SET_IE_FLAG_PROBE_REQ=3,
		SET_IE_FLAG_ASSOC_RSP=4, SET_IE_FLAG_ASSOC_REQ=5};
enum {CONFIG_ERR_NO_ERR=0, CONFIG_ERR_OOB_INTERFACE_READ_ERR=1,
		CONFIG_ERR_DECRYPTION_CRC_ERR=2, CONFIG_ERR_2_4_CH_NOT_SUPPORTED=3,
		CONFIG_ERR_5_0_CH_NOT_SUPPORTED=4, CONFIG_ERR_SIGNAL_TOO_WEAK=5,
		CONFIG_ERR_NET_AUTH_FAIL=6, CONFIG_ERR_NET_ASSOC_FAIL=7,
		CONFIG_ERR_NO_DHCP_RESPONSE=8, CONFIG_ERR_FAIL_DHCP_CONFIG=9,
		CONFIG_ERR_IP_ADDR_CONFLICT=10, CONFIG_ERR_CANNOT_CONNECT_TO_REG=11,
		CONFIG_ERR_MUL_PBC_DETECTED=12, CONFIG_ERR_ROGUE_ACT_SUSPECTED=13,
		CONFIG_ERR_DEV_BUSY=14, CONFIG_ERR_SETUP_LOCKED=15,
		CONFIG_ERR_MESSAGE_TIMEOUT=16, CONFIG_ERR_REG_SESSION_TIMEOUT=17,
		CONFIG_ERR_DEV_PASS_AUTH_FAIL=18};
enum { WEP_DISABLED=0, WEP64=1, WEP128=2 };
enum { KEY_ASCII=0, KEY_HEX=1 };
enum { PROTOCOL_START=0, PROTOCOL_PBC_OVERLAPPING=1,
		PROTOCOL_TIMEOUT=2, PROTOCOL_SUCCESS=3 };

struct eap_rr_t {
	unsigned char type __PACK__;	// The bytes after this are the data corresponding to the RR type
};

struct eap_wsc_t {
	unsigned char type __PACK__;
	unsigned char vendor_id[3] __PACK__;
	unsigned long vendor_type __PACK__;
	unsigned char op_code __PACK__;
	unsigned char flags __PACK__;
};

struct eap_t {
	unsigned char code __PACK__;		// Identifies the type of EAP packet.
	unsigned char identifier __PACK__;	// Aids in matching responses with requests.
	unsigned short length __PACK__; 	// Length of EAP packet including code, id, len, data fields
};

struct eapol_t {
	unsigned char protocol_version __PACK__;
	unsigned char packet_type __PACK__;			// This makes it odd in number !
	unsigned short packet_body_length __PACK__;
};

struct ethernet_t {
	unsigned char  ether_dhost[ETHER_ADDRLEN] __PACK__;    /* destination ethernet address */
	unsigned char  ether_shost[ETHER_ADDRLEN] __PACK__;    /* source ethernet address */
	unsigned short ether_type __PACK__;                    /* packet type ID */
};

typedef struct _DOT11_WSC_ASSOC_IND{
        unsigned char   EventId;
        unsigned char   IsMoreEvent;
        char            MACAddr[ETHER_ADDRLEN];
        unsigned short  AssocIELen;
        char            AssocIE[PROBEIELEN];
	  unsigned char wscIE_included;
}DOT11_WSC_ASSOC_IND;

typedef struct _DOT11_PROBE_REQUEST_IND{
        unsigned char   EventId;
        unsigned char   IsMoreEvent;
        char            MACAddr[6];
        unsigned short  ProbeIELen;
        char            ProbeIE[PROBEIELEN];
}DOT11_PROBE_REQUEST_IND;

typedef struct _DOT11_EAP_PACKET{
	unsigned char	EventId;
	unsigned char	IsMoreEvent;
	unsigned short  packet_len;
	unsigned char	packet[1550];
} DOT11_EAP_PACKET;

typedef struct _DOT11_WSC_PIN_IND{
	unsigned char	EventId;
	unsigned char	IsMoreEvent;
	unsigned char	code[256];
} DOT11_WSC_PIN_IND;

typedef struct _DOT11_SET_WSCIE {
	unsigned char EventId;
	unsigned char IsMoreEvent;
	unsigned short Flag;
	unsigned short RSNIELen;
	unsigned char RSNIE[MAX_WSC_IE_LEN];
}DOT11_SET_WSCIE;

typedef struct _DOT11_DISCONNECT_REQ{
        unsigned char   EventId;
        unsigned char   IsMoreEvent;
        unsigned short  Reason;
        char            MACAddr[ETHER_ADDRLEN];
}DOT11_DISCONNECT_REQ;

#ifdef CLIENT_MODE
#define	WIFI_WPS		0x01000000

typedef enum { BAND_11B=1, BAND_11G=2, BAND_11BG=3, BAND_11A=4 } BAND_TYPE_T;

typedef enum _Capability {
    cESS 		= 0x01,
    cIBSS		= 0x02,
    cPollable		= 0x04,
    cPollReq		= 0x01,
    cPrivacy		= 0x10,
    cShortPreamble	= 0x20,
} Capability;

typedef enum _Synchronization_Sta_State{
    STATE_Min		= 0,
    STATE_No_Bss	= 1,
    STATE_Bss		= 2,
    STATE_Ibss_Active	= 3,
    STATE_Ibss_Idle	= 4,
    STATE_Act_Receive	= 5,
    STATE_Pas_Listen	= 6,
    STATE_Act_Listen	= 7,
    STATE_Join_Wait_Beacon = 8,
    STATE_Max		= 9
} Synchronization_Sta_State;

typedef struct _OCTET_STRING {
    unsigned char *Octet;
    unsigned short Length;
} OCTET_STRING;

typedef enum _BssType {
    infrastructure = 1,
    independent = 2,
} BssType;

typedef	struct _IbssParms {
    unsigned short	atimWin;
} IbssParms;

typedef struct _bss_info {
    unsigned char state;
    unsigned char channel;
    unsigned char txRate;
    unsigned char bssid[6];
    unsigned char rssi, sq;	// RSSI  and signal strength
    unsigned char ssid[MAX_SSID_LEN+1];
} bss_info;

typedef struct _BssDscr {
    unsigned char bdBssId[6];
    unsigned char bdSsIdBuf[MAX_SSID_LEN];
    OCTET_STRING  bdSsId;
    BssType bdType;
    unsigned short bdBcnPer;			// beacon period in Time Units
    unsigned char bdDtimPer;			// DTIM period in beacon periods
    unsigned long bdTstamp[2];			// 8 Octets from ProbeRsp/Beacon
    IbssParms bdIbssParms;			// empty if infrastructure BSS
    unsigned short bdCap;				// capability information
    unsigned char ChannelNumber;			// channel number
    unsigned long bdBrates;
    unsigned long bdSupportRates;		
    unsigned char bdsa[6];			// SA address
    unsigned char rssi, sq;			// RSSI and signal strength
    unsigned char network;			// 1: 11B, 2: 11G, 4:11G
} BssDscr, *pBssDscr;

typedef struct _sitesurvey_status {
    unsigned char number;
    unsigned char pad[3];
    BssDscr bssdb[MAX_BSS_DESC];
} SS_STATUS_T, *SS_STATUS_Tp;

struct wps_ie_info {
	unsigned char rssi;
	unsigned char data[MAX_WSC_IE_LEN];	
};

typedef struct _sitesurvey_ie {
    unsigned char number;
    unsigned char pad[3];
    struct wps_ie_info ie[MAX_BSS_DESC];
} SS_IE_T, *SS_IE_Tp;

#endif // CLIENT_MODE

typedef struct sta_ctx {
	int used;
#ifdef SUPPORT_UPNP
	char ip_addr[IP_ADDRLEN];
	unsigned char setip;
	time_t time_stamp;
#endif
	int locked;
	int state;
	int eap_reqid;
	unsigned char addr[ETHER_ADDRLEN];
	unsigned char msg_addr[ETHER_ADDRLEN]; // for Intel SDK
	unsigned char uuid[UUID_LEN];
	int tx_size;
	unsigned char tx_buffer[TX_BUFFER_SIZE];	
	int reg_timeout;
	int timeout;
	int retry;
	int config_method;
	int device_password_id;
	DH *dh_enrollee;
	DH *dh_registrar;	
	unsigned char dh_shared_key[PUBLIC_KEY_LEN];
	unsigned char dh_digest_key[BYTE_LEN_256B];	
	unsigned char auth_key[BYTE_LEN_256B];	
	unsigned char key_wrap_key[BYTE_LEN_128B];	
	unsigned char EMSK[BYTE_LEN_256B];		
	unsigned char nonce_enrollee[NONCE_LEN];
	unsigned char nonce_registrar[NONCE_LEN];		
	unsigned char r_s1[NONCE_LEN];
	unsigned char r_s2[NONCE_LEN];
	unsigned char e_s1[NONCE_LEN];
	unsigned char e_s2[NONCE_LEN];
	unsigned char r_h1[BYTE_LEN_256B];
	unsigned char r_h2[BYTE_LEN_256B];
	unsigned char e_h1[BYTE_LEN_256B];
	unsigned char e_h2[BYTE_LEN_256B];
	unsigned char last_tx_msg_buffer[TX_BUFFER_SIZE*2];	
	int last_tx_msg_size;
	unsigned char *last_rx_msg;
	int last_rx_msg_size;	
	int auth_type_flags;	
	int encrypt_type_flags;
	unsigned char Assoc_wscIE_included;
	unsigned char invoke_security_gen;
#ifdef BLOCKED_ROGUE_STA
	unsigned char blocked;
#endif
	unsigned char ap_role;
#if defined(CLIENT_MODE) && defined(SUPPORT_REGISTRAR)
	unsigned char config_state;
#endif
	unsigned char do_not_rescan;
	unsigned char allow_reconnect_count;
} STA_CTX, *STA_CTX_Tp;

typedef struct pbc_node_context *pbc_node_ptr;
struct pbc_node_context {
     	time_t time_stamp;
	unsigned char uuid[UUID_LEN];
	unsigned char addr[ETHER_ADDRLEN];
     	pbc_node_ptr next_pbc_sta;
};

#ifdef PREVENT_PROBE_DEADLOCK
struct probe_node {
	unsigned char used;
	char ProbeIE[PROBEIELEN];
	int ProbeIELen;
	char ProbeMACAddr[6];
	time_t time_stamp;
	unsigned char sent;
};
#endif

struct blocked_sta {
	unsigned char error_count; 
	unsigned char used;
	int expired_time;
	unsigned char addr[ETHER_ADDRLEN];
};


#pragma pack(push, 4)
typedef struct context {
#ifdef BLOCKED_ROGUE_STA
	int blocked_expired_time;
#endif	
	unsigned char is_ap;	
	int start;
	int	socket;
	int mode;
	int upnp;
	int role;
	int original_role; 
	int use_ie;
	int config_state;
	int config_method;
	int auth_type;
	int auth_type_flags;	
	int auth_type_flash;
	int encrypt_type;
	int encrypt_type_flags;	
	int encrypt_type_flash;
	int connect_type;
	int manual_config;
	int pb_pressed;
	int pb_pressed_time;
	int pin_assigned;
	int peer_pin_id;
	int device_category_id;
	int device_sub_category_id;
	int rf_band;
	int device_password_id;
	int config_err;
	int os_ver;
	int rx_size;
	int lock;
	int network_key_len;	
	int tx_timeout;
	int resent_limit;
	int reg_timeout;
	int wait_reinit;
	int pb_timeout;
	int pin_timeout;
	int setSelectedRegTimeout;
	int assigned_auth_type;
	int assigned_encrypt_type;
	int wait_reboot;
	int num_sta;
	unsigned int num_ext_registrar;
#ifdef SUPPORT_UPNP	
	unsigned int TotalSubscriptions;
#endif

#ifdef NO_IWCONTROL
	int wl_chr_fd;
#else
	int	fifo;
#endif

#ifdef CLIENT_MODE	
	int join_idx;
	int connect_fail;
	int connect_method;
	//long target_time;
#endif	

#ifdef MUL_PBC_DETECTTION
	int SessionOverlapTimeout;
#endif

#ifdef PREVENT_PROBE_DEADLOCK
	unsigned int probe_list_count;
#endif
		
#ifdef TEST
	int	test;
#endif

#ifdef  DEBUG
	int	debug;
#endif
	
	char cfg_filename[100];
	char wlan_interface_name[40];
#ifdef SUPPORT_UPNP
	char lan_interface_name[40];
	char SetSelectedRegistrar_ip[IP_ADDRLEN];
	unsigned char status_changed;
	unsigned char upnp_wait_reboot_timeout;
	struct subscription_info upnp_subscription_info[MAX_SUBSCRIPTION_NUM];
	STA_CTX cached_sta;
#ifdef USE_MINI_UPNP
	mini_upnp_CTX_T upnp_info;
#endif
#endif	
	char fifo_name[100];
	char pin_code[PIN_LEN+1];
	char original_pin_code[PIN_LEN+1]; 
	char peer_pin_code[PIN_LEN+1];
	char pid_filename[100];
	unsigned char uuid[UUID_LEN];
	char manufacturer[MAX_MANUFACT_LEN+1];
	char model_name[MAX_MODEL_NAME_LEN+1];
	char model_num[MAX_MODEL_NUM_LEN+1];	
	char serial_num[MAX_SERIAL_NUM_LEN+1];
	unsigned char device_oui[OUI_LEN];
	char device_name[MAX_DEVICE_NAME_LEN+1];
	unsigned char rx_buffer[RX_BUFFER_SIZE*2];
	unsigned char our_addr[ETHER_ADDRLEN];
	char SSID[MAX_SSID_LEN+1];
	unsigned char network_key[MAX_NETWORK_KEY_LEN+1];
	unsigned char wep_key2[MAX_NETWORK_KEY_LEN+1];
	unsigned char wep_key3[MAX_NETWORK_KEY_LEN+1];
	unsigned char wep_key4[MAX_NETWORK_KEY_LEN+1];	
	unsigned char wep_transmit_key;
	unsigned char nonce_enrollee[NONCE_LEN];	
	unsigned char assigned_ssid[MAX_SSID_LEN+1];
	unsigned char assigned_network_key[MAX_NETWORK_KEY_LEN+1];
	unsigned char assigned_wep_key_1[MAX_WEP_KEY_LEN+1];
	unsigned char assigned_wep_key_2[MAX_WEP_KEY_LEN+1];
	unsigned char assigned_wep_key_3[MAX_WEP_KEY_LEN+1];
	unsigned char assigned_wep_key_4[MAX_WEP_KEY_LEN+1];
	unsigned char assigned_wep_transmit_key;
	unsigned char assigned_wep_key_len;
	unsigned char assigned_wep_key_format;
	STA_CTX *sta[MAX_STA_NUM];
#ifdef CLIENT_MODE	
	SS_STATUS_T ss_status;
	SS_IE_T ss_ie;
#endif	
	unsigned char registration_on; 
	WSC_pthread_mutex_t RegMutex;
	STA_CTX_Tp sta_invoke_reg;
#ifdef MUL_PBC_DETECTTION
	pbc_node_ptr active_pbc_staList;
	unsigned char active_pbc_sta_count;
	WSC_pthread_mutex_t PBCMutex;
	int disable_MulPBC_detection;
	int PBC_overlapping_LED_time_out;
	int WPS_PBC_overlapping_GPIO_number;
#endif
#ifdef PREVENT_PROBE_DEADLOCK
	struct probe_node probe_list[MAX_WSC_PROBE_STA];
#endif
#ifdef BLOCKED_ROGUE_STA
	struct blocked_sta blocked_sta_list[MAX_BLOCKED_STA_NUM];
#endif
	int disable_configured_by_exReg;
#ifdef CLIENT_MODE
	STA_CTX_Tp sta_to_clear;
#endif
	int WPS_START_LED_GPIO_number;
	int WPS_END_LED_unconfig_GPIO_number;
	int WPS_END_LED_config_GPIO_number;
	int No_ifname_for_flash_set;

	/* --- Forrest added, 2007.10.31. */
	int LedTimeout;
	int WPS_ERROR_LED_time_out;
	int WPS_ERROR_LED_GPIO_number;
	int WPS_SUCCESS_LED_time_out;
	int WPS_SUCCESS_LED_GPIO_number;
	/* Forrest added, 2007.10.31. --- */
	//cathy
	int restart_timeout;
	//add by peteryu for WZC in WEP
	int fix_wzc_wep; // disable/enable the issue for WZC in WEP 
	int wps_triggered;      // wps has been triggered 
	int config_by_ext_reg;  // configured by external registrar 
} CTX_T, *CTX_Tp;
		
#pragma pack(pop)


#if defined(SUPPORT_UPNP) && !defined(USE_MINI_UPNP)
typedef struct _IPCon {
  char *ifname;
} _IPCon;

typedef struct _IPCon *IPCon;
#endif


#define MIB_SET(key, val) do { \
		if (mib_set(key, (void *)val) == 0) { \
			fprintf(stderr, "MIB set error (%s:%d)\n", __FILE__, __LINE__); \
		} \
	} while (0)


/*================================================================*/

// exported variables and routines of wsc.c
extern unsigned char WSC_VENDOR_ID[3];
extern unsigned char wsc_prime_num[];

// exported variables and routines of txpkt.c
extern int send_wlan(CTX_Tp pCtx, unsigned char *data, int size);
extern int send_eap_reqid(CTX_Tp pCtx, STA_CTX_Tp pSta);
extern int send_wsc_done(CTX_Tp pCtx, STA_CTX_Tp pSta);
extern int send_wsc_start(CTX_Tp pCtx, STA_CTX_Tp pSta);
extern int send_eap_fail(CTX_Tp pCtx, STA_CTX_Tp pSta);
extern int send_wsc_nack(CTX_Tp pCtx, STA_CTX_Tp pSta, int err_code);
extern int send_wsc_ack(CTX_Tp pCtx, STA_CTX_Tp pSta);
extern int send_wsc_M8(CTX_Tp pCtx, STA_CTX_Tp pSta);
extern int send_wsc_M6(CTX_Tp pCtx, STA_CTX_Tp pSta);
extern int send_wsc_M4(CTX_Tp pCtx, STA_CTX_Tp pSta);
extern int send_wsc_M2(CTX_Tp pCtx, STA_CTX_Tp pSta);
#ifdef SUPPORT_ENROLLEE
extern int send_wsc_M7(CTX_Tp pCtx, STA_CTX_Tp pSta);
extern int send_wsc_M5(CTX_Tp pCtx, STA_CTX_Tp pSta);
extern int send_wsc_M3(CTX_Tp pCtx, STA_CTX_Tp pSta);
extern int send_wsc_M1(CTX_Tp pCtx, STA_CTX_Tp pSta);
extern int send_eap_rspid(CTX_Tp pCtx, STA_CTX_Tp pSta);
extern int send_eapol_start(CTX_Tp pCtx, STA_CTX_Tp pSta);
#endif
#ifdef SUPPORT_UPNP
extern int send_upnp_to_wlan(CTX_Tp pCtx, STA_CTX_Tp pSta, struct WSC_packet *packet);
#endif


// exported variables and routines of rxpkt.c
#ifdef SUPPORT_ENROLLEE
int pktHandler_reqid(CTX_Tp pCtx, STA_CTX_Tp pSta, int id);
int pktHandler_wsc_start(CTX_Tp pCtx, STA_CTX_Tp pSta);
#endif
extern int pktHandler_rspid(CTX_Tp pCtx, STA_CTX_Tp pSta, unsigned char *id, int len);
extern int pktHandler_wsc_ack(CTX_Tp pCtx, STA_CTX_Tp pSta, struct eap_wsc_t *wsc);
extern int pktHandler_wsc_nack(CTX_Tp pCtx, STA_CTX_Tp pSta, struct eap_wsc_t *wsc);
extern int pktHandler_wsc_done(CTX_Tp pCtx, STA_CTX_Tp pSta);
extern int pktHandler_eap_fail(CTX_Tp pCtx, STA_CTX_Tp pSta);
extern int pktHandler_wsc_msg(CTX_Tp pCtx, STA_CTX_Tp pSta, struct eap_wsc_t * wsc, int len);
#ifdef SUPPORT_UPNP
extern int pktHandler_upnp_select_msg(CTX_Tp pCtx, STA_CTX_Tp pSta, struct WSC_packet *packet);
#endif


// exported variables and routines of utils.c
extern int wlioctl_get_mib(	char *interfacename , char* mibname ,int result );
#ifdef DEBUG
void debug_out(unsigned char *label, unsigned char *data, int data_length);
#endif
extern void convert_bin_to_str(unsigned char *bin, int len, char *out);
extern unsigned char *add_tlv(unsigned char *data, unsigned short id, int len, void *val);
extern unsigned char *append(unsigned char *src, unsigned char *data, int data_len);
extern int wlioctl_set_led(char *interface, int flag);
extern int wlioctl_set_wsc_ie(char *interface, char *data, int len, int id, int flag);
extern int derive_key(CTX_Tp pCtx, STA_CTX_Tp pSta);
extern int output_the_config(CTX_Tp pCtx, int is_local);
extern void signal_webs(int condition);
#ifdef WINDOW7//add by peteryu
extern void func_off_wlan_tx(CTX_Tp pCtx);
#endif
extern int validate_pin_code(unsigned long code);
extern DH *generate_dh_parameters(int prime_len, unsigned char *data, int generator);
extern void reset_sta(CTX_Tp pCtx, STA_CTX_Tp pSta, int need_free);
extern void reset_ctx_state(CTX_Tp pCtx);
extern void hmac_sha256(unsigned char *text, int text_len, unsigned char *key, int key_len, unsigned char *digest, int *digest_len);
extern void Encrypt_aes_128_cbc(unsigned char *key, unsigned char *iv, unsigned char *plaintext, unsigned int plainlen, unsigned char *ciphertext, unsigned int *cipherlen);
extern void Decrypt_aes_128_cbc(unsigned char *key,  unsigned char *iv, unsigned char *plaintext, unsigned int *plainlen, unsigned char *ciphertext, unsigned int cipherlen);
extern void wsc_kdf(
	unsigned char *key,                // pointer to authentication key 
	int             key_len,            // length of authentication key 
	unsigned char *text,               // pointer to data stream 
	int	text_len,           // length of data stream 
	int 	expect_key_len,   //expect total key length in bit number
	unsigned char *digest             // caller digest to be filled in 
	);

extern int build_beacon_ie(CTX_Tp pCtx, unsigned char selected, unsigned short passid, \
				unsigned short method, unsigned char *data);
extern int build_probe_rsp_ie(CTX_Tp pCtx, unsigned char selected, unsigned short passid, \
				unsigned short method, unsigned char *data);
extern int build_assoc_response_ie(CTX_Tp pCtx, unsigned char *data);
extern int build_provisioning_service_ie(unsigned char *data);
extern unsigned char *search_tag(unsigned char *data, unsigned short id, int len, int *out_len);
extern int IssueDisconnect(char *interface, unsigned char *pucMacAddr, unsigned short reason);

#ifdef CLIENT_MODE
extern int wlioctl_scan_reqest(char *interface, int *pStatus);
extern int wlioctl_scan_result(char *interface, SS_STATUS_Tp pStatus);
extern int build_probe_request_ie(CTX_Tp pCtx, unsigned short passid, 
				unsigned char *data);
extern int build_assoc_request_ie(CTX_Tp pCtx, unsigned char *data);
extern int getWlJoinRequest(char *interface, pBssDscr pBss, unsigned char *res);
extern int getWlJoinResult(char *interface, unsigned char *res);
#endif

extern void client_set_WlanDriver_WscEnable(const CTX_Tp pCtx, const int wps_enabled);

#ifdef SUPPORT_UPNP
#ifndef USE_MINI_UPNP
extern IPCon IPCon_New(char *ifname);
extern IPCon IPCon_Destroy(IPCon this);
extern struct in_addr *IPCon_GetIpAddr(IPCon this);
extern char *IPCon_GetIpAddrByStr(IPCon this);
#endif
//extern int isUpnpSubscribed(CTX_Tp pCtx);
extern void convert_bin_to_str_UPnP(unsigned char *bin, int len, char *out);
extern void reset_sta_UPnP(CTX_Tp pCtx, STA_CTX_Tp pSta);
#endif
extern void clear_SetSelectedRegistrar_flag(CTX_Tp pCtx);
extern int check_wep_key_format(unsigned char *msg, int msg_len, unsigned char *key_format, unsigned char *key_len, unsigned char *msg_out, int *msg_out_len);
#ifdef MUL_PBC_DETECTTION
extern void search_active_pbc_sta(CTX_Tp pCtx, unsigned char *addr, unsigned char *uuid);
extern void remove_active_pbc_sta(CTX_Tp pCtx, STA_CTX_Tp pSta, unsigned char mode);
extern void SwitchSessionOverlap_LED_On(CTX_Tp pCtx);
#endif // MUL_PBC_DETECTTION
#ifdef BLOCKED_ROGUE_STA
extern int search_blocked_list(CTX_Tp pCtx, unsigned char *addr);
extern void update_blocked_list(CTX_Tp pCtx, STA_CTX_Tp pSta);
extern struct blocked_sta *add_into_blocked_list(CTX_Tp pCtx, STA_CTX_Tp pSta);
extern void disassociate_blocked_list(CTX_Tp pCtx);
extern void countdown_blocked_list(CTX_Tp pCtx);
#endif // BLOCKED_ROGUE_STA
extern void enable_WPS_LED(void);
//Brad remove 2008-01-02
//#if defined (CONFIG_RTL8186_KB) || defined (CONFIG_RTL8186_TR) || defined(CONFIG_RTL865X_SC)
extern void report_WPS_STATUS(int status);
//#endif
#endif // INCLUDE_WSC_H

