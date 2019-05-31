/**********************************************************************
 Copyright (c), 1991-2009, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 �ļ�����: vdsl.h
 �ļ�����: ʵ��ͨ����VDSLD����ͨ�ţ���ȡ��ص�VDSL��·��Ϣ
 �޶���¼:
        1. ����: liannan
           ����: 2008-11-10
           ����: �����ļ�
**********************************************************************/

#ifndef _VDSL_H_
#define _VDSL_H_

#include "common_msg_handler.h"
#include "warnlog.h"
#include "tbsmsg.h"

//#define VDSL_DEBUG

#ifdef VDSL_DEBUG
#define VDSL_TRACE(fmt, args...)        TBS_TRACE(MID_VDSL, fmt "\n", ##args)
#else
#define VDSL_TRACE(fmt, args...)
#endif


#define PATH_VDSL "InternetGatewayDevice.X_TWSZ-COM_Vdsl."

/*��ʾ��Ϣ֮��Ķ�Ӧ��ϵ*/
struct VDSL_MsgMap {
	unsigned short reqMsg;
	unsigned short ackMsg;
	char nodename[32];
	int  (*proc_AckMsg)(int id, void *msg);
};

typedef struct
{
    unsigned short usSrcMID;       /* Դģ��MID */
    unsigned short usDstMID;       /* Ŀ��ģ��MID */
    unsigned long ulMsgID;         /* ��ϢID----Ӧ�ó����Զ��� */
    unsigned short usMsgType;      /* ��Ϣ����----Ӧ�ó����Զ��� */
    unsigned short usReserve;      /* ���� */
    unsigned long ulBodyLength;    /* ��Ϣ�峤�� */
    unsigned long Result;
}ST_ACKMSGHEAD;

typedef struct {
    unsigned long Result;
    unsigned short BMEID;
    unsigned short PortID;
    unsigned char PortStatus;
} port_status_brief_t;

typedef struct {
    ST_MSGHEAD stMsgHead;
    port_status_brief_t port_status_brief;
}msg_port_status_brief_t;

typedef struct {
    unsigned long Result;
    unsigned short BMEID;
    unsigned short PortID;
    unsigned long DsLineRate;
    unsigned long UsLineRate;
    unsigned long FastDsPayloadRate;
    unsigned long SlowDsPayloadRate;
    unsigned long FastUsPayloadRate;
    unsigned long SlowUsPayloadRate;
    unsigned char FastDsNumParityBytes;
    unsigned char SlowDsNumParityBytes;
    unsigned char FastUsNumParityBytes;
    unsigned char SlowUsNumParityBytes;
    unsigned short DsDelay;
    unsigned short UsDelay;
    unsigned short TransmitPower;
    unsigned short DsTrainingMargin;
    unsigned short DsAttainableRate;
    unsigned short DsAttainableLineRate;
    unsigned short SlowDsLineProtection;
    unsigned short UsTrainingMargin;
    unsigned short UsAttainableRate;
    unsigned short UsAttainableLineRate;
    unsigned short SlowUsLineProtection;
    unsigned char NeItuVendorId[8];
    unsigned char FeItuVendorId[8];
    unsigned short FeTransmitPower;
    unsigned short LoopLength;
    unsigned short ghsNearEndLoopLength;
    unsigned short ghsFarEndLoopLength;
    unsigned short SlowUsFFTProtection;
    unsigned short SlowDsFFTProtection;
    unsigned char FastDsCodeWordSize;
    unsigned char SlowDsCodeWordSize;
    unsigned char FastUsCodeWordSize;
    unsigned char SlowUsCodeWordSize;
    unsigned char DsInterleaverDepth;
    unsigned char UsInterleaverDepth;
    unsigned char DsInterleaverBlkLen;
    unsigned char UsInterleaverBlkLen;
    unsigned char CurrentFramingMode;
    unsigned char BandType;
    unsigned char NumOfBandsUsDs;
    unsigned char LineType[20];
} port_status_t;

typedef struct {
    ST_MSGHEAD stMsgHead;
    port_status_t port_status;
}msg_port_status_t;

typedef struct {
    unsigned long Result;
    unsigned short BMEID;
    unsigned short PortID;
    unsigned long EnetTxFrameCount;
    unsigned long EnetOctXmitCount;
    unsigned long TxPauseFrameCount;
    unsigned long BroadCastFrameCount;
    unsigned long EnetDropFrameCount;
    unsigned long AllignErrorCount;
    unsigned long LongFrameCount;
    unsigned long RxErrorCount;
    unsigned long EnetCrcErrorCount;
    unsigned long CarrierSenseErrorCount;
    unsigned long EnetRxFrameCount;
    unsigned long EnetOctRcvCount;
    unsigned long RxPauseFrameCount;
    unsigned long RxHdlcCount;
    unsigned long RxHdecCount;
    unsigned long RxHdscCount;
    unsigned long RxHdmcCount;
    unsigned long RxHdDefTransCount;
    unsigned long RxHdlcErrorFrameCount;
    unsigned long RxHdlcInvalidFrameCount;
    unsigned long RxHdlcDropFrameCount;
    unsigned short NeLOS;
    unsigned short NeLOF;
    unsigned short NeLOM;
    unsigned short NeESE;
    unsigned short NeNCD_S;
    unsigned short NeLCD_S;
    unsigned short NeNCD_F;
    unsigned short NeLCD_F;
    unsigned short FeLOS;
    unsigned short FeRDI;
    unsigned short FeLPR;
    unsigned short FeLOM;
    unsigned short FeESE;
    unsigned short FeNCD_S;
    unsigned short FeLCD_S;
    unsigned short FeNCD_F;
    unsigned short FeLCD_F;
    unsigned short FePO;
    unsigned short NeSES;
    unsigned short NeES;
    unsigned int NeFECS;
    unsigned int NeLOSS;
    unsigned int NeUnavl;
    unsigned int FeSES;
    unsigned int FeES;
    unsigned int FeFECS;
    unsigned int FeLOSS;
    unsigned int FeUnavl;
    unsigned int NeFEC_S;
    unsigned int NeFEC_F;
    unsigned int NeCRC_S;
    unsigned int NeCRC_F;
    unsigned int NeHEC_S;
    unsigned int NeHEC_F;
    unsigned int FeFEC_S;
    unsigned int FeFEC_F;
    unsigned int FeCRC_S;
    unsigned int FeCRC_F;
    unsigned int FeHEC_S;
    unsigned int FeHEC_F;
    unsigned int numUSRcell_S;
    unsigned int numUSRcell_F;
    unsigned int numIDLEcell_S;
    unsigned int numIDLEcell_F;
    unsigned int numIDLEcellBe_S;
    unsigned int numIDLEcellBe_F;
    unsigned int numTxBlk_S;
    unsigned int numTxBlk_F;
    unsigned int numRxBlk_S;
    unsigned int numRxBlk_F;
    unsigned int NeFECUnCrr_S;
    unsigned int NeFECUnCrr_F;
    unsigned short sixPtLineHold;
    unsigned short numReInit;
    unsigned short numInitFailure;
    unsigned char NeBER_S;
    unsigned char NeBER_F;

} vdsl_status_t;

typedef struct {
    ST_MSGHEAD stMsgHead;
    vdsl_status_t vdsl_status;
}msg_vdsl_status_t;

typedef struct {
    unsigned long Result;
    unsigned short BMEID;
    unsigned short PortID;
    long SnrMargin;
    unsigned long Attn;
    long SnrAvgMargin;
    long SnrAvg;
}port_snr_status_t;

typedef struct {
    ST_MSGHEAD stMsgHead;
    port_snr_status_t port_snr_status;
}msg_port_snr_status_t;

typedef struct {
    unsigned long Result;
    unsigned char VersionStr[250];
} bme_version_t;

typedef struct {
    ST_MSGHEAD stMsgHead;
    bme_version_t bme_version;
}msg_bme_version_t;



int VDSL_MsgProcess(ST_MSG *pstMsg);
#endif

