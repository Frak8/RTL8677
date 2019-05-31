/*
* Copyright c                  Realtek Semiconductor Corporation, 2008  
* All rights reserved.
* 
* Program : napt table driver
* Abstract : 
* Author : hyking (hyking_liu@realsil.com.cn)  
*/

/*      @doc RTL_LAYEREDDRV_API

        @module rtl865x_nat.c - RTL865x Home gateway controller Layered driver API documentation       |
        This document explains the API interface of the table driver module. Functions with rtl865x prefix
        are external functions.
        @normal Hyking Liu (Hyking_liu@realsil.com.cn) <date>

        Copyright <cp>2008 Realtek<tm> Semiconductor Cooperation, All Rights Reserved.

        @head3 List of Symbols |
        Here is a list of all functions and variables in this module.
        
        @index | RTL_LAYEREDDRV_API
*/

#include <net/rtl/rtl_types.h>
#include <net/rtl/rtl_glue.h>
#include <net/rtl/rtl865x_netif.h>
#include "common/mbuf.h"
#include "AsicDriver/rtl865x_asicCom.h"
//#include "assert.h"

//#include "rtl865xc_swNic.h"
//#include <common/types.h>
#include "AsicDriver/rtl865x_hwPatch.h"		/* define for chip related spec */
#ifdef CONFIG_RTL_LAYERED_ASIC_DRIVER

#include "AsicDriver/rtl865x_asicL4.h"
#else
#include "common/rtl8651_aclLocal.h"
#include "rtl865xC_tblAsicDrv.h"
#endif

#include "common/rtl_errno.h"
//#include <net/rtl/rtl_queue.h>
#include "AsicDriver/rtl865xc_asicregs.h"
#include "common/rtl865x_eventMgr.h"
#include "l3Driver/rtl865x_ip.h"

#include <net/rtl/rtl865x_nat.h>
#include "rtl865x_nat_local.h"

//#include "rtl865x_ppp.h"
#include "common/rtl865x_netif_local.h"
#include "l3Driver/rtl865x_ppp_local.h"
//#include "l3Driver/rtl865x_route.h"
#include <net/rtl/rtl865x_outputQueue.h>
#if	defined(CONFIG_RTL_HW_QOS_SUPPORT)
#include <net/rtl/rtl865x_arp_api.h>
#include "l3Driver/rtl865x_route.h"
#endif

#ifdef CONFIG_RTL_PROC_DEBUG
#include <linux/seq_file.h>
#endif

#if defined (CONFIG_RTL_IPTABLES_RULE_2_ACL) && defined(CONFIG_RTL8676_Dynamic_ACL)
extern int enable_port_mapping;
#include <linux/netfilter_ipv4/ip_tables.h>	
#include "l3Driver/rtl865x_route.h"
#endif
#include <linux/jiffies.h>


struct nat_table nat_tbl;
static int32 rtl865x_enableNaptFourWay=FALSE;
#if 0
static int32 rtl865x_nat_callbackFn_for_del_ip(void *param);

static int32 rtl865x_nat_register_event(void);

static int32 rtl865x_nat_callbackFn_for_del_ip(void *param)
{
	int i;
	rtl865x_tblAsicDrv_extIntIpParam_t *natip;
	struct nat_entry *nat_out, *nat_in;
	struct nat_tuple nat_tuple;
	natip=(rtl865x_tblAsicDrv_extIntIpParam_t *)param;

	for(i=0; i<RTL8651_TCPUDPTBL_SIZE; i++)
	{
		if((nat_tbl.nat_bucket[i].flags & NAT_OUTBOUND) && (nat_tbl.nat_bucket[i].ext_ip_==natip->extIpAddr))
		{
			memcpy(&nat_tuple, &nat_tbl.nat_bucket[i].tuple_info, sizeof(struct nat_tuple));
			nat_out = &nat_tbl.nat_bucket[i];
			nat_in = &nat_tbl.nat_bucket[nat_out->in];
			rtl8651_delAsicNaptTcpUdpTable(nat_out->in, nat_out->in);
			rtl8651_delAsicNaptTcpUdpTable(nat_out->out, nat_out->out);
			memset(nat_in, 0, sizeof(*nat_in));
			memset(nat_out, 0, sizeof(*nat_out));

			if(nat_tbl.connNum > 0)
			{
				nat_tbl.connNum--;
			}	
		}

	}
	return EVENT_CONTINUE_EXECUTE;
}

static int32 _rtl865x_nat_unRegister_event(void)
{
	rtl865x_event_Param_t eventParam;
	
	eventParam.eventLayerId=DEFAULT_LAYER3_EVENT_LIST_ID;
	eventParam.eventId=EVENT_DEL_IP;
	eventParam.eventPriority=0;
	eventParam.event_action_fn=rtl865x_nat_callbackFn_for_del_ip;
	rtl865x_unRegisterEvent(&eventParam);

	return SUCCESS;
}

static int32 _rtl865x_nat_register_event(void)
{
	rtl865x_event_Param_t eventParam;
	
	eventParam.eventLayerId=DEFAULT_LAYER3_EVENT_LIST_ID;
	eventParam.eventId=EVENT_DEL_IP;
	eventParam.eventPriority=0;
	eventParam.event_action_fn=rtl865x_nat_callbackFn_for_del_ip;
	rtl865x_registerEvent(&eventParam);

	return SUCCESS;
}
#endif

static int32 _rtl865x_nat_init(void)
{
	rtl865x_tblAsicDrv_naptTcpUdpParam_t naptTcpUdp;
	uint32 flowTblIdx;
	
	memset(nat_tbl.nat_bucket, 0, 
		sizeof(struct nat_entry)*RTL8651_TCPUDPTBL_SIZE);

	nat_tbl.connNum = 0;
	nat_tbl.tcp_timeout = TCP_TIMEOUT; //24*60*60;
	nat_tbl.udp_timeout = UDP_TIMEOUT; //60*15;
	nat_tbl.freeEntryNum=RTL8651_TCPUDPTBL_SIZE;

	/* Set ASIC timeout value */
	rtl8651_setAsicNaptTcpLongTimeout(TCP_TIMEOUT);
	rtl8651_setAsicNaptTcpMediumTimeout(TCP_TIMEOUT);
	rtl8651_setAsicNaptTcpFastTimeout(TCP_TIMEOUT);
	rtl8651_setAsicNaptUdpTimeout(UDP_TIMEOUT);

	/*enable 865xC enhanced hash1*/
	_rtl8651_enableEnhancedHash1();
	
	/* Initial ASIC NAT Table */
	memset( &naptTcpUdp, 0, sizeof(naptTcpUdp) );
	naptTcpUdp.isCollision = 1;
	naptTcpUdp.isCollision2 = 1;
	for(flowTblIdx=0; flowTblIdx<RTL8651_TCPUDPTBL_SIZE; flowTblIdx++)
		rtl8651_setAsicNaptTcpUdpTable(TRUE, flowTblIdx, &naptTcpUdp );

	//rtl865x_nat_register_event();
		
	return SUCCESS;
}



static struct nat_entry * _rtl865x_nat_outbound_lookup(struct nat_tuple *nat_tuple)
{
	struct nat_entry *nat_out;
	uint32 i,hash;

	hash = rtl8651_naptTcpUdpTableIndex((uint8)nat_tuple->proto, nat_tuple->int_host.ip, nat_tuple->int_host.port, 
											nat_tuple->rem_host.ip, nat_tuple->rem_host.port);
	if(rtl865x_enableNaptFourWay==TRUE)
	{
		for(i=0; i<4; i++)
		{
			nat_out = &nat_tbl.nat_bucket[hash];
			if (!memcmp(nat_out, nat_tuple, sizeof(*nat_tuple)) &&
				(nat_out->flags&NAT_OUTBOUND))
			{
				return nat_out;
			}
			hash=(hash&0xFFFFFFFC)+(hash+1)%4;
			assert(hash<=RTL8651_TCPUDPTBL_SIZE);
		}
	}
	else
	{
		nat_out = &nat_tbl.nat_bucket[hash];
		if (!memcmp(nat_out, nat_tuple, sizeof(*nat_tuple)) &&
			(nat_out->flags&NAT_OUTBOUND))
			return nat_out;
	}
	return (struct nat_entry *)0;
}

static struct nat_entry * _rtl865x_nat_inbound_lookup(struct nat_tuple *nat_tuple)
{
	struct nat_entry *nat_in;
	uint32 hash;

	hash = rtl8651_naptTcpUdpTableIndex((uint8)nat_tuple->proto, nat_tuple->rem_host.ip, nat_tuple->rem_host.port, 
											nat_tuple->ext_host.ip, nat_tuple->ext_host.port);
	
	
	nat_in = &nat_tbl.nat_bucket[hash];
	if (!memcmp(nat_in, nat_tuple, sizeof(*nat_tuple)) && 
		(nat_in->flags&NAT_INBOUND))
	{
			return nat_in;
	}
	
	return (struct nat_entry *)0;
}

#if defined (CONFIG_RTL_INBOUND_COLLISION_AVOIDANCE)
static int _rtl865x_isEntryPreReserved(uint32 index)
{
	struct nat_entry *natEntry;
	if(index>=RTL8651_TCPUDPTBL_SIZE)
	{
		return FALSE;
	}
	
	natEntry= &nat_tbl.nat_bucket[index];

	if((natEntry->flags & NAT_PRE_RESERVED))
	{
		if(jiffies>=natEntry->reserveTime)
		{
			if(jiffies>(natEntry->reserveTime+RESERVE_EXPIRE_TIME*HZ))
			{
				/*pre-reserve become invalid now*/
				natEntry->flags &= (~(NAT_PRE_RESERVED));
				natEntry->reserveTime=0;
				return FALSE;
			}
			else
			{
				return TRUE;
			}
		}
		else
		{
			/*timer overflow*/
			if(((0xFFFFFFFF-natEntry->reserveTime)+jiffies+1)>(RESERVE_EXPIRE_TIME*HZ))
			{
				natEntry->flags &= (~(NAT_PRE_RESERVED));
				natEntry->reserveTime=0;
				return FALSE;
			}
			else
			{
				return TRUE;
			}
		}
		return TRUE;
	}

	return FALSE;
}

static int _rtl865x_PreReserveEntry(uint32 index)
{
	struct nat_entry *natEntry;
	if(index>=RTL8651_TCPUDPTBL_SIZE)
	{
		return FAILED;
	}
	natEntry= &nat_tbl.nat_bucket[index];
	
	if(NAT_INUSE(natEntry))
	{
		/*already used by other napt connection, can not reserve it*/
		natEntry->flags &= (~(NAT_PRE_RESERVED));
		natEntry->reserveTime=0;
	}
	else
	{
		natEntry->flags|=NAT_PRE_RESERVED;
		natEntry->reserveTime=jiffies;
	}
	return SUCCESS;
}


static int _rtl865x_getNaptHashInfo( uint32 protocol, ipaddr_t intIp, uint32 intPort,
                        ipaddr_t extIp, uint32 extPort,
                        ipaddr_t remIp, uint32 remPort, 
                        rtl865x_naptHashInfo_t *naptHashInfo)
{
	
	uint32 in, out;
	uint32  i,index;
	struct nat_entry *nat_in, *nat_out;
	struct nat_entry *natEntry;
	
	if(naptHashInfo==NULL)
	{
		return FAILED;
	}
	
	memset(naptHashInfo, 0, sizeof(rtl865x_naptHashInfo_t));
	
	in = rtl8651_naptTcpUdpTableIndex((uint8)protocol, remIp, remPort, extIp, extPort);
	out = rtl8651_naptTcpUdpTableIndex((uint8)protocol, intIp, intPort, remIp, remPort);

	if(rtl865x_enableNaptFourWay==TRUE)
	{
		uint32 outAvailIdx=0xFFFFFFFF;
		index=out;
		for(i=0;i<4;i++)
		{
			natEntry = &nat_tbl.nat_bucket[index];
			if (NAT_INUSE(natEntry) || _rtl865x_isEntryPreReserved(index))
			{
			
			}
			else
			{
				if(index==in)
				{
					/*collide with inbound*/
				}
				else
				{
					out=index;
					break;
				}
			}
			index=(index&0xFFFFFFFC)+(index+1)%4;
			assert(index<=RTL8651_TCPUDPTBL_SIZE);
				
		}

		if(i>=4)
		{
			/*only one empty entry, but collide with its own inbound*/
			if(outAvailIdx!=0xFFFFFFFF)
			{
				out=outAvailIdx;
			}
		}
		else
		{
			/*proper empty entry has been found*/
		}
	}
	
	naptHashInfo->outIndex=out;
	naptHashInfo->inIndex=in;

	if((in&0xFFFFFFFC)==(out&0xFFFFFFFC))
	{
		naptHashInfo->sameFourWay=1;
	}	

	if(in==out)
	{
		naptHashInfo->sameLocation=1;
		
		nat_out = &nat_tbl.nat_bucket[out];
		if(NAT_INUSE(nat_out)|| _rtl865x_isEntryPreReserved(out))
		{
			naptHashInfo->outCollision=1;
		}

		naptHashInfo->inCollision=1;
	}
	else
	{
		nat_out = &nat_tbl.nat_bucket[out];
		nat_in = &nat_tbl.nat_bucket[in];
		
		if(NAT_INUSE(nat_out) || _rtl865x_isEntryPreReserved(out))
		{
			naptHashInfo->outCollision=1;
		}
		
		if (NAT_INUSE(nat_in) ||  _rtl865x_isEntryPreReserved(in))
		{
			naptHashInfo->inCollision=1;
		}
	}
	

	index=in;
	naptHashInfo->inFreeCnt=0;
	for(i=0;i<4;i++)
	{
		natEntry = &nat_tbl.nat_bucket[index];
		if (NAT_INUSE(natEntry) || _rtl865x_isEntryPreReserved(index))
		{
		
		}
		else
		{
			naptHashInfo->inFreeCnt++;
		}
		index=(index&0xFFFFFFFC)+(index+1)%4;
		assert(index<=RTL8651_TCPUDPTBL_SIZE);
	}
	#if 0
	printk("%s:%d:%s (%u.%u.%u.%u:%u -> %u.%u.%u.%u:%u -> %u.%u.%u.%u:%u) ,out is %d,in is %d\n",
			__FUNCTION__,__LINE__,protocol?"tcp":"udp", 
			NIPQUAD(intIp), intPort, NIPQUAD(extIp), extPort, NIPQUAD(remIp), remPort, out, in);	
	#endif
	return SUCCESS;
}

int rtl865x_optimizeExtPort(unsigned short origDelta, unsigned int rangeSize, unsigned short *newDelta)
{
	int i;
	int msb;
	unsigned int bitShift;

	msb=0;
	for(i=0;i<16;i++)
	{
		if((1<<i) & rangeSize)
		{
			msb=i;
		}
	}

	if(((1<<msb)+1)>rangeSize)
	{
		if(msb>1)
		{
			msb--;
		}
	}
		
 	*newDelta=0;
	if(msb<10)
	{
		bitShift=0x01;
		for(i=0;i<=msb;i++)
		{
			if(i==0)/*bit0 keep the same*/
			{
				if(origDelta&bitShift)
				{
					 *newDelta|=bitShift;
				}
			}
			else /*original bit1~ bit_maxPower mapped to bit_maxPower~bit1*/
			{
				if(origDelta&bitShift) 
				{
					 *newDelta |=(0x1<<(msb+1-i));
				}
			}

			bitShift=bitShift<<1;
		}
	}
	else
	{
		bitShift=0x01;
		
		for(i=0;i<=msb;i++)
		{
			if(i==0)	/*bit0 keep the same*/
			{
				if(origDelta&bitShift) 
				{
					*newDelta |=bitShift;
				}
			}
			else if (i<10) /*bit1~ bit9 mapped to bit 9~bit1*/
			{
				if(origDelta&bitShift) 
				{
					*newDelta  |=(0x1<<(10-i));
				}
			}
			else/*other bits keep the same*/
			{
				if(origDelta&bitShift) 
				{
					*newDelta  |=bitShift;
				}
			}

			bitShift=bitShift<<1;
		}


	}
	return SUCCESS;
}

int rtl865x_getAsicNaptHashScore( uint32 protocol, ipaddr_t intIp, uint32 intPort,
					                        ipaddr_t extIp, uint32 extPort,
					                        ipaddr_t remIp, uint32 remPort, 
					                        uint32 *naptHashScore)
{
	 rtl865x_naptHashInfo_t naptHashInfo;
	_rtl865x_getNaptHashInfo( protocol, intIp, intPort, extIp, extPort, remIp, remPort, &naptHashInfo);
	/*initialize napt hash score*/
	*naptHashScore=100;

	/*note:we can not change outbound index*/
	
	if(naptHashInfo.inCollision==FALSE)
	{
		if(naptHashInfo.inFreeCnt==4)
		{
			if(!naptHashInfo.sameFourWay)
			{
				*naptHashScore=100;
			}
			else
			{
				if(!naptHashInfo.sameLocation)
				{
					*naptHashScore=80;
				}
				else
				{
					*naptHashScore=0;
				}
			}
		}
		else if (naptHashInfo.inFreeCnt==3)
		{
			if(!naptHashInfo.sameFourWay)
			{
				*naptHashScore=80;
			}
			else
			{
				if(!naptHashInfo.sameLocation)
				{
					*naptHashScore=70;
				}
				else
				{
					*naptHashScore=0;
				}
			}
		}
		else if (naptHashInfo.inFreeCnt==2)
		{
			if(!naptHashInfo.sameFourWay)
			{
				*naptHashScore=70;
			}
			else
			{
				if(!naptHashInfo.sameLocation==FALSE)
				{
					*naptHashScore=60;
				}
				else
				{
					*naptHashScore=0;
				}
			}
		}
		else if (naptHashInfo.inFreeCnt==1)
		{
			if(naptHashInfo.sameFourWay==FALSE)
			{
				*naptHashScore=60;
			}
			else
			{
				*naptHashScore=0;
					
			}
		}
		else
		{
			*naptHashScore=0;
		}
		

	}
	else
	{
		/*worst case:inbound is collision*/
		*naptHashScore=0;
	}
	
	return SUCCESS;

}

int32 rtl865x_preReserveConn( uint32 protocol, ipaddr_t intIp, uint32 intPort,
					                        ipaddr_t extIp, uint32 extPort,
					                        ipaddr_t remIp, uint32 remPort)
{

	 rtl865x_naptHashInfo_t naptHashInfo;
	_rtl865x_getNaptHashInfo( protocol, intIp, intPort, extIp, extPort, remIp, remPort, &naptHashInfo);

	if(naptHashInfo.outCollision==FALSE)
	{
		_rtl865x_PreReserveEntry(naptHashInfo.outIndex);
	}

	if(naptHashInfo.inCollision==FALSE)
	{
		_rtl865x_PreReserveEntry(naptHashInfo.inIndex);
	}
	
	return SUCCESS;
}	

#endif
static int32 _rtl865x_addNaptConnection( uint32 protocol, ipaddr_t intIp, uint32 intPort,
                        ipaddr_t extIp, uint32 extPort,
                        ipaddr_t remIp, uint32 remPort,
                        int32 flags )
{
	int32 retval;
	rtl865x_tblAsicDrv_naptTcpUdpParam_t asic_nat;
	struct nat_entry *nat_in, *nat_out;
	struct nat_tuple nat_tuple;
	uint32 in, out, offset, ipidx, i;
	uint16 very,selEidx_out;
#if	defined(CONFIG_RTL_HW_QOS_SUPPORT)
	rtl865x_route_t		rt;
	rtl865x_arpMapping_entry_t	arpInfo;
	ipaddr_t		sip, dip;
	uint16		sport, dport;
	rtl865x_AclRule_t		rule4[2], rule2[2];
	int32		priority[2], defPriority[2];
	int32 		upDown[2], defUpDown[2];//0: uplink; 1: downlink
#endif
	
	uint32 outCollision=FALSE;
	uint32 inCollision=FALSE;

	/* QL: debug */
	//printk("%s %d protocol:%d intIp:%x intPort:%d extIp:%x extPort:%d remIp:%x remPort:%d flags:%d\n", __func__, __LINE__,
	//		protocol, intIp, intPort, extIp, extPort, remIp, remPort, flags);
	
	/* Make sure natip */
	retval = rtl865x_getIpIdxByExtIp(extIp, &ipidx);
	if(retval != SUCCESS)
		return RTL_EINVALIDINPUT;
	
	memset(&nat_tuple, 0, sizeof(struct nat_tuple));
	nat_tuple.int_host.ip			= intIp;
	nat_tuple.int_host.port			= intPort;
	nat_tuple.ext_host.ip			= extIp;
	nat_tuple.ext_host.port			= extPort;
	nat_tuple.rem_host.ip			= remIp;
	nat_tuple.rem_host.port		= remPort;
	nat_tuple.proto				= protocol;
	
	nat_out = _rtl865x_nat_outbound_lookup(&nat_tuple);
	if (nat_out)
	{
		return RTL_EENTRYALREADYEXIST;
	}
	
	nat_in = _rtl865x_nat_inbound_lookup(&nat_tuple);
	if(nat_in)
	{
		return RTL_EENTRYALREADYEXIST;
	}

	offset = (extPort&0x0000ffff)>>10;
	very = rtl8651_naptTcpUdpTableIndex(((uint8)protocol) |HASH_FOR_VERI , remIp, remPort, 0, 0);
	
	selEidx_out = extPort&0x3ff;
	in = rtl8651_naptTcpUdpTableIndex((uint8)protocol, remIp, remPort, extIp, extPort);
	out = rtl8651_naptTcpUdpTableIndex((uint8)protocol, intIp, intPort, remIp, remPort);
	/*support outbound 4-way*/
	if(rtl865x_enableNaptFourWay==TRUE)
	{

		uint32 hash=out;
		uint32 outAvailIdx=0xFFFFFFFF;
		
		for(i=0;i<4;i++)
		{
			nat_out = &nat_tbl.nat_bucket[hash];
			if (NAT_INUSE(nat_out))
			{
			
			}
			else
			{
				outAvailIdx=hash;
				if(hash==in)
				{
					/*collision with inbound*/
				}
				else
				{
					out=hash;
					break;
				}
			}
			hash=(hash&0xFFFFFFFC)+(hash+1)%4;
			assert(hash<=RTL8651_TCPUDPTBL_SIZE);
				
		}


		if(i>=4)
		{
			/*only one empty entry,but collide with its own inbound*/
			if(outAvailIdx!=0xFFFFFFFF)
			{
				out=outAvailIdx;
			}
		}
	}

	
	nat_out = &nat_tbl.nat_bucket[out];
	nat_in = &nat_tbl.nat_bucket[in];

	if ( NAT_INUSE(nat_out) && NAT_INUSE(nat_in))
	{
		/*both outbound and inbound has been occupied*/
		return RTL_EINVALIDINPUT;
	}
	
	if(out==in)
	{
		outCollision=FALSE;
		inCollision=TRUE;
		/*we don't support this case at present, otherwise, when delete napt connection must be very very careful*/
		//return RTL_EENTRYALREADYEXIST;
	}
	else
	{
		if (NAT_INUSE(nat_out))
		{
			outCollision=TRUE;
		}

		if(NAT_INUSE(nat_in))
		{
			inCollision=TRUE;
		}
	}
#ifdef CONFIG_HARDWARE_NAT_DEBUG
	rtlglue_printf("LR(%s):  %s (%u.%u.%u.%u:%u -> %u.%u.%u.%u:%u) g:(%u.%u.%u.%u:%u)\n",
			("add_nat"), ((protocol)? "tcp": "udp"), 
			NIPQUAD(intIp), intPort, NIPQUAD(remIp), remPort, NIPQUAD(extIp), extPort);
#endif

#if defined (CONFIG_RTL_HALF_NAPT_ACCELERATION)

#else
	if((outCollision==TRUE) || (inCollision==TRUE))
	{
		/*we must make sure both direction can be written into asic*/
		return RTL_EINVALIDINPUT;
	}
#endif

	if ( outCollision==FALSE)
	{
		memset(nat_out, 0, sizeof(struct nat_entry));
		*((struct nat_tuple *)nat_out)=nat_tuple;
		nat_out->out=out;
		if( inCollision==FALSE)
		{
			nat_out->in=in;
		}
		else
		{
			nat_out->in=0xFFFFFFFF;
		}
		
		nat_out->natip_idx	=ipidx;	
		SET_NAT_FLAGS(nat_out, NAT_OUTBOUND);
	}
	else
	{
		nat_out=NULL;
	}

	if( inCollision==FALSE)
	{
		memset(nat_in, 0, sizeof(struct nat_entry));
		*((struct nat_tuple *)nat_in) = nat_tuple;
		nat_in->in = in;
		if ( outCollision==FALSE)
		{
			 nat_in->out = out;
		}
		else
		{
			 nat_in->out=0xFFFFFFFF;
		}
		nat_in->natip_idx = ipidx;
		SET_NAT_FLAGS(nat_in, NAT_INBOUND);
	}
	else
	{
		nat_in=NULL;
	}

#if	defined(CONFIG_RTL_HW_QOS_SUPPORT)
	sip = intIp;
	dip = remIp;
	sport = intPort;
	dport = remPort;
	
	//Initial
	for(i=0;i<2;i++)
	{
		priority[i] = 0;
		defPriority[i]=-1;
		upDown[i]=-1;
		defUpDown[i]=-1;
	}
	
	for(i=0;i<2;i++)
	{
		if (rtl865x_getRouteEntry(sip, &rt)==SUCCESS)
		{
			/*	check ip base rule firstly	*/

			memset(&rule4[i], 0, sizeof(rtl865x_AclRule_t));
			rule4[i].ruleType_ = (protocol==RTL865X_PROTOCOL_TCP?RTL865X_ACL_TCP:RTL865X_ACL_UDP);
			rule4[i].srcIpAddr_ = sip;
			rule4[i].dstIpAddr_ = dip;
			rule4[i].tcpSrcPortLB_ = sport;
			rule4[i].tcpDstPortLB_ = dport;
			rule4[i].netifIdx_ = _rtl865x_getNetifIdxByNameExt(rt.dstNetif->name);
			
			if (rtl865x_qosCheckNaptPriority(&rule4[i])==SUCCESS)
			{
				priority[i] = rule4[i].priority_;		/* matched priority	*/
				upDown[i]=rule4[i].upDown_;
				//break;

				if (i==0)
				{
					sip = remIp;
					dip = intIp;
					sport = remPort;
					dport = intPort;
					continue;
				}
				 
			}
			else if (i==0)
			{
				sip = remIp;
				dip = intIp;
				sport = remPort;
				dport = intPort;
				continue;
			}
			else
			{
				defPriority[i] = rule4[i].priority_;
				defUpDown[i]=rule4[i].upDown_;
			}
		}
		else
		{
			sip = remIp;
			dip = intIp;
			sport = remPort;
			dport = intPort;
		}
	}

	{
		sip = intIp;
		for(i=0;i<2;i++)
		{
			if (rtl865x_getArpMapping(sip, &arpInfo)==SUCCESS && rtl865x_getRouteEntry(sip, &rt)==SUCCESS)
			{
				/*	check mac base rule secondly	*/
				memset(&rule2[i], 0, sizeof(rtl865x_AclRule_t));
				rule2[i].ruleType_ = RTL865X_ACL_MAC;
				memcpy(rule2[i].srcMac_.octet, arpInfo.mac.octet, ETHER_ADDR_LEN);
				memset(rule2[i].srcMacMask_.octet, 0xff, ETHER_ADDR_LEN);
				//get netif index of DIP route for each dir.
				rule2[i].netifIdx_ = _rtl865x_getNetifIdxByNameExt(rt.dstNetif->name);

				if (rtl865x_qosCheckNaptPriority(&rule2[i])==SUCCESS)
				{
					priority[i] = rule2[i].priority_;		/* matched priority	*/
					upDown[i]=rule2[i].upDown_;
					//break;

					 if(i==0)
					{
						sip = remIp;
						continue;
					}
					 
				}
				else if(i==0)
				{
					sip = remIp;
					continue;
				}
				else
				{
					/*if no src mac match rule exist, then check dst mac match rule.*/
					sip = intIp;
					dip = remIp;
					for(i=0;i<2;i++)
					{
						if (rtl865x_getArpMapping(dip, &arpInfo)==SUCCESS)
						{
							rtl865x_getRouteEntry(sip, &rt);
							/*	check mac base rule secondly	*/
							memset(&rule2[i], 0, sizeof(rtl865x_AclRule_t));
							rule2[i].ruleType_ = RTL865X_ACL_MAC;
							memcpy(rule2[i].dstMac_.octet, arpInfo.mac.octet, ETHER_ADDR_LEN);
							memset(rule2[i].dstMacMask_.octet, 0xff, ETHER_ADDR_LEN);
							rule2[i].netifIdx_ = _rtl865x_getNetifIdxByNameExt(rt.dstNetif->name);

							if (rtl865x_qosCheckNaptPriority(&rule2[i])==SUCCESS)
							{
								priority[i] = rule2[i].priority_;		/* matched priority	*/
								upDown[i]=rule2[i].upDown_;
								//break;
								
								if(i==0)
								{
									sip = remIp;
									dip = intIp;
									continue;
								}
								
							}
							else if(i==0)
							{
								sip = remIp;
								dip = intIp;
								continue;
							}
							else
							{
								defPriority[i] = rule2[i].priority_;
								defUpDown[i]=rule2[i].upDown_;
							}
						}
						else
						{
							sip = remIp;
							dip = intIp;
						}
					}
				}
			}
			else
			{
				sip = remIp;
			}
		}
	}

	for(i=0;i<2;i++)
	{
		if (rule4[i].aclIdx&&rule2[i].aclIdx)
		{
			priority[i] = (rule4[i].aclIdx<rule2[i].aclIdx)?rule4[i].priority_:rule2[i].priority_;
			upDown[i]=(rule4[i].aclIdx<rule2[i].aclIdx)?rule4[i].upDown_:rule2[i].upDown_;
		}
		else if (rule4[i].aclIdx)
		{
			priority[i] = rule4[i].priority_;
			upDown[i]=rule4[i].upDown_;
		}
		else if (rule2[i].aclIdx)
		{
			priority[i] = rule2[i].priority_;
			upDown[i]=rule2[i].upDown_;
		}
		else if (defPriority[i]>-1)
		{
			priority[i] = defPriority[i];
			upDown[i]=defUpDown[i];
		}
	}
#endif

	for(i=0; i<2; i++) {
		
		if(i==0)
		{
			/*check writing outbound connection into asic*/
			if((outCollision==TRUE) || (nat_out==NULL))
			{
				/*shouldn't be written into asic*/
				continue;
			}
		}
		else if(i==1)
		{
			/*check writing inbound connection into asic*/
			if((inCollision==TRUE)||(nat_in==NULL))
			{
				/*shouldn't be written into asic*/
				continue;
			}
		}
		else
		{
			break;
		}
		//If qos enabled, not add inbound napt
		//That is all downlink pkt will be trapped to CPU for software QoS
		if((flags==FLAG_QOS_ENABLE)&&(i==1))
			break;
		
		memset(&asic_nat, 0, sizeof(asic_nat));
		asic_nat.insideLocalIpAddr	= intIp;
		asic_nat.insideLocalPort		= intPort;
		asic_nat.isCollision			= 0;
		asic_nat.isCollision2		= 0;
		asic_nat.isDedicated		= 0;
		asic_nat.isStatic			= 1;
		asic_nat.isTcp			= (protocol==RTL865X_PROTOCOL_TCP)? 1: 0;
		asic_nat.isValid			= 1;
		asic_nat.offset			= ((i==0)?offset : (extPort & 0x3f));
		asic_nat.selEIdx			= ((i==0)?selEidx_out: very &0x3ff);
		asic_nat.selExtIPIdx		= ((i==0)?ipidx:((extPort & 0x3ff) >> 6));
		//asic_nat.tcpFlag			= (((in!=out)? 0x2:0x0) | ((i==0)? 1: 0));
		/*enhanced hash1 doesn't support outbound/inbound share one connection*/
		asic_nat.tcpFlag			= (0x2 | ((i==0)? 1: 0));
		asic_nat.ageSec			= (protocol==RTL865X_PROTOCOL_TCP)? nat_tbl.tcp_timeout:nat_tbl.udp_timeout;
#if	defined(CONFIG_RTL_HW_QOS_SUPPORT)
#if 1
		asic_nat.priValid			=	FALSE;
		if(i==0&&upDown[i]==0)//Because ((i==0)?out: in)
		{
			//out: uplink
			asic_nat.priority			=	priority[i];
			asic_nat.priValid			=	TRUE;
		}
		if (i==1&&upDown[i]==1)
		{
			//in: downlink
			asic_nat.priority			=	priority[i];
			asic_nat.priValid			=	TRUE;
		}
#else
		asic_nat.priority			=	priority;
		asic_nat.priValid			=	TRUE;
#endif
		
#else
		asic_nat.priValid			=	FALSE;
#endif
		
		rtl8651_setAsicNaptTcpUdpTable(1, ((i==0)?out: in), &asic_nat);
	}





	nat_tbl.connNum++;
	
	if((outCollision==FALSE) && (nat_out!=NULL))
	{
		if(nat_tbl.freeEntryNum>0)
		{
			nat_tbl.freeEntryNum--;
		}
	}
		
	if((inCollision==FALSE) && (nat_in!=NULL))
	{
		if(nat_tbl.freeEntryNum>0)
		{
			nat_tbl.freeEntryNum--;
		}
	}
	
	return SUCCESS;
}


static int32 _rtl865x_delNaptConnection( uint32 protocol, ipaddr_t intIp, uint32 intPort,
                        ipaddr_t extIp, uint32 extPort,
                        ipaddr_t remIp, uint32 remPort )
{
	struct nat_entry *nat_out, *nat_in;
	struct nat_tuple nat_tuple;

	memset(&nat_tuple, 0, sizeof(struct nat_tuple));
	nat_tuple.int_host.ip			= intIp;
	nat_tuple.int_host.port			= intPort;
	nat_tuple.ext_host.ip			= extIp;
	nat_tuple.ext_host.port			= extPort;
	nat_tuple.rem_host.ip			= remIp;
	nat_tuple.rem_host.port		= remPort;
	nat_tuple.proto				= protocol;

	nat_out = _rtl865x_nat_outbound_lookup(&nat_tuple);
	nat_in =  _rtl865x_nat_inbound_lookup(&nat_tuple);

	if ((nat_out==NULL) && (nat_in==NULL))
	{
		return RTL_EENTRYNOTFOUND;
	}

	if(nat_out==nat_in)	
	{
		rtl8651_delAsicNaptTcpUdpTable(nat_out->out, nat_out->out);
		memset(nat_out, 0, sizeof(*nat_out));
		nat_tbl.freeEntryNum++;
		
	}
	else
	{
		if(nat_out)
		{
			rtl8651_delAsicNaptTcpUdpTable(nat_out->out, nat_out->out);
			memset(nat_out, 0, sizeof(*nat_out));
			nat_tbl.freeEntryNum++;
		}	

		if(nat_in)
		{
			rtl8651_delAsicNaptTcpUdpTable(nat_in->in, nat_in->in);
			memset(nat_in, 0, sizeof(*nat_in));
			nat_tbl.freeEntryNum++;
		}
	}
	#ifdef CONFIG_HARDWARE_NAT_DEBUG
	/*2007-12-19*/
	rtlglue_printf("LR(%s):  %s (%u.%u.%u.%u:%u -> %u.%u.%u.%u:%u) g:(%u.%u.%u.%u:%u)\n",
			("del_nat"), ((protocol)? "tcp": "udp"), 
			NIPQUAD(intIp), intPort, NIPQUAD(remIp), remPort, NIPQUAD(extIp), extPort);
	#endif



	if(nat_tbl.connNum>0)
	{
		nat_tbl.connNum--;
	}
	
	return SUCCESS;
}


static int32 _rtl865x_naptSync( uint32 protocol, ipaddr_t intIp, uint32 intPort,
			ipaddr_t extIp, uint32 extPort,
			ipaddr_t remIp, uint32 remPort, uint32 refresh )
{
	rtl865x_tblAsicDrv_naptTcpUdpParam_t asic_nat_out;
	rtl865x_tblAsicDrv_naptTcpUdpParam_t asic_nat_in;
	struct nat_entry *nat_out,*nat_in;
	struct nat_tuple nat_tuple;
	int32 rc;

	memset(&nat_tuple, 0, sizeof(struct nat_tuple));
	nat_tuple.int_host.ip			= intIp;
	nat_tuple.int_host.port			= intPort;
	nat_tuple.ext_host.ip			= extIp;
	nat_tuple.ext_host.port			= extPort;
	nat_tuple.rem_host.ip			= remIp;
	nat_tuple.rem_host.port		= remPort;
	nat_tuple.proto				= protocol;
	
	nat_out = _rtl865x_nat_outbound_lookup(&nat_tuple);
	nat_in = _rtl865x_nat_inbound_lookup(&nat_tuple);

	if( (!nat_out) && (!nat_in))
	{
		return 0;

	}

	memset(&asic_nat_out ,0 ,sizeof(rtl865x_tblAsicDrv_naptTcpUdpParam_t));
	memset(&asic_nat_in,0 ,sizeof(rtl865x_tblAsicDrv_naptTcpUdpParam_t));
		
	if((nat_out!=NULL) && (nat_in!=NULL))
	{
		rc = rtl8651_getAsicNaptTcpUdpTable(nat_out->out, &asic_nat_out);
		assert(rc==SUCCESS);
		
		rc = rtl8651_getAsicNaptTcpUdpTable(nat_in->in, &asic_nat_in);
		assert(rc==SUCCESS);
		return (asic_nat_out.ageSec>asic_nat_in.ageSec)? asic_nat_out.ageSec: asic_nat_in.ageSec;
	}
	else if((nat_out!=NULL) && (nat_in==NULL))
	{
		rc = rtl8651_getAsicNaptTcpUdpTable(nat_out->out, &asic_nat_out);
		assert(rc==SUCCESS);
		return asic_nat_out.ageSec;
	}
	else if((nat_out==NULL) && (nat_in!=NULL))
	{
		rc = rtl8651_getAsicNaptTcpUdpTable(nat_in->in, &asic_nat_in);
		assert(rc==SUCCESS);
		return asic_nat_in.ageSec;
		
	}
	else
	{
		return 0;
	}

	return 0;
	
	
}


/*
@func int32 | rtl865x_addNaptConnection |add a napt entry.
@parm uint32 | protocol | protocol.
@parm ipaddr_t | intIp | internal ip address.
@parm uint32 | intPort | internal port.
@parm ipaddr_t | extIp | external ip address.
@parm uint32 | extPort | external port.
@parm ipaddr_t | remIp | remote ip address.
@parm uint32 | remPort | remote port.
@parm int32 | flags | flags.
@rvalue SUCCESS | success.
@rvalue FAILED | failed.
@rvalue RTL_EINVALIDINPUT | invalid input.
@rvalue RTL_EENTRYALREADYEXIST | route entry is already exist.
@rvalue RTL_ENOFREEBUFFER | not enough memory in system.
@comm
	value of protocol should be RTL865X_PROTOCOL_TCP/RTL865X_PROTOCOL_UDP
*/
int32 rtl865x_addNaptConnection( uint32 protocol, ipaddr_t intIp, uint32 intPort,
                        ipaddr_t extIp, uint32 extPort,
                        ipaddr_t remIp, uint32 remPort,
                        int32 flags )
{
	int32 retval = FAILED;

	retval = _rtl865x_addNaptConnection(protocol,intIp,intPort,extIp,extPort,remIp,remPort, flags);

	return retval;
}

/*
@func int32 | rtl865x_delNaptConnection |delete a napt entry.
@parm uint32 | protocol | protocol.
@parm ipaddr_t | intIp | internal ip address.
@parm uint32 | intPort | internal port.
@parm ipaddr_t | extIp | external ip address.
@parm uint32 | extPort | external port.
@parm ipaddr_t | remIp | remote ip address.
@parm uint32 | remPort | remote port.

@rvalue SUCCESS | success.
@rvalue FAILED | failed.
@rvalue RTL_EENTRYNOTFOUND | not found this entry in system.
@comm
*/
int32 rtl865x_delNaptConnection( uint32 protocol, ipaddr_t intIp, uint32 intPort,
                        ipaddr_t extIp, uint32 extPort,
                        ipaddr_t remIp, uint32 remPort )
{
	int32 retval = FAILED;

	retval = _rtl865x_delNaptConnection(protocol,intIp,intPort,extIp,extPort,remIp,remPort);

	return retval;
}

int32 rtl865x_naptSync( uint32 protocol, ipaddr_t intIp, uint32 intPort,
			ipaddr_t extIp, uint32 extPort,
			ipaddr_t remIp, uint32 remPort, uint32 refresh )
{
	return _rtl865x_naptSync(protocol,intIp,intPort,extIp,extPort,remIp,remPort,refresh);
}

#if	defined(CONFIG_RTL_HW_QOS_SUPPORT)
inline static int32 rtl865x_naptSetAsicWithPriority(struct nat_entry *entry, int32 priority)
{
	rtl865x_tblAsicDrv_naptTcpUdpParam_t asic_nat;
	int32		idx;

	//If qos enabled, not add inbound napt
	//That is all downlink pkt will be trapped to CPU for software QoS
	if(entry->flags&NAT_INBOUND)
		return SUCCESS;
	
	//printk("--%s(%d),entry(%p)\n",__FUNCTION__,__LINE__,entry);
	idx = (entry->flags&NAT_INBOUND)?entry->in:entry->out;
	
	rtl8651_getAsicNaptTcpUdpTable(idx, &asic_nat);
	asic_nat.priority = priority;
	asic_nat.priValid = TRUE;
	rtl8651_setAsicNaptTcpUdpTable(1, idx, &asic_nat);

	return SUCCESS;
}

static int32 rtl865x_naptCallbackFn_for_qosChange(void *param)
{
	int num, i;
	struct nat_entry *nat_this, *nat_that;
	ipaddr_t		sip, dip;
	uint16		sport, dport;
	rtl865x_route_t		rt;
	rtl865x_arpMapping_entry_t	arpInfo;
	rtl865x_AclRule_t		rule4, rule2;
	int32		priority=-1, defPriority=-1;

	num = i = 0;

	while(num < nat_tbl.connNum && i < RTL8651_TCPUDPTBL_SIZE)
	{
		if(NAT_INUSE(&nat_tbl.nat_bucket[i]))
		{
			nat_this = &nat_tbl.nat_bucket[i];
			if (nat_this->flags&NAT_INBOUND)
			{
				if(nat_this->out!=0xFFFFFFFF)
				{
					nat_that = &nat_tbl.nat_bucket[nat_this->out];
				}
				else
				{
					/*no outbound*/
					nat_that = NULL;
				}
				/*get src/dst info for each dir */
				sip = nat_this->rem_ip_;
				dip = nat_this->ext_ip_;
				sport = nat_this->rem_port_;
				dport = nat_this->ext_port_;
			}
			else/*for NAT_OUTBOUND*/
			{
				if(nat_this->in!=0xFFFFFFFF)
				{
					nat_that = &nat_tbl.nat_bucket[nat_this->in];
				}
				else
				{
					/*no inbound*/
					nat_that=NULL;
				}
				
				sip = nat_this->int_ip_;
				dip = nat_this->rem_ip_;
				sport = nat_this->int_port_;
				dport = nat_this->rem_port_;
			}
			
			if (nat_this->flags&NAT_PRI_PROCESSED)
			{
				CLR_NAT_FLAGS(nat_this, NAT_PRI_PROCESSED);
				num++;
			}
			else 
			{
				if (rtl865x_getRouteEntry(sip, &rt)==SUCCESS)
				{
					memset(&rule4, 0, sizeof(rtl865x_AclRule_t));
					rule4.ruleType_ = (nat_this->proto_==RTL865X_PROTOCOL_TCP?RTL865X_ACL_TCP:RTL865X_ACL_UDP);
					rule4.srcIpAddr_ = sip;
					rule4.dstIpAddr_ = dip;
					rule4.tcpSrcPortLB_ = sport;
					rule4.tcpDstPortLB_ = dport;
					rule4.netifIdx_ = _rtl865x_getNetifIdxByNameExt(rt.dstNetif->name);
					if(rule4.netifIdx_ < 0 || rule4.netifIdx_ >= NETIF_NUMBER)
					{
						printk("===%s %s(%d) Can't get netif(%s)\n",__FILE__,__FUNCTION__,__LINE__,rt.dstNetif->name);						
					}

					if(rule4.netifIdx_ >=0 && rule4.netifIdx_ < NETIF_NUMBER)
					{
						if (rtl865x_qosCheckNaptPriority(&rule4)!=SUCCESS)
						{
							if (nat_this->flags&NAT_INBOUND)
							{
								rule4.dstIpAddr_ = nat_this->int_ip_;
								rule4.tcpDstPortLB_ = nat_this->int_port_;
								if (rtl865x_qosCheckNaptPriority(&rule4)!=SUCCESS)
									defPriority = rule4.priority_;
							}
							else
								defPriority = rule4.priority_;
						}
					}	
					if (rtl865x_getArpMapping(sip, &arpInfo)==SUCCESS)
					{
						memset(&rule2, 0, sizeof(rtl865x_AclRule_t));
						rule2.ruleType_ = RTL865X_ACL_MAC;
						memcpy(rule2.srcMac_.octet, arpInfo.mac.octet, ETHER_ADDR_LEN);
						memset(rule2.srcMacMask_.octet, 0xff, ETHER_ADDR_LEN);
						rule2.netifIdx_ = rule4.netifIdx_;//_rtl865x_getNetifIdxByNameExt(rt.dstNetif->name);
						if(rule4.netifIdx_ < 0 || rule4.netifIdx_ >= NETIF_NUMBER)
						{
							printk("===%s %s(%d) Can't get netif(%s)\n",__FILE__,__FUNCTION__,__LINE__,rt.dstNetif->name);						
						}
						
						if(rule4.netifIdx_ >=0 && rule4.netifIdx_ < NETIF_NUMBER)
							if (rtl865x_qosCheckNaptPriority(&rule2)!=SUCCESS)
							{
								if (nat_this->flags&NAT_INBOUND)
								{
									/*If souce mac match fail, then do dest mac match*/
									if(rtl865x_getArpMapping(nat_this->int_ip_, &arpInfo)==SUCCESS)
									{
										memset(&rule2, 0, sizeof(rtl865x_AclRule_t));
										rule2.ruleType_ = RTL865X_ACL_MAC;
										memcpy(rule2.dstMac_.octet, arpInfo.mac.octet, ETHER_ADDR_LEN);
										memset(rule2.dstMacMask_.octet, 0xff, ETHER_ADDR_LEN);
										rule2.netifIdx_ = _rtl865x_getNetifIdxByNameExt(rt.dstNetif->name);
										if (rtl865x_qosCheckNaptPriority(&rule2)!=SUCCESS)
											defPriority = rule2.priority_;
									}
								}
								else
									defPriority = rule2.priority_;
							}
					}

					if (rule4.aclIdx&& rule2.aclIdx)
					{
						priority = (rule4.aclIdx<rule2.aclIdx)?rule4.priority_:rule2.priority_;
					}
					else if (rule4.aclIdx)
					{
						priority = rule4.priority_;
					}
					else if (rule2.aclIdx)
					{
						priority = rule2.priority_;
					}

					if (priority>-1)
					{
						
						rtl865x_naptSetAsicWithPriority(nat_this, priority);
						
						if(nat_that!=NULL)
						{
							rtl865x_naptSetAsicWithPriority(nat_that, priority);
						}
						
						if (nat_this->flags&NAT_PRI_HALF_PROCESSED)
						{
							CLR_NAT_FLAGS(nat_this, NAT_PRI_HALF_PROCESSED);
							num++;
						}
						else
						{
							if(nat_that!=NULL)
							{
								SET_NAT_FLAGS(nat_that, NAT_PRI_PROCESSED);
							}
							else
							{
								/*only half accelerated*/
								num++;
							}
						}
					}
					else
					{
						if (nat_this->flags&NAT_PRI_HALF_PROCESSED)
						{
							rtl865x_naptSetAsicWithPriority(nat_this, defPriority>-1?defPriority:0);
							assert(nat_that!=NULL);
							if(nat_that!=NULL)
							{
								rtl865x_naptSetAsicWithPriority(nat_that, defPriority>-1?defPriority:0);
							}
							CLR_NAT_FLAGS(nat_this, NAT_PRI_HALF_PROCESSED);
							num++;
						}
						else
						{
							if(nat_that!=NULL)
							{
								SET_NAT_FLAGS(nat_that, NAT_PRI_HALF_PROCESSED);
							}
							else
							{
								/*only half accelerated*/
								num++;
							}
						}
					}
				}
				else
				{
					if (nat_this->flags&NAT_PRI_HALF_PROCESSED)
					{
						CLR_NAT_FLAGS(nat_this, NAT_PRI_HALF_PROCESSED);
						num++;
					}
					else
					{
						if(nat_that!=NULL)
						{
							SET_NAT_FLAGS(nat_that, NAT_PRI_HALF_PROCESSED);
						}
						else
						{
							/*only half accelerated*/
							num++;
						}
					}
				}
			}
		}

		i++;
	}
	return EVENT_CONTINUE_EXECUTE;
}


static int32 rtl865x_napt_register_qosEvent(void)
{
	rtl865x_event_Param_t eventParam;
	
	eventParam.eventLayerId=DEFAULT_LAYER2_EVENT_LIST_ID;
	eventParam.eventId=EVENT_CHANGE_QOSRULE;
	eventParam.eventPriority=0;
	eventParam.event_action_fn=rtl865x_naptCallbackFn_for_qosChange;
	rtl865x_registerEvent(&eventParam);

	eventParam.eventId=EVENT_FLUSH_QOSRULE;
	rtl865x_registerEvent(&eventParam);

	eventParam.eventLayerId=DEFAULT_LAYER3_EVENT_LIST_ID;
	eventParam.eventId=EVENT_ADD_ARP;
	rtl865x_registerEvent(&eventParam);

	return SUCCESS;
}

static int32 rtl865x_napt_unRegister_qosEvent(void)
{
	rtl865x_event_Param_t eventParam;
	
	eventParam.eventLayerId=DEFAULT_LAYER2_EVENT_LIST_ID;
	eventParam.eventId=EVENT_CHANGE_QOSRULE;
	eventParam.eventPriority=0;
	eventParam.event_action_fn=rtl865x_naptCallbackFn_for_qosChange;
	rtl865x_unRegisterEvent(&eventParam);

	eventParam.eventId=EVENT_FLUSH_QOSRULE;
	rtl865x_unRegisterEvent(&eventParam);

	eventParam.eventLayerId=DEFAULT_LAYER3_EVENT_LIST_ID;
	eventParam.eventId=EVENT_ADD_ARP;
	rtl865x_unRegisterEvent(&eventParam);

	return SUCCESS;
}
#endif

/*
@func int32 | rtl865x_setNatFourWay |enable 4way hash algorithm.
@parm int32 | enable | enable or disable.
@rvalue SUCCESS | success.
@comm
	default is enable in system.
*/
int32 rtl865x_setNatFourWay(int32 enable)
{
	 _set4WayHash(enable);
	rtl865x_enableNaptFourWay=enable;
	return SUCCESS;
}

/*
@func int32 | rtl865x_nat_init |initialize napt table.
@rvalue SUCCESS | success.
@comm	
*/
int32 rtl865x_nat_init(void)
{
	int32 retval = FAILED;
#if	defined(CONFIG_RTL_HW_QOS_SUPPORT)
	rtl865x_napt_unRegister_qosEvent();
#endif

	retval = _rtl865x_nat_init();
	rtl865x_setNatFourWay(TRUE);

#if	defined(CONFIG_RTL_HW_QOS_SUPPORT)
	rtl865x_napt_register_qosEvent();
#endif
	return retval;
}

int32 rtl865x_nat_reinit(void)
{
	return rtl865x_nat_init();

}

#ifdef CONFIG_RTL_PROC_DEBUG
int32 rtl865x_flushAllNaptConnection(void)
{
	uint32 i,outIndex,inIndex;
	struct nat_entry *nat_out=NULL, *nat_in=NULL, *tmp=NULL;
	for(i=0;i<RTL8651_TCPUDPTBL_SIZE;i++)
	{
		tmp = &nat_tbl.nat_bucket[i];

		if(NAT_INUSE(tmp))
		{
			outIndex=tmp->out;
			inIndex=tmp->in;
			if(outIndex!=0xFFFFFFFF)
			{
				nat_out=&nat_tbl.nat_bucket[outIndex];
			}
			else
			{
				nat_out=NULL;
			}

			if(inIndex!=0xFFFFFFFF)
			{
				nat_in=&nat_tbl.nat_bucket[inIndex];	
			}
			else
			{
				nat_in=NULL;	
			}

			if((nat_out==NULL) &&(nat_in==NULL))
			{
				rtl8651_delAsicNaptTcpUdpTable(i, i);
				continue;
			}
			
			if(nat_out==nat_in)
			{
				if(nat_out->flags&NAT_OUTBOUND)
				{
					rtl8651_delAsicNaptTcpUdpTable(outIndex, outIndex);
					memset(nat_out, 0, sizeof(*nat_out));
					nat_tbl.freeEntryNum++;
				}
				else if(nat_in->flags&NAT_INBOUND)
				{
					rtl8651_delAsicNaptTcpUdpTable(inIndex, inIndex);
					memset(nat_in, 0, sizeof(*nat_in));
					nat_tbl.freeEntryNum++;
				}
				else
				{
					/*fatal error*/
					return RTL_EENTRYNOTFOUND;
				}
			}
			else
			{
				if((nat_out!=NULL) && (nat_out->flags&NAT_OUTBOUND))
				{
					rtl8651_delAsicNaptTcpUdpTable(outIndex, outIndex);
					memset(nat_out, 0, sizeof(*nat_out));
					nat_tbl.freeEntryNum++;
				}
				
				if((nat_in!=NULL) && (nat_in->flags&NAT_INBOUND))		
				{
					rtl8651_delAsicNaptTcpUdpTable(inIndex,inIndex);
					memset(nat_in, 0, sizeof(*nat_in));
					nat_tbl.freeEntryNum++;
				}
			}
			
			if(nat_tbl.connNum>0)
			{
				nat_tbl.connNum--;
			}
		
		}
	}
	
	return SUCCESS;
}

int32 rtl865x_sw_napt_seq_read(struct seq_file *s, void *v)
{

	int i;
	struct nat_entry *natEntryPtr;
	int len=0;
	
	len = seq_printf(s, "%s\n", "sw napt table:");
	
	for(i=0; i<RTL8651_TCPUDPTBL_SIZE; i++)
	{
		natEntryPtr= &nat_tbl.nat_bucket[i];
		if(NAT_INUSE(natEntryPtr))
		{
			if(natEntryPtr->flags&NAT_OUTBOUND)
			{
				len += seq_printf(s, "[%4d]%s:%d.%d.%d.%d:%d---->%d.%d.%d.%d:%d---->%d.%d.%d.%d:%d\n      flags:0x%x,outbound:(%d),inbound:(%d)\n",
				i,natEntryPtr->proto_==1?"tcp":"udp" ,NIPQUAD(natEntryPtr->int_ip_),natEntryPtr->int_port_,
				NIPQUAD(natEntryPtr->ext_ip_),natEntryPtr->ext_port_,NIPQUAD(natEntryPtr->rem_ip_),natEntryPtr->rem_port_,natEntryPtr->flags,natEntryPtr->out, natEntryPtr->in);
			}

			if(natEntryPtr->flags&NAT_INBOUND)
			{
				len += seq_printf(s, "[%4d]%s:%d.%d.%d.%d:%d<----%d.%d.%d.%d:%d<----%d.%d.%d.%d:%d\n      flags:0x%x, outbound:(%d), inbound:(%d)\n",
				i,natEntryPtr->proto_==1?"tcp":"udp" ,NIPQUAD(natEntryPtr->int_ip_),natEntryPtr->int_port_,
				NIPQUAD(natEntryPtr->ext_ip_),natEntryPtr->ext_port_,NIPQUAD(natEntryPtr->rem_ip_),natEntryPtr->rem_port_,natEntryPtr->flags,natEntryPtr->out,natEntryPtr->in);
			}
		}
	
	}
	
	len += seq_printf(s, "total napt connection number is %d\n",nat_tbl.connNum);
	len += seq_printf(s, "total free entry number is %d\n",nat_tbl.freeEntryNum);
	return 0;
}

int32  rtl865x_sw_napt_seq_write( struct file *filp, const char *buff,unsigned long len, loff_t *off )
{
	char 	tmpbuf[64];
	uint32	delIndex,inIndex=0,outIndex=0;
	char		*strptr, *cmd_addr;
	char		*tokptr;
	struct nat_entry *nat_out=NULL, *nat_in=NULL, *tmp=NULL;
	
	if (buff && !copy_from_user(tmpbuf, buff, len)) {
		tmpbuf[len] = '\0';
		strptr=tmpbuf;
		cmd_addr = strsep(&strptr," ");
		if (cmd_addr==NULL)
		{
			goto errout;
		}

		if (!memcmp(cmd_addr, "flush", 5))
		{
			rtl865x_flushAllNaptConnection();
		}
		else if (!memcmp(cmd_addr, "del", 3))
		{
			tokptr = strsep(&strptr," ");
			if (tokptr==NULL)
			{
				goto errout;
			}

			delIndex=simple_strtol(tokptr, NULL, 0);
			if(delIndex>RTL8651_TCPUDPTBL_SIZE)
			{
				printk("error input!\n");
				return len;
			}
			tmp = &nat_tbl.nat_bucket[delIndex];

			if(NAT_INUSE(tmp))
			{
				outIndex=tmp->out;
				inIndex=tmp->in;
				if(outIndex!=0xFFFFFFFF)
				{
					nat_out=&nat_tbl.nat_bucket[outIndex];
				}
				else
				{
					nat_out=NULL;
				}

				if(inIndex!=0xFFFFFFFF)
				{
					nat_in=&nat_tbl.nat_bucket[inIndex];	
				}
				else
				{
					nat_in=NULL;	
				}
			
				if ((nat_out==NULL) && (nat_in==NULL))
				{
					rtl8651_delAsicNaptTcpUdpTable(delIndex, delIndex);
					goto errout;
				}

				if(nat_out==nat_in)	
				{
					rtl8651_delAsicNaptTcpUdpTable(outIndex, outIndex);
					memset(nat_out, 0, sizeof(*nat_out));
					nat_tbl.freeEntryNum++;
				}
				else
				{
					if((nat_out!=NULL) && (nat_out->flags&NAT_OUTBOUND))
					{
						rtl8651_delAsicNaptTcpUdpTable(outIndex, outIndex);
						memset(nat_out, 0, sizeof(*nat_out));
						nat_tbl.freeEntryNum++;
					}
					
					if((nat_in!=NULL) && ( nat_in->flags&NAT_INBOUND))		
					{
						rtl8651_delAsicNaptTcpUdpTable(inIndex,inIndex);
						memset(nat_in, 0, sizeof(*nat_in));
						nat_tbl.freeEntryNum++;
					}
				}
				
				if(nat_tbl.connNum>0)
				{
					nat_tbl.connNum--;
				}
				printk("del napt flow,outbound:%d,inbound:%d\n", outIndex, inIndex);
			
			}
			
		}
		else
		{
			goto errout;
		}
	}
	else
	{
errout:
		return len;
	}

	return len;
}

#endif


#if defined (CONFIG_RTL_IPTABLES_RULE_2_ACL) && defined(CONFIG_RTL8676_Dynamic_ACL)
void rtl865x_ADD_dynamic_acl_L34_permit(struct nf_conntrack_tuple* upstream_tuple
													,struct nf_conntrack_tuple* downstream_tuple,struct  dst_entry* dst)
{

	u_int32_t sip, dip, gip;
	u_int16_t sp, dp, gp, proto=0;

	rtl865x_route_t		s_rt,d_rt,g_rt;

	proto   = upstream_tuple->dst.protonum;
	sip		= upstream_tuple->src.u3.ip;
	dip 	= upstream_tuple->dst.u3.ip;
	gip 	= downstream_tuple->dst.u3.ip;
	sp  	= (proto==IPPROTO_TCP)? upstream_tuple->src.u.tcp.port: upstream_tuple->src.u.udp.port;
	dp  	= (proto==IPPROTO_TCP)? upstream_tuple->dst.u.tcp.port: upstream_tuple->dst.u.udp.port;
	gp  	= (proto==IPPROTO_TCP)? downstream_tuple->dst.u.tcp.port: downstream_tuple->dst.u.udp.port;


	if( proto!=IPPROTO_TCP && proto!=IPPROTO_UDP )
		return;

	if (rtl865x_getRouteEntry(sip, &s_rt) != SUCCESS) 
		return;

	if (rtl865x_getRouteEntry(dip, &d_rt) != SUCCESS) 
		return;

	if (rtl865x_getRouteEntry(gip, &g_rt) != SUCCESS) 
		return;

	#if 0
	if(( s_rt.dstNetif == d_rt.dstNetif) || (d_rt.is_internal == 1)){
		// connection initialized from wan
		sip		= ct->tuplehash[1].tuple.src.u3.ip;
		dip 	= ct->tuplehash[1].tuple.dst.u3.ip;
		gip 	= ct->tuplehash[0].tuple.dst.u3.ip;
		sp  	= (proto==IPPROTO_TCP)? ct->tuplehash[1].tuple.src.u.tcp.port: ct->tuplehash[1].tuple.src.u.udp.port;
		dp  	= (proto==IPPROTO_TCP)? ct->tuplehash[1].tuple.dst.u.tcp.port: ct->tuplehash[1].tuple.dst.u.udp.port;
		gp  	= (proto==IPPROTO_TCP)? ct->tuplehash[0].tuple.dst.u.tcp.port: ct->tuplehash[0].tuple.dst.u.udp.port;
		if (rtl865x_getRouteEntry(sip, &s_rt) != SUCCESS || rtl865x_getRouteEntry(dip, &d_rt) != SUCCESS) {
			HP_printk_0412NAPT("[HP]Leave %s @ %d  (rtl865x_getRouteEntry failed)\n",__func__,__LINE__);
			return;
		}
	}
	#endif

	/*  only hw-acc forwarding pkts (two different subnet ) */
	if( s_rt.dstNetif == d_rt.dstNetif )
		return;
	
				
	/* upstream */
	#ifdef CONFIG_RTL_MULTI_ETH_WAN
	if(d_rt.dstNetif->is_wan == 0)
		rtl865x_addIp(sip, sip,IP_TYPE_NAT,d_rt.dstNetif->name); 
	
	if (!enable_port_mapping) {
	#endif
		rtl865x_add_filter_L34_permit(sip,sp,dip,dp,proto,s_rt.dstNetif->name);			
	#ifdef CONFIG_RTL_MULTI_ETH_WAN
	}
	else
	{			
		if (sip == gip)
			rtl865x_add_filter_L34_redirect(sip,sp,dip,dp,gip,proto,0,s_rt.dstNetif->name,dst);
		else
			rtl865x_add_filter_L34_redirect(sip,sp,dip,dp,gip,proto,1,s_rt.dstNetif->name,dst);

		rtl865x_add_filter_L34_permit(sip,sp,dip,dp,proto,s_rt.dstNetif->name);			
	}
	#endif

	/*downstream */		
	if(d_rt.dstNetif->is_wan == 0) /* no NAPT , pure routing */
		rtl865x_add_filter_L34_permit(dip,dp,gip,gp,proto,d_rt.dstNetif->name);
	else /* NAPT */
		rtl865x_add_filter_L34_permit(dip,dp,gip,gp,proto,g_rt.dstNetif->name);
	
}

void rtl865x_DELETE_dynamic_acl_L34_permit(struct nf_conn *ct)
{

	u_int32_t sip, dip, gip;
	u_int16_t sp, dp, gp, proto=0;

	rtl865x_route_t		s_rt,d_rt;

	proto   = ct->tuplehash[0].tuple.dst.protonum;
	sip		= ct->tuplehash[0].tuple.src.u3.ip;
	dip 	= ct->tuplehash[0].tuple.dst.u3.ip;
	gip 	= ct->tuplehash[1].tuple.dst.u3.ip;
	sp  	= (proto==IPPROTO_TCP)? ct->tuplehash[0].tuple.src.u.tcp.port: ct->tuplehash[0].tuple.src.u.udp.port;
	dp  	= (proto==IPPROTO_TCP)? ct->tuplehash[0].tuple.dst.u.tcp.port: ct->tuplehash[0].tuple.dst.u.udp.port;
	gp  	= (proto==IPPROTO_TCP)? ct->tuplehash[1].tuple.dst.u.tcp.port: ct->tuplehash[1].tuple.dst.u.udp.port;


	if( proto!=IPPROTO_TCP && proto!=IPPROTO_UDP )
		return;

	if (rtl865x_getRouteEntry(sip, &s_rt) != SUCCESS) 
		return;

	if (rtl865x_getRouteEntry(dip, &d_rt) != SUCCESS) 
		return;


	if(( s_rt.dstNetif == d_rt.dstNetif) || (d_rt.is_internal == 1)){
		// connection initialized from wan
		sip		= ct->tuplehash[1].tuple.src.u3.ip;
		dip 	= ct->tuplehash[1].tuple.dst.u3.ip;
		gip 	= ct->tuplehash[0].tuple.dst.u3.ip;
		sp  	= (proto==IPPROTO_TCP)? ct->tuplehash[1].tuple.src.u.tcp.port: ct->tuplehash[1].tuple.src.u.udp.port;
		dp  	= (proto==IPPROTO_TCP)? ct->tuplehash[1].tuple.dst.u.tcp.port: ct->tuplehash[1].tuple.dst.u.udp.port;
		gp  	= (proto==IPPROTO_TCP)? ct->tuplehash[0].tuple.dst.u.tcp.port: ct->tuplehash[0].tuple.dst.u.udp.port;
		if (rtl865x_getRouteEntry(sip, &s_rt) != SUCCESS || rtl865x_getRouteEntry(dip, &d_rt) != SUCCESS) {
			return;
		}
	}

	
	if( s_rt.dstNetif == d_rt.dstNetif )
		return;	


	/*  upstream*/ 
	#ifdef CONFIG_RTL_MULTI_ETH_WAN
	if(d_rt.dstNetif->is_wan == 0)
		rtl865x_delIp(sip);
	rtl865x_del_filter_L34_redirect(sip,sp,dip,dp,proto);
	#endif
	rtl865x_del_filter_L34_permit(sip,sp,dip,dp,proto);	

	/* downstream */
	rtl865x_del_filter_L34_permit(dip,dp,gip,gp,proto);	
	
}

#endif


