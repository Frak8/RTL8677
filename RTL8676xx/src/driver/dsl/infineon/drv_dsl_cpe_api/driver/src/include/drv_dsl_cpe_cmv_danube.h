/******************************************************************************

                               Copyright (c) 2007
                            Infineon Technologies AG
                     Am Campeon 1-12; 81726 Munich, Germany

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/

#ifndef _DRV_DSL_CPE_CMV_DANUBE_H
#define _DRV_DSL_CPE_CMV_DANUBE_H

#ifdef __cplusplus
   extern "C" {
#endif

/** Defines the length of the fixed header */
#define DSL_CMV_HEADER_LENGTH             4
/** Defines the max. length of a complete firmware message (including header) */
#define DSL_MAX_CMV_MSG_LENGTH           16
/** Defines the max. payload length of a message (excluding header) */
#define DSL_MAX_CMV_MSG_PAYLOAD_LENGTH   (DSL_MAX_CMV_MSG_LENGTH - DSL_CMV_HEADER_LENGTH)

/**
   Defines all possible CMV groups
*/
typedef enum {
   DSL_CMV_GROUP_CNTL = 1,
   DSL_CMV_GROUP_STAT = 2,
   DSL_CMV_GROUP_INFO = 3,
   DSL_CMV_GROUP_TEST = 4,
   DSL_CMV_GROUP_OPTN = 5,
   DSL_CMV_GROUP_RATE = 6,
   DSL_CMV_GROUP_PLAM = 7,
   DSL_CMV_GROUP_CNFG = 8
} DSL_CmvGroup_t;

/**
   Defines all opcode types
*/
typedef enum {
   DSL_CMV_OPCODE_H2D_CMV_READ = 0x00,
   DSL_CMV_OPCODE_H2D_CMV_WRITE = 0x04,
   DSL_CMV_OPCODE_H2D_CMV_INDICATE_REPLY = 0x10,
   DSL_CMV_OPCODE_H2D_ERROR_OPCODE_UNKNOWN =0x20,
   DSL_CMV_OPCODE_H2D_ERROR_CMV_UNKNOWN =0x30,

   DSL_CMV_OPCODE_D2H_CMV_READ_REPLY =0x01,
   DSL_CMV_OPCODE_D2H_CMV_WRITE_REPLY = 0x05,
   DSL_CMV_OPCODE_D2H_CMV_INDICATE = 0x11,
   DSL_CMV_OPCODE_D2H_ERROR_OPCODE_UNKNOWN = 0x21,
   DSL_CMV_OPCODE_D2H_ERROR_CMV_UNKNOWN = 0x31,
   DSL_CMV_OPCODE_D2H_ERROR_CMV_READ_NOT_AVAILABLE = 0x41,
   DSL_CMV_OPCODE_D2H_ERROR_CMV_WRITE_ONLY = 0x51,
   DSL_CMV_OPCODE_D2H_ERROR_CMV_READ_ONLY = 0x61,

   DSL_CMV_OPCODE_H2D_DEBUG_READ_DM = 0x02,
   DSL_CMV_OPCODE_H2D_DEBUG_READ_PM = 0x06,
   DSL_CMV_OPCODE_H2D_DEBUG_WRITE_DM = 0x0a,
   DSL_CMV_OPCODE_H2D_DEBUG_WRITE_PM = 0x0e,

   DSL_CMV_OPCODE_D2H_DEBUG_READ_DM_REPLY = 0x03,
   DSL_CMV_OPCODE_D2H_DEBUG_READ_FM_REPLY = 0x07,
   DSL_CMV_OPCODE_D2H_DEBUG_WRITE_DM_REPLY = 0x0b,
   DSL_CMV_OPCODE_D2H_DEBUG_WRITE_FM_REPLY = 0x0f,
   DSL_CMV_OPCODE_D2H_ERROR_ADDR_UNKNOWN = 0x33,

   DSL_CMV_OPCODE_D2H_AUTONOMOUS_MODEM_READY_MSG = 0xf1
} DSL_CmvOpcode_t;

/** Defines all possible addresses */
typedef enum {
   /** Info group */
   DSL_CMV_ADDRESS_INFO_DATA_LOGGING_BUFFER = 7,
   DSL_CMV_ADDRESS_INFO_DELT_SNR_PS_DS = 10,
   DSL_CMV_ADDRESS_INFO_SNRA_DS = 11,
   DSL_CMV_ADDRESS_INFO_BAT_US = 22,
   DSL_CMV_ADDRESS_INFO_BAT_DS = 23,
   DSL_CMV_ADDRESS_INFO_GAIN_US = 24,
   DSL_CMV_ADDRESS_INFO_GAIN_DS = 25,
   DSL_CMV_ADDRESS_INFO_DS_BIT_LOAD_FIRST = 26,
   DSL_CMV_ADDRESS_INFO_DS_BIT_LOAD_LAST = 27,
   DSL_CMV_ADDRESS_INFO_CMsgs1 = 30,
   DSL_CMV_ADDRESS_INFO_RMsgs1 = 31,
   DSL_CMV_ADDRESS_INFO_RCMSGRA = 33,
   DSL_CMV_ADDRESS_INFO_EXCESS_MARGIN = 47,
   DSL_CMV_ADDRESS_INFO_VERSION = 54,
   DSL_CMV_ADDRESS_INFO_VENDORID_FE = 56,
   DSL_CMV_ADDRESS_INFO_SERIALNUM_FE = 57,
   DSL_CMV_ADDRESS_INFO_VERSIONNUM_FE = 58,
   DSL_CMV_ADDRESS_INFO_VENDORID_NE = 60,
   DSL_CMV_ADDRESS_INFO_VERSIONNUM_NE = 61,
   DSL_CMV_ADDRESS_INFO_SERIALNUM_NE = 62,
   DSL_CMV_ADDRESS_INFO_VENDORID_G994_FE = 64,
   DSL_CMV_ADDRESS_INFO_VENDORID_G994_NE = 65,
   DSL_CMV_ADDRESS_INFO_SELFTEST = 66,
   DSL_CMV_ADDRESS_INFO_XTSE_FE = 67,
   DSL_CMV_ADDRESS_INFO_LINE_STATUS_DS = 68,
   DSL_CMV_ADDRESS_INFO_LINE_STATUS_US = 69,
   DSL_CMV_ADDRESS_INFO_DELT_PREV_STATS_US = 70,
   DSL_CMV_ADDRESS_INFO_DELT_PREV_STATS_DS = 71,
   DSL_CMV_ADDRESS_INFO_DELT_HLIN_PS_DS = 72,
   DSL_CMV_ADDRESS_INFO_DELT_HLIN_PS_US = 73,
   DSL_CMV_ADDRESS_INFO_DELT_HLOG_PS_DS = 74,
   DSL_CMV_ADDRESS_INFO_DELT_HLOG_PS_US = 75,
   DSL_CMV_ADDRESS_INFO_DELT_QLN_PS_DS = 76,
   DSL_CMV_ADDRESS_INFO_DELT_QLN_PS_US = 77,
   DSL_CMV_ADDRESS_INFO_SNRA_US = 78,
   DSL_CMV_ADDRESS_INFO_DELT_SNR_PS_US = 78,
   DSL_CMV_ADDRESS_INFO_DELT_MT_DS = 79,
   DSL_CMV_ADDRESS_INFO_DELT_MT_US = 80,
   DSL_CMV_ADDRESS_INFO_ME_HDLC_TX_BUFFER = 81,
   DSL_CMV_ADDRESS_INFO_ME_HDLC_RX_BUFFER = 82,
   DSL_CMV_ADDRESS_INFO_ME_HDLC_PARAMS = 83,
   DSL_CMV_ADDRESS_INFO_RX_CLEAR_EOC = 85,
   DSL_CMV_ADDRESS_INFO_DEBUGTRAIL_TRIGGER = 86,
   DSL_CMV_ADDRESS_INFO_DEBUGTRAIL_COLLECT = 87,
   DSL_CMV_ADDRESS_INFO_GPIO_CONTROL = 91,
   DSL_CMV_ADDRESS_INFO_LATENCY_PATH_PROPS_DS = 92,
   DSL_CMV_ADDRESS_INFO_LATENCY_PATH_PROPS_US = 93,
   DSL_CMV_ADDRESS_INFO_TX_POWER = 94,
   DSL_CMV_ADDRESS_INFO_PMD = 102,
   DSL_CMV_ADDRESS_INFO_SYSTEM_INTERFACE = 103,
   DSL_CMV_ADDRESS_INFO_REBOOT_INT_CONFIG = 108,
   DSL_CMV_ADDRESS_INFO_REBOOT_INT_INFO = 109,

   /** OPTN group */
   DSL_CMV_ADDRESS_OPTN_MODECONTROL = 0,
   DSL_CMV_ADDRESS_OPTN_ALG_CONTROL = 2,
   DSL_CMV_ADDRESS_OPTN_MODECONTROL1 = 7,
   DSL_CMV_ADDRESS_OPTN_STATE_MACHINE_CTRL = 9,
   DSL_CMV_ADDRESS_OPTN_UTOPIA_ADDRESS = 13,
   DSL_CMV_ADDRESS_OPTN_ALG_CONTROL2 = 16,
   DSL_CMV_ADDRESS_OPTN_EVENT_INTS_CTRL = 24,

   /** STAT group */
   DSL_CMV_ADDRESS_STAT_MACRO_STATE = 0,
   DSL_CMV_ADDRESS_STAT_MODE = 1,
   DSL_CMV_ADDRESS_STAT_MISC = 4,
   DSL_CMV_ADDRESS_STAT_FAIL = 5,
   DSL_CMV_ADDRESS_STAT_PATH_DS = 12,
   DSL_CMV_ADDRESS_STAT_PATH_US = 13,
   DSL_CMV_ADDRESS_STAT_ME_HDLC = 14,
   DSL_CMV_ADDRESS_STAT_MODE1 = 17,
   DSL_CMV_ADDRESS_STAT_POWER_STATE = 18,
   DSL_CMV_ADDRESS_STAT_L3_REQUEST = 20,
   DSL_CMV_ADDRESS_STAT_L3_FAILURE_REASON = 21,

   /** RATE group */
   DSL_CMV_ADDRESS_RATE_UsRate = 0,
   DSL_CMV_ADDRESS_RATE_DsRate = 1,
   DSL_CMV_ADDRESS_RATE_INTL_DELAY_DS = 2,
   DSL_CMV_ADDRESS_RATE_INTL_DELAY_US = 3,
   DSL_CMV_ADDRESS_RATE_DATA_NFEC_US_LP1 = 10,
   DSL_CMV_ADDRESS_RATE_DATA_NFEC_US_LP0 = 11,
   DSL_CMV_ADDRESS_RATE_DATA_NFEC_DS_LP1 = 18,
   DSL_CMV_ADDRESS_RATE_DATA_NFEC_DS_LP0 = 19,

   /** PLAM group */
   DSL_CMV_ADDRESS_PLAM_NE_FAILURES = 0,
   DSL_CMV_ADDRESS_PLAM_FE_FAILURES = 1,
   DSL_CMV_ADDRESS_PLAM_CRC_NE = 2,
   DSL_CMV_ADDRESS_PLAM_FEC_NE = 3,
   DSL_CMV_ADDRESS_PLAM_ES_NE = 7,
   DSL_CMV_ADDRESS_PLAM_SES_NE = 8,
   DSL_CMV_ADDRESS_PLAM_LOSS_NE = 9,
   DSL_CMV_ADDRESS_PLAM_UAS_NE = 10,
   DSL_CMV_ADDRESS_PLAM_HEC_NE = 11,
   DSL_CMV_ADDRESS_PLAM_NEAR_END_ECS_CNT = 6,
   DSL_CMV_ADDRESS_PLAM_NEAR_END_UASL_CNT = 10,
   DSL_CMV_ADDRESS_PLAM_TOTAL_CELLS_NE = 16,
   DSL_CMV_ADDRESS_PLAM_USER_TOTAL_CELLS_NE = 18,
   DSL_CMV_ADDRESS_PLAM_IBE_NE = 19,
   DSL_CMV_ADDRESS_PLAM_CRC_FE = 24,
   DSL_CMV_ADDRESS_PLAM_FEC_FE = 28,
   DSL_CMV_ADDRESS_PLAM_ES_FE = 33,
   DSL_CMV_ADDRESS_PLAM_SES_FE = 34,
   DSL_CMV_ADDRESS_PLAM_LOSS_FE = 35,
   DSL_CMV_ADDRESS_PLAM_UAS_FE = 36,
   DSL_CMV_ADDRESS_PLAM_HEC_FE = 37,
   DSL_CMV_ADDRESS_PLAM_IBE_FE = 42,
   DSL_CMV_ADDRESS_PLAM_TOTAL_CELLS_FE = 43,
/*$$   DSL_CMV_ADDRESS_PLAM_USER_TOTAL_CELLS_FE = ??,*/
   DSL_CMV_ADDRESS_PLAM_SNRM_0_1DB = 45,
   DSL_CMV_ADDRESS_PLAM_SNRM = 46,

   /** CNTL group */
   DSL_CMV_ADDRESS_CNTL_MODEM_CONTROL = 0,
   DSL_CMV_ADDRESS_CNTL_ME_HDLC = 2,
   DSL_CMV_ADDRESS_CNTL_L3_REQUEST = 3,

   /** CNFG group */
   DSL_CMV_ADDRESS_CNFG_DATA_RATEUS_FLAG_ADSL2_RP = 12,
   DSL_CMV_ADDRESS_CNFG_DATA_RATEUS_FLAG_ADSL2_MP = 13,
   DSL_CMV_ADDRESS_CNFG_DATA_RATEUS_FLAG_ADSL2_LP = 14,
   DSL_CMV_ADDRESS_CNFG_DATA_RATEUS_FLAG_ADSL2_TP = 15,
   DSL_CMV_ADDRESS_CNFG_DATA_INTLVDEPTH_US = 16,
   DSL_CMV_ADDRESS_CNFG_DATA_RATEUS_FLAG_ADSL2_KP = 17,
   DSL_CMV_ADDRESS_CNFG_DATA_RATEUS_FLAG_ADSL2_MSGLp = 19,
   DSL_CMV_ADDRESS_CNFG_DATA_RATEUS_FLAG_ADSL2_MSGc = 20,
   DSL_CMV_ADDRESS_CNFG_DATA_RATEDS_FLAG_ADSL2_RP = 23,
   DSL_CMV_ADDRESS_CNFG_DATA_RATEDS_FLAG_ADSL2_MP = 24,
   DSL_CMV_ADDRESS_CNFG_DATA_RATEDS_FLAG_ADSL2_LP = 25,
   DSL_CMV_ADDRESS_CNFG_DATA_RATEDS_FLAG_ADSL2_TP = 26,
   DSL_CMV_ADDRESS_CNFG_DATA_INTLVDEPTH_DS = 27,
   DSL_CMV_ADDRESS_CNFG_DATA_RATEDS_FLAG_ADSL2_KP = 28,
   DSL_CMV_ADDRESS_CNFG_DATA_RATEDS_FLAG_ADSL2_MSGLp = 30,
   DSL_CMV_ADDRESS_CNFG_DATA_RATEDS_FLAG_ADSL2_MSGc = 31
} DSL_CmvAddress_t;

/** Defines possible indeces for DSL_CMV_ADDRESS_INFO_PMD */
typedef enum {
   DSL_CMV_PMD_INDEX_NOMPSD_US = 0,
   DSL_CMV_PMD_INDEX_NOMPSD_DS = 1,
   DSL_CMV_PMD_INDEX_PCB_US = 6,
   DSL_CMV_PMD_INDEX_PCB_DS = 7,
   DSL_CMV_PMD_INDEX_RMSGI_US = 10,
   DSL_CMV_PMD_INDEX_RMSGI_DS = 11,
   DSL_CMV_PMD_INDEX_NOMINAL_PSD_US = 14,
   DSL_CMV_PMD_INDEX_NOMINAL_PSD_DS = 15
} DSL_CMV_PMD_Index;

/** Defines possible indeces for DSL_CMV_ADDRESS_INFO_SYSTEM_INTERFACE */
typedef enum {
   DSL_CMV_SYSTEM_INTERFACE_INDEX_CONFIG = 12,
   DSL_CMV_SYSTEM_INTERFACE_INDEX_STATUS = 13
} DSL_CMV_SYSTEM_INTERFACE_Index;

/** Defines all configuration possibilities for the sytem interface.
    \attention Not all of the configurations are supported by the firmware and
    thus also not used within DSL CPE API by now. */
typedef enum {
   DSL_CMV_PTM_HDLC_ENCAPSULATION_US = 0x0001,
   DSL_CMV_PTM_64_65_OCTET_ENCAP_SHORT_US = 0x0008,
   DSL_CMV_PTM_64_65_OCTET_ENCAP_PRE_US = 0x0010,
   DSL_CMV_PTM_64_65_OCTET_ENCAP_SUPPORT_US = 0x0020,
   DSL_CMV_PTM_HDLC_ENCAPSULATION_DS = 0x0040,
   DSL_CMV_PTM_64_65_OCTET_ENCAP_SHORT_DS = 0x0200,
   DSL_CMV_PTM_64_65_OCTET_ENCAP_PRE_DS = 0x0400,
   DSL_CMV_PTM_64_65_OCTET_ENCAP_SUPPORT_DS = 0x0800,
   DSL_CMV_PTM_SELECTED = 0x1000,
   DSL_CMV_ATM_SELECTED = 0x2000,
   DSL_CMV_SEL_TPS_TC = 0x8000
} DSL_CMV_SYSTEM_INTERFACE_Config;


/** Defines possible indeces for DSL_CMV_ADDRESS_INFO_DELT_PREV_STATS_DS and
   DSL_CMV_ADDRESS_INFO_DELT_PREV_STATS_US */
typedef enum {
   DSL_CMV_DELT_PREV_STATS_INDEX_HLIN_SC = 2
} DSL_CMV_DELT_PREV_STATS_t;

/** Defines possible indeces for DSL_CMV_ADDRESS_INFO_DELT_MT_DS and
   DSL_CMV_ADDRESS_INFO_DELT_MT_US */
typedef enum {
   DSL_CMV_DELT_INDEX_MT_HLOG = 0,
   DSL_CMV_DELT_INDEX_MT_QLN = 1,
   DSL_CMV_DELT_INDEX_MT_SNR = 2
} DSL_CMV_DELT_MT_t;

typedef enum {
   DSL_CMV_STAT_FAIL_SUCCESS = 0,
   DSL_CMV_STAT_FAIL_CONFIGURATION = 1,
   DSL_CMV_STAT_FAIL_CONFIGURATION_NOT_FEASIBLE = 2,
   DSL_CMV_STAT_FAIL_COMMUNICATION_PROBLEM = 3,
   DSL_CMV_STAT_FAIL_NO_PEER_DETECTED = 4,
   DSL_CMV_STAT_FAIL_UNKNOWN = 5
} DSL_CmvStatFailReason_t;

typedef enum {
   DSL_CMV_STAT_MODE_SEE_STAT17 = 0,
   DSL_CMV_STAT_MODE_T1_413 = 1,
   DSL_CMV_STAT_MODE_G992_2_A = 2,
   DSL_CMV_STAT_MODE_G992_1_A = 4,
   DSL_CMV_STAT_MODE_G992_1_B = 8,
   DSL_CMV_STAT_MODE_G992_3_A = 0x100,
   DSL_CMV_STAT_MODE_G992_3_B = 0x200,
   DSL_CMV_STAT_MODE_G992_3_I = 0x400,
   DSL_CMV_STAT_MODE_G992_3_J = 0x800,
   DSL_CMV_STAT_MODE_G992_3_L = 0x1000,
   DSL_CMV_STAT_MODE_G992_3_M = 0x2000,
   DSL_CMV_STAT_MODE_G992_5_B = 0x4000,
   DSL_CMV_STAT_MODE_G992_5_A = 0x8000
} DSL_CmvStatMode1_t;

typedef enum {
   DSL_CMV_STAT_MODE_SEE_STAT1 = 0,
   DSL_CMV_STAT_MODE_G992_5_I = 1,
   DSL_CMV_STAT_MODE_G992_5_J = 2,
   DSL_CMV_STAT_MODE_G992_5_M = 4
} DSL_CmvStatMode17_t;

/* argument codes for DSL_CMV_ADDRESS_CNTL_MODEM_CONTROL */
#define DSL_CMV_ADDRESS_ARG_CNTL_MODEM_START             0x2
/** it's not defined now */
/*#define DSL_CMV_ADDRESS_ARG_CNTL_MODEM_STOP              0xx*/

#ifdef __cplusplus
}
#endif

#endif /* _DRV_DSL_CPE_CMV_DANUBE_H */
