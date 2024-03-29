/**************************************************************
***************************************************************
**
** BME Firmware, Copyright (C) 2000-2002
** Ikanos Communication, Fremont, CA 
** All rights reserved.
**
** The information and contents of this file are the
** proprietary information of Ikanos Communication and may not
** be disclosed or used without the formal written approval of 
** Ikanos Communication
**
***************************************************************
$Revision: 1.1 $
$Date: 2008/07/18 11:09:30 $
**************************************************************/
#ifndef _XDSL_RMC_H_
#define _XDSL_RMC_H_

/* Must be called a power up to set up RxCpu structure */
void RxCpuInit( void );

/* Main interface function to set RxCpu state */
void RxCpuSendCommand(u8 Line, u8 CmdId, u32 Frame, u16 * pParam);

/* Line:		port number 0 -> 3 VDSL */

/* CmdId:		State of RxCpu, one of: */
/* [] represents a u16 of pParam */

u16	RxCpuGetCurrentFrame( u8 Line);

enum {
	RCPU_LINE_DOWN = 0,

	/**************************************************************/
	/* G.HS Phase */
	/**************************************************************/
	RCPU_LINE_UP,
	/* [MSB: FFT Scale shift  ; LSB: Num tone (11 => 2^11 = 2048 tones)]
	   [MSB: Windowing enable ; LSB: Num Beta Sample]
	   [Nb tones in bit table] 	*/

	/**************************************************************/
	/* Training Phase */
	/**************************************************************/
	RCPU_TR_CONFIG,
	/* [MSB: FFT Scale shift  ; LSB: Num tone (11 => 2^11 = 2048 tones)]
	   [MSB: Windowing enable ; LSB: Num Beta Sample]
	   [MSB: TEQ scale shift  ; LSB: Num taps]
	   [Num Prefix samples]
	   [Num suffix samples]
	   [Nb tones in bit table (| RCPU_CONFIG_LINE_UP to Enable VBus)] */

	RCPU_TR_DETECT,
	RCPU_TR_NO_FEQ_NO_DATA,
	RCPU_TR_VBUS_OUT,
	RCPU_TR_TEQ_BYP_NO_DATA,
	RCPU_TR_TEQ_OUT,

	RCPU_TR_TEQ_NORM,
	/* [TEQ scale shift] */

	RCPU_TR_SKIP_SAMPLE,
	/* [# of samples to skip]
	   [# of times   to skip]
	   [4 bit skip tag ] */

	RCPU_TR_UPBO,
	RCPU_TR_FFT_OUT,
	RCPU_TR_FEQ_TRAIN,
	RCPU_TR_TST_SNR,
	RCPU_TR_NO_DATA,
	RCPU_TR_SLICER_NO_UPDATE,

	/**************************************************************/
	/* Analyze and Exchange - Rcpu states */
	/**************************************************************/
	RCPU_MED_FEQ_TRAIN,
	RCPU_MED_NO_FEQ_NO_DATA,
	RCPU_MED_START_SNR,
	RCPU_MED_NO_DATA,
	RCPU_MED_SLICER_SNR,
	RCPU_MED_SLICER_DATA,
	
	RCPU_MED_SLICER_NO_UPDATE,

	/**************************************************************/
	/* Showtime - Rcpu states */
	/**************************************************************/
	RCPU_SHOW_START,
	/* [Pi: Number of bytes generated by bit table] */
	// ( no bit table offset any more) 

	RCPU_SHOW_SNR,
	// No param
	// Provides SNR on 1024 samples once during showtime

	/**************************************************************/
	/* Miscellenous states */
	/**************************************************************/
	RCPU_MISC_SEQUENCING,
	/* [MSB: 1st Ch# - LSB: 2nd Ch#]
	   [MSB: 3rd Ch# - LSB: 4th Ch#] */
};

/* Special flag to start from Config state: */
#define RCPU_CONFIG_LINE_UP		0x8000


/* Frame:		Frame number for action and type describing the number: */
#define RCPU_FRAME_ABSOLUTE		0
#define RCPU_FRAME_RELATIVE		0x40000000
#define RCPU_FRAME_IMMEDIATE	0x80000000


/* pParam:		Parameter to provide for the state */

/**************************************************************/
/***   T A G    definitions									  */
/**************************************************************/
/* No data output */
#define RCPU_TAG_NO_OUT					0

/* Time domaine directly from VBUS */
#define RCPU_TAG_VBUS_OUT				1

/* Time domaine after TEQ */
#define RCPU_TAG_TEQ_OUT				2

/* Frequency domaine */
#define RCPU_TAG_FFT_OUT				3

/* Frequency domaine after FEQ */
#define RCPU_TAG_FEQ_OUT				4

/* Tones packed in bytes sorted sequentially */
#define RCPU_TAG_SLICER_SEQ_OUT			5

/* Tones packed in bytes sorted as bit table */
#define RCPU_TAG_SLICER_SORT_OUT		6

/* Out put of the Reverb checker accumulator */
#define RCPU_TAG_ACC_OUT				7

/* Pseudo Random Generator output */
#define RCPU_TAG_PRG_OUT				8



/**************************************************************/
/***    S T A T E S   D E S C R I P T I O N 				  */
/**************************************************************/
/*
  RCPU_LINE_DOWN:			To power down a line. 
  RCPU_LINE_UP:				To power up a line, this sets FFT parameters. FFT out for all tones.
  RCPU_TR_CONFIG:			First state to be set for OPTraining phase, sets TEQ parameters, no out
  RCPU_TR_DETECT:			To detect signal on the line. SNR calculated every 32 symbols.
  RCPU_TR_NO_FEQ_NO_DATA:	Must be after Detect to stop SNR, no data, no FEQ
  RCPU_TR_VBUS_OUT:			To get Time domain samples before TEQ
  RCPU_TR_TEQ_BYP_NO_DATA:	To bypass TEQ to write new coeffs in TEQ
  RCPU_TR_TEQ_OUT:			To get Time domain samples for TEQ coeffs calculation
  RCPU_TR_TEQ_NORM:			To re-enable TEQ, FFT out
  RCPU_TR_SKIP_SAMPLE:		To skip samples, does not change output
  RCPU_TR_UPBO:				To get SNR (128 symbol). No output
  RCPU_TR_FFT_OUT:			To get FFT out and stop SNR. (can be set even if SNR not running)
  RCPU_TR_FEQ_TRAIN:		To train FEQ, no output
  RCPU_TR_TST_SNR:			To get SNR estimation during OPTraining - For Fast Track only
  RCPU_TR_NO_DATA:			To have no data out after FEQ has been trained
  RCPU_TR_SLICER_NO_UPDATE:	To get Slicer output (for Synchro detection) without updating FEQ coeffs
  RCPU_MED_FEQ_TRAIN:		To train FEQ on Medley, no data
  RCPU_MED_NO_FEQ_NO_DATA:	To bypass FEQ, to allow Read/Write FEQ coeffs
  RCPU_MED_START_SNR:		To get SNR estimation
  RCPU_MED_NO_DATA:			To get no data after FEQ trained
  RCPU_MED_SLICER_DATA:		To get slicer output (FEQ coeffs are updated)
  RCPU_MED_SLICER_NO_UPDATE:To stop FEQ coefs update for read operation
  RCPU_SHOW_START:			To start Showtime phase
  RCPU_MISC_SEQUENCING:		To modify channel sequencing
*/




#endif
