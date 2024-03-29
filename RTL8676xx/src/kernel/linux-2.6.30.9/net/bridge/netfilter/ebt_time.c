/*
  Description: EBTables time match extension kernelspace module.
  Authors:  Song Wang <songw@broadcom.com>, ported from netfilter/iptables
            The following is the original disclaimer.

  This is a module which is used for time matching
  It is using some modified code from dietlibc (localtime() function)
  that you can find at http://www.fefe.de/dietlibc/
  This file is distributed under the terms of the GNU General Public
  License (GPL). Copies of the GPL can be obtained from: ftp://prep.ai.mit.edu/pub/gnu/GPL
  2001-05-04 Fabrice MARIE <fabrice@netfilter.org> : initial development.
  2001-21-05 Fabrice MARIE <fabrice@netfilter.org> : bug fix in the match code,
     thanks to "Zeng Yu" <zengy@capitel.com.cn> for bug report.
  2001-26-09 Fabrice MARIE <fabrice@netfilter.org> : force the match to be in LOCAL_IN or PRE_ROUTING only.
  2001-30-11 Fabrice : added the possibility to use the match in FORWARD/OUTPUT with a little hack,
     added Nguyen Dang Phuoc Dong <dongnd@tlnet.com.vn> patch to support timezones.
*/

#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/netfilter_bridge/ebtables.h>
#include <linux/netfilter_bridge/ebt_time.h>
#include <linux/time.h>
#include <linux/netfilter/x_tables.h>

static unsigned char debug;
#if 0
MODULE_PARM(debug, "0-1b");
MODULE_PARM_DESC(debug, "debug=1 is turn on debug messages");
MODULE_AUTHOR("Song Wang <songw@broadcom.com>");
MODULE_DESCRIPTION("Match timestamp");
#endif
MODULE_LICENSE("GPL");

#define DEBUG_MSG(...) if (debug) printk (KERN_DEBUG "ebt_time: " __VA_ARGS__)

struct tm
{
	int tm_sec;                   /* Seconds.     [0-60] (1 leap second) */
	int tm_min;                   /* Minutes.     [0-59] */
	int tm_hour;                  /* Hours.       [0-23] */
	int tm_mday;                  /* Day.         [1-31] */
	int tm_mon;                   /* Month.       [0-11] */
	int tm_year;                  /* Year - 1900.  */
	int tm_wday;                  /* Day of week. [0-6] */
	int tm_yday;                  /* Days in year.[0-365] */
	int tm_isdst;                 /* DST.         [-1/0/1]*/

	long int tm_gmtoff;           /* we don't care, we count from GMT */
	const char *tm_zone;          /* we don't care, we count from GMT */
};

void localtime(const time_t *timepr, struct tm *r);

static bool
ebt_time_mt(const struct sk_buff *skb, const struct xt_match_param *par)
{
    const struct ebt_time_info *info = par->matchinfo;

	struct tm currenttime;                          /* time human readable */
	u_int8_t days_of_week[7] = {64, 32, 16, 8, 4, 2, 1};
	u_int16_t packet_time;
	struct timeval kerneltimeval;
	time_t packet_local_time;

	/* if kerneltime=1, we don't read the skb->timestamp but kernel time instead */
	/* we all use kernel time ,because time precision is not so important, so we not need modify skb struct*/
/*
	if (info->kerneltime)
	{
*/
		do_gettimeofday(&kerneltimeval);
		packet_local_time = kerneltimeval.tv_sec;
/*
	}
	else
		packet_local_time = skb->stamp.tv_sec;
*/
	/* Transform the timestamp of the packet, in a human readable form */
	localtime(&packet_local_time, &currenttime);
	DEBUG_MSG("currenttime: Y-%d M-%d D-%d H-%d M-%d S-%d, Day: W-%d\n",
		currenttime.tm_year, currenttime.tm_mon, currenttime.tm_mday,
		currenttime.tm_hour, currenttime.tm_min, currenttime.tm_sec,
		currenttime.tm_wday);

	/* check if we match this timestamp, we start by the days... */
	if (info->days_match != 0) {
		if ((days_of_week[currenttime.tm_wday] & info->days_match) != days_of_week[currenttime.tm_wday])
			return false; /* the day doesn't match */
	}

	/* ... check the time now */
	packet_time = (currenttime.tm_hour * 60) + currenttime.tm_min;
	if ((packet_time < info->time_start) || (packet_time > info->time_stop))
		return false;

	/* here we match ! */
	return true;    
}

static bool ebt_time_mt_check(const struct xt_mtchk_param *par)
{
    struct ebt_time_info *info = par->matchinfo;
    
	/* we use the kerneltime if we are in forward or output */
	info->kerneltime = 1;

	/* Now check the coherence of the data ... */
	if ((info->time_start > 1439) ||        /* 23*60+59 = 1439*/
	    (info->time_stop  > 1439))
	{
		printk(KERN_WARNING "ebt_time: invalid argument\n");
		return false;
	}

	return true;
    
}

static struct xt_match ebt_time_mt_reg __read_mostly = {
	.name		= EBT_TIME_MATCH,
	.revision	= 0,
	.family		= NFPROTO_BRIDGE,
	.match		= ebt_time_mt,
	.checkentry	= ebt_time_mt_check,
	.matchsize	= XT_ALIGN(sizeof(struct ebt_time_info)),
	.me		= THIS_MODULE,
};

static int __init init(void)
{
	DEBUG_MSG("ebt_time loading\n");
	return xt_register_match(&ebt_time_mt_reg);
}

static void __exit fini(void)
{
	xt_unregister_match(&ebt_time_mt_reg);
	DEBUG_MSG("ebt_time unloaded\n");
}

module_init(init);
module_exit(fini);


/* The part below is borowed and modified from dietlibc */

/* seconds per day */
#define SPD 24*60*60

void localtime(const time_t *timepr, struct tm *r) {
	time_t i;
	time_t timep;
	extern struct timezone sys_tz;
	const unsigned int __spm[12] =
		{ 0,
		  (31),
		  (31+28),
		  (31+28+31),
		  (31+28+31+30),
		  (31+28+31+30+31),
		  (31+28+31+30+31+30),
		  (31+28+31+30+31+30+31),
		  (31+28+31+30+31+30+31+31),
		  (31+28+31+30+31+30+31+31+30),
		  (31+28+31+30+31+30+31+31+30+31),
		  (31+28+31+30+31+30+31+31+30+31+30),
		};
	register time_t work;

	timep = (*timepr) - (sys_tz.tz_minuteswest * 60);
	work=timep%(SPD);
	r->tm_sec=work%60; work/=60;
	r->tm_min=work%60; r->tm_hour=work/60;
	work=timep/(SPD);
	r->tm_wday=(4+work)%7;
	for (i=1970; ; ++i) {
		register time_t k= (!(i%4) && ((i%100) || !(i%400)))?366:365;
		if (work>k)
			work-=k;
		else
			break;
	}
	r->tm_year=i-1900;
	for (i=11; i && __spm[i]>work; --i) ;
	r->tm_mon=i;
	r->tm_mday=work-__spm[i]+1;
}
