/* Kernel module to match connection tracking information. */

/* (C) 1999-2001 Paul `Rusty' Russell
 * (C) 2002-2005 Netfilter Core Team <coreteam@netfilter.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/module.h>
#include <linux/skbuff.h>
#include <net/netfilter/nf_conntrack.h>
#include <linux/netfilter/x_tables.h>
#include <linux/netfilter/xt_state.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Rusty Russell <rusty@rustcorp.com.au>");
MODULE_DESCRIPTION("ip[6]_tables connection tracking state match module");
MODULE_ALIAS("ipt_state");
MODULE_ALIAS("ip6t_state");

static bool
state_mt(const struct sk_buff *skb, const struct xt_match_param *par)
{
	const struct xt_state_info *sinfo = par->matchinfo;
	enum ip_conntrack_info ctinfo;
	unsigned int statebit;

	if (nf_ct_is_untracked(skb))
		statebit = XT_STATE_UNTRACKED;
	else if (!nf_ct_get(skb, &ctinfo))
		statebit = XT_STATE_INVALID;
	else
		statebit = XT_STATE_BIT(ctinfo);

	return (sinfo->statemask & statebit);
}

static bool state_mt_check(const struct xt_mtchk_param *par)
{
	if (nf_ct_l3proto_try_module_get(par->match->family) < 0) {
		printk(KERN_WARNING "can't load conntrack support for "
				    "proto=%u\n", par->match->family);
		return false;
	}
	return true;
}

static void state_mt_destroy(const struct xt_mtdtor_param *par)
{
	nf_ct_l3proto_module_put(par->match->family);
}

#if defined(CONFIG_RTL_IPTABLES_RULE_2_ACL)
static int state_match2acl(const char *tablename,
			  const void *ip,
			  const struct xt_match *match,
			  void *matchinfo,
			  rtl865x_AclRule_t *acl_rule,
			  unsigned int *invflags)
{
	const struct xt_state_info *sinfo = (struct xt_state_info *)matchinfo;
	
	if(sinfo->statemask & XT_STATE_BIT(IP_CT_ESTABLISHED))
	{
		return RTL865X_ESTABLISH_RULE;
	}
	/*if state rule is added, don't add this rule to rtl865x ACL table...skip this rule now.*/
	return RTL865X_SKIP_THIS_RULE;
}
#endif

static struct xt_match state_mt_reg[] __read_mostly = {
	{
		.name		= "state",
		.family		= NFPROTO_IPV4,
		.checkentry	= state_mt_check,
		.match		= state_mt,
		.destroy	= state_mt_destroy,
		.matchsize	= sizeof(struct xt_state_info),
		.me		= THIS_MODULE,
#if defined(CONFIG_RTL_IPTABLES_RULE_2_ACL)
		.match2acl	= state_match2acl,
#endif
	},
	{
		.name		= "state",
		.family		= NFPROTO_IPV6,
		.checkentry	= state_mt_check,
		.match		= state_mt,
		.destroy	= state_mt_destroy,
		.matchsize	= sizeof(struct xt_state_info),
		.me		= THIS_MODULE,
#if defined(CONFIG_RTL_IPTABLES_RULE_2_ACL)
		.match2acl	= state_match2acl,
#endif
	},
};

static int __init state_mt_init(void)
{
	return xt_register_matches(state_mt_reg, ARRAY_SIZE(state_mt_reg));
}

static void __exit state_mt_exit(void)
{
	xt_unregister_matches(state_mt_reg, ARRAY_SIZE(state_mt_reg));
}

module_init(state_mt_init);
module_exit(state_mt_exit);
