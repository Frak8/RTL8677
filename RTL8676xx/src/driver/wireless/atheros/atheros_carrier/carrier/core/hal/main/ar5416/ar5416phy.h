/*
 * Copyright (c) 2002-2005 Sam Leffler, Errno Consulting
 * Copyright (c) 2002-2005 Atheros Communications, Inc.
 * All rights reserved.
 *
 * $Id: //depot/sw/carriersrc/release/1.3.1/src/madwifi/hal/main/ar5416/ar5416phy.h#1 $
 */
#ifndef _DEV_ATH_AR5416PHY_H_
#define _DEV_ATH_AR5416PHY_H_

/* PHY registers */
#define AR_PHY_BASE     0x9800      /* base address of phy regs */
#define AR_PHY(_n)      (AR_PHY_BASE + ((_n)<<2))

#define AR_PHY_TEST             0x9800          /* PHY test control */
#define PHY_AGC_CLR             0x10000000      /* disable AGC to A2 */
#define RFSILENT_BB             0x00002000      /* shush bb */

/* TX99_11N_CHANGE begin */
#define	AR_PHY_TESTCTRL				0x9808		/* PHY Test Control/Status */
#define	AR_PHY_TESTCTRL_TXHOLD		0x3800		/* Select Tx hold */
#define AR_PHY_TESTCTRL_TXSRC_ALT	0x00000080	/* Select input to tsdac along with bit 1 */
#define AR_PHY_TESTCTRL_TXSRC_ALT_S	7
#define AR_PHY_TESTCTRL_TXSRC_SRC	0x00000002	/* Used with bit 7 */
#define AR_PHY_TESTCTRL_TXSRC_SRC_S	1
/* TX99_11N_CHANGE end */


#define AR_PHY_TURBO        0x9804      /* frame control register */
#define AR_PHY_FC_TURBO_MODE        0x00000001  /* Set turbo mode bits */
#define AR_PHY_FC_TURBO_SHORT       0x00000002  /* Set short symbols to turbo mode setting */
#define AR_PHY_FC_DYN2040_EN        0x00000004      /* Enable dyn 20/40 mode */
#define AR_PHY_FC_DYN2040_PRI_ONLY      0x00000008      /* dyn 20/40 - primary only */
#define AR_PHY_FC_DYN2040_PRI_CH    0x00000010      /* dyn 20/40 - primary ch offset (0=+10MHz, 1=-10MHz)*/
#define AR_PHY_FC_DYN2040_EXT_CH        0x00000020      /* dyn 20/40 - ext ch spacing (0=20MHz/ 1=25MHz) */
#define AR_PHY_FC_HT_EN             0x00000040      /* ht enable */
#define AR_PHY_FC_SHORT_GI_40       0x00000080      /* allow short GI for HT 40 */
#define AR_PHY_FC_WALSH             0x00000100      /* walsh spatial spreading for 2 chains,2 streams TX */
#define AR_PHY_FC_SINGLE_HT_LTF1        0x00000200      /* single length (4us) 1st HT long training symbol */

#define AR_PHY_TIMING2      0x9810      /* Timing Control 2 */
#define AR_PHY_TIMING2_USE_FORCE    0x00001000
#define AR_PHY_TIMING2_FORCE_VAL    0x00000fff

#define AR_PHY_TIMING3      0x9814      /* Timing control 3 */
#define AR_PHY_TIMING3_DSC_MAN  0xFFFE0000
#define AR_PHY_TIMING3_DSC_MAN_S    17
#define AR_PHY_TIMING3_DSC_EXP  0x0001E000
#define AR_PHY_TIMING3_DSC_EXP_S    13

#define AR_PHY_CHIP_ID          0x9818      /* PHY chip revision ID */
#define AR_PHY_CHIP_ID_REV_0    	0x80 /* 5416 Rev 0 (owl 1.0) BB */
#define AR_PHY_CHIP_ID_REV_1    	0x81 /* 5416 Rev 1 (owl 2.0) BB */
#define AR_PHY_CHIP_ID_SOWL_REV_0   0xb0 /* 9160 Rev 0 (sowl 1.0) BB */
#ifdef AR9100
#define AR_PHY_CHIP_ID_REV_2        0XA0 /* Howl 1.0 BB */
#endif

#define AR_PHY_ACTIVE       0x981C      /* activation register */
#define AR_PHY_ACTIVE_EN    0x00000001  /* Activate PHY chips */
#define AR_PHY_ACTIVE_DIS   0x00000000  /* Deactivate PHY chips */

#define AR_PHY_RF_CTL2                      0x9824
#define AR_PHY_TX_END_DATA_START  0x000000FF
#define AR_PHY_TX_END_DATA_START_S  0
#define AR_PHY_TX_END_PA_ON       0x0000FF00
#define AR_PHY_TX_END_PA_ON_S       8


#define AR_PHY_RF_CTL3                  0x9828
#define AR_PHY_TX_END_TO_A2_RX_ON       0x00FF0000
#define AR_PHY_TX_END_TO_A2_RX_ON_S     16

#define AR_PHY_ADC_CTL      0x982C
#define AR_PHY_ADC_CTL_OFF_INBUFGAIN    0x00000003
#define AR_PHY_ADC_CTL_OFF_INBUFGAIN_S  0
#define AR_PHY_ADC_CTL_OFF_PWDDAC   0x00002000
#define AR_PHY_ADC_CTL_OFF_PWDBANDGAP   0x00004000 /* BB Rev 4.2+ only */
#define AR_PHY_ADC_CTL_OFF_PWDADC   0x00008000 /* BB Rev 4.2+ only */
#define AR_PHY_ADC_CTL_ON_INBUFGAIN 0x00030000
#define AR_PHY_ADC_CTL_ON_INBUFGAIN_S   16

#define AR_PHY_ADC_SERIAL_CTL       0x9830
#define AR_PHY_SEL_INTERNAL_ADDAC   0x00000000
#define AR_PHY_SEL_EXTERNAL_RADIO   0x00000001

#define AR_PHY_RF_CTL4                    0x9834
#define AR_PHY_RF_CTL4_TX_END_XPAB_OFF    0xFF000000
#define AR_PHY_RF_CTL4_TX_END_XPAB_OFF_S  24
#define AR_PHY_RF_CTL4_TX_END_XPAA_OFF    0x00FF0000
#define AR_PHY_RF_CTL4_TX_END_XPAA_OFF_S  16
#define AR_PHY_RF_CTL4_FRAME_XPAB_ON      0x0000FF00
#define AR_PHY_RF_CTL4_FRAME_XPAB_ON_S    8
#define AR_PHY_RF_CTL4_FRAME_XPAA_ON      0x000000FF
#define AR_PHY_RF_CTL4_FRAME_XPAA_ON_S    0

/* TX99_11N_CHANGE begin */
#define	AR_PHY_BB_XP_PA_CTL	0x9838
#define AR_PHY_BB_XPAA_ACTIVE_HIGH	0x00000001
#define	AR_PHY_BB_XPAB_ACTIVE_HIGH	0x00000002
#define	AR_PHY_BB_XPAB_ACTIVE_HIGH_S	1

#define AR_PHY_TSTDAC_CONST	0x983C
#define AR_PHY_TSTDAC_CONST_Q	0x0003FE00
#define AR_PHY_TSTDAC_CONST_Q_S	9
#define AR_PHY_TSTDAC_CONST_I	0x000001FF
/* TX99_11N_CHANGE end */

#define AR_PHY_SETTLING     0x9844
#define AR_PHY_SETTLING_SWITCH  0x00003F80
#define AR_PHY_SETTLING_SWITCH_S    7

#define AR_PHY_RXGAIN       0x9848
#define AR_PHY_RXGAIN_TXRX_ATTEN    0x0003F000
#define AR_PHY_RXGAIN_TXRX_ATTEN_S  12
#define AR_PHY_RXGAIN_TXRX_RF_MAX   0x007C0000
#define AR_PHY_RXGAIN_TXRX_RF_MAX_S 18
#define AR9280_PHY_RXGAIN_TXRX_ATTEN    0x00003F80
#define AR9280_PHY_RXGAIN_TXRX_ATTEN_S  7
#define AR9280_PHY_RXGAIN_TXRX_MARGIN   0x001FC000
#define AR9280_PHY_RXGAIN_TXRX_MARGIN_S 14

#define AR_PHY_DESIRED_SZ   0x9850
#define AR_PHY_DESIRED_SZ_ADC       0x000000FF
#define AR_PHY_DESIRED_SZ_ADC_S     0
#define AR_PHY_DESIRED_SZ_PGA       0x0000FF00
#define AR_PHY_DESIRED_SZ_PGA_S     8
#define AR_PHY_DESIRED_SZ_TOT_DES   0x0FF00000
#define AR_PHY_DESIRED_SZ_TOT_DES_S 20

#define AR_PHY_FIND_SIG      0x9858
#define AR_PHY_FIND_SIG_FIRSTEP  0x0003F000
#define AR_PHY_FIND_SIG_FIRSTEP_S        12
#define AR_PHY_FIND_SIG_FIRPWR   0x03FC0000
#define AR_PHY_FIND_SIG_FIRPWR_S         18

#define AR_PHY_AGC_CTL1      0x985C
#define AR_PHY_AGC_CTL1_COARSE_LOW       0x00007F80
#define AR_PHY_AGC_CTL1_COARSE_LOW_S         7
#define AR_PHY_AGC_CTL1_COARSE_HIGH      0x003F8000
#define AR_PHY_AGC_CTL1_COARSE_HIGH_S        15

#define AR_PHY_AGC_CONTROL  0x9860      /* chip calibration and noise floor setting */
#define AR_PHY_AGC_CONTROL_CAL  0x00000001  /* do internal calibration */
#define AR_PHY_AGC_CONTROL_NF   0x00000002  /* do noise-floor calculation */
#define AR_PHY_AGC_CONTROL_ENABLE_NF     0x00008000 /* Enable noise floor calibration to happen */
#define AR_PHY_AGC_CONTROL_NO_UPDATE_NF  0x00020000 /* Don't update noise floor automatically */

#define AR_PHY_CCA              0x9864
#define AR_PHY_MINCCA_PWR       0x0FF80000
#define AR_PHY_MINCCA_PWR_S     19
#define AR_PHY_CCA_THRESH62     0x0007F000
#define AR_PHY_CCA_THRESH62_S   12
#define AR9280_PHY_MINCCA_PWR       0x1FF00000
#define AR9280_PHY_MINCCA_PWR_S     20
#define AR9280_PHY_CCA_THRESH62     0x000FF000
#define AR9280_PHY_CCA_THRESH62_S   12

#define AR_PHY_CCA_MAX_GOOD_VALUE      -85
#define AR_PHY_CCA_MAX_HIGH_VALUE      -62
#define AR_PHY_CCA_MIN_BAD_VALUE       -121    

#define AR_PHY_SFCORR_LOW    0x986C
#define AR_PHY_SFCORR_LOW_USE_SELF_CORR_LOW  0x00000001
#define AR_PHY_SFCORR_LOW_M2COUNT_THR_LOW    0x00003F00
#define AR_PHY_SFCORR_LOW_M2COUNT_THR_LOW_S  8
#define AR_PHY_SFCORR_LOW_M1_THRESH_LOW  0x001FC000
#define AR_PHY_SFCORR_LOW_M1_THRESH_LOW_S    14
#define AR_PHY_SFCORR_LOW_M2_THRESH_LOW  0x0FE00000
#define AR_PHY_SFCORR_LOW_M2_THRESH_LOW_S    21

#define AR_PHY_SFCORR       0x9868
#define AR_PHY_SFCORR_M2COUNT_THR    0x0000001F
#define AR_PHY_SFCORR_M2COUNT_THR_S  0
#define AR_PHY_SFCORR_M1_THRESH  0x00FE0000
#define AR_PHY_SFCORR_M1_THRESH_S    17
#define AR_PHY_SFCORR_M2_THRESH  0x7F000000
#define AR_PHY_SFCORR_M2_THRESH_S    24

#define AR_PHY_SLEEP_CTR_CONTROL    0x9870
#define AR_PHY_SLEEP_CTR_LIMIT      0x9874
#define AR_PHY_SYNTH_CONTROL        0x9874
#define AR_PHY_SLEEP_SCAL       0x9878

#define AR_PHY_PLL_CTL          0x987c      /* PLL control register */
#define AR_PHY_PLL_CTL_40       0xaa        /* 40 MHz */
#define AR_PHY_PLL_CTL_40_5413  0x04
#define AR_PHY_PLL_CTL_44       0xab        /* 44 MHz for 11b, 11g */
#define AR_PHY_PLL_CTL_44_2133  0xeb        /* 44 MHz for 11b, 11g */
#define AR_PHY_PLL_CTL_40_2133  0xea        /* 40 MHz for 11a, turbos */

#define AR_PHY_RX_DELAY     0x9914      /* analog pow-on time (100ns) */
#define AR_PHY_RX_DELAY_DELAY   0x00003FFF  /* delay from wakeup to rx ena */

#define AR_PHY_TIMING_CTRL4(_i)     (0x9920 + ((_i) << 12))  /* timing control */
#define AR_PHY_TIMING_CTRL4_IQCORR_Q_Q_COFF 0x01F   /* Mask for kcos_theta-1 for q correction */
#define AR_PHY_TIMING_CTRL4_IQCORR_Q_Q_COFF_S   0   /* shift for Q_COFF */
#define AR_PHY_TIMING_CTRL4_IQCORR_Q_I_COFF 0x7E0   /* Mask for sin_theta for i correction */
#define AR_PHY_TIMING_CTRL4_IQCORR_Q_I_COFF_S   5   /* Shift for sin_theta for i correction */
#define AR_PHY_TIMING_CTRL4_IQCORR_ENABLE   0x800   /* enable IQ correction */
#define AR_PHY_TIMING_CTRL4_IQCAL_LOG_COUNT_MAX 0xF000  /* Mask for max number of samples (logarithmic) */
#define AR_PHY_TIMING_CTRL4_IQCAL_LOG_COUNT_MAX_S   12  /* Shift for max number of samples */
#define AR_PHY_TIMING_CTRL4_DO_IQCAL    0x10000     /* perform IQ calibration */

#define AR_PHY_TIMING_CTRL4_ENABLE_SPUR_RSSI	0x80000000
#define	AR_PHY_TIMING_CTRL4_ENABLE_SPUR_FILTER	0x40000000	/* Enable spur filter */
#define	AR_PHY_TIMING_CTRL4_ENABLE_CHAN_MASK	0x20000000
#define	AR_PHY_TIMING_CTRL4_ENABLE_PILOT_MASK	0x10000000

#define AR_PHY_TIMING5      0x9924
#define AR_PHY_TIMING5_CYCPWR_THR1  0x000000FE
#define AR_PHY_TIMING5_CYCPWR_THR1_S    1

#define AR_PHY_POWER_TX_RATE1   0x9934
#define AR_PHY_POWER_TX_RATE2   0x9938
#define AR_PHY_POWER_TX_RATE_MAX    0x993c
#define AR_PHY_POWER_TX_RATE_MAX_TPC_ENABLE 0x00000040

#define AR_PHY_FRAME_CTL    0x9944
#define AR_PHY_FRAME_CTL_TX_CLIP    0x00000038
#define AR_PHY_FRAME_CTL_TX_CLIP_S  3

#define AR_PHY_TXPWRADJ     0x994C      /* BB Rev 4.2+ only */
#define AR_PHY_TXPWRADJ_CCK_GAIN_DELTA  0x00000FC0
#define AR_PHY_TXPWRADJ_CCK_GAIN_DELTA_S    6
#define AR_PHY_TXPWRADJ_CCK_PCDAC_INDEX 0x00FC0000
#define AR_PHY_TXPWRADJ_CCK_PCDAC_INDEX_S   18

#define AR_PHY_RADAR_EXT      0x9940      /* extension channel radar settings */
#define AR_PHY_RADAR_EXT_ENA  0x00004000  /* Enable extension channel radar detection */

#define AR_PHY_RADAR_0      0x9954      /* radar detection settings */
#define AR_PHY_RADAR_0_ENA  0x00000001  /* Enable radar detection */
#define AR_PHY_RADAR_0_FFT_ENA  0x80000000  /* Enable FFT data */
#define AR_PHY_RADAR_0_INBAND   0x0000003e  /* Inband pulse threshold */
#define AR_PHY_RADAR_0_INBAND_S 1
#define AR_PHY_RADAR_0_PRSSI    0x00000FC0  /* Pulse rssi threshold */
#define AR_PHY_RADAR_0_PRSSI_S  6
#define AR_PHY_RADAR_0_HEIGHT   0x0003F000  /* Pulse height threshold */
#define AR_PHY_RADAR_0_HEIGHT_S 12
#define AR_PHY_RADAR_0_RRSSI    0x00FC0000  /* Radar rssi threshold */
#define AR_PHY_RADAR_0_RRSSI_S  18
#define AR_PHY_RADAR_0_FIRPWR   0x7F000000  /* Radar firpwr threshold */
#define AR_PHY_RADAR_0_FIRPWR_S 24

#define	AR_PHY_RADAR_1      0x9958		/* AR5413+ radar settigns */
#define	AR_PHY_RADAR_1_RELPWR_ENA   0x00800000	/* enable to check radar relative power */
#define	AR_PHY_RADAR_1_USE_FIR128   0x00400000	/* enable to use the average inband power
                                                 * measured over 128 cycles
                                                 */
#define	AR_PHY_RADAR_1_RELPWR_THRESH    0x003F0000	/* relative pwr thresh */
#define	AR_PHY_RADAR_1_RELPWR_THRESH_S	16
#define	AR_PHY_RADAR_1_BLOCK_CHECK	0x00008000	/* Enable to block radar check if weak
                                                 * OFDM sig or pkt is immediately after
                                                 * tx to rx transition
                                                 */
#define	AR_PHY_RADAR_1_MAX_RRSSI    0x00004000	/* Enable to use max rssi */
#define	AR_PHY_RADAR_1_RELSTEP_CHECK    0x00002000	/* Enable to use pulse relative step check */
#define	AR_PHY_RADAR_1_RELSTEP_THRESH   0x00001F00	/* Pulse relative step threshold */
#define AR_PHY_RADAR_1_RELSTEP_THRESH_S	8
#define	AR_PHY_RADAR_1_MAXLEN       0x000000FF	/* Max length of radar pulse */
#define	AR_PHY_RADAR_1_MAXLEN_S 	0

#define AR_PHY_SWITCH_CHAIN_0     0x9960
#define AR_PHY_SWITCH_COM         0x9964

#define AR_PHY_SIGMA_DELTA  0x996C      /* AR5312 only */
#define AR_PHY_SIGMA_DELTA_ADC_SEL  0x00000003
#define AR_PHY_SIGMA_DELTA_ADC_SEL_S    0
#define AR_PHY_SIGMA_DELTA_FILT2    0x000000F8
#define AR_PHY_SIGMA_DELTA_FILT2_S  3
#define AR_PHY_SIGMA_DELTA_FILT1    0x00001F00
#define AR_PHY_SIGMA_DELTA_FILT1_S  8
#define AR_PHY_SIGMA_DELTA_ADC_CLIP 0x01FFE000
#define AR_PHY_SIGMA_DELTA_ADC_CLIP_S   13

#define AR_PHY_RESTART      0x9970      /* restart */
#define AR_PHY_RESTART_DIV_GC   0x001C0000  /* bb_ant_fast_div_gc_limit */
#define AR_PHY_RESTART_DIV_GC_S 18

#define AR_PHY_RFBUS_REQ        0x997C
#define AR_PHY_RFBUS_REQ_EN     0x00000001

#define	AR_PHY_TIMING7		0x9980		/* Spur mitigation masks */
#define	AR_PHY_TIMING8		0x9984
#define	AR_PHY_TIMING8_PILOT_MASK_2	0x000FFFFF
#define	AR_PHY_TIMING8_PILOT_MASK_2_S	0

#define	AR_PHY_BIN_MASK2_1	0x9988
#define	AR_PHY_BIN_MASK2_2	0x998c
#define	AR_PHY_BIN_MASK2_3	0x9990
#define	AR_PHY_BIN_MASK2_4	0x9994

#define	AR_PHY_BIN_MASK_1	0x9900
#define	AR_PHY_BIN_MASK_2	0x9904
#define	AR_PHY_BIN_MASK_3	0x9908

#define	AR_PHY_MASK_CTL		0x990c		/* What are these for?? */

#define	AR_PHY_BIN_MASK2_4_MASK_4	0x00003FFF
#define	AR_PHY_BIN_MASK2_4_MASK_4_S	0

#define	AR_PHY_TIMING9		0x9998
#define	AR_PHY_TIMING10		0x999c
#define	AR_PHY_TIMING10_PILOT_MASK_2	0x000FFFFF
#define	AR_PHY_TIMING10_PILOT_MASK_2_S	0

#define	AR_PHY_TIMING11			0x99a0		/* Spur Mitigation control */
#define	AR_PHY_TIMING11_SPUR_DELTA_PHASE	0x000FFFFF
#define	AR_PHY_TIMING11_SPUR_DELTA_PHASE_S	0
#define	AR_PHY_TIMING11_SPUR_FREQ_SD		0x3FF00000
#define	AR_PHY_TIMING11_SPUR_FREQ_SD_S		20
#define AR_PHY_TIMING11_USE_SPUR_IN_AGC		0x40000000
#define AR_PHY_TIMING11_USE_SPUR_IN_SELFCOR	0x80000000

#define AR_PHY_RX_CHAINMASK     0x99a4
#define AR_PHY_EXT_CCA0         0x99b8
#define AR_PHY_EXT_CCA0_THRESH62    0x000000FF
#define AR_PHY_EXT_CCA0_THRESH62_S  0
#define AR_PHY_EXT_CCA          0x99bc
#define AR_PHY_EXT_CCA_CYCPWR_THR1      0x0000FE00
#define AR_PHY_EXT_CCA_CYCPWR_THR1_S    9
#define AR_PHY_EXT_CCA_THRESH62 0x007F0000
#define AR_PHY_EXT_CCA_THRESH62_S       16
#define AR_PHY_EXT_MINCCA_PWR   0xFF800000
#define AR_PHY_EXT_MINCCA_PWR_S 23
#define AR9280_PHY_EXT_MINCCA_PWR       0x01FF0000
#define AR9280_PHY_EXT_MINCCA_PWR_S     16

#define AR_PHY_SFCORR_EXT       0x99c0
#define AR_PHY_SFCORR_EXT_M1_THRESH       0x0000007F   // [06:00]
#define AR_PHY_SFCORR_EXT_M1_THRESH_S     0
#define AR_PHY_SFCORR_EXT_M2_THRESH       0x00003F80   // [13:07]
#define AR_PHY_SFCORR_EXT_M2_THRESH_S     7
#define AR_PHY_SFCORR_EXT_M1_THRESH_LOW   0x001FC000   // [20:14]
#define AR_PHY_SFCORR_EXT_M1_THRESH_LOW_S 14
#define AR_PHY_SFCORR_EXT_M2_THRESH_LOW   0x0FE00000   // [27:21]
#define AR_PHY_SFCORR_EXT_M2_THRESH_LOW_S 21
#define AR_PHY_SFCORR_SPUR_SUBCHNL_SD_S 28

#define AR_PHY_HALFGI           0x99D0      /* Timing control 3 */
#define AR_PHY_HALFGI_DSC_MAN   0x0007FFF0
#define AR_PHY_HALFGI_DSC_MAN_S 4
#define AR_PHY_HALFGI_DSC_EXP   0x0000000F
#define AR_PHY_HALFGI_DSC_EXP_S 0

#define AR_PHY_HEAVY_CLIP_ENABLE    0x99E0

#define AR_PHY_M_SLEEP      0x99f0      /* sleep control registers */
#define AR_PHY_REFCLKDLY    0x99f4
#define AR_PHY_REFCLKPD     0x99f8

#define AR_PHY_CALMODE      0x99f0
/* PHY IQ calibration results */
#define AR_PHY_IQCAL_RES_PWR_MEAS_I(_i)     (0x9c10 + ((_i) << 12))  /* power measurement for I */
#define AR_PHY_IQCAL_RES_PWR_MEAS_Q(_i)     (0x9c14 + ((_i) << 12))  /* power measurement for Q */
#define AR_PHY_IQCAL_RES_IQ_CORR_MEAS(_i)   (0x9c18 + ((_i) << 12))  /* IQ correlation measurement */

#define AR_PHY_CURRENT_RSSI 0x9c1c      /* rssi of current frame rx'd */
#define AR9280_PHY_CURRENT_RSSI 0x9c3c

#define AR_PHY_RFBUS_GRANT       0x9C20
#define AR_PHY_RFBUS_GRANT_EN    0x00000001

#define AR_PHY_MODE     0xA200  /* Mode register */
#define AR_PHY_MODE_AR2133  0x08    /* AR2133 */
#define AR_PHY_MODE_AR5111  0x00    /* AR5111/AR2111 */
#define AR_PHY_MODE_AR5112  0x08    /* AR5112*/
#define AR_PHY_MODE_DYNAMIC 0x04    /* dynamic CCK/OFDM mode */
#define AR_PHY_MODE_RF2GHZ  0x02    /* 2.4 GHz */
#define AR_PHY_MODE_RF5GHZ  0x00    /* 5 GHz */
#define AR_PHY_MODE_CCK     0x01    /* CCK */
#define AR_PHY_MODE_OFDM    0x00    /* OFDM */

#define AR_PHY_CCK_TX_CTRL  0xA204
#define AR_PHY_CCK_TX_CTRL_JAPAN    0x00000010

#define AR_PHY_CCK_DETECT                           0xA208
#define AR_PHY_CCK_DETECT_WEAK_SIG_THR_CCK          0x0000003F
#define AR_PHY_CCK_DETECT_WEAK_SIG_THR_CCK_S        0
#define AR_PHY_CCK_DETECT_ANT_SWITCH_TIME           0x00001FC0 // [12:6] settling time for antenna switch
#define AR_PHY_CCK_DETECT_ANT_SWITCH_TIME_S         6
#define AR_PHY_CCK_DETECT_BB_ENABLE_ANT_FAST_DIV    0x2000

#define AR_PHY_GAIN_2GHZ    0xA20C
#define AR_PHY_GAIN_2GHZ_RXTX_MARGIN    0x00FC0000
#define AR_PHY_GAIN_2GHZ_RXTX_MARGIN_S  18
#define AR_PHY_GAIN_2GHZ_BSW_MARGIN     0x00003C00
#define AR_PHY_GAIN_2GHZ_BSW_MARGIN_S   10
#define AR_PHY_GAIN_2GHZ_BSW_ATTEN      0x0000001F
#define AR_PHY_GAIN_2GHZ_BSW_ATTEN_S    0

#define AR_PHY_GAIN_2GHZ_XATTEN2_MARGIN     0x003E0000
#define AR_PHY_GAIN_2GHZ_XATTEN2_MARGIN_S   17
#define AR_PHY_GAIN_2GHZ_XATTEN1_MARGIN     0x0001F000
#define AR_PHY_GAIN_2GHZ_XATTEN1_MARGIN_S   12
#define AR_PHY_GAIN_2GHZ_XATTEN2_DB         0x00000FC0
#define AR_PHY_GAIN_2GHZ_XATTEN2_DB_S       6
#define AR_PHY_GAIN_2GHZ_XATTEN1_DB         0x0000003F
#define AR_PHY_GAIN_2GHZ_XATTEN1_DB_S       0

#define AR_PHY_CCK_RXCTRL4  0xA21C
#define AR_PHY_CCK_RXCTRL4_FREQ_EST_SHORT   0x01F80000
#define AR_PHY_CCK_RXCTRL4_FREQ_EST_SHORT_S 19

#define AR_PHY_DAG_CTRLCCK  0xA228
#define AR_PHY_DAG_CTRLCCK_EN_RSSI_THR  0x00000200 /* BB Rev 4.2+ only */
#define AR_PHY_DAG_CTRLCCK_RSSI_THR 0x0001FC00 /* BB Rev 4.2+ only */
#define AR_PHY_DAG_CTRLCCK_RSSI_THR_S   10     /* BB Rev 4.2+ only */

#define AR_PHY_FORCE_CLKEN_CCK              0xA22C
#define AR_PHY_FORCE_CLKEN_CCK_MRC_MUX      0x00000040

#define AR_PHY_POWER_TX_RATE3   0xA234
#define AR_PHY_POWER_TX_RATE4   0xA238

#define AR_PHY_SCRM_SEQ_XR  0xA23C
#define AR_PHY_HEADER_DETECT_XR 0xA240
#define AR_PHY_CHIRP_DETECTED_XR    0xA244
#define AR_PHY_BLUETOOTH    0xA254

#define AR_PHY_TPCRG1   0xA258  /* ar2413 power control */
#define AR_PHY_TPCRG1_NUM_PD_GAIN   0x0000c000
#define AR_PHY_TPCRG1_NUM_PD_GAIN_S 14

#define AR_PHY_TPCRG1_PD_GAIN_1    0x00030000
#define AR_PHY_TPCRG1_PD_GAIN_1_S  16
#define AR_PHY_TPCRG1_PD_GAIN_2    0x000C0000
#define AR_PHY_TPCRG1_PD_GAIN_2_S  18
#define AR_PHY_TPCRG1_PD_GAIN_3    0x00300000
#define AR_PHY_TPCRG1_PD_GAIN_3_S  20


#define AR_PHY_VIT_MASK2_M_46_61 0xa3a0
#define AR_PHY_MASK2_M_31_45     0xa3a4
#define AR_PHY_MASK2_M_16_30     0xa3a8
#define AR_PHY_MASK2_M_00_15     0xa3ac
#define AR_PHY_MASK2_P_15_01     0xa3b8
#define AR_PHY_MASK2_P_30_16     0xa3bc
#define AR_PHY_MASK2_P_45_31     0xa3c0
#define AR_PHY_MASK2_P_61_45     0xa3c4
#define AR_PHY_SPUR_REG          0x994c

/* enable vit puncture per rate, 8 bits, lsb is low rate */
#define AR_PHY_SPUR_REG_MASK_RATE_CNTL       (0xFF << 18)
#define AR_PHY_SPUR_REG_MASK_RATE_CNTL_S     18

#define AR_PHY_SPUR_REG_ENABLE_MASK_PPM      0x20000     /* bins move with freq offset */
#define AR_PHY_SPUR_REG_MASK_RATE_SELECT     (0xFF << 9) /* use mask1 or mask2, one per rate */
#define AR_PHY_SPUR_REG_MASK_RATE_SELECT_S   9
#define AR_PHY_SPUR_REG_ENABLE_VIT_SPUR_RSSI 0x100
#define AR_PHY_SPUR_REG_SPUR_RSSI_THRESH     0x7F
#define AR_PHY_SPUR_REG_SPUR_RSSI_THRESH_S   0

#define AR_PHY_PILOT_MASK_01_30   0xa3b0
#define AR_PHY_PILOT_MASK_31_60   0xa3b4

#define AR_PHY_CHANNEL_MASK_01_30 0x99d4
#define AR_PHY_CHANNEL_MASK_31_60 0x99d8

#define AR_PHY_ANALOG_SWAP      0xa268
#define AR_PHY_SWAP_ALT_CHAIN   0x00000040

#define AR_PHY_TPCRG5   0xA26C /* ar2413 power control */
#define AR_PHY_TPCRG5_PD_GAIN_OVERLAP   0x0000000F
#define AR_PHY_TPCRG5_PD_GAIN_OVERLAP_S     0
#define AR_PHY_TPCRG5_PD_GAIN_BOUNDARY_1    0x000003F0
#define AR_PHY_TPCRG5_PD_GAIN_BOUNDARY_1_S  4
#define AR_PHY_TPCRG5_PD_GAIN_BOUNDARY_2    0x0000FC00
#define AR_PHY_TPCRG5_PD_GAIN_BOUNDARY_2_S  10
#define AR_PHY_TPCRG5_PD_GAIN_BOUNDARY_3    0x003F0000
#define AR_PHY_TPCRG5_PD_GAIN_BOUNDARY_3_S  16
#define AR_PHY_TPCRG5_PD_GAIN_BOUNDARY_4    0x0FC00000
#define AR_PHY_TPCRG5_PD_GAIN_BOUNDARY_4_S  22

#define AR_PHY_POWER_TX_RATE5   0xA38C
#define AR_PHY_POWER_TX_RATE6   0xA390

#define AR_PHY_CAL_CHAINMASK    0xA39C

#define AR_PHY_POWER_TX_SUB     0xA3C8
#define AR_PHY_POWER_TX_RATE7   0xA3CC
#define AR_PHY_POWER_TX_RATE8   0xA3D0
#define AR_PHY_POWER_TX_RATE9   0xA3D4

#define AR_PHY_CH1_CCA          0xa864
#define AR_PHY_CH1_MINCCA_PWR   0x0FF80000
#define AR_PHY_CH1_MINCCA_PWR_S 19
#define AR9280_PHY_CH1_MINCCA_PWR   0x1FF00000
#define AR9280_PHY_CH1_MINCCA_PWR_S 20


#define AR_PHY_CH2_CCA          0xb864
#define AR_PHY_CH2_MINCCA_PWR   0x0FF80000
#define AR_PHY_CH2_MINCCA_PWR_S 19

#define AR_PHY_CH1_EXT_CCA          0xa9bc
#define AR_PHY_CH1_EXT_MINCCA_PWR   0xFF800000
#define AR_PHY_CH1_EXT_MINCCA_PWR_S 23
#define AR9280_PHY_CH1_EXT_MINCCA_PWR   0x01FF0000
#define AR9280_PHY_CH1_EXT_MINCCA_PWR_S 16

#define AR_PHY_CH2_EXT_CCA          0xb9bc
#define AR_PHY_CH2_EXT_MINCCA_PWR   0xFF800000
#define AR_PHY_CH2_EXT_MINCCA_PWR_S 23

#endif  /* _DEV_ATH_AR5416PHY_H_ */
