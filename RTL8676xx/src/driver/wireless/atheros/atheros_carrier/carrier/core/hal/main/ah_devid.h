/*-
 * Copyright (c) 2002-2005 Sam Leffler, Errno Consulting, Atheros
 * Communications, Inc.  All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that the following conditions are met:
 * 1. The materials contained herein are unmodified and are used
 *    unmodified.
 * 2. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following NO
 *    ''WARRANTY'' disclaimer below (''Disclaimer''), without
 *    modification.
 * 3. Redistributions in binary form must reproduce at minimum a
 *    disclaimer similar to the Disclaimer below and any redistribution
 *    must be conditioned upon including a substantially similar
 *    Disclaimer requirement for further binary redistribution.
 * 4. Neither the names of the above-listed copyright holders nor the
 *    names of any contributors may be used to endorse or promote
 *    product derived from this software without specific prior written
 *    permission.
 *
 * NO WARRANTY
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ''AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF NONINFRINGEMENT,
 * MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE
 * FOR SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGES.
 *
 * $Id: //depot/sw/carriersrc/release/1.3.1/src/madwifi/hal/main/ah_devid.h#2 $
 */

#ifndef _DEV_ATH_DEVID_H_
#define _DEV_ATH_DEVID_H_

#define ATHEROS_VENDOR_ID   0x168c      /* Atheros PCI vendor ID */
/*
 * NB: all Atheros-based devices should have a PCI vendor ID
 *     of 0x168c, but some vendors, in their infinite wisdom
 *     do not follow this so we must handle them specially.
 */
#define ATHEROS_3COM_VENDOR_ID  0xa727      /* 3Com 3CRPAG175 vendor ID */
#define ATHEROS_3COM2_VENDOR_ID 0x10b7      /* 3Com 3CRDAG675 vendor ID */

/* AR5210 (for reference) */
#define AR5210_DEFAULT          0x1107          /* No eeprom HW default */
#define AR5210_PROD             0x0007          /* Final device ID */
#define AR5210_AP               0x0207          /* Early AP11s */

/* AR5211 */
#define AR5211_DEFAULT          0x1112          /* No eeprom HW default */
#define AR5311_DEVID            0x0011          /* Final ar5311 devid */
#define AR5211_DEVID            0x0012          /* Final ar5211 devid */
#define AR5211_LEGACY           0xff12          /* Original emulation board */
#define AR5211_FPGA11B          0xf11b          /* 11b emulation board */

/* AR5212 */
#define AR5212_DEFAULT          0x1113          /* No eeprom HW default */
#define AR5212_DEVID            0x0013          /* Final ar5212 devid */
#define AR5212_FPGA             0xf013          /* Emulation board */
#define AR5212_DEVID_IBM        0x1014          /* IBM minipci ID */
#define AR5212_AR5312_REV2      0x0052          /* AR5312 WMAC (AP31) */
#define AR5212_AR5312_REV7      0x0057          /* AR5312 WMAC (AP30-040) */
#define AR5212_AR2313_REV8      0x0058          /* AR2313 WMAC (AP43-030) */
#define AR5212_AR2315_REV6      0x0086          /* AR2315 WMAC (AP51-Lite) */
#define AR5212_AR2315_REV7      0x0087          /* AR2315 WMAC (AP51-Full) */
#define AR5212_AR2317_REV1      0x0090          /* AR2317 WMAC (AP61-Lite) */
#define AR5212_AR2317_REV2      0x0091          /* AR2317 WMAC (AP61-Full) */

/* AR5212 compatible devid's also attach to 5212 */
#define AR5212_DEVID_0014   0x0014
#define AR5212_DEVID_0015   0x0015
#define AR5212_DEVID_0016   0x0016
#define AR5212_DEVID_0017   0x0017
#define AR5212_DEVID_0018   0x0018
#define AR5212_DEVID_0019   0x0019
#define AR5212_AR2413       0x001a          /* AR2413 aka Griffin-lite */
#define AR5212_AR5413       0x001b          /* Eagle */
#define AR5212_AR5424       0x001c          /* Condor (PCI express) */
#define AR5212_AR2417       0x001d          /* Nala */
#define AR5212_AR2425       0x001c          /* Swan */
#define AR5212_DEVID_FF19   0xff19          /* XXX PCI express */
#define AR5212_DEVID_EMU	0xff1a			/* Nala emulation device id */
/* AR5213 */
#define AR5213_SREV_1_0     0x0055
#define AR5213_SREV_REG     0x4020

/* AR5416 - owl */
#define AR5416_DEVID_EMU        0xff1d  /* owl emulation device id */
#define AR5416_DEVID_EMU_PCIE   0xff1c  /* owl emulation device id */
#define AR5416_DEVID_PCI        0x0023  /* AR5416 PCI (CB/MB) (Owl)*/
#define AR5416_DEVID_PCIE       0x0024  /* AR5416 PCI-E (XB) (Owl) */
#define AR5416_DEVID_AR9160_PCI 0x0027	/* AR9160 PCI (Sowl) */
#define AR5416_DEVID_AR9280_PCI   0x0029        /* PCI (Merlin) */
#define AR5416_DEVID_AR9280_PCIE  0x002a        /* PCIE (Merlin) */
#define AR5416_AR9100_DEVID     0x000B	/* ar9100 (Howl) */

#define AR_SUBVENDOR_ID_NOG     0x0e11      /* No 11G subvendor ID */
#define AR_SUBVENDOR_ID_NEW_A   0x7065      /* Update device to new RD */
#endif /* _DEV_ATH_DEVID_H */
