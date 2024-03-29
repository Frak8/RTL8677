/*-
 * Copyright (c) 2002-2004 Sam Leffler, Errno Consulting, Atheros
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
 * $Id: //depot/sw/carriersrc/release/1.3.1/src/madwifi/hal/main/linux/ah_osdep.h#1 $
 */
#ifndef _ATH_AH_OSDEP_H_
#define _ATH_AH_OSDEP_H_
/*
 * Atheros Hardware Access Layer (HAL) OS Dependent Definitions.
 */

/*
 * Starting with 2.6.4 the kernel supports a configuration option
 * to pass parameters in registers.  If this is enabled we must
 * mark all function interfaces in+out of the HAL to pass parameters
 * on the stack as this is the convention used internally (for
 * maximum portability).
 *
 * XXX A lot of functions have __ahdecl in their definition but not declaration
 * So compile breaks.
 * Since This is only an issue for i386 which has regparam enabled, instead of
 * changing the vanilla FC3 kernel, for now, remove the regparm
 * disabling.
 */

/* CONFIG_REGPARM has been removed from 2.6.20 onwards.
 * Since this is relevant only for i386 architectures, changing check to
 * i386.
 */

#ifdef __i386__
#define __ahdecl    __attribute__((regparm(0)))
#else
#define __ahdecl
#endif
#ifndef __packed
#define __packed    __attribute__((__packed__))
#endif

/*
 * When building the HAL proper we use no GPL-contaminated include
 * files and must define these types ourself.  Beware of these being
 * mismatched against the contents of <linux/types.h>
 */
#ifndef _LINUX_TYPES_H
/* NB: arm defaults to unsigned so be explicit */
typedef signed char int8_t;
typedef short int16_t;
typedef int int32_t;
typedef long long int64_t;

typedef unsigned char u_int8_t;
typedef unsigned short u_int16_t;
typedef unsigned int u_int32_t;
typedef unsigned long long u_int64_t;

typedef unsigned int size_t;
typedef unsigned int u_int;
typedef unsigned long u_long;
typedef void *va_list;
#endif

/*
 * Linux/BSD gcc compatibility shims.
 */
#define __printflike(_a,_b) \
    __attribute__ ((__format__ (__printf__, _a, _b)))
#define __va_list   va_list
#define OS_INLINE   __inline

typedef void* HAL_SOFTC;
typedef int HAL_BUS_TAG;
typedef void* HAL_BUS_HANDLE;
typedef u_int32_t HAL_BUS_ADDR;         /* XXX architecture dependent */

/*
 * Delay n microseconds.
 */
extern  void __ahdecl ath_hal_delay(int);
#define OS_DELAY(_n)    ath_hal_delay(_n)

extern  void* __ahdecl ath_hal_ioremap(u_int32_t addr, u_int32_t len);
#define OS_REMAP(_addr, _len)       ath_hal_ioremap(_addr, _len)

#define OS_MEMZERO(_a, _n)  ath_hal_memzero((_a), (_n))
extern void __ahdecl ath_hal_memzero(void *, size_t);
#define OS_MEMCPY(_d, _s, _n)   ath_hal_memcpy(_d,_s,_n)
extern void * __ahdecl ath_hal_memcpy(void *, const void *, size_t);

#ifndef abs
#define abs(_a)     __builtin_abs(_a)
#endif

struct ath_hal;
extern  u_int32_t __ahdecl ath_hal_getuptime(struct ath_hal *);
#define OS_GETUPTIME(_ah)   ath_hal_getuptime(_ah)

/*
 * Byte order/swapping support.
 */
#define AH_LITTLE_ENDIAN    1234
#define AH_BIG_ENDIAN       4321

#if AH_BYTE_ORDER == AH_BIG_ENDIAN
/*
 * This could be optimized but since we only use it for
 * a few registers there's little reason to do so.
 */
static inline u_int32_t
__bswap32(u_int32_t _x)
{
    return ((u_int32_t)(
          (((const u_int8_t *)(&_x))[0]    ) |
          (((const u_int8_t *)(&_x))[1]<< 8) |
          (((const u_int8_t *)(&_x))[2]<<16) |
          (((const u_int8_t *)(&_x))[3]<<24))
    );
}
#define __bswap16(_x) ( (u_int16_t)( (((const u_int8_t *)(&_x))[0] ) |\
                         ( ( (const u_int8_t *)( &_x ) )[1]<< 8) ) )
#else
#define __bswap32(_x)	(_x)
#define __bswap16(_x)	(_x)
#endif

/*
 * Register read/write; we assume the registers will always
 * be memory-mapped.  Note that register accesses are done
 * using target-specific functions when debugging is enabled
 * (AH_DEBUG) or we are explicitly configured this way.  The
 * latter is used on some platforms where the full i/o space
 * cannot be directly mapped.
 *
 * The hardware registers are native little-endian byte order.
 * Big-endian hosts are handled by enabling hardware byte-swap
 * of register reads and writes at reset.  But the PCI clock
 * domain registers are not byte swapped!  Thus, on big-endian
 * platforms we have to byte-swap thoese registers specifically.
 * Most of this code is collapsed at compile time because the
 * register values are constants.
 */
#if AH_BYTE_ORDER == AH_BIG_ENDIAN

#ifdef __LINUX_ARM_ARCH__
/*
 * IXP platform needs a uin32
 */
#define _OS_REG_WRITE(_ah, _reg, _val) do {             \
    writel((_val),                                      \
            (uint32_t)((volatile u_int32_t *)((_ah)->ah_sh + (_reg)))); \
} while(0)
#define _OS_REG_READ(_ah, _reg) \
        readl((uint32_t)(volatile u_int32_t *)((_ah)->ah_sh + (_reg)))

#elif defined (__LINUX_MIPS32_ARCH__) || defined (__LINUX_MIPS64_ARCH__)
/*
 * Others want iomem *
 */
#define _OS_REG_WRITE(_ah, _reg, _val) do {             \
    writel((_val),                                      \
            ((volatile u_int32_t *)((_ah)->ah_sh + (_reg)))); \
} while(0)
#define _OS_REG_READ(_ah, _reg) \
        readl((volatile u_int32_t *)((_ah)->ah_sh + (_reg)))

#elif defined (AH_REGOPS_SWSWAP)

#define _OS_REG_WRITE(_ah, _reg, _val) do {             \
    writel(__bswap32((_val)),                                      \
            ((volatile u_int32_t *)((_ah)->ah_sh + (_reg)))); \
} while(0)
#define _OS_REG_READ(_ah, _reg) \
        __bswap32(readl((volatile u_int32_t *)((_ah)->ah_sh + (_reg))))
#else /*__LINUX_ARM_ARCH__*/
#define _OS_REG_WRITE(_ah, _reg, _val) do {				    \
	if ( (_reg) >= 0x4000 && (_reg) < 0x5000)			    \
		*((volatile u_int32_t *)((_ah)->ah_sh + (_reg))) =	    \
			__bswap32((_val));				    \
	else								    \
		*((volatile u_int32_t *)((_ah)->ah_sh + (_reg))) = (_val);  \
} while (0)
#define _OS_REG_READ(_ah, _reg) \
	(((_reg) >= 0x4000 && (_reg) < 0x5000) ? \
		__bswap32(*((volatile u_int32_t *)((_ah)->ah_sh + (_reg)))) : \
		*((volatile u_int32_t *)((_ah)->ah_sh + (_reg))))
#endif /*__LINUX_ARM_ARCH__*/


#else /* AH_LITTLE_ENDIAN */
#define _OS_REG_WRITE(_ah, _reg, _val) do { \
    *((volatile u_int32_t *)((_ah)->ah_sh + (_reg))) = (_val); \
} while (0)
#define _OS_REG_READ(_ah, _reg) \
    *((volatile u_int32_t *)((_ah)->ah_sh + (_reg)))
#endif /* AH_BYTE_ORDER */



#ifdef AR9100
#define REG_WRITE(_reg, _val)	*((volatile u_int32_t*)(_reg)) = (_val)
#define REG_READ(_reg)			*((volatile u_int32_t*)(_reg))
#define REG_SET_BIT(_r, _f)		REG_WRITE((_r), REG_READ(_r) | (_f))
#define REG_CLR_BIT(_r, _f)		REG_WRITE((_r), REG_READ(_r) & ~(_f))
#endif


#if defined(AH_DEBUG) || defined(AH_REGOPS_FUNC) || defined(AH_DEBUG_ALQ)
/* use functions to do register operations */
#define OS_REG_WRITE(_ah, _reg, _val)   ath_hal_reg_write(_ah, _reg, _val)
#define OS_REG_READ(_ah, _reg)      ath_hal_reg_read(_ah, _reg)

extern  void __ahdecl ath_hal_reg_write(struct ath_hal *ah,
        u_int reg, u_int32_t val);
extern  u_int32_t __ahdecl ath_hal_reg_read(struct ath_hal *ah, u_int reg);
#else
/* inline register operations */
#define OS_REG_WRITE(_ah, _reg, _val)   _OS_REG_WRITE(_ah, _reg, _val)
#define OS_REG_READ(_ah, _reg)      _OS_REG_READ(_ah, _reg)
#endif /* AH_DEBUG || AH_REGFUNC || AH_DEBUG_ALQ */

#ifdef AH_DEBUG_ALQ
extern  void __ahdecl OS_MARK(struct ath_hal *, u_int id, u_int32_t value);
#else
#define OS_MARK(_ah, _id, _v)
#endif

/*
 * Linux-specific attach/detach methods needed for module reference counting.
 *
 * XXX We can't use HAL_STATUS because the type isn't defined at this
 *     point (circular dependency); we wack the type and patch things
 *     up in the function.
 *
 * NB: These are intentionally not marked __ahdecl since they are
 *     compiled with the default calling convetion and are not called
 *     from within the HAL.
 */
#define AH_USE_EEPROM     0x00000001
extern  struct ath_hal *_ath_hal_attach(u_int16_t devid, HAL_SOFTC,
        HAL_BUS_TAG, HAL_BUS_HANDLE, u_int32_t flags, void* status);
extern  void ath_hal_detach(struct ath_hal *);


#ifdef AR9100
void ath_hal_ahb_mac_reset(void);
#endif

#endif /* _ATH_AH_OSDEP_H_ */
