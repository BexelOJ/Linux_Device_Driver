//-------------------------------------------
// ldd_20261005_atomic.c
//-------------------------------------------

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/atomic.h>
#include <linux/kthread.h>
#include <linux/delay.h>

static atomic_t counter;

static struct task_struct* thread1;
static struct task_struct* thread2;

//-------------------------------------------

static int worker_function(void* data)
{
    int i;

    for (i = 0; i < 100000; i++)
    {
        atomic_inc(&counter);

        if (kthread_should_stop())
            break;
    }

    return 0;
}

//-------------------------------------------

static int __init atomic_init_module(void)
{
    printk(KERN_INFO
        "ldd_20261005_atomic: init\n");

    atomic_set(&counter, 0);

    thread1 = kthread_run(
        worker_function,
        NULL,
        "atomic_thread1"
    );

    thread2 = kthread_run(
        worker_function,
        NULL,
        "atomic_thread2"
    );

    if (IS_ERR(thread1) ||
        IS_ERR(thread2))
    {
        return -ENOMEM;
    }

    msleep(1000);

    printk(KERN_INFO
        "Atomic counter = %d\n",
        atomic_read(&counter));

    return 0;
}

//-------------------------------------------

static void __exit atomic_exit_module(void)
{
    if (!IS_ERR_OR_NULL(thread1))
        kthread_stop(thread1);

    if (!IS_ERR_OR_NULL(thread2))
        kthread_stop(thread2);

    printk(KERN_INFO
        "ldd_20261005_atomic: exit\n");
}

//-------------------------------------------

module_init(atomic_init_module);
module_exit(atomic_exit_module);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Lr");
MODULE_DESCRIPTION("Atomic operation demonstration");

//-------------------------------------------


