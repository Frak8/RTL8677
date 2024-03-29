/*
 * Copyright (c) 2002-2005 Sam Leffler, Errno Consulting
 * Copyright (c) 2002-2005 Atheros Communications, Inc.
 * All rights reserved.
 *
 */
#ifndef _ATH_AR5416_EEP_H_
#define _ATH_AR5416_EEP_H_

#define __packed __attribute__((__packed__))

/*
 * Enum to indentify the eeprom mappings 
 */
typedef enum {
	EEP_MAP_DEFAULT = 0x0,
	EEP_MAP_4KBITS
}HAL_5416_EEP_MAP;
/*
** SWAP Functions
** used to read EEPROM data, which is apparently stored in little
** endian form.  We have included both forms of the swap functions,
** one for big endian and one for little endian.  The indices of the
** array elements are the differences
*/
#if AH_BYTE_ORDER == AH_BIG_ENDIAN

#define AR5416_EEPROM_MAGIC         0x5aa5
#define SWAP16(_x) ( (a_uint16_t)( (((const a_uint8_t *)(&_x))[0] ) |\
                     ( ( (const a_uint8_t *)( &_x ) )[1]<< 8) ) )

#define SWAP32(_x) ((a_uint32_t)(                       \
                    (((const a_uint8_t *)(&_x))[0]) |        \
                    (((const a_uint8_t *)(&_x))[1]<< 8) |    \
                    (((const a_uint8_t *)(&_x))[2]<<16) |    \
                    (((const a_uint8_t *)(&_x))[3]<<24)))

#else // AH_BYTE_ORDER

#define AR5416_EEPROM_MAGIC         0xa55a
#define    SWAP16(_x) ( (a_uint16_t)( (((const a_uint8_t *)(&_x))[1] ) |\
                        ( ( (const a_uint8_t *)( &_x ) )[0]<< 8) ) )

#define SWAP32(_x) ((a_uint32_t)(                       \
                    (((const a_uint8_t *)(&_x))[3]) |        \
                    (((const a_uint8_t *)(&_x))[2]<< 8) |    \
                    (((const a_uint8_t *)(&_x))[1]<<16) |    \
                    (((const a_uint8_t *)(&_x))[0]<<24)))

#endif // AH_BYTE_ORDER

#define AR5416_EEPROM_MAGIC_OFFSET  0x0
/* reg_off = 4 * (eep_off) */
#define AR5416_EEPROM_S             2
#define AR5416_EEPROM_OFFSET        0x2000
#ifdef AR9100
#define AR5416_EEPROM_START_ADDR    0x1fff1000
#else
#define AR5416_EEPROM_START_ADDR    0x503f1200
#endif
#define AR5416_EEPROM_MAX           0xae0
#define IS_EEP_MINOR_V3(_ahp) (ar5416EepromGet((_ahp), EEP_MINOR_REV)  >= AR5416_EEP_MINOR_VER_3)

#define owl_get_ntxchains(_txchainmask) \
    (((_txchainmask >> 2) & 1) + ((_txchainmask >> 1) & 1) + (_txchainmask & 1))

/* RF silent fields in \ */
#define EEP_RFSILENT_ENABLED        0x0001  /* bit 0: enabled/disabled */
#define EEP_RFSILENT_ENABLED_S      0       /* bit 0: enabled/disabled */
#define EEP_RFSILENT_POLARITY       0x0002  /* bit 1: polarity */
#define EEP_RFSILENT_POLARITY_S     1       /* bit 1: polarity */
#define EEP_RFSILENT_GPIO_SEL       0x001c  /* bits 2..4: gpio PIN */
#define EEP_RFSILENT_GPIO_SEL_S     2       /* bits 2..4: gpio PIN */

#define AR5416_EEP_NO_BACK_VER       0x1
#define AR5416_EEP_VER               0xE
#define AR5416_EEP_VER_MINOR_MASK    0x0FFF
#define AR5416_EEP_MINOR_VER_2       0x2
#define AR5416_EEP_MINOR_VER_3       0x3
#define AR5416_EEP_MINOR_VER_7       0x7
#define AR5416_EEP_MINOR_VER_9       0x9

#define AR5416_EEPROM_MODAL_SPURS    5
#define AR5416_MAX_RATE_POWER        63
#define AR5416_NUM_PDADC_VALUES      128
#define AR5416_NUM_RATES             16
#define AR5416_BCHAN_UNUSED          0xFF
#define AR5416_MAX_PWR_RANGE_IN_HALF_DB 64
#define AR5416_OPFLAGS_11A           0x01
#define AR5416_OPFLAGS_11G           0x02
#define AR5416_OPFLAGS_5G_HT40       0x04
#define AR5416_OPFLAGS_2G_HT40       0x08
#define AR5416_OPFLAGS_5G_HT20       0x10
#define AR5416_OPFLAGS_2G_HT20       0x20
#define AR5416_EEPMISC_BIG_ENDIAN    0x01
#define FREQ2FBIN(x,y) ((y) ? ((x) - 2300) : (((x) - 4800) / 5))
#define AR5416_ANT_16S               25
/* Delta from which to start power to pdadc table */
#define AR5416_PWR_TABLE_OFFSET  -5
#define AR5416_LEGACY_CHAINMASK     1

#ifdef WIN32
#pragma pack(push, ar5416, 1)
#endif

typedef enum {
    EEP_NFTHRESH_5,
    EEP_NFTHRESH_2,
    EEP_MAC_MSW,
    EEP_MAC_MID,
    EEP_MAC_LSW,
    EEP_REG_0,
    EEP_REG_1,
    EEP_OP_CAP,
    EEP_OP_MODE,
    EEP_RF_SILENT,
    EEP_OB_5,
    EEP_DB_5,
    EEP_OB_2,
    EEP_DB_2,
    EEP_MINOR_REV,
    EEP_TX_MASK,
    EEP_RX_MASK,
#ifdef MAGPIE_MERLIN
    EEP_FSTCLK_5G,
    EEP_RXGAIN_TYPE,
#endif
} EEPROM_PARAM;

typedef enum Ar5416_Rates {
    rate6mb,  rate9mb,  rate12mb, rate18mb,
    rate24mb, rate36mb, rate48mb, rate54mb,
    rate1l,   rate2l,   rate2s,   rate5_5l,
    rate5_5s, rate11l,  rate11s,  rateXr,
    rateHt20_0, rateHt20_1, rateHt20_2, rateHt20_3,
    rateHt20_4, rateHt20_5, rateHt20_6, rateHt20_7,
    rateHt40_0, rateHt40_1, rateHt40_2, rateHt40_3,
    rateHt40_4, rateHt40_5, rateHt40_6, rateHt40_7,
    rateDupCck, rateDupOfdm, rateExtCck, rateExtOfdm,
    Ar5416RateSize
} AR5416_RATES;

#include <adf_os_prepack.h>
typedef adf_os_packed_pre struct spurChanStruct {
    a_uint16_t spurChan;
    a_uint8_t  spurRangeLow;
    a_uint8_t  spurRangeHigh;
} adf_os_packed_post SPUR_CHAN;


typedef adf_os_packed_pre struct CalTargetPowerLegacy {
    a_uint8_t  bChannel;
    a_uint8_t  tPow2x[4];
} adf_os_packed_post CAL_TARGET_POWER_LEG;

typedef adf_os_packed_pre struct CalTargetPowerHt {
    a_uint8_t  bChannel;
    a_uint8_t  tPow2x[8];
} adf_os_packed_post CAL_TARGET_POWER_HT;

#if AH_BYTE_ORDER == AH_BIG_ENDIAN
typedef adf_os_packed_pre struct CalCtlEdges {
    a_uint8_t  bChannel;
    a_uint8_t  flag   :2,
             tPower :6;
} adf_os_packed_post CAL_CTL_EDGES;
#else
typedef adf_os_packed_pre struct CalCtlEdges {
    a_uint8_t  bChannel;
    a_uint8_t  tPower :6,
             flag   :2;
} adf_os_packed_post CAL_CTL_EDGES;
#endif

/* EEPROM Default bit map definations */
#define owl_get_eepdef_ver(_ahp)   \
    (((_ahp)->ah_eeprom.map.def.baseEepHeader.version >> 12) & 0xF)
#define owl_get_eepdef_rev(_ahp)   \
    (((_ahp)->ah_eeprom.map.def.baseEepHeader.version) & 0xFFF)

// 16-bit offset location start of calibration struct
#define AR5416_EEPDEF_START_LOC         256
#define AR5416_EEPDEF_NUM_5G_CAL_PIERS      8
#define AR5416_EEPDEF_NUM_2G_CAL_PIERS      4
#define AR5416_EEPDEF_NUM_5G_20_TARGET_POWERS  8
#define AR5416_EEPDEF_NUM_5G_40_TARGET_POWERS  8
#define AR5416_EEPDEF_NUM_2G_CCK_TARGET_POWERS 3
#define AR5416_EEPDEF_NUM_2G_20_TARGET_POWERS  4
#define AR5416_EEPDEF_NUM_2G_40_TARGET_POWERS  4
#define AR5416_EEPDEF_NUM_CTLS              24
#define AR5416_EEPDEF_NUM_BAND_EDGES        8
#define AR5416_EEPDEF_NUM_PD_GAINS          4
#define AR5416_EEPDEF_PD_GAINS_IN_MASK      4
#define AR5416_EEPDEF_PD_GAIN_ICEPTS        5
#define AR5416_EEPDEF_MAX_CHAINS            3

#define AR5416_EEPDEF_NUM_ANT_CHAIN_FIELDS     7
#define AR5416_EEPDEF_NUM_ANT_COMMON_FIELDS    4
#define AR5416_EEPDEF_SIZE_ANT_CHAIN_FIELD     3
#define AR5416_EEPDEF_SIZE_ANT_COMMON_FIELD    4
#define AR5416_EEPDEF_ANT_CHAIN_MASK           0x7
#define AR5416_EEPDEF_ANT_COMMON_MASK          0xf
#define AR5416_EEPDEF_CHAIN_0_IDX              0
#define AR5416_EEPDEF_CHAIN_1_IDX              1
#define AR5416_EEPDEF_CHAIN_2_IDX              2

typedef adf_os_packed_pre struct BaseEepDefHeader {
    a_uint16_t  length;
    a_uint16_t  checksum;
    a_uint16_t  version;
    a_uint8_t   opCapFlags;
    a_uint8_t   eepMisc;
    a_uint16_t  regDmn[2];
    a_uint8_t   macAddr[6];
    a_uint8_t   rxMask;
    a_uint8_t   txMask;
    a_uint16_t  rfSilent;
    a_uint16_t  blueToothOptions;
    a_uint16_t  deviceCap;
    a_uint32_t  binBuildNumber;
    a_uint8_t   deviceType;
#if defined(MAGPIE_MERLIN) || defined(K2)
    a_uint8_t   pwdclkind;
    a_uint8_t   fastClk5g;
    a_uint8_t   divChain;  
    a_uint8_t   rxGainType;
    a_uint8_t   dacHiPwrMode;    // #bits used: 1, bit0: 1: use the DAC high power mode, 0: don't use the DAC high power mode (1 for MB91 only)
    a_uint8_t   openLoopPwrCntl; // #bits used: 1, bit0: 1: use :open loop power control, 0: use closed loop power control
    a_uint8_t   dacLpMode;       // #bits used: bit0.  
    a_uint8_t   txGainType;      // #bits used: all, indicates high power tx gain table support. 0: original 1: high power
    a_uint8_t   rcChainMask;     // bits used: bit0, This flag will be "1" if the card is an HB93 1x2, otherwise, it will be "0".
    a_uint8_t   futureBase[24];
#else
     a_uint8_t   futureBase[33];
#endif
} adf_os_packed_post BASE_EEPDEF_HEADER; // 64 B

typedef adf_os_packed_pre struct ModalEepDefHeader {
    a_uint32_t  antCtrlChain[AR5416_EEPDEF_MAX_CHAINS];       // 12
    a_uint32_t  antCtrlCommon;                         // 4
    a_uint8_t   antennaGainCh[AR5416_EEPDEF_MAX_CHAINS];      // 3
    a_uint8_t   switchSettling;                        // 1
    a_uint8_t   txRxAttenCh[AR5416_EEPDEF_MAX_CHAINS];        // 3
    a_uint8_t   rxTxMarginCh[AR5416_EEPDEF_MAX_CHAINS];       // 3
    a_uint8_t   adcDesiredSize;                        // 1
    a_uint8_t   pgaDesiredSize;                        // 1
    a_uint8_t   xlnaGainCh[AR5416_EEPDEF_MAX_CHAINS];         // 3
    a_uint8_t   txEndToXpaOff;                         // 1
    a_uint8_t   txEndToRxOn;                           // 1
    a_uint8_t   txFrameToXpaOn;                        // 1
    a_uint8_t   thresh62;                              // 1
    a_uint8_t   noiseFloorThreshCh[AR5416_EEPDEF_MAX_CHAINS]; // 3
    a_uint8_t   xpdGain;                               // 1
    a_uint8_t   xpd;                                   // 1
    a_uint8_t   iqCalICh[AR5416_EEPDEF_MAX_CHAINS];           // 1
    a_uint8_t   iqCalQCh[AR5416_EEPDEF_MAX_CHAINS];           // 1
    a_uint8_t   pdGainOverlap;                         // 1
    a_uint8_t   ob;                                    // 1 -> For AR9280, this is Chain 0
    a_uint8_t   db;                                    // 1 -> For AR9280, this is Chain 0
    a_uint8_t   xpaBiasLvl;                            // 1
    a_uint8_t   pwrDecreaseFor2Chain;                  // 1
    a_uint8_t   pwrDecreaseFor3Chain;                  // 1 -> 48 B
    a_uint8_t   txFrameToDataStart;                    // 1
    a_uint8_t   txFrameToPaOn;                         // 1
    a_uint8_t   ht40PowerIncForPdadc;                  // 1
    a_uint8_t   bswAtten[AR5416_EEPDEF_MAX_CHAINS];           // 3
    a_uint8_t   bswMargin[AR5416_EEPDEF_MAX_CHAINS];          // 3
    a_uint8_t   swSettleHt40;                          // 1
#if defined(MAGPIE_MERLIN) || defined(K2)
    a_uint8_t   xatten2Db[AR5416_EEPDEF_MAX_CHAINS];          // 3 -> New for AR9280 (0xa20c/b20c 11:6)
    a_uint8_t   xatten2Margin[AR5416_EEPDEF_MAX_CHAINS];      // 3 -> New for AR9280 (0xa20c/b20c 21:17)
    a_uint8_t   ob_ch1;                                // 1 -> ob and db become chain specific from AR9280
    a_uint8_t   db_ch1;                                // 1
    a_uint8_t   lna_cntl_sp          : 1,              // 1 Spare bit
               force_xpaon          : 1,              // Force XPA bit for 5G mode
               local_bias           : 1,              // enable local bias
               femBandSelectUsed    : 1,              //
               xlnabufin            : 1,              //
               xlnaisel             : 2,              //
               xlnabufmode          : 1;              //
    a_uint8_t   futureModalMerlin;                     // 1
    a_uint16_t  xpaBiasLvlFreq[3];                     // 3
    a_uint8_t   futureModal[6];                        // 6 , old value:22
#else
    a_uint8_t   futureModal[22];                       // 22
#endif

    SPUR_CHAN spurChans[AR5416_EEPROM_MODAL_SPURS];   // 20 B
} adf_os_packed_post MODAL_EEPDEF_HEADER;                          // == 100 B

typedef adf_os_packed_pre struct CalCtlDataEepDef {
    CAL_CTL_EDGES  ctlEdges[AR5416_EEPDEF_MAX_CHAINS][AR5416_EEPDEF_NUM_BAND_EDGES];
} adf_os_packed_post CAL_CTL_DATA_EEPDEF;

typedef adf_os_packed_pre struct calDataPerFreqEepDef {
    a_uint8_t pwrPdg[AR5416_EEPDEF_NUM_PD_GAINS][AR5416_EEPDEF_PD_GAIN_ICEPTS];
    a_uint8_t vpdPdg[AR5416_EEPDEF_NUM_PD_GAINS][AR5416_EEPDEF_PD_GAIN_ICEPTS];
} adf_os_packed_post CAL_DATA_PER_FREQ_EEPDEF;


typedef adf_os_packed_pre struct eepMapDef {
    BASE_EEPDEF_HEADER      baseEepHeader;         // 64 B
    a_uint8_t             custData[64];                   // 64 B
    MODAL_EEPDEF_HEADER     modalHeader[2];        // 200 B
    a_uint8_t             calFreqPier5G[AR5416_EEPDEF_NUM_5G_CAL_PIERS];
    a_uint8_t             calFreqPier2G[AR5416_EEPDEF_NUM_2G_CAL_PIERS];
    CAL_DATA_PER_FREQ_EEPDEF    calPierData5G[AR5416_EEPDEF_MAX_CHAINS][AR5416_EEPDEF_NUM_5G_CAL_PIERS];
    CAL_DATA_PER_FREQ_EEPDEF    calPierData2G[AR5416_EEPDEF_MAX_CHAINS][AR5416_EEPDEF_NUM_2G_CAL_PIERS];
    CAL_TARGET_POWER_LEG calTargetPower5G[AR5416_EEPDEF_NUM_5G_20_TARGET_POWERS];
    CAL_TARGET_POWER_HT  calTargetPower5GHT20[AR5416_EEPDEF_NUM_5G_20_TARGET_POWERS];
    CAL_TARGET_POWER_HT  calTargetPower5GHT40[AR5416_EEPDEF_NUM_5G_40_TARGET_POWERS];
    CAL_TARGET_POWER_LEG calTargetPowerCck[AR5416_EEPDEF_NUM_2G_CCK_TARGET_POWERS];
    CAL_TARGET_POWER_LEG calTargetPower2G[AR5416_EEPDEF_NUM_2G_20_TARGET_POWERS];
    CAL_TARGET_POWER_HT  calTargetPower2GHT20[AR5416_EEPDEF_NUM_2G_20_TARGET_POWERS];
    CAL_TARGET_POWER_HT  calTargetPower2GHT40[AR5416_EEPDEF_NUM_2G_40_TARGET_POWERS];
    a_uint8_t             ctlIndex[AR5416_EEPDEF_NUM_CTLS];
    CAL_CTL_DATA_EEPDEF         ctlData[AR5416_EEPDEF_NUM_CTLS];
    a_uint8_t             padding;
} adf_os_packed_post ar5416_eeprom_def_t;  // EEPDEF_MAP


/* EEPROM 4K bit map definations */
#define owl_get_eep4k_ver(_ahp)   \
    (((_ahp)->ah_eeprom.map.map4k.baseEepHeader.version >> 12) & 0xF)
#define owl_get_eep4k_rev(_ahp)   \
    (((_ahp)->ah_eeprom.map.map4k.baseEepHeader.version) & 0xFFF)

// 16-bit offset location start of calibration struct
#define AR5416_EEP4K_START_LOC         64
#define AR5416_EEP4K_NUM_2G_CAL_PIERS      3
#define AR5416_EEP4K_NUM_2G_CCK_TARGET_POWERS 3
#define AR5416_EEP4K_NUM_2G_20_TARGET_POWERS  3
#define AR5416_EEP4K_NUM_2G_40_TARGET_POWERS  3
#define AR5416_EEP4K_NUM_CTLS              12
#define AR5416_EEP4K_NUM_BAND_EDGES        4
#define AR5416_EEP4K_NUM_PD_GAINS          2
#define AR5416_EEP4K_PD_GAINS_IN_MASK      4
#define AR5416_EEP4K_PD_GAIN_ICEPTS        5
#define AR5416_EEP4K_MAX_CHAINS            1

typedef adf_os_packed_pre struct BaseEep4kHeader {
    a_uint16_t  length;
    a_uint16_t  checksum;
    a_uint16_t  version;
    a_uint8_t   opCapFlags;
    a_uint8_t   eepMisc;
    a_uint16_t  regDmn[2];
    a_uint8_t   macAddr[6];
    a_uint8_t   rxMask;
    a_uint8_t   txMask;
    a_uint16_t  rfSilent;
    a_uint16_t  blueToothOptions;
    a_uint16_t  deviceCap;
    a_uint32_t  binBuildNumber;
    a_uint8_t   deviceType;
    a_uint8_t   futureBase[1];
} adf_os_packed_post BASE_EEP4K_HEADER; // 32 B


typedef adf_os_packed_pre struct ModalEep4kHeader {
    a_uint32_t  antCtrlChain[AR5416_EEP4K_MAX_CHAINS];       // 4
    a_uint32_t  antCtrlCommon;                         // 4
    a_uint8_t   antennaGainCh[AR5416_EEP4K_MAX_CHAINS];      // 1
    a_uint8_t   switchSettling;                        // 1
    a_uint8_t   txRxAttenCh[AR5416_EEP4K_MAX_CHAINS];        // 1
    a_uint8_t   rxTxMarginCh[AR5416_EEP4K_MAX_CHAINS];       // 1
    a_uint8_t   adcDesiredSize;                        // 1
    a_uint8_t   pgaDesiredSize;                        // 1
    a_uint8_t   xlnaGainCh[AR5416_EEP4K_MAX_CHAINS];         // 1
    a_uint8_t   txEndToXpaOff;                         // 1
    a_uint8_t   txEndToRxOn;                           // 1
    a_uint8_t   txFrameToXpaOn;                        // 1
    a_uint8_t   thresh62;                              // 1
    a_uint8_t   noiseFloorThreshCh[AR5416_EEP4K_MAX_CHAINS]; // 1
    a_uint8_t   xpdGain;                               // 1
    a_uint8_t   xpd;                                   // 1
    a_uint8_t   iqCalICh[AR5416_EEP4K_MAX_CHAINS];           // 1
    a_uint8_t   iqCalQCh[AR5416_EEP4K_MAX_CHAINS];           // 1
    a_uint8_t   pdGainOverlap;                         // 1
    a_uint8_t   ob;                                    // 1 -> For AR9285, this is Chain 0
    a_uint8_t   db;                                    // 1 -> For AR9285, this is Chain 0
    a_uint8_t   xpaBiasLvl;                            // 1
    a_uint8_t   txFrameToDataStart;                    // 1
    a_uint8_t   txFrameToPaOn;                         // 1
    a_uint8_t   ht40PowerIncForPdadc;                  // 1
    a_uint8_t   bswAtten[AR5416_EEP4K_MAX_CHAINS];           // 1
    a_uint8_t   bswMargin[AR5416_EEP4K_MAX_CHAINS];          // 1
    a_uint8_t   swSettleHt40;                          // 1
    a_uint8_t   xatten2Db[AR5416_EEP4K_MAX_CHAINS];          // 1
    a_uint8_t   xatten2Margin[AR5416_EEP4K_MAX_CHAINS];      // 1
#ifdef K2
    a_uint8_t   db2;                                   // 1
    a_uint8_t   version;                               // 1
    a_uint8_t   ob_23;                                 // 1
    a_uint8_t   ob_4;                                  // 1
    a_uint8_t   db_23;                                 // 1
    a_uint8_t   db_4;                                  // 1
    a_uint8_t   db2_23;                                // 1
    a_uint8_t   db2_4;                                 // 1
    a_uint8_t   futureModal[4];                        // 4
#else
    a_uint8_t   futureModal[12];                        // 12
#endif
    SPUR_CHAN spurChans[AR5416_EEPROM_MODAL_SPURS];   // 20 B
} adf_os_packed_post MODAL_EEP4K_HEADER;                          // == 68 B

typedef adf_os_packed_pre struct CalCtlDataEep4k {
    CAL_CTL_EDGES  ctlEdges[AR5416_EEP4K_MAX_CHAINS][AR5416_EEP4K_NUM_BAND_EDGES];
} adf_os_packed_post CAL_CTL_DATA_EEP4K;

typedef adf_os_packed_pre struct calDataPerFreqEep4k {
    a_uint8_t pwrPdg[AR5416_EEP4K_NUM_PD_GAINS][AR5416_EEP4K_PD_GAIN_ICEPTS];
    a_uint8_t vpdPdg[AR5416_EEP4K_NUM_PD_GAINS][AR5416_EEP4K_PD_GAIN_ICEPTS];
} adf_os_packed_post CAL_DATA_PER_FREQ_EEP4K;

typedef adf_os_packed_pre struct eepMap4k {
    BASE_EEP4K_HEADER    baseEepHeader;         // 32 B
    a_uint8_t             custData[20];          // 20 B
    MODAL_EEP4K_HEADER   modalHeader;        // 68 B
    a_uint8_t             calFreqPier2G[AR5416_EEP4K_NUM_2G_CAL_PIERS];
    CAL_DATA_PER_FREQ_EEP4K    calPierData2G[AR5416_EEP4K_MAX_CHAINS][AR5416_EEP4K_NUM_2G_CAL_PIERS];
    CAL_TARGET_POWER_LEG calTargetPowerCck[AR5416_EEP4K_NUM_2G_CCK_TARGET_POWERS];
    CAL_TARGET_POWER_LEG calTargetPower2G[AR5416_EEP4K_NUM_2G_20_TARGET_POWERS];
    CAL_TARGET_POWER_HT  calTargetPower2GHT20[AR5416_EEP4K_NUM_2G_20_TARGET_POWERS];
    CAL_TARGET_POWER_HT  calTargetPower2GHT40[AR5416_EEP4K_NUM_2G_40_TARGET_POWERS];
    a_uint8_t             ctlIndex[AR5416_EEP4K_NUM_CTLS];
    CAL_CTL_DATA_EEP4K   ctlData[AR5416_EEP4K_NUM_CTLS];
    a_uint8_t             padding;
} adf_os_packed_post ar5416_eeprom_4k_t;  // EEP4K_MAP


typedef adf_os_packed_pre struct eepMap {
	union {
		ar5416_eeprom_def_t def;
		ar5416_eeprom_4k_t map4k;
	}map;
} adf_os_packed_post ar5416_eeprom_t;  // EEP_MAP
#include <adf_os_postpack.h>
#ifdef WIN32
#pragma pack(pop, ar5416)
#endif

//#define EEPROM_DUMP 1   // TODO: Remove by FCS

/*
 *  Control inclusion of Target Power Override (<= 14.4)
 *  define any of the following 7 lines to enable the respective mode's override.
 */
#if 0
#if AH_AR5416_OVRD_TGT_PWR_5G == 1 || \
    AH_AR5416_OVRD_TGT_PWR_5GHT20 == 1 || \
    AH_AR5416_OVRD_TGT_PWR_5GHT40 == 1 || \
    AH_AR5416_OVRD_TGT_PWR_CCK == 1 || \
    AH_AR5416_OVRD_TGT_PWR_2G == 1 || \
    AH_AR5416_OVRD_TGT_PWR_2GHT20 == 1 || \
    AH_AR5416_OVRD_TGT_PWR_2GHT40 == 1
#define AH_AR5416_OVRD_TGT_PWR  /* Control general feature to override target powers */
#endif
#endif



/**************************************************************************
 * interpolate
 *
 * Returns signed interpolated or the scaled up interpolated value
 */
static adf_os_inline a_int16_t
interpolate(a_uint16_t target, a_uint16_t srcLeft, a_uint16_t srcRight,
    a_int16_t targetLeft, a_int16_t targetRight)
{
    a_int16_t rv;

    if (srcRight == srcLeft)
    {
        rv = targetLeft;
    }
    else
    {
        rv = (a_int16_t)( ((target - srcLeft) * targetRight +
            (srcRight - target) * targetLeft) / (srcRight - srcLeft) );
    }
    return rv;
}

/**************************************************************************
 * fbin2freq
 *
 * Get channel value from binary representation held in eeprom
 * RETURNS: the frequency in MHz
 */
static adf_os_inline a_uint16_t
fbin2freq(a_uint8_t fbin, HAL_BOOL is2GHz)
{
    /*
    * Reserved value 0xFF provides an empty definition both as
    * an fbin and as a frequency - do not convert
    */
    if (fbin == AR5416_BCHAN_UNUSED)
    {
        return fbin;
    }

    return (a_uint16_t)((is2GHz) ? (2300 + fbin) : (4800 + 5 * fbin));
}

/**************************************************************
 * getLowerUppderIndex
 *
 * Return indices surrounding the value in sorted integer lists.
 * Requirement: the input list must be monotonically increasing
 *     and populated up to the list size
 * Returns: match is set if an index in the array matches exactly
 *     or a the target is before or after the range of the array.
 */
static adf_os_inline HAL_BOOL
getLowerUpperIndex(a_uint8_t target, a_uint8_t *pList, a_uint16_t listSize,
    a_uint16_t *indexL, a_uint16_t *indexR)
{
    a_uint16_t i;

    /*
    * Check first and last elements for beyond ordered array cases.
    */
    if (target <= pList[0])
    {
        *indexL = *indexR = 0;
        return AH_TRUE;
    }
    if (target >= pList[listSize-1])
    {
        *indexL = *indexR = (a_uint16_t)(listSize - 1);
        return AH_TRUE;
    }

    /* look for value being near or between 2 values in list */
    for (i = 0; i < listSize - 1; i++)
    {
        /*
        * If value is close to the current value of the list
        * then target is not between values, it is one of the values
        */
        if (pList[i] == target)
        {
            *indexL = *indexR = i;
            return AH_TRUE;
        }
        /*
        * Look for value being between current value and next value
        * if so return these 2 values
        */
        if (target < pList[i + 1])
        {
            *indexL = i;
            *indexR = (a_uint16_t)(i + 1);
            return AH_FALSE;
        }
    }
    HALASSERT(0);
    return AH_FALSE;
}

/**************************************************************
 * ar5416FillVpdTable
 *
 * Fill the Vpdlist for indices Pmax-Pmin
 * Note: pwrMin, pwrMax and Vpdlist are all in dBm * 4
 */
static adf_os_inline HAL_BOOL
ar5416FillVpdTable(a_uint8_t pwrMin, a_uint8_t pwrMax, a_uint8_t *pPwrList,
    a_uint8_t *pVpdList, a_uint16_t numIntercepts, a_uint8_t *pRetVpdList)
{
    a_uint16_t  i, k;
    a_uint8_t   currPwr = pwrMin;
    a_uint16_t  idxL = 0, idxR = 0;

    HALASSERT(pwrMax > pwrMin);
    for (i = 0; i <= (pwrMax - pwrMin) / 2; i++)
    {
        getLowerUpperIndex(currPwr, pPwrList, numIntercepts,
            &(idxL), &(idxR));
        if (idxR < 1)
            idxR = 1;           /* extrapolate below */
        if (idxL == numIntercepts - 1)
            idxL = (a_uint16_t)(numIntercepts - 2);   /* extrapolate above */
        if (pPwrList[idxL] == pPwrList[idxR])
            k = pVpdList[idxL];
        else
            k = (a_uint16_t)( ((currPwr - pPwrList[idxL]) * pVpdList[idxR] + (pPwrList[idxR] - currPwr) * pVpdList[idxL]) /
                (pPwrList[idxR] - pPwrList[idxL]) );
        HALASSERT(k < 256);
        pRetVpdList[i] = (a_uint8_t)k;
        currPwr += 2;               /* half dB steps */
    }

    return AH_TRUE;
}

/******************************************************************************/
/*!
**  \brief Analog Shift Register Read-Modify-Write
**
**  This routine will either perform the standard RMW procedure, or apply the
**  Merlin specific write process depending on the setting of the configuration
**  parameter.  For AP it's on by default, for Station it's controlled by
**  registry parameters.
**
**  \param ah   HAL instance variable
**  \param reg  register offset to program
**  \param val  value to insert into the register.
**  \return N/A
*/

static adf_os_inline void
analagShiftRegRMW(struct ath_hal *ah, a_uint32_t reg, a_uint32_t mask, a_uint32_t shift, a_uint32_t val)
{
    a_uint32_t               regVal;
    struct ath_hal_private  *ap = AH_PRIVATE(ah);
    
    /*
    ** Code Begins
    ** Get the register value, and mask the appropriate bits
    */

     regVal = OS_REG_READ(ah,reg) & ~mask;
     
     /*
     ** Insert the value into the appropriate slot
     */
     
     regVal |= (val << shift) & mask;
     
     /*
     ** Determine if we use standard write or the "magic" write
     */

     OS_REG_WRITE(ah,reg,regVal);
     
     if (ap->ah_config.ath_hal_analogShiftRegWAR) {
        OS_DELAY(100);
     }

    return;
}
#endif  /* _ATH_AR5416_EEP_H_ */
