/**
 * @ingroup adf_os_public
 * @file adf_os_lock.h
 * This file abstracts locking operations.
 */

#ifndef _ADF_CMN_OS_LOCK_H
#define _ADF_CMN_OS_LOCK_H

#include <adf_os_types.h>
#include <adf_os_lock_pvt.h>

/**
 * @brief Platform spinlock object
 */
typedef __adf_os_spinlock_t        adf_os_spinlock_t;

/**
 * @brief Platform mutex object
 */
typedef __adf_os_mutex_t        adf_os_mutex_t;

/**
 * @brief Initialize a mutex
 *
 * @param[in] m mutex to initialize
 */
static adf_os_inline a_status_t  adf_os_init_mutex(adf_os_mutex_t *m)
{
    return __adf_os_init_mutex(m);
}

/**
 * @brief Take the mutex
 *
 * @param[in] m mutex to take
 */
static adf_os_inline int adf_os_mutex_acquire(adf_os_mutex_t *m)
{
    return (__adf_os_mutex_acquire(m));
}

/**
 * @brief Give the mutex
 *
 * @param[in] m mutex to give
 */
static adf_os_inline void adf_os_mutex_release(adf_os_mutex_t *m)
{
    __adf_os_mutex_release(m);
}

/**
 * @brief Initialize a spinlock
 *
 * @param[in] lock spinlock object pointer
 */
static adf_os_inline a_status_t
adf_os_spinlock_init(adf_os_spinlock_t *lock)
{
    return __adf_os_spinlock_init(lock);
}


/**
 * @brief Acquire a spinlock by disabling the interrupts
 *
 * @param[in]  lock     spinlock object pointer
 * @param[out] flags    flags used to hold interrupt state
 */
static adf_os_inline void
adf_os_spin_lock_irq(adf_os_spinlock_t *lock, a_uint32_t *flags)
{
    __adf_os_spin_lock_irq(lock,flags);
}


/**
 * @brief Release a spinlock & restore the irq
 *
 * @param[in] lock  spinlock object pointer
 * @param[in] flags flags filled in by @ref adf_os_spin_lock_irq
 */
static adf_os_inline void
adf_os_spin_unlock_irq(adf_os_spinlock_t *lock, a_uint32_t *flags)
{
    __adf_os_spin_unlock_irq(lock,flags);
}


/**
 * @brief locks the spinlock mutex in soft irq context
 * 
 * @param[in] lock  spinlock object pointer
 */
static adf_os_inline void
adf_os_spin_lock_bh(adf_os_spinlock_t   *lock)
{
    __adf_os_spin_lock_bh(lock);
}


/**
 * @brief unlocks the spinlock mutex in soft irq context
 * 
 * @param[in] lock  spinlock object pointer
 */
static adf_os_inline void
adf_os_spin_unlock_bh(adf_os_spinlock_t *lock)
{
    __adf_os_spin_unlock_bh(lock);
}


/**
 * @brief Execute the input function with spinlock held and interrupt disabled.
 *
 * @param[in] hdl       OS handle
 * @param[in] lock      spinlock to be held for the critical region
 * @param[in] func      critical region function that to be executed
 * @param[in] context   context of the critical region function
 * 
 * @return Boolean status returned by the critical region function
 */
static adf_os_inline a_bool_t
adf_os_spinlock_irq_exec(adf_os_handle_t           hdl,
                         adf_os_spinlock_t        *lock,
                         adf_os_irqlocked_func_t  func,
                         void                     *arg)
{
    return __adf_os_spinlock_irq_exec(hdl, lock, func, arg);
}

#endif
