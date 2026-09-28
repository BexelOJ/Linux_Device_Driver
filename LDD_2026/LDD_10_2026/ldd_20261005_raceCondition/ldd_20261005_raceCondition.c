//-------------------------------------------
// ldd_20261005_spinlock.c
//-------------------------------------------

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/spinlock.h>
#include <linux/kthread.h>
#include <linux/delay.h>

static spinlock_t lock;

static int sharedCounter;

static struct task_struct* thread1;
static struct task_struct* thread2;

//-------------------------------------------

static int worker_function(void* data)
{
    int i;

    for (i = 0; i < 100000; i++)
    {
        unsigned long flags;

        spin_lock_irqsave(&lock, flags);

        sharedCounter++;

        spin_unlock_irqrestore(&lock, flags);

        if (kthread_should_stop())
            break;
    }

    return 0;
}

//-------------------------------------------

static int __init spinlock_init(void)
{
    printk(KERN_INFO
        "ldd_20261005_spinlock: init\n");

    spin_lock_init(&lock);

    sharedCounter = 0;

    thread1 = kthread_run(
        worker_function,
        NULL,
        "spin_thread1"
    );

    thread2 = kthread_run(
        worker_function,
        NULL,
        "spin_thread2"
    );

    if (IS_ERR(thread1) || IS_ERR(thread2))
    {
        printk(KERN_ERR
            "Failed to create threads\n");

        return -ENOMEM;
    }

    msleep(1000);

    printk(KERN_INFO
        "Final counter = %d\n",
        sharedCounter);

    return 0;
}

//-------------------------------------------

static void __exit spinlock_exit(void)
{
    if (!IS_ERR_OR_NULL(thread1))
        kthread_stop(thread1);

    if (!IS_ERR_OR_NULL(thread2))
        kthread_stop(thread2);

    printk(KERN_INFO
        "ldd_20261005_spinlock: exit\n");
}

//-------------------------------------------

module_init(spinlock_init);
module_exit(spinlock_exit);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Spinlock demonstration");

//-------------------------------------------


