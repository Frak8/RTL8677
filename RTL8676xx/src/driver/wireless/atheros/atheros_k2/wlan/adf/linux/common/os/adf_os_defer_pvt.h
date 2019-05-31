#ifndef _ADF_CMN_OS_DEFER_PVT_H
#define _ADF_CMN_OS_DEFER_PVT_H

#include <linux/version.h>
#include <linux/workqueue.h>
#include <linux/interrupt.h>

#include <adf_os_types.h>

typedef struct tasklet_struct  __adf_os_bh_t;

#if LINUX_VERSION_CODE  <= KERNEL_VERSION(2,6,19)
typedef struct work_struct     __adf_os_work_t;
#else
/**
 * wrapper around the real task func
 */
typedef struct {
    struct work_struct   work;
    adf_os_defer_fn_t    fn;
    void                 *arg;
}__adf_os_work_t;
extern void __adf_os_defer_func(struct work_struct *work);
#endif

typedef void (*__adf_os_bh_fn_t)(unsigned long arg);

static inline a_status_t 
__adf_os_init_work(adf_os_handle_t    hdl,
                   __adf_os_work_t      *work,
                   adf_os_defer_fn_t    func,
                   void                 *arg)
{
#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,19)
    INIT_WORK(work, func, arg);
#else
    INIT_WORK(&work->work, __adf_os_defer_func);
#endif

    return A_STATUS_OK;
}

static inline  a_status_t __adf_os_init_bh(adf_os_handle_t  hdl,
                                     struct tasklet_struct *bh,
                                     adf_os_defer_fn_t  func,
                                     void               *arg)
{
     tasklet_init(bh, (__adf_os_bh_fn_t)func, (unsigned long)arg);
  
     return A_STATUS_OK;
}

static inline a_status_t 
__adf_os_sched_work(adf_os_handle_t hdl, __adf_os_work_t  * work)
{
#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,19)
    schedule_work(work);
#else
    schedule_work(&work->work);
#endif
    return A_STATUS_OK;
}
static inline a_status_t  __adf_os_sched_bh(adf_os_handle_t hdl, 
                                     struct tasklet_struct * bh)
{
    tasklet_schedule(bh);
     
    return A_STATUS_OK;
}

static inline a_status_t 
__adf_os_disable_work(adf_os_handle_t hdl, __adf_os_work_t  * work)
{
   /**
    * XXX:???
    */
    return A_STATUS_OK;
}
static inline a_status_t 
__adf_os_disable_bh(adf_os_handle_t hdl, struct tasklet_struct *bh)
{
    tasklet_kill(bh);

    return A_STATUS_OK;
}
#endif /*_ADF_CMN_OS_DEFER_PVT_H*/
