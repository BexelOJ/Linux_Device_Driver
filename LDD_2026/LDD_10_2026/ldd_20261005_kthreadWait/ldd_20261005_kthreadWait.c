//-------------------------------------------
// ldd_20261005_kthreadWait.c
//-------------------------------------------

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/kthread.h>
#include <linux/wait.h>
#include <linux/delay.h>

static struct task_struct* workerThread;

static wait_queue_head_t waitQueue;

static bool condition;

//-------------------------------------------
// Kernel Thread
//-------------------------------------------

static int worker_function(void* data)
{
    printk(KERN_INFO
        "kthreadWait: thread started\n");

    while (!kthread_should_stop())
    {
        /*
         * Sleep until:
         *
         * condition == true
         *
         * OR
         *
         * kthread_stop() is called.
         */

        wait_event_interruptible(
            waitQueue,
            condition || kthread_should_stop()
        );

        if (kthread_should_stop())
            break;

        printk(KERN_INFO
            "kthreadWait: condition satisfied\n");

        condition = false;
    }

    printk(KERN_INFO
        "kthreadWait: thread exiting\n");

    return 0;
}

//-------------------------------------------
// Module Init
//-------------------------------------------

static int __init kthread_wait_init(void)
{
    printk(KERN_INFO
        "ldd_20261005_kthreadWait: init\n");

    init_waitqueue_head(&waitQueue);

    workerThread = kthread_run(
        worker_function,
        NULL,
        "ldd_kthread_wait"
    );

    if (IS_ERR(workerThread))
    {
        printk(KERN_ERR
            "Failed to create kernel thread\n");

        return PTR_ERR(workerThread);
    }

    /*
     * Simulate an event.
     */
    msleep(3000);

    condition = true;

    wake_up_interruptible(&waitQueue);

    return 0;
}

//-------------------------------------------
// Module Exit
//-------------------------------------------

static void __exit kthread_wait_exit(void)
{
    printk(KERN_INFO
        "ldd_20261005_kthreadWait: exit\n");

    if (workerThread)
    {
        kthread_stop(workerThread);

        workerThread = NULL;
    }
}

//-------------------------------------------

module_init(kthread_wait_init);
module_exit(kthread_wait_exit);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Lr");
MODULE_DESCRIPTION("Kernel thread wait demonstration");

//-------------------------------------------


