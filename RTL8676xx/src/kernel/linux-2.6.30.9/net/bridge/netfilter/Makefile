#
# Makefile for the netfilter modules for Link Layer filtering on a bridge.
#

obj-$(CONFIG_BRIDGE_NF_EBTABLES) += ebtables.o

# tables
obj-$(CONFIG_BRIDGE_EBT_AROUTE) += ebtable_aroute.o
obj-$(CONFIG_BRIDGE_EBT_PROUTE) += ebtable_proute.o
obj-$(CONFIG_BRIDGE_EBT_BROUTE) += ebtable_broute.o
obj-$(CONFIG_BRIDGE_EBT_T_FILTER) += ebtable_filter.o
obj-$(CONFIG_BRIDGE_EBT_T_NAT) += ebtable_nat.o

#matches
obj-$(CONFIG_BRIDGE_EBT_802_3) += ebt_802_3.o
obj-$(CONFIG_BRIDGE_EBT_AMONG) += ebt_among.o
obj-$(CONFIG_BRIDGE_EBT_ARP) += ebt_arp.o
obj-$(CONFIG_BRIDGE_EBT_IP) += ebt_ip.o
obj-$(CONFIG_BRIDGE_EBT_IP6) += ebt_ip6.o
obj-$(CONFIG_BRIDGE_EBT_LIMIT) += ebt_limit.o
obj-$(CONFIG_BRIDGE_EBT_MARK) += ebt_mark_m.o
obj-$(CONFIG_BRIDGE_EBT_PKTTYPE) += ebt_pkttype.o
obj-$(CONFIG_BRIDGE_EBT_STP) += ebt_stp.o
obj-$(CONFIG_BRIDGE_EBT_TIME) += ebt_time.o
obj-$(CONFIG_BRIDGE_EBT_VLAN) += ebt_vlan.o
obj-$(CONFIG_BRIDGE_EBT_TBSVLAN) += ebt_tbs_vlan_m.o

# targets
obj-$(CONFIG_BRIDGE_EBT_ARPREPLY) += ebt_arpreply.o
obj-$(CONFIG_BRIDGE_EBT_MARK_T) += ebt_mark.o
obj-$(CONFIG_BRIDGE_EBT_DNAT) += ebt_dnat.o
obj-$(CONFIG_BRIDGE_EBT_REDIRECT) += ebt_redirect.o
obj-$(CONFIG_BRIDGE_EBT_SNAT) += ebt_snat.o
obj-$(CONFIG_BRIDGE_EBT_FTOS) += ebt_ftos.o
obj-$(CONFIG_BRIDGE_EBT_WMM_MARK) += ebt_wmm_mark.o
obj-$(CONFIG_BRIDGE_EBT_VLAN_T) += ebt_vlan_t.o
obj-$(CONFIG_BRIDGE_EBT_CLASSIFY)+= ebt_classify.o

# watchers
obj-$(CONFIG_BRIDGE_EBT_LOG) += ebt_log.o
obj-$(CONFIG_BRIDGE_EBT_ULOG) += ebt_ulog.o
obj-$(CONFIG_BRIDGE_EBT_NFLOG) += ebt_nflog.o
