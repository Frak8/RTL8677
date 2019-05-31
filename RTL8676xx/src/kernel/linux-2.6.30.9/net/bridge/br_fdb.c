/*
 *	Forwarding database
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
#include <linux/init.h>
#include <linux/rculist.h>
#include <linux/spinlock.h>
#include <linux/times.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/jhash.h>
#include <linux/random.h>
#include <asm/atomic.h>
#include <asm/unaligned.h>
#include "br_private.h"

#if defined (CONFIG_RTL_819X) || defined(CONFIG_RTL_8676HWNAT)
#include <net/rtl/rtl_types.h>
#include <net/rtl/rtl865x_fdb_api.h>

#if defined (CONFIG_RTL_LAYERED_DRIVER_L2)
#if defined (CONFIG_RTL865X_LANPORT_RESTRICTION)
#include "lan_restrict.h"
#endif
#endif
#if defined(CONFIG_RTL_LAYERED_DRIVER) && defined(CONFIG_RTL_LAYERED_DRIVER_L2)
#include <net/rtl/rtl_queue.h>
#include "../../drivers/net/rtl819x/AsicDriver/rtl865x_asicCom.h"
#include "../../drivers/net/rtl819x/AsicDriver/rtl865x_asicL2.h"
#include "../../drivers/net/rtl819x/l2Driver/rtl865x_fdb.h"
#endif
#endif

#if defined(CONFIG_RTL_819X) || defined(CONFIG_RTL_8676HWNAT)
static int fdb_entry_max = 2048;
static int fdb_entry_num = 0;
#endif

#ifdef CONFIG_RTL8676_BRIDGE_FASTPATH
#include "../../drivers/net/brg_shortcut.h" 
#endif

#ifdef CONFIG_RTL_IPTABLES_RULE_2_ACL
#include <linux/netfilter_ipv4/ip_tables.h>
#endif

#if defined (CONFIG_RTL_IGMP_SNOOPING)
#include <net/rtl/rtl865x_netif.h>
#include <net/rtl/rtl_nic.h>
extern int IGMPProxyOpened;
void add_igmp_ext_entry(	struct net_bridge_fdb_entry *fdb ,unsigned char *srcMac , unsigned char portComeIn);
void update_igmp_ext_entry(	struct net_bridge_fdb_entry *fdb ,unsigned char *srcMac , unsigned char portComeIn);
void del_igmp_ext_entry(	struct net_bridge_fdb_entry *fdb ,unsigned char *srcMac , unsigned char portComeIn );
static void br_igmp_fdb_expired(struct net_bridge_fdb_entry *ent);
#endif

static struct kmem_cache *br_fdb_cache __read_mostly;
static int fdb_insert(struct net_bridge *br, struct net_bridge_port *source,
		      const unsigned char *addr);

static u32 fdb_salt __read_mostly;

int __init br_fdb_init(void)
{
	br_fdb_cache = kmem_cache_create("bridge_fdb_cache",
					 sizeof(struct net_bridge_fdb_entry),
					 0,
					 SLAB_HWCACHE_ALIGN, NULL);
	if (!br_fdb_cache)
		return -ENOMEM;

	get_random_bytes(&fdb_salt, sizeof(fdb_salt));
	return 0;
}

void br_fdb_fini(void)
{
	kmem_cache_destroy(br_fdb_cache);
}


/* if topology_changing then use forward_delay (default 15 sec)
 * otherwise keep longer (default 5 minutes)
 */
static inline unsigned long hold_time(const struct net_bridge *br)
{
	return br->topology_change ? br->forward_delay : br->ageing_time;
}

static inline int has_expired(const struct net_bridge *br,
				  const struct net_bridge_fdb_entry *fdb)
{
	return !fdb->is_static
		&& time_before_eq(fdb->ageing_timer + hold_time(br), jiffies);
}

static inline int br_mac_hash(const unsigned char *mac)
{
	/* use 1 byte of OUI cnd 3 bytes of NIC */
	u32 key = get_unaligned((u32 *)(mac + 2));
	return jhash_1word(key, fdb_salt) & (BR_HASH_SIZE - 1);
}

extern void rtl8676_fdb_delete(unsigned short l2Type,  const unsigned char *addr);
static inline void fdb_delete(struct net_bridge_fdb_entry *f)
{
	#ifdef CONFIG_RTL8676_BRIDGE_FASTPATH
	brgScDelete(f->addr.addr);
	#endif
#if defined(CONFIG_RTL_LAYERED_DRIVER) && defined(CONFIG_RTL_LAYERED_DRIVER_L2)
	rtl8676_fdb_delete(RTL865x_L2_TYPEII, f->addr.addr);
#endif

#if defined(CONFIG_RTL8676_Dynamic_ACL)
	rtl8676_del_L2Unicast_hwacc_agingout_mac(f->addr.addr);	
#endif		

	hlist_del_rcu(&f->hlist);
	br_fdb_put(f);

}

void br_fdb_changeaddr(struct net_bridge_port *p, const unsigned char *newaddr)
{
	struct net_bridge *br = p->br;
	int i;

	spin_lock_bh(&br->hash_lock);

	/* Search all chains since old address/hash is unknown */
	for (i = 0; i < BR_HASH_SIZE; i++) {
		struct hlist_node *h;
		hlist_for_each(h, &br->hash[i]) {
			struct net_bridge_fdb_entry *f;

			f = hlist_entry(h, struct net_bridge_fdb_entry, hlist);
			if (f->dst == p && f->is_local) {
				/* maybe another port has same hw addr? */
				struct net_bridge_port *op;
				list_for_each_entry(op, &br->port_list, list) {
					if (op != p &&
					    !compare_ether_addr(op->dev->dev_addr,
								f->addr.addr)) {
						f->dst = op;
						goto insert;
					}
				}

				/* delete old one */
				fdb_delete(f);
				goto insert;
			}
		}
	}
 insert:
	/* insert new address,  may fail if invalid address or dup. */
	fdb_insert(br, p, newaddr);

	spin_unlock_bh(&br->hash_lock);
}

#if defined (CONFIG_RTL_IGMP_SNOOPING)
static void br_igmp_fdb_expired(struct net_bridge_fdb_entry *ent)
{
	int i2;
	unsigned long igmp_walktimeout;	
	unsigned char *DA;
	unsigned char *SA;
	#if defined	(MCAST_TO_UNICAST)
	struct net_device *dev=NULL;
	#endif
	
	igmp_walktimeout = 	jiffies - IGMP_EXPIRE_TIME;	
	    
	//IGMP_EXPIRE_TIME
	for(i2=0 ; i2 < FDB_IGMP_EXT_NUM ; i2++)
	{
		if(ent->igmp_fdb_arr[i2].valid == 1){

			// when timeout expire
			if(time_before_eq(ent->igmp_fdb_arr[i2].ageing_time, igmp_walktimeout))
			{
				DEBUG_PRINT("%s:%d\n",__FUNCTION__,__LINE__);	
				SA = ent->igmp_fdb_arr[i2].SrcMac;					
				DEBUG_PRINT("expired src mac:%02x,%02x,%02x,%02x,%02x,%02x\n",
					SA[0],SA[1],SA[2],SA[3],SA[4],SA[5]);								

				DA = ent->addr.addr;					
				DEBUG_PRINT("fdb:%02x:%02x:%02x-%02x:%02x:%02x\n",
					DA[0],DA[1],DA[2],DA[3],DA[4],DA[5]);				



				/*---for process wlan client expired start---*/								
				#if defined	(MCAST_TO_UNICAST)
				dev = __dev_get_by_name(&init_net, RTL_PS_WLAN0_DEV_NAME);	
	
				
				if (dev) {			
					unsigned char StaMacAndGroup[20];
					memcpy(StaMacAndGroup, DA , 6);
					memcpy(StaMacAndGroup+6, SA, 6);
				#if defined(CONFIG_COMPAT_NET_DEV_OPS)
					if (dev->do_ioctl != NULL) {
						dev->do_ioctl(dev, (struct ifreq*)StaMacAndGroup, 0x8B81);
				#else
					if (dev->netdev_ops->ndo_do_ioctl != NULL) {
						dev->netdev_ops->ndo_do_ioctl(dev, (struct ifreq*)StaMacAndGroup, 0x8B81);
				#endif
						DEBUG_PRINT("(fdb expire) wlan0 ioctl to DEL! M2U entry da:%02x:%02x:%02x-%02x:%02x:%02x; sa:%02x:%02x:%02x-%02x:%02x:%02x\n",
							StaMacAndGroup[0],StaMacAndGroup[1],StaMacAndGroup[2],StaMacAndGroup[3],StaMacAndGroup[4],StaMacAndGroup[5],
							StaMacAndGroup[6],StaMacAndGroup[7],StaMacAndGroup[8],StaMacAndGroup[9],StaMacAndGroup[10],StaMacAndGroup[11]);
					}
				}
					
				#if defined (CONFIG_RTL_92D_SUPPORT)
				dev = __dev_get_by_name(&init_net, RTL_PS_WLAN1_DEV_NAME);	
	
				if (dev) {		
					unsigned char StaMacAndGroup[20];
					memcpy(StaMacAndGroup, DA , 6);
					memcpy(StaMacAndGroup+6, SA, 6);
				#if defined(CONFIG_COMPAT_NET_DEV_OPS)
					if (dev->do_ioctl != NULL) {
						dev->do_ioctl(dev, (struct ifreq*)StaMacAndGroup, 0x8B81);
				#else
					if (dev->netdev_ops->ndo_do_ioctl != NULL) {
						dev->netdev_ops->ndo_do_ioctl(dev, (struct ifreq*)StaMacAndGroup, 0x8B81);
				#endif
						DEBUG_PRINT("(fdb expire) wlan0 ioctl to DEL! M2U entry da:%02x:%02x:%02x-%02x:%02x:%02x; sa:%02x:%02x:%02x-%02x:%02x:%02x\n",
							StaMacAndGroup[0],StaMacAndGroup[1],StaMacAndGroup[2],StaMacAndGroup[3],StaMacAndGroup[4],StaMacAndGroup[5],
							StaMacAndGroup[6],StaMacAndGroup[7],StaMacAndGroup[8],StaMacAndGroup[9],StaMacAndGroup[10],StaMacAndGroup[11]);
					}
				}
				#endif
				#endif			
				/*---for process wlan client expired end---*/

			
				del_igmp_ext_entry(ent , SA , ent->igmp_fdb_arr[i2].port);

	
				if ( (ent->portlist & 0x7f)==0){
					ent->group_src &=  ~(1 << 1); // eth0 all leave
				}
			
				if ( (ent->portlist & 0x80)==0){
					ent->group_src &=  ~(1 << 2); // wlan0 all leave
				}
			
			
			}			
			
		}		
		
	}		
	
}
#endif
void br_fdb_cleanup(unsigned long _data)
{
	struct net_bridge *br = (struct net_bridge *)_data;
	unsigned long delay = hold_time(br);
	unsigned long next_timer = jiffies + br->forward_delay;
	int i;

	spin_lock_bh(&br->hash_lock);
	for (i = 0; i < BR_HASH_SIZE; i++) {
		struct net_bridge_fdb_entry *f;
		struct hlist_node *h, *n;

		hlist_for_each_entry_safe(f, h, n, &br->hash[i], hlist) {
			unsigned long this_timer;
			#if defined	(CONFIG_RTL_IGMP_SNOOPING)
			if(	f->is_static &&
				f->igmpFlag &&
				MULTICAST_MAC(f->addr.addr))
			{
										
				br_igmp_fdb_expired(f);
				if(time_before_eq(f->ageing_timer +300*HZ,  jiffies))
				{
					DEBUG_PRINT("fdb_delete:f->addr.addr is 0x%02x:%02x:%02x-%02x:%02x:%02x\n",
					f->addr.addr[0],f->addr.addr[1],f->addr.addr[2],f->addr.addr[3],f->addr.addr[4],f->addr.addr[5]);	
					fdb_delete(f);
				}
				
			}
			#endif

			if (f->is_static)
				continue;

			#ifdef CONFIG_RTL8676_BRIDGE_FASTPATH
			this_timer = f->ageing_timer + delay;
			if (time_before_eq(this_timer, jiffies))
			{
				unsigned long tick;
				if (brgScFind(f->dst->dev, f->addr.addr, &tick))
					f->ageing_timer = tick;
				else
				fdb_delete(f);
			}
			else if (time_before(this_timer, next_timer))
				next_timer = this_timer;
			
			continue;
#endif


			this_timer = f->ageing_timer + delay;
			if (time_before_eq(this_timer, jiffies))
				fdb_delete(f);
			else if (time_before(this_timer, next_timer))
				next_timer = this_timer;
			
		}
	}
	spin_unlock_bh(&br->hash_lock);

	/* Add HZ/4 to ensure we round the jiffies upwards to be after the next
	 * timer, otherwise we might round down and will have no-op run. */
	mod_timer(&br->gc_timer, round_jiffies(next_timer + HZ/4));
}

/* Completely flush all dynamic entries in forwarding database.*/
void br_fdb_flush(struct net_bridge *br)
{
	int i;

	spin_lock_bh(&br->hash_lock);
	for (i = 0; i < BR_HASH_SIZE; i++) {
		struct net_bridge_fdb_entry *f;
		struct hlist_node *h, *n;
		hlist_for_each_entry_safe(f, h, n, &br->hash[i], hlist) {
		#if defined (CONFIG_RTL_IGMP_SNOOPING)
			if(	f->is_static && 
				f->igmpFlag &&
				MULTICAST_MAC(f->addr.addr) )
			{
				br_igmp_fdb_expired(f);
				if(time_before_eq(f->ageing_timer + 300*HZ,  jiffies))
				{
					fdb_delete(f);
				}
			}
		#endif
			if (!f->is_static)
				fdb_delete(f);
		}
	}
	spin_unlock_bh(&br->hash_lock);
}

/* Flush all entries refering to a specific port.
 * if do_all is set also flush static entries
 */
void br_fdb_delete_by_port(struct net_bridge *br,
			   const struct net_bridge_port *p,
			   int do_all)
{
	int i;

	spin_lock_bh(&br->hash_lock);
	for (i = 0; i < BR_HASH_SIZE; i++) {
		struct hlist_node *h, *g;

		hlist_for_each_safe(h, g, &br->hash[i]) {
			struct net_bridge_fdb_entry *f
				= hlist_entry(h, struct net_bridge_fdb_entry, hlist);
			if (f->dst != p)
				continue;

			if (f->is_static && !do_all)
				continue;
			/*
			 * if multiple ports all have the same device address
			 * then when one port is deleted, assign
			 * the local entry to other port
			 */
			if (f->is_local) {
				struct net_bridge_port *op;
				list_for_each_entry(op, &br->port_list, list) {
					if (op != p &&
					    !compare_ether_addr(op->dev->dev_addr,
								f->addr.addr)) {
						f->dst = op;
						goto skip_delete;
					}
				}
			}

			fdb_delete(f);
		skip_delete: ;
		}
	}
	spin_unlock_bh(&br->hash_lock);
}

/* No locking or refcounting, assumes caller has no preempt (rcu_read_lock) */
__IRAM_PP_LOW struct net_bridge_fdb_entry *__br_fdb_get(struct net_bridge *br,
					  const unsigned char *addr)
{
	struct hlist_node *h;
	struct net_bridge_fdb_entry *fdb;

	hlist_for_each_entry_rcu(fdb, h, &br->hash[br_mac_hash(addr)], hlist) {
		if (!compare_ether_addr(fdb->addr.addr, addr)) {
			if (unlikely(has_expired(br, fdb)))
			{
                                #if defined(CONFIG_RTL_IPTABLES_FAST_PATH)
					#define RTL_LAN_FID 0
					#define FDB_DYNAMIC 0x2
                                int32 hwage = 0;
                                ether_addr_t *macAddr;
 
                                macAddr = (ether_addr_t *)(addr);
                                //lookup LAN mac agetimer
  				#ifdef CONFIG_RTL_819X_SWCORE
                                hwage = rtl_get_hw_fdb_age(RTL_LAN_FID, macAddr, FDB_DYNAMIC);
				#else
				hwage = 150;
				#endif
                                switch(hwage)
                                {
                                        case 450:
                                                fdb->ageing_timer = jiffies -hold_time(br) + 300*HZ;                                                   
                                                break;
                                        case 300:
                                                fdb->ageing_timer = jiffies  -hold_time(br) + 150*HZ;
                                                break;
                                        case 150:
                                                fdb->ageing_timer = jiffies  -hold_time(br) + 1*HZ;
                                                break;
                                        default:
                                                fdb = NULL;
                                }
                                
                                return fdb;
                                #endif

                                break;
                        }
			return fdb;
		}
	}

	return NULL;
}

/* Interface used by ATM hook that keeps a ref count */
__IRAM_PP_LOW struct net_bridge_fdb_entry *br_fdb_get(struct net_bridge *br,
					unsigned char *addr)
{
	struct net_bridge_fdb_entry *fdb;

	rcu_read_lock();
	fdb = __br_fdb_get(br, addr);
	if (fdb && !atomic_inc_not_zero(&fdb->use_count))
		fdb = NULL;
	rcu_read_unlock();
	return fdb;
}

static void fdb_rcu_free(struct rcu_head *head)
{
	struct net_bridge_fdb_entry *ent
		= container_of(head, struct net_bridge_fdb_entry, rcu);	
	kmem_cache_free(br_fdb_cache, ent);
#if defined(CONFIG_RTL_819X) || defined(CONFIG_RTL_8676HWNAT)
	fdb_entry_num--;
	if(fdb_entry_num < 0)
	{
		printk("fdb entry num error!!!!\n");
		fdb_entry_num = 0;
	}
#endif
}

/* Set entry up for deletion with RCU  */
void br_fdb_put(struct net_bridge_fdb_entry *ent)
{
	if (atomic_dec_and_test(&ent->use_count))
		call_rcu(&ent->rcu, fdb_rcu_free);
}

/*
 * Fill buffer with forwarding table records in
 * the API format.
 */
int br_fdb_fillbuf(struct net_bridge *br, void *buf,
		   unsigned long maxnum, unsigned long skip)
{
	struct __fdb_entry *fe = buf;
	int i, num = 0;
	struct hlist_node *h;
	struct net_bridge_fdb_entry *f;

	memset(buf, 0, maxnum*sizeof(struct __fdb_entry));

	rcu_read_lock();
	for (i = 0; i < BR_HASH_SIZE; i++) {
		hlist_for_each_entry_rcu(f, h, &br->hash[i], hlist) {
			if (num >= maxnum)
				goto out;

			if (has_expired(br, f))
				continue;

			if (skip) {
				--skip;
				continue;
			}

			/* convert from internal format to API */
			memcpy(fe->mac_addr, f->addr.addr, ETH_ALEN);

			/* due to ABI compat need to split into hi/lo */
			fe->port_no = f->dst->port_no;
			fe->port_hi = f->dst->port_no >> 8;

			fe->is_local = f->is_local;
			if (!f->is_static)
				fe->ageing_timer_value = jiffies_to_clock_t(jiffies - f->ageing_timer);
			++fe;
			++num;
		}
	}

 out:
	rcu_read_unlock();

	return num;
}

int br_fdb_ifname_get_mac(char *ifname,int port_no,struct list_head *plisthead)
{
	int i;
	struct net_device *dev=__dev_get_by_name(&init_net, ifname);
	struct net_bridge *br=netdev_priv(dev);
	struct hlist_node *h;
	struct net_bridge_fdb_entry *f;
	MAC_LIST *pstmac;
	
	
	rcu_read_lock();
	for (i = 0; i < BR_HASH_SIZE; i++) {
		hlist_for_each_entry_rcu(f, h, &br->hash[i], hlist) {

			if (has_expired(br, f))
				continue;
			if((f->is_local == 0)&&(f->dst->port_no==port_no))
			{
				pstmac = (MAC_LIST *)kmalloc(sizeof(MAC_LIST), GFP_ATOMIC);
				if(pstmac) {
					memcpy(pstmac->mac, f->addr.addr, ETH_ALEN);
					list_add(&pstmac->list, plisthead);
				}
			}
			
		}
	}

 out:
	rcu_read_unlock();
	return 1;
}
EXPORT_SYMBOL_GPL(br_fdb_ifname_get_mac);

static inline struct net_bridge_fdb_entry *fdb_find(struct hlist_head *head,
						    const unsigned char *addr)
{
	struct hlist_node *h;
	struct net_bridge_fdb_entry *fdb;

	hlist_for_each_entry_rcu(fdb, h, head, hlist) {
		if (!compare_ether_addr(fdb->addr.addr, addr))
			return fdb;
	}
	return NULL;
}


extern void rtl8676_fdb_new(unsigned char is_static, const unsigned char *addr);
extern void rtl8676_update_L2_check(void *orig_br_port, void *updated_br_port, const char *name, const unsigned char *addr);
#if defined (CONFIG_VLAN_MODE)
static struct net_bridge_fdb_entry *fdb_create(struct hlist_head *head,
					       struct net_bridge_port *source,
					       const unsigned char *addr,
					       int is_local,
					       unsigned short vid)
#else

static struct net_bridge_fdb_entry *fdb_create(struct hlist_head *head,
					       struct net_bridge_port *source,
					       const unsigned char *addr,
					       int is_local)
#endif					       
{
	struct net_bridge_fdb_entry *fdb;
#if defined (CONFIG_RTL_IGMP_SNOOPING)
	int i3;
#endif
#if defined(CONFIG_RTL_819X) || defined(CONFIG_RTL_8676HWNAT)
	if(fdb_entry_num >= fdb_entry_max)
		return NULL;
#endif

	fdb = kmem_cache_alloc(br_fdb_cache, GFP_ATOMIC);
	if (fdb) {
		memcpy(fdb->addr.addr, addr, ETH_ALEN);
		atomic_set(&fdb->use_count, 1);
#if defined(CONFIG_RTL_819X) || defined(CONFIG_RTL_8676HWNAT)
		fdb_entry_num++;
#endif
		hlist_add_head_rcu(&fdb->hlist, head);

		fdb->dst = source;
		fdb->is_local = is_local;
		fdb->is_static = is_local;
		fdb->ageing_timer = jiffies;

#if defined(CONFIG_VLAN_MODE)
        fdb->vid = vid;
#endif
        
/*August 20110531 Start*/
#ifdef CONFIG_NEW_PORTMAPPING
		//initial the fdb sw_name 
		fdb->sw_name = NULL;
#endif
/*August 20110531 End*/		

#if defined (CONFIG_RTL_IGMP_SNOOPING)	
		fdb->group_src = 0;
		fdb->igmpFlag=0;
		for(i3=0 ; i3<FDB_IGMP_EXT_NUM ;i3++){
			fdb->igmp_fdb_arr[i3].valid = 0;
			fdb->portUsedNum[i3] = 0;		
		}

#endif

#if defined(CONFIG_RTL_LAYERED_DRIVER) && defined(CONFIG_RTL_LAYERED_DRIVER_L2)
#ifdef CONFIG_RTL_MULTI_LAN_DEV
				{
			extern int32 _rtl865x_addFilterDatabaseEntry(uint16 l2Type, uint16 fid, ether_addr_t * macAddr, uint32 type, uint32 portMask, uint8 auth, uint8 SrcBlk);
			//printk("name: %s\n", source->dev->name);		
			if (!is_local && memcmp(source->dev->name, RTL_PS_ETH_NAME,3)!=0 
						   && memcmp(source->dev->name, RTL_DRV_WAN0_NETIF_NAME,4)!=0)
				{
				//printk("Add L2 %.02x %.02x %.02x %.02x on cpu port!\n", addr[2], addr[3], addr[4], addr[5]);
				_rtl865x_addFilterDatabaseEntry(
					(source->dev->priv_flags & (IFF_DOMAIN_WAN | IFF_DOMAIN_WLAN)) ? RTL865x_L2_TYPEIII :
					RTL865x_L2_TYPEI, 0 /*RTL_LAN_FID*/,	(ether_addr_t *)addr, 
													0 /*FDB_TYPE_FWD*/, 0x100/*CPU_PORT_MASK*/, FALSE, FALSE);
				}
			}

#endif
		rtl8676_fdb_new(fdb->is_static, addr);
#endif		
	}
	return fdb;
}

static int fdb_insert(struct net_bridge *br, struct net_bridge_port *source,
		  const unsigned char *addr)
{
	struct hlist_head *head = &br->hash[br_mac_hash(addr)];
	struct net_bridge_fdb_entry *fdb;
#if defined (CONFIG_RTL_IGMP_SNOOPING)	
	if(((addr[0]==0xff) && (addr[1]==0xff) && (addr[2]==0xff) && (addr[3]==0xff) && (addr[4]==0xff) && (addr[5]==0xff))||
		((addr[0]==0) && (addr[1]==0) && (addr[2]==0) && (addr[3]==0) && (addr[4]==0) && (addr[5]==0)))
	{
		return -EINVAL;
	}
#else	
	if (!is_valid_ether_addr(addr))
		return -EINVAL;
#endif
	fdb = fdb_find(head, addr);
	if (fdb) {
		/* it is okay to have multiple ports with same
		 * address, just use the first one.
		 */
		if (fdb->is_local)
			return 0;

		printk(KERN_WARNING "%s adding interface with same address "
		       "as a received packet\n",
		       source->dev->name);
		fdb_delete(fdb);
	}
    
#if defined(CONFIG_VLAN_MODE)
    if (!fdb_create(head, source, addr, 1, 0))
#else
	if (!fdb_create(head, source, addr, 1))
#endif        
		return -ENOMEM;

	return 0;
}

int br_fdb_insert(struct net_bridge *br, struct net_bridge_port *source,
		  const unsigned char *addr)
{
	int ret;

	spin_lock_bh(&br->hash_lock);
	ret = fdb_insert(br, source, addr);
	spin_unlock_bh(&br->hash_lock);
	return ret;
}

#ifdef CONFIG_NEW_PORTMAPPING
int br_fdb_add_swname(struct net_bridge *br, struct net_bridge_port *source, 
							const unsigned char *addr, char* phy_name)
{

	struct hlist_head *head = &br->hash[br_mac_hash(addr)];
	struct net_bridge_fdb_entry *fdb;

	fdb = fdb_find(head, addr);
	if(likely(fdb))
	{
		if((unsigned long)fdb->sw_name != (unsigned long)phy_name)
		{
			fdb->sw_name = phy_name;
			return 0;
		}
		else
			return 1;
	}
	return -2;
}
#endif

#if defined(CONFIG_VLAN_MODE)
void br_fdb_update(struct net_bridge *br, struct net_bridge_port *source,
		   const unsigned char *addr,
		   unsigned short vid)
#else
void br_fdb_update(struct net_bridge *br, struct net_bridge_port *source,
		   const unsigned char *addr)
#endif		   
{
	struct hlist_head *head = &br->hash[br_mac_hash(addr)];
	struct net_bridge_fdb_entry *fdb;

	/* some users want to always flood. */
	if (hold_time(br) == 0)
		return;

	/* ignore packets unless we are using this port */
	if (!(source->state == BR_STATE_LEARNING ||
	      source->state == BR_STATE_FORWARDING))
		return;

	fdb = fdb_find(head, addr);
	if (likely(fdb)) {
		/* attempt to update an entry for a local interface */
		if (unlikely(fdb->is_local)) {
			if (net_ratelimit())
				printk(KERN_WARNING "%s: received packet with "
				       " own address as source address %x-%x-%x-%x-%x-%x\n",
				       source->dev->name, fdb->addr.addr[0], fdb->addr.addr[1], fdb->addr.addr[2], fdb->addr.addr[3], fdb->addr.addr[4],
				       fdb->addr.addr[5]);
		} else {
			/* fastpath: update of existing entry */
#if defined(CONFIG_RTL_LAYERED_DRIVER) && defined(CONFIG_RTL_LAYERED_DRIVER_L2)
			rtl8676_update_L2_check(fdb->dst, source, source->dev->name, addr);
#endif			
			fdb->dst = source;
			fdb->ageing_timer = jiffies;
		}
	} else {
		spin_lock(&br->hash_lock);
		if (!fdb_find(head, addr))
        #if defined(CONFIG_VLAN_MODE)
		    fdb_create(head, source, addr, 0, vid);
        #else
			fdb_create(head, source, addr, 0);
        #endif
		/* else  we lose race and someone else inserts
		 * it first, don't bother updating
		 */
		spin_unlock(&br->hash_lock);
	}
}

#if defined (CONFIG_RTL_IGMP_SNOOPING)
int chk_igmp_ext_entry(
	struct net_bridge_fdb_entry *fdb ,
	unsigned char *srcMac)
{

	int i2;
	unsigned char *add;
	add = fdb->addr.addr;

	for(i2=0 ; i2 < FDB_IGMP_EXT_NUM ; i2++){
		if(fdb->igmp_fdb_arr[i2].valid == 1){
			if(!memcmp(fdb->igmp_fdb_arr[i2].SrcMac , srcMac ,6)){
				return 1;
			}
		}
	}
	return 0;
}

extern int bitmask_to_id(unsigned char val);

void add_igmp_ext_entry(	struct net_bridge_fdb_entry *fdb ,
	unsigned char *srcMac , unsigned char portComeIn)
{

//	if(fdb->igmp_ext_array == NULL)
//		return 0;

	int i2;
	unsigned char *add;
	add = fdb->addr.addr;	

	DEBUG_PRINT("add_igmp,DA=%02x:%02x:%02x:%02x:%02x:%02x ; SA=%02x:%02x:%02x:%02x:%02x:%02x\n",
		add[0],add[1],add[2],add[3],add[4],add[5],
		srcMac[0],srcMac[1],srcMac[2],srcMac[3],srcMac[4],srcMac[5]);
	
	for(i2=0 ; i2 < FDB_IGMP_EXT_NUM ; i2++){
		if(fdb->igmp_fdb_arr[i2].valid == 0){
			fdb->igmp_fdb_arr[i2].valid = 1	;
			fdb->igmp_fdb_arr[i2].ageing_time = jiffies ;			
			memcpy(fdb->igmp_fdb_arr[i2].SrcMac , srcMac ,6);
			fdb->igmp_fdb_arr[i2].port = portComeIn ;
			fdb->portlist |= portComeIn;
			fdb->portUsedNum[bitmask_to_id(portComeIn)]++;
			DEBUG_PRINT("portUsedNum[%d]=%d\n\n",bitmask_to_id(portComeIn) , fdb->portUsedNum[bitmask_to_id(portComeIn)]);
			return ;
		}
	}
	DEBUG_PRINT("%s:entry Rdy existed!!!\n", __FUNCTION__);	
}

void update_igmp_ext_entry(	struct net_bridge_fdb_entry *fdb ,
	unsigned char *srcMac , unsigned char portComeIn)
{
	int i2;
	unsigned char *add;
	add = fdb->addr.addr;	

		DEBUG_PRINT("update_igmp,DA=%02x:%02x:%02x:%02x:%02x:%02x ; SA=%02x:%02x:%02x:%02x:%02x:%02x\n",
		add[0],add[1],add[2],add[3],add[4],add[5],
		srcMac[0],srcMac[1],srcMac[2],srcMac[3],srcMac[4],srcMac[5]);

	for(i2=0 ; i2 < FDB_IGMP_EXT_NUM ; i2++){
		if(fdb->igmp_fdb_arr[i2].valid == 1){
			if(!memcmp(fdb->igmp_fdb_arr[i2].SrcMac , srcMac ,6)){

				fdb->igmp_fdb_arr[i2].ageing_time = jiffies ;
				//DEBUG_PRINT("update jiffies ok!\n");
				if(fdb->igmp_fdb_arr[i2].port != portComeIn){

					unsigned char port_orig = fdb->igmp_fdb_arr[i2].port ;					
					int index = bitmask_to_id(port_orig);

					fdb->portUsedNum[index]-- ;
					DEBUG_PRINT("(--) portUsedNum[%d]=%d\n",index , fdb->portUsedNum[index] );					
					if(fdb->portUsedNum[index] <= 0){
						fdb->portlist &= ~(port_orig);
						if(fdb->portUsedNum[index]< 0){
							DEBUG_PRINT("!! portNum[%d] < 0 at (update_igmp_ext_entry)\n",index);
							fdb->portUsedNum[index] = 0 ;
						}
					}					

					
					fdb->portUsedNum[bitmask_to_id(portComeIn)]++;
					DEBUG_PRINT("(++) portUsedNum[%d]=%d\n",bitmask_to_id(portComeIn) , fdb->portUsedNum[bitmask_to_id(portComeIn)] );										
					fdb->portlist |= portComeIn;						

					
					fdb->igmp_fdb_arr[i2].port = portComeIn ;					
					DEBUG_PRINT("	!!! portlist be updated:%x !!!!\n",fdb->portlist);
					
				}
				return ;
			}			
		}		
	}

	DEBUG_PRINT("%s: ...fail!!\n", __FUNCTION__);
}


void del_igmp_ext_entry(	struct net_bridge_fdb_entry *fdb ,unsigned char *srcMac , unsigned char portComeIn )
{
	int i2;
	unsigned char *add;
	add = fdb->addr.addr;	


				

	
	for(i2=0 ; i2 < FDB_IGMP_EXT_NUM ; i2++){
		if(fdb->igmp_fdb_arr[i2].valid == 1){
			if(!memcmp(fdb->igmp_fdb_arr[i2].SrcMac , srcMac ,6))
			{
				fdb->igmp_fdb_arr[i2].ageing_time -=  300*HZ; // make it expired	
				fdb->igmp_fdb_arr[i2].valid = 0;
				DEBUG_PRINT("\ndel_igmp_ext_entry,DA=%02x:%02x:%02x:%02x:%02x:%02x ; SA=%02x:%02x:%02x:%02x:%02x:%02x success!!!\n",
				add[0],add[1],add[2],add[3],add[4],add[5],
				srcMac[0],srcMac[1],srcMac[2],srcMac[3],srcMac[4],srcMac[5]);
				
				//DEBUG_PRINT("%s:success!!\n", __FUNCTION__);
				
				if(portComeIn != 0){
					int index;
					index = bitmask_to_id(portComeIn);
					fdb->portUsedNum[index]--;
					if(fdb->portUsedNum[index] <= 0){
						DEBUG_PRINT("portUsedNum[%d] == 0 ,update portlist from (%x)  " ,index ,fdb->portlist);
						fdb->portlist &= ~ portComeIn;
						DEBUG_PRINT("to (%x) \n" ,fdb->portlist);
						
						if(fdb->portUsedNum[index] < 0){
						DEBUG_PRINT("!! portUsedNum[%d]=%d < 0 at (del_igmp_ext_entry)  \n" ,index ,fdb->portUsedNum[index]);
						fdb->portUsedNum[index] = 0;
						}
					}else{
						DEBUG_PRINT("(del) portUsedNum[%d] = %d \n" ,index, fdb->portUsedNum[index]);
					}
				
				}	
				DEBUG_PRINT("\n");
				return ;
			}			
		}
	}

	DEBUG_PRINT("%s:entry not existed!!\n\n", __FUNCTION__);	
}




#endif

#if defined (CONFIG_RTL_LOCAL_PUBLIC)
int rtl865x_checkMacAddrLocation(unsigned char *addr, unsigned int *isElanMac, unsigned int *isWlanMac)
{
	struct net_device *brDev;
	struct net_bridge *br;
	struct net_bridge_fdb_entry *fdb;

	if((addr==NULL) || (isElanMac==NULL) || (isWlanMac==NULL))
	{
		return -1;
	}

	*isElanMac=0;
	*isWlanMac=0;

	brDev = __dev_get_by_name(&init_net,RTL_PS_BR0_DEV_NAME);
      	if ( brDev== NULL)
	{
		return -1;
	}

	if (!(brDev->priv_flags & IFF_EBRIDGE))
	{
		
		return -1;
	}
	else 
	{
		br = (struct net_bridge *)netdev_priv(brDev);
	}
	
	
	fdb=__br_fdb_get(br, addr);

	if(fdb==NULL)
	{
		return -1;
	}
	
	if(strncmp(fdb->dst->dev->name, RTL_PS_ETH_NAME,3)==0)
	{
		*isElanMac=1;
		
	}
	else if(strncmp(fdb->dst->dev->name, RTL_PS_WLAN_NAME,4)==0)
	{
		*isWlanMac=1;
	}	
	else
	{
		return -1;
	}

	return 0;
	
}
#endif


#if defined(CONFIG_IGMP_SNOOPING)

//#define MCAST_TO_UNICAST 1
#ifdef MCAST_TO_UNICAST
/*
 mcast to unicast for wireless
 add by kuangsuwen
*/
static int mc_config_mcast_to_unicast(int mode, unsigned char *dest, unsigned char *src)
{
#if 0
    printk("%s mcast2unicast entry: mcast_mac:"NMACQUAD_FMT", host_mac:"NMACQUAD_FMT"\n", 
    		mode? "add" : "del",
    		NMACQUAD(dest), NMACQUAD(src));
#endif

    struct net_device *dev = __dev_get_by_name("wlan0");
    if (dev) {
        unsigned char StaMacAndGroup[20];
        memcpy(StaMacAndGroup, dest, 6);
        memcpy(StaMacAndGroup+6, src, 6);
        if (dev->do_ioctl != NULL)
            dev->do_ioctl(dev, (struct ifreq*)StaMacAndGroup, mode ? 0x8B80 : 0x8B81);
    }
	
	return 0;
}
#endif

static void mc_debuginfo(struct net_bridge *br)
{
	struct net_bridge_mc_fdb_entry *dst;
	struct list_head *lh;
	int cnt = 0;

	printk("\n=============================================================\n");
	list_for_each_rcu(lh, &br->mc_list) {
	    dst = (struct net_bridge_mc_fdb_entry *) list_entry(lh, struct net_bridge_mc_fdb_entry, list);
		printk("brport:%d, mcast_mac:"NMACQUAD_FMT", host_mac:"NMACQUAD_FMT", filtermode:%d, srcip:"NIPQUAD_FMT"\n", 
			dst->dst->port_no, NMACQUAD(dst->addr.addr), NMACQUAD(dst->host.addr), 
			dst->src_entry.filt_mode, NIPQUAD(dst->src_entry.src.s_addr));
		cnt++;
	}
	printk("Total: %d\n", cnt);
	printk("=============================================================\n");
}

#if CONFIG_IGMP_SNOOPING_TIMEOUT
static void query_timeout(unsigned long ptr)
{
	struct net_bridge_mc_fdb_entry *dst;
	struct list_head *tmp;
	struct list_head *lh;
	struct net_bridge *br;

	br = (struct net_bridge *) ptr;

	spin_lock_bh(&br->mcl_lock);
	list_for_each_safe_rcu(lh, tmp, &br->mc_list) {
	    dst = (struct net_bridge_mc_fdb_entry *) list_entry(lh, struct net_bridge_mc_fdb_entry, list);
	    if (jiffies > dst->tstamp) {
    		list_del_rcu(&dst->list);
    		kfree(dst);
	    }
	}
	spin_unlock_bh(&br->mcl_lock);

	mod_timer(&br->igmp_timer, jiffies + TIMER_CHECK_TIMEOUT*HZ);
}
#endif
static int br_mc_fdb_update(struct net_bridge *br, struct net_bridge_port *prt, unsigned char *dest, unsigned char *host, int mode, struct in_addr *src)
{
	struct net_bridge_mc_fdb_entry *dst;
	struct list_head *lh;
	int ret = 0;
	int filt_mode;

    if(mode == SNOOP_IN_ADD)
      filt_mode = MCAST_INCLUDE;
    else
      filt_mode = MCAST_EXCLUDE;

	list_for_each_rcu(lh, &br->mc_list) {
	    dst = (struct net_bridge_mc_fdb_entry *) list_entry(lh, struct net_bridge_mc_fdb_entry, list);
	    if ((!memcmp(&dst->addr, dest, ETH_ALEN)) &&
            (src->s_addr == dst->src_entry.src.s_addr) &&
            (filt_mode == dst->src_entry.filt_mode) &&
	        (!memcmp(&dst->host, host, ETH_ALEN)) &&
            (dst->dst == prt))
        {
		    dst->tstamp = jiffies + QUERY_TIMEOUT*HZ;
		    ret = 1;
        }
	}

	return ret;
}

static struct net_bridge_mc_fdb_entry *br_mc_fdb_get(struct net_bridge *br, struct net_bridge_port *prt, unsigned char *dest, unsigned char *host, int mode, struct in_addr *src)
{
	struct net_bridge_mc_fdb_entry *dst;
	struct list_head *lh;
	int filt_mode;

    if(mode == SNOOP_IN_CLEAR)
      filt_mode = MCAST_INCLUDE;
    else
      filt_mode = MCAST_EXCLUDE;

	list_for_each_rcu(lh, &br->mc_list) {
	    dst = (struct net_bridge_mc_fdb_entry *) list_entry(lh, struct net_bridge_mc_fdb_entry, list);
	    if ((!memcmp(&dst->addr, dest, ETH_ALEN)) &&
            (!memcmp(&dst->host, host, ETH_ALEN)) &&
            (filt_mode == dst->src_entry.filt_mode) &&
            (dst->src_entry.src.s_addr == src->s_addr))
        {
    		if (dst->dst == prt)
    		    return dst;
	    }
	}

	return NULL;
}

extern mac_addr upnp_addr;

int br_mc_fdb_add(struct net_bridge *br, struct net_bridge_port *prt, unsigned char *dest, unsigned char *host, int mode, struct in_addr *src)
{
	struct net_bridge_mc_fdb_entry *mc_fdb;

    if (br_igmp_debug)
    {
    	mc_debuginfo(br);
        printk("--- add mc entry ---, mode:%d, dest:"NMACQUAD_FMT", host:"NMACQUAD_FMT"\n", mode, NMACQUAD(dest), NMACQUAD(host));
        //dump_stack();
    }

    if(!br || !prt || !dest || !host)
        return 0;

    if((SNOOP_IN_ADD != mode) && (SNOOP_EX_ADD != mode))
        return 0;

	if (!memcmp(dest, &upnp_addr, ETH_ALEN))
	    return 0;

	if (br_mc_fdb_update(br, prt, dest, host, mode, src))
	    return 0;

    #ifdef MCAST_TO_UNICAST
	if (br_mcast_to_unicast)
	{
	    if (SNOOP_EX_ADD == mode)
	        mc_config_mcast_to_unicast(1, dest, host);
	}
    #endif

	mc_fdb = kmalloc(sizeof(struct net_bridge_mc_fdb_entry), GFP_KERNEL);
	if (!mc_fdb)
	    return ENOMEM;
	memcpy(mc_fdb->addr.addr, dest, ETH_ALEN);
	memcpy(mc_fdb->host.addr, host, ETH_ALEN);
	memcpy(&mc_fdb->src_entry, src, sizeof(struct in_addr));
	mc_fdb->src_entry.filt_mode =
                  (mode == SNOOP_IN_ADD) ? MCAST_INCLUDE : MCAST_EXCLUDE;
	mc_fdb->dst = prt;
	mc_fdb->tstamp = jiffies + QUERY_TIMEOUT*HZ;

	spin_lock_bh(&br->mcl_lock);
	list_add_tail_rcu(&mc_fdb->list, &br->mc_list);
	spin_unlock_bh(&br->mcl_lock);
#if CONFIG_IGMP_SNOOPING_TIMEOUT
	if (!br->start_timer) {
    	init_timer(&br->igmp_timer);
	    br->igmp_timer.expires = jiffies + TIMER_CHECK_TIMEOUT*HZ;
	    br->igmp_timer.function = query_timeout;
	    br->igmp_timer.data = (unsigned long) br;
	    add_timer(&br->igmp_timer);
	    br->start_timer = 1;
	}
#endif
	return 1;
}

void br_mc_fdb_cleanup(struct net_bridge *br)
{
	struct net_bridge_mc_fdb_entry *dst;
	struct list_head *lh;
	struct list_head *tmp;

	spin_lock_bh(&br->mcl_lock);
	list_for_each_safe_rcu(lh, tmp, &br->mc_list) {
	    dst = (struct net_bridge_mc_fdb_entry *) list_entry(lh, struct net_bridge_mc_fdb_entry, list);
	    list_del_rcu(&dst->list);
	    kfree(dst);
	}
	spin_unlock_bh(&br->mcl_lock);
}

/*
void br_mc_fdb_remove_grp(struct net_bridge *br, struct net_bridge_port *prt, unsigned char *dest)
{
	struct net_bridge_mc_fdb_entry *dst;
	struct list_head *lh;
	struct list_head *tmp;

	spin_lock_bh(&br->mcl_lock);
	list_for_each_safe_rcu(lh, tmp, &br->mc_list) {
	    dst = (struct net_bridge_mc_fdb_entry *) list_entry(lh, struct net_bridge_mc_fdb_entry, list);
	    if ((!memcmp(&dst->addr, dest, ETH_ALEN)) && (dst->dst == prt)) {
		list_del_rcu(&dst->list);
		kfree(dst);
	    }
	}
	spin_unlock_bh(&br->mcl_lock);
}
*/

int br_mc_fdb_remove(struct net_bridge *br, struct net_bridge_port *prt, unsigned char *dest, unsigned char *host, int mode, struct in_addr *src)
{
	struct net_bridge_mc_fdb_entry *mc_fdb;

    if (br_igmp_debug)
    {
    	mc_debuginfo(br);
        printk("--- remove mc entry ---, mode:%d, dest:"NMACQUAD_FMT", host:"NMACQUAD_FMT"\n", mode, NMACQUAD(dest), NMACQUAD(host));
        //dump_stack();
    }
    if((SNOOP_IN_CLEAR != mode) && (SNOOP_EX_CLEAR != mode))
        return 0;

    #ifdef MCAST_TO_UNICAST
	if (br_mcast_to_unicast)
	{
	    if (SNOOP_EX_CLEAR == mode)
	        mc_config_mcast_to_unicast(0, dest, host);
	}
    #endif

	if ((mc_fdb = br_mc_fdb_get(br, prt, dest, host, mode, src))) {
	    spin_lock_bh(&br->mcl_lock);
	    list_del_rcu(&mc_fdb->list);
	    kfree(mc_fdb);
	    spin_unlock_bh(&br->mcl_lock);

	    return 1;
	}

	return 0;
}

struct net_bridge_mc_fdb_entry *br_mc_fdb_find(struct net_bridge *br, struct net_bridge_port *prt, unsigned char *dest, unsigned char *host, struct in_addr *src)
{
	struct net_bridge_mc_fdb_entry *dst;
	struct list_head *lh;

	list_for_each_rcu(lh, &br->mc_list) {
	    dst = (struct net_bridge_mc_fdb_entry *) list_entry(lh, struct net_bridge_mc_fdb_entry, list);
	    if ((!memcmp(&dst->addr, dest, ETH_ALEN)) &&
            (!memcmp(&dst->host, host, ETH_ALEN)) &&
            (dst->src_entry.src.s_addr == src->s_addr))
        {
    		if (dst->dst == prt)
    		    return dst;
	    }
	}

	return NULL;
}

#endif

