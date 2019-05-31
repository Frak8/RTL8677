/*-
 * Copyright (c) 2002-2004 Sam Leffler, Errno Consulting
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer,
 *    without modification.
 * 2. Redistributions in binary form must reproduce at minimum a disclaimer
 *    similar to the "NO WARRANTY" disclaimer below ("Disclaimer") and any
 *    redistribution must be conditioned upon including a substantially
 *    similar Disclaimer requirement for further binary redistribution.
 * 3. Neither the names of the above-listed copyright holders nor the names
 *    of any contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *
 * NO WARRANTY
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF NONINFRINGEMENT, MERCHANTIBILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
 * THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR SPECIAL, EXEMPLARY,
 * OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
 * IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGES.
 *
 * $Id: //depot/sw/carriersrc/branches/magpie_host_dev/split_src/host/madwifi/include/compat.h#7 $
 */
#ifndef _ATH_COMPAT_H_
#define _ATH_COMPAT_H_
/*
 * BSD/Linux compatibility shims.  These are used mainly to
 * minimize differences when importing necesary BSD code.
 */
#define ASF_BYTESZ    8           /* number of bits/byte */

#include <linux/kernel.h>
#ifndef asf_roundup
#define asf_roundup(x, y)   ((((x)+((y)-1))/(y))*(y))  /* to any y */
#endif

#define howmany(x, y)   (((x)+((y)-1))/(y))

/* Bit map related macros. */
#define asf_bitmap_setbit(a,i) ((a)[(i)/ASF_BYTESZ] |= 1<<((i)%ASF_BYTESZ))
#define asf_bitmap_clrbit(a,i) ((a)[(i)/ASF_BYTESZ] &= ~(1<<((i)%ASF_BYTESZ)))
#define isset(a,i)  ((a)[(i)/ASF_BYTESZ] & (1<<((i)%ASF_BYTESZ)))
#define isclr(a,i)  (((a)[(i)/ASF_BYTESZ] & (1<<((i)%ASF_BYTESZ))) == 0)

#ifndef adf_os_packed_post
#define adf_os_packed_post    __attribute__((__packed__))
#endif
#define __printflike(_a,_b) \
    __attribute__ ((__format__ (__printf__, _a, _b)))
//#define __offsetof(t,m) offsetof(t,m)

#ifndef ALIGNED_POINTER
/*
 * ALIGNED_POINTER is a boolean macro that checks whether an address
 * is valid to fetch data elements of type t from on this architecture.
 * This does not reflect the optimal alignment, just the possibility
 * (within reasonable limits). 
 *
 */
#define ALIGNED_POINTER(p,t)    1
#endif

#ifdef __KERNEL__
#include <asm/page.h>

#define KASSERT(exp, msg) do {          \
    if (unlikely(!(exp))) {         \
        printk msg;         \
        BUG();              \
    }                   \
} while (0)
#endif /* __KERNEL__ */

/*
 * NetBSD/FreeBSD defines for file version.
 */
#define __FBSDID(_s)
#define __KERNEL_RCSID(_n,_s)
#endif /* _ATH_COMPAT_H_ */
