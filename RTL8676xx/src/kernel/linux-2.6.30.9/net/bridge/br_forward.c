/*
 *	Forwarding decision
 *	Linux ethernet bridge
 *
 *	Authors:
 *	Lennert Buytenhek		<buytenh@gnu.org>
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License
 *	as published by the Free Software Foundation; either version
 *	2 of the License, or (at your option) any later version.
 */

#include <linux/kernel.h>
#include <linux/netdevice.h>
#include <linux/skbuff.h>
#include <linux/if_vlan.h>
#include <linux/netfilter_bridge.h>
#include "br_private.h"
#include <net/rtl/rtl_alias.h>
#if defined(CONFIG_RTL_FAST_BRIDGE)
#include <net/rtl/fastpath/fast_br.h>
#endif

#ifdef CONFIG_RTL8676_BRIDGE_FASTPATH  	
#include "../../drivers/net/brg_shortcut.h"  
#endif
#if defined (CONFIG_RTL_IGMP_SNOOPING)
#include <linux/ip.h>
#include <linux/in.h>
#if defined (CONFIG_RTL_MLD_SNOOPING)
#include <linux/ipv6.h>
#include <linux/in6.h>
#endif
#include <linux/igmp.h>
#include <net/checksum.h>
#include <net/rtl/rtl865x_igmpsnooping_glue.h>
#include <net/rtl/rtl865x_igmpsnooping.h>
#include <net/rtl/rtl865x_netif.h>
#include <net/rtl/rtl_nic.h>
#ifdef CONFIG_RTL_MULTI_ETH_WAN
#include <linux/if_smux.h>
#endif

extern int igmpsnoopenabled;
extern unsigned int brIgmpModuleIndex;
extern unsigned int br0SwFwdPortMask;
#if defined (CONFIG_RTL_MLD_SNOOPING)
extern int mldSnoopEnabled;
#endif
#endif


extern int wlan_mbssid_block;
#ifdef CONFIG_NEW_PORTMAPPING
static inline int br_flood_check_portmap(struct sk_buff *skb, struct net_bridge_port *p);
#endif
/* Don't forward packets to originating port or forwarding diasabled */
static inline int should_deliver(const struct net_bridge_port *p,
				 const struct sk_buff *skb)
{
#if 1 // WAN2WAN_BLOCKING
	/*linux-2.6.19*/
	if (skb->dev == p->dev ||
	    p->state != BR_STATE_FORWARDING ||
	    // Kaohj, not bridged between WAN connections [TR-068, I-122]
	    // not bridged between virtual ELAN, we have only one NIC port
	    ((skb->dev && skb->dev->priv_flags & IFF_DOMAIN_WAN) &&
	    (p->dev->priv_flags & IFF_DOMAIN_WAN)) ||
	#ifdef CONFIG_RTL_8676HWNAT	
	    ((skb->dev && skb->dev->priv_flags & IFF_DOMAIN_ELAN) &&
	    (p->dev->priv_flags & IFF_DOMAIN_ELAN) && skb->trap_by_ingress_acl != 1)
	#else
		((skb->dev && skb->dev->priv_flags & IFF_DOMAIN_ELAN) &&
	    (p->dev->priv_flags & IFF_DOMAIN_ELAN))
	#endif
	    )
		return 0;
		// maybe this is better
		// (skb->dev->priv_flags ^ p->dev->priv_flags) == 0 then forward, else block 
	#if defined (CONFIG_NET_WIRELESS) || defined (CONFIG_WIRELESS)
	// Kaohj --- check for wireless bridge blocking
	//printk("wlan_blocking=%d\n", p->br->wlan_blocking);
	if (p->br->wlan_blocking != WLAN_BLOCK_DISABLE) {
		if (p->br->wlan_blocking == WLAN_BLOCK_ALL) {
			if ((skb->dev && skb->dev->priv_flags & IFF_DOMAIN_WLAN) ||
				(p->dev->priv_flags & IFF_DOMAIN_WLAN))
				return 0;
		}
		else if (p->br->wlan_blocking == WLAN_BLOCK_ELAN) {
			if (((skb->dev && skb->dev->priv_flags & IFF_DOMAIN_WLAN) ||
				(p->dev->priv_flags & IFF_DOMAIN_WLAN)) &&
				((skb->dev && skb->dev->priv_flags & IFF_DOMAIN_ELAN) ||
				(p->dev->priv_flags & IFF_DOMAIN_ELAN)))
				return 0;
		}
		else if (p->br->wlan_blocking == WLAN_BLOCK_WAN) {
			if (((skb->dev && skb->dev->priv_flags & IFF_DOMAIN_WLAN) ||
				(p->dev->priv_flags & IFF_DOMAIN_WLAN)) &&
				((skb->dev && skb->dev->priv_flags & IFF_DOMAIN_WAN) ||
				(p->dev->priv_flags & IFF_DOMAIN_WAN)))
				return 0;
		}
	}

	 //����(wlan_mbssid_block=1)�͹ر�(wlan_mbssid_block=0)��SSID֮�����  .
	if (wlan_mbssid_block==1) {  
		if ((skb->dev && skb->dev->priv_flags & IFF_DOMAIN_WLAN) &&
			((p->dev->priv_flags & IFF_DOMAIN_WLAN) || (p->dev->priv_flags & IFF_DOMAIN_ELAN))){
			return 0;
		}
	}
#endif // CONFIG_NET_WIRELESS
	return 1;
#else
	return (skb->dev != p->dev && p->state == BR_STATE_FORWARDING);
#endif
}

static inline unsigned packet_length(const struct sk_buff *skb)
{
	return skb->len - (skb->protocol == htons(ETH_P_8021Q) ? VLAN_HLEN : 0);
}

int br_dev_queue_push_xmit(struct sk_buff *skb)
{
	/* drop mtu oversized packets except gso */
	if (packet_length(skb) > skb->dev->mtu && !skb_is_gso(skb))
		kfree_skb(skb);
	else {
		/* ip_refrag calls ip_fragment, doesn't copy the MAC header. */
		if (nf_bridge_maybe_copy_header(skb))
			kfree_skb(skb);
		else {
			skb_push(skb, ETH_HLEN);
#if defined(CONFIG_RTL_FAST_BRIDGE)
			{	
				struct net_bridge_fdb_entry *dst;
				const unsigned char *dest = eth_hdr(skb)->h_dest;
				if (!is_multicast_ether_addr(dest))
				{					
					dst = __br_fdb_get(skb->dev->br_port->br, dest);
					if(dst != NULL && dst->dst->dev == skb->dev)
					{
						int retval = 0;
						fast_br_cache_entry entry;
						memset(&entry,0,sizeof(fast_br_cache_entry));
						memcpy(&entry.mac_addr,dest,6);
						entry.ageing_timer = jiffies;
						entry.to_dev = skb->dev;
						entry.valid = 1;

						retval = rtl_add_fast_br_entry(&entry);

						#if 0
						if(skb->dev->br_port)
							printk("------%s(%d),br(%s)\n",__FUNCTION__,__LINE__,skb->dev->br_port->br->dev->name);
						printk("========%s(%d),skb->dev(%s),retval(%d)\n",__FUNCTION__,__LINE__,skb->dev->name,retval);
						#endif
					}
				}
			}
#endif

#ifndef CONFIG_NEW_PORTMAPPING
			/* For Portmapping */
			#ifdef CONFIG_RTL_MULTI_ETH_WAN
			
			if ( skb->dev && 
            alias_name_are_eq(skb->dev->name,ALIASNAME_NAS,ALIASNAME_MWNAS)
			//(skb->dev->name[0] == 'n') 		
			&& skb->from_dev && (skb->from_dev->priv_flags&(IFF_DOMAIN_ELAN|IFF_DOMAIN_WLAN)))//which is smux device				 

			{
				if (!smuxUpstreamPortmappingCheck(skb)) {
					kfree_skb(skb);
					return 0;
				}
			}
			#endif
#endif			
			// Kevin
			#ifdef CONFIG_RTL8676_BRIDGE_FASTPATH
			if(skb->protocol != ETH_P_ARP) //Kevin, there exists unicast arp request , it should not add into bridge shortcut 
										//ref: http://serverfault.com/questions/81651/strange-why-does-linux-respond-to-ping-with-arp-request-after-last-ping-reply
				brgShortcutLearnDestItf(skb, skb->dev);
			#endif
			if((skb->priority <= 0x1b)&&(skb->priority >= 0x14))//prio 1b-14   7-0
				{
					struct vlan_ethhdr *veth = (struct vlan_ethhdr *)(skb->data);
					if(veth->h_vlan_proto == htons(ETH_P_8021Q))
						{
						  veth->h_vlan_TCI &= 0x1fff;//clear 802.1p
						  veth->h_vlan_TCI |= htons((skb->priority - 20) << 13);//000(802.1p 3bit) 0 000000000000(vlan id 12bit)
						}
				}
			dev_queue_xmit(skb);
		}
	}

	return 0;
}

int br_forward_finish(struct sk_buff *skb)
{
	return NF_HOOK(PF_BRIDGE, NF_BR_POST_ROUTING, skb, NULL, skb->dev,
		       br_dev_queue_push_xmit);

}

static void __br_deliver(const struct net_bridge_port *to, struct sk_buff *skb)
{
	skb->dev = to->dev;

	/*linux-2.6.19*/
	/*patch from linux 2.4*/
	// Kaohj, bridged PPPoE
	if (to->dev->brpppoe && !( eth_hdr(skb)->h_proto == 0x8863 ||
		eth_hdr(skb)->h_proto == 0x8864)) {
		kfree_skb(skb);
		return;
	}

	NF_HOOK(PF_BRIDGE, NF_BR_LOCAL_OUT, skb, NULL, skb->dev,
			br_forward_finish);
}

static void __br_forward(const struct net_bridge_port *to, struct sk_buff *skb)
{
	struct net_device *indev;

	if (skb_warn_if_lro(skb)) {
		kfree_skb(skb);
		return;
	}

	/*linux-2.6.19*/
	/*patch from linux 2.4*/
	// Kaohj, bridged PPPoE
	if (to->dev->brpppoe && !( eth_hdr(skb)->h_proto == 0x8863 ||
		eth_hdr(skb)->h_proto == 0x8864)) {
		kfree_skb(skb);
//printk("%s the eth_hdr(skb)->h_proto is 0x%x\n", __func__, eth_hdr(skb)->h_proto);
//printk("the to->dev->brppoe is %d\n", to->dev->brpppoe);
//printk("%s. %d . %s skb->dev is %s  to->dev is %s\n", __FILE__, __LINE__, __func__, skb->dev->name, to->dev->name);
		//printk("%s : %d : %s\n", __FILE__, __LINE__, __func__);
		return;
	}

	indev = skb->dev;
	skb->dev = to->dev;
	skb_forward_csum(skb);

    //��ģʽ�·�WAN�ཻ������BR_NETFILTER QOS����
#ifdef CONFIG_BRIDGE_NETFILTER
    if(skb->nf_bridge)
        if(!(indev&&(indev->priv_flags&IFF_DOMAIN_WAN)) && 
            !(skb->dev&&(skb->dev->priv_flags&IFF_DOMAIN_WAN)))
            skb->nf_bridge->mask |= BRNF_NF_BRIDGE_NO_WAN;
#endif
    
#if defined(CONFIG_VLAN_MODE)
    if(vlan_forward(&skb, indev, skb->dev) == NF_DROP)
    {
        kfree_skb(skb);
        return;
    }
#endif

	NF_HOOK(PF_BRIDGE, NF_BR_FORWARD, skb, indev, skb->dev,
			br_forward_finish);
}

/* called with rcu_read_lock */
void br_deliver(const struct net_bridge_port *to, struct sk_buff *skb)
{
	if (should_deliver(to, skb)) {
		__br_deliver(to, skb);
		return;
	}

	kfree_skb(skb);
}

/* called with rcu_read_lock */
void br_forward(const struct net_bridge_port *to, struct sk_buff *skb)
{
	if (should_deliver(to, skb)) {
		__br_forward(to, skb);
		return;
	}

	kfree_skb(skb);
}

/* called under bridge lock */
static void br_flood(struct net_bridge *br, struct sk_buff *skb,
	void (*__packet_hook)(const struct net_bridge_port *p,
			      struct sk_buff *skb))
{
	struct net_bridge_port *p;
	struct net_bridge_port *prev;

	// Kevin
    #ifdef CONFIG_RTL8676_BRIDGE_FASTPATH  	
    int dir=DIR_LAN;
	if (skb->dev) {
		if (skb->dev->priv_flags & IFF_DOMAIN_WAN)//wan
			dir = DIR_WAN;		
		brgEntryDelete((unsigned short *)(eth_hdr(skb)->h_source), (unsigned short *)(eth_hdr(skb)->h_dest), dir);
	}
    #endif

	prev = NULL;

	list_for_each_entry_rcu(p, &br->port_list, list) {

		//AUG_DBG("p->dev->name: %s. p->dev->priv_flags: 0x%x\n", p->dev->name, p->dev->priv_flags);
		
		/*linux-2.6.19*/
		// Kaohj --- block multicast traffic from pvc to pvc
		// from_dev: put at br2684.c->br2684_push() to keep the device info we are arriving on
		// For routing case. The multicast source comes from pvc0(routing interface) and will
		// be flooded to pvc1(bridging interface).
		// Purpose: preventing multicast traffic from WAN (pvc) to another WAN (pvc)
		if (skb->from_dev && (skb->from_dev->priv_flags &
		 p->dev->priv_flags & IFF_DOMAIN_WAN)) {
		 	// Mason Yu		 	
		 	continue;
		}
		
		skb->floodflag = PACTET_FLOOD;
			   
#ifdef CONFIG_NEW_PORTMAPPING
		{
			if(0 == br_flood_check_portmap(skb, p))
				continue;
		}
#endif

		if (should_deliver(p, skb)) {
			if (prev != NULL) {
				struct sk_buff *skb2;

				if ((skb2 = skb_clone(skb, GFP_ATOMIC)) == NULL) {
					br->dev->stats.tx_dropped++;
					kfree_skb(skb);
					return;
				}
				
		        skb2->floodflag = PACTET_FLOOD;
				__packet_hook(prev, skb2);
			}

			prev = p;
		}
	}

	if (prev != NULL) {
		__packet_hook(prev, skb);
		return;
	}

	kfree_skb(skb);
}


/* called with rcu_read_lock */
void br_flood_deliver(struct net_bridge *br, struct sk_buff *skb)
{
	br_flood(br, skb, __br_deliver);
}

/* called under bridge lock */
void br_flood_forward(struct net_bridge *br, struct sk_buff *skb)
{
	br_flood(br, skb, __br_forward);
}

#if defined (CONFIG_RTL_IGMP_SNOOPING)
int bitmask_to_id(unsigned char val)
{
	int i;
	for (i=0; i<8; i++) {
		if (val & (1 <<i))
			break;
	}

	if(i>=8)
	{
		i=7;
	}
	return (i);
}

static void br_multicast(struct net_bridge *br, unsigned int fwdPortMask, struct sk_buff *skb, int clone, 
		  void (*__packet_hook)(const struct net_bridge_port *p, struct sk_buff *skb))
{
//	char i;
	struct net_bridge_port *prev;
	struct net_bridge_port *p, *n;
	unsigned short port_bitmask=0;
        if (clone) {
                struct sk_buff *skb2;
                                                                                                                                              
                if ((skb2 = skb_clone(skb, GFP_ATOMIC)) == NULL) {
                        br->dev->stats.tx_dropped++;
                        return;
                }
                                                                                                                                              
                skb = skb2;
        }
                                                                                                                                              
	prev = NULL;
	
	list_for_each_entry_safe(p, n, &br->port_list, list) {
		port_bitmask = (1 << p->port_no);
                if ((port_bitmask & fwdPortMask) && should_deliver(p, skb)) {
                        if (prev != NULL) {
                                struct sk_buff *skb2;
                                                                                                                                 
                                if ((skb2 = skb_clone(skb, GFP_ATOMIC)) == NULL) {
                                        br->dev->stats.tx_dropped++;
                                        kfree_skb(skb);
                                        return;
                                }
                                        
                                __packet_hook(prev, skb2);
                        }
                                                                                                                       
                        prev = p;
                }
	}

        if (prev != NULL) {
                __packet_hook(prev, skb);
                return;
        }

	kfree_skb(skb);
}

void br_multicast_deliver(struct net_bridge *br, unsigned int fwdPortMask, struct sk_buff *skb, int clone)
{
	br_multicast(br, fwdPortMask, skb, clone, __br_deliver);
}
void br_multicast_forward(struct net_bridge *br, unsigned int fwdPortMask, struct sk_buff *skb, int clone)
{
	br_multicast(br, fwdPortMask, skb, clone, __br_forward);
}

extern struct net_bridge *bridge0;
extern int ipMulticastFastFwd;
extern int needCheckMfc;
#if defined (CONFIG_IP_MROUTE)
extern int rtl865x_checkMfcCache(struct net *net,__be32 origin,__be32 mcastgrp);
#endif

#if defined(CONFIG_RTL_MLD_SNOOPING)
extern int re865x_getIpv6TransportProtocol(struct ipv6hdr* ipv6h);
#endif

int rtl865x_ipMulticastFastFwd(struct sk_buff *skb)
{
	const unsigned char *dest = NULL;
	unsigned char *ptr;
	struct iphdr *iph=NULL;
	unsigned char proto=0;
	unsigned char reserved=0;
	int ret=-1;
	struct udphdr *udphd = NULL;
	
	struct net_bridge_port *prev;
	struct net_bridge_port *p, *n;
	struct rtl_multicastDataInfo multicastDataInfo;
	struct rtl_multicastFwdInfo multicastFwdInfo;
	struct sk_buff *skb2;
	
	unsigned short port_bitmask=0;
//	#if defined (CONFIG_RTL_MLD_SNOOPING)
//	struct ipv6hdr * ipv6h=NULL;
//	#endif
	unsigned int fwdCnt;
	
	/*check fast forward enable or not*/
	if(ipMulticastFastFwd==0)
	{
		return -1;
	}
	
	/*check dmac is multicast or not*/
	dest=eth_hdr(skb)->h_dest;
	if((dest[0]&0x01)==0)
	{
		return -1;
	}
	
	//printk("%s:%d,dest is 0x%x-%x-%x-%x-%x-%x\n",__FUNCTION__,__LINE__,dest[0],dest[1],dest[2],dest[3],dest[4],dest[5]);
	if(igmpsnoopenabled==0)
	{
		return -1;
	}
	
	/*check bridge0 exist or not*/
	if((bridge0==NULL) ||(bridge0->dev->flags & IFF_PROMISC))
	{
		return -1;
	}

	if((skb->dev==NULL) ||(strncmp(skb->dev->name,RTL_PS_BR0_DEV_NAME,3)==0))
	{
		return -1;
	}

	/*check igmp snooping enable or not, and check dmac is ipv4 multicast mac or not*/
	if  ((dest[0]==0x01) && (dest[1]==0x00) && (dest[2]==0x5e))
	{
		//printk("%s:%d,skb->dev->name is %s\n",__FUNCTION__,__LINE__,skb->dev->name );
		ptr=(unsigned char *)eth_hdr(skb)+12;
		/*check vlan tag exist or not*/
		if(*(int16 *)(ptr)==(int16)htons(0x8100))
		{
			ptr=ptr+4;
		}

		/*check it's ipv4 packet or not*/
		if(*(int16 *)(ptr)!=(int16)htons(ETH_P_IP))
		{
			return -1;
		}
		
		iph=(struct iphdr *)(ptr+2);
	
		if(iph->daddr== 0xEFFFFFFA)
		{
			/*for microsoft upnp*/
			reserved=1;
		}
		
		/*only speed up udp and tcp*/
		proto =  iph->protocol;  

        //for RIP
        if(proto == IPPROTO_UDP)
    	{
    	    udphd = (struct udphdr *)((u8*)iph+sizeof(struct iphdr));
			if(udphd && (udphd->dest == htons(RIP_PROTO_PORT)))
				reserved=1;
    	}
		
		//printk("%s:%d,proto is %d\n",__FUNCTION__,__LINE__,proto);
		 if(((proto ==IPPROTO_UDP) ||(proto ==IPPROTO_TCP)) && (reserved ==0))
		{
			
			#if defined (CONFIG_IP_MROUTE)
			/*multicast data comes from wan, need check multicast forwardig cache*/
			if((strncmp(skb->dev->name,RTL_PS_WAN0_DEV_NAME,4)==0) && needCheckMfc )
			{
				if(rtl865x_checkMfcCache(&init_net,iph->saddr,iph->daddr)!=0)
				{
					return -1;
				}
			}
			#endif
			
			multicastDataInfo.ipVersion=4;
			multicastDataInfo.sourceIp[0]=  (unsigned int)(iph->saddr);
			multicastDataInfo.groupAddr[0]=  (unsigned int)(iph->daddr);
			
			ret= rtl_getMulticastDataFwdInfo(brIgmpModuleIndex, &multicastDataInfo, &multicastFwdInfo);
			//printk("%s:%d,ret is %d\n",__FUNCTION__,__LINE__,ret);
			if(ret!=0)
			{

				return -1;
			}


			//printk("%s:%d,br0SwFwdPortMask is 0x%x,multicastFwdInfo.fwdPortMask is 0x%x\n",__FUNCTION__,__LINE__,br0SwFwdPortMask,multicastFwdInfo.fwdPortMask);
			#if defined (CONFIG_RTL_HARDWARE_MULTICAST)
			if((skb->srcVlanId!=0) && (skb->srcPort!=0xFFFF))
			{
				/*multicast data comes from ethernet port*/
				if( (br0SwFwdPortMask & multicastFwdInfo.fwdPortMask)==0)
				{
					/*hardware forwarding ,let slow path handle packets trapped to cpu*/
					return -1;
				}
			}
			#endif
			/*bugid:48678 �鲥���ݴ�vlan�������޷���������*/
			list_for_each_entry_safe(p, n, &bridge0->port_list, list) 
			{
				port_bitmask = (1 << p->port_no);
				if ((port_bitmask & multicastFwdInfo.fwdPortMask) && (skb->dev != p->dev && p->state == BR_STATE_FORWARDING)) 
				{
					/*��������м�����Ŀ�������vlanͷ*/
					if (skb->protocol == __constant_htons(ETH_P_8021Q)) {
		                   unsigned short proto;
		                   struct vlan_hdr *vhdr = (struct vlan_hdr *)(skb->data);
		        		   proto = vhdr->h_vlan_encapsulated_proto;
		                   skb = skb_share_check(skb, GFP_ATOMIC);

		                   if (skb) {
		                            memmove(skb->data - ETH_HLEN + VLAN_HLEN,
		                                     skb->data - ETH_HLEN, 12);
		                            skb_pull(skb, VLAN_HLEN);
		                            skb->mac_header+= VLAN_HLEN;
		                   }
		                   /* ����skb->protocolΪvlan��ǩ����ӵ�Э�����ͣ�֮ǰ��protocolΪ0x8100(��ETH_P_8021Q)*/
		                   skb->protocol = proto;
		         	}
					break;
				}
			}
			skb_push(skb, ETH_HLEN);
			
			prev = NULL;
			fwdCnt=0;
			list_for_each_entry_safe(p, n, &bridge0->port_list, list) 
			{
				port_bitmask = (1 << p->port_no);
				if ((port_bitmask & multicastFwdInfo.fwdPortMask) && (skb->dev != p->dev && p->state == BR_STATE_FORWARDING)) 
				{
					if (prev != NULL) 
					{                                                                                       
						if ((skb2 = skb_clone(skb, GFP_ATOMIC)) == NULL) 
						{
							bridge0->dev->stats.tx_dropped++;
							kfree_skb(skb);
							return 0;
						}
						skb2->dev=prev->dev;
						//printk("%s:%d,prev->dev->name is %s\n",__FUNCTION__,__LINE__,prev->dev->name);
						#if defined(CONFIG_COMPAT_NET_DEV_OPS)
						prev->dev->hard_start_xmit(skb2, prev->dev);
						#else
						prev->dev->netdev_ops->ndo_start_xmit(skb2,prev->dev);
						#endif                  
						fwdCnt++;
					}
						                                                                               
					prev = p;
				}
			}

			if (prev != NULL) 
			{
				skb->dev=prev->dev;
				//printk("%s:%d,prev->dev->name is %s\n",__FUNCTION__,__LINE__,prev->dev->name);
			       #if defined(CONFIG_COMPAT_NET_DEV_OPS)
				prev->dev->hard_start_xmit(skb, prev->dev);
				#else
				prev->dev->netdev_ops->ndo_start_xmit(skb,prev->dev);
				#endif                            
				fwdCnt++;
			}

			if(fwdCnt==0)
			{
				/*avoid memory leak*/
				skb_pull(skb, ETH_HLEN);
				return -1;
			}
			
			return 0;

		}

	}

#if 0 //defined (CONFIG_RTL_MLD_SNOOPING)
	/*check igmp snooping enable or not, and check dmac is ipv4 multicast mac or not*/
	if  ((dest[0]==0x33) && (dest[1]==0x33) && (dest[2]!=0xff))
	{
		struct net_bridge_port *p;
		if(mldSnoopEnabled==0)
		{
			return -1;
		}
		
		/*due to ipv6 passthrough*/
		p= rcu_dereference(skb->dev->br_port);
		if(p==NULL)
		{
			return -1;
		}
		
		//printk("%s:%d,skb->dev->name is %s\n",__FUNCTION__,__LINE__,skb->dev->name );
		ptr=(unsigned char *)eth_hdr(skb)+12;
		/*check vlan tag exist or not*/
		if(*(int16 *)(ptr)==(int16)htons(0x8100))
		{
			ptr=ptr+4;
		}

		/*check it's ipv6 packet or not*/
		if(*(int16 *)(ptr)!=(int16)htons(ETH_P_IPV6))
		{
			return -1;
		}
		
		ipv6h=(struct ipv6hdr *)(ptr+2);
		proto =  re865x_getIpv6TransportProtocol(ipv6h);
		
		//printk("%s:%d,proto is %d\n",__FUNCTION__,__LINE__,proto);
		 if((proto ==IPPROTO_UDP) ||(proto ==IPPROTO_TCP))
		{			
			multicastDataInfo.ipVersion=6;
			memcpy(&multicastDataInfo.sourceIp, &ipv6h->saddr, sizeof(struct in6_addr));
			memcpy(&multicastDataInfo.groupAddr, &ipv6h->daddr, sizeof(struct in6_addr));
			/*
			printk("%s:%d,sourceIp is %x-%x-%x-%x\n",__FUNCTION__,__LINE__,
				multicastDataInfo.sourceIp[0],multicastDataInfo.sourceIp[1],multicastDataInfo.sourceIp[2],multicastDataInfo.sourceIp[3]);
			printk("%s:%d,groupAddr is %x-%x-%x-%x\n",__FUNCTION__,__LINE__,
				multicastDataInfo.groupAddr[0],multicastDataInfo.groupAddr[1],multicastDataInfo.groupAddr[2],multicastDataInfo.groupAddr[3]);
			*/
			ret= rtl_getMulticastDataFwdInfo(brIgmpModuleIndex, &multicastDataInfo, &multicastFwdInfo);
			//printk("%s:%d,ret is %d\n",__FUNCTION__,__LINE__,ret);
			if(ret!=0)
			{
				if(multicastFwdInfo.unknownMCast)
				{
					multicastFwdInfo.fwdPortMask=0xFFFFFFFF;
				}
				else
				{
					return -1;
				}
	
			}
			
			//printk("%s:%d,multicastFwdInfo.fwdPortMask is 0x%x\n",__FUNCTION__,__LINE__,multicastFwdInfo.fwdPortMask);
			
			skb_push(skb, ETH_HLEN);

			prev = NULL;
			fwdCnt=0;
			list_for_each_entry_safe(p, n, &bridge0->port_list, list) 
			{
				port_bitmask = (1 << p->port_no);
				if ((port_bitmask & multicastFwdInfo.fwdPortMask) && (skb->dev != p->dev && p->state == BR_STATE_FORWARDING)) 
				{
					if (prev != NULL) 
					{                                                                                       
						if ((skb2 = skb_clone(skb, GFP_ATOMIC)) == NULL) 
						{
							kfree_skb(skb);
							return 0;
						}
						skb2->dev=prev->dev;
						//printk("%s:%d,prev->dev->name is %s\n",__FUNCTION__,__LINE__,prev->dev->name);
						#if defined(CONFIG_COMPAT_NET_DEV_OPS)
						prev->dev->hard_start_xmit(skb2, prev->dev);
						#else
						prev->dev->netdev_ops->ndo_start_xmit(skb2,prev->dev);
						#endif                             
						fwdCnt++;
					}
						                                                                               
					prev = p;
				}
			}

			if (prev != NULL) 
			{
				skb->dev=prev->dev;
				//printk("%s:%d,prev->dev->name is %s\n",__FUNCTION__,__LINE__,prev->dev->name);
			       #if defined(CONFIG_COMPAT_NET_DEV_OPS)
				prev->dev->hard_start_xmit(skb, prev->dev);
				#else
				prev->dev->netdev_ops->ndo_start_xmit(skb,prev->dev);
				#endif                            
				fwdCnt++;
			}
			
			if(fwdCnt==0)
			{
				//printk("%s:%d\n",__FUNCTION__,__LINE__);
				/*avoid memory leak*/
				skb_pull(skb, ETH_HLEN);
				return -1;	
			}
				
			return 0;	
		}

	}
#endif

	return -1;	
}

#endif



#ifdef CONFIG_NEW_PORTMAPPING
/**
check wether should skb flood to the port in the portmapping association

return : 0, should continue to the next port;
		 1, go ahead;
**/
static inline int br_flood_check_portmap(struct sk_buff *skb, struct net_bridge_port *p)
{

		if(skb->switch_port && p->fgroup)
		{
			int32_t sw_num;

			uint16_t tmp_fgroup;

#ifdef CONFIG_RTL_8676HWNAT
	#ifdef	CONFIG_RTL_MULTI_ETH_WAN
			if((p->dev->priv_flags & IFF_DOMAIN_WAN) && (p->dev->priv_flags & IFF_OSMUX))
				tmp_fgroup = *(p->fgroup) >> 1;
			else
				tmp_fgroup = *(p->fgroup);
	#else
			tmp_fgroup = *(p->fgroup);
	#endif
#else		
			tmp_fgroup = *(p->fgroup);
#endif
//if( skb->switch_port[0] == 'e')//
if(alias_name_are_eq(skb->switch_port,ALIASNAME_ETH,ALIASNAME_ETH0DOT))
{
#ifdef CONFIG_RTL_8676HWNAT
                           
             TOKEN_NUM(skb->switch_port,&sw_num);             
             sw_num-=simple_strtol((const char *)ORIGINATE_NUM, NULL, 0);
            // sw_num-=atoi_num(ORIGINATE_NUM);
            //get the 6th char of eth0.*
				//sw_num = (int32_t)((skb->switch_port)[5] - '0' - 2);//
 
#else		
                TOKEN_NUM(skb->switch_port,&sw_num);            
				//get the 8th char of eth0_sw*
				//sw_num = (int32_t)((skb->switch_port)[7] - '0');//		
#endif

}
//else if(skb->switch_port[5] == '-')//
else if(alias_name_is_eq(CMD_NCMP,skb->switch_port,ALIASNAME_WLAN0_VAP))
{
        TOKEN_NUM(skb->switch_port,&sw_num);
        sw_num+=5;
//        sw_num = 5 + (skb->switch_port)[9] - '0';//

}

			else
				sw_num = 4;

			if(!(tmp_fgroup & (1 << sw_num)))
			{
				return 0;
			}
		}

		return 1;
	
}

#endif	

