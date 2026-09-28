//-------------------------------------------
// ldd_20261005_waitQueue.c
//-------------------------------------------

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/wait.h>
#include <linux/kthread.h>
#include <linux/delay.h>

static DECLARE_WAIT_QUEUE_HEAD(myWaitQueue);

static bool condition;

static struct task_struct* workerThread;

//-------------------------------------------

static int worker_function(void* data)
{
    printk(KERN_INFO
        "waitQueue: thread waiting...\n");

    wait_event_interruptible(
        myWaitQueue,
        condition || kthread_should_stop()
    );

    if (kthread_should_stop())
        return 0;

    printk(KERN_INFO
        "waitQueue: condition became true\n");

    return 0;
}

//-------------------------------------------

static int __init wait_queue_init(void)
{
    printk(KERN_INFO
        "ldd_20261005_waitQueue: init\n");

    condition = false;

    workerThread = kthread_run(
        worker_function,
        NULL,
        "waitqueue_worker"
    );

    if (IS_ERR(workerThread))
        return PTR_ERR(workerThread);

    /*
     * Simulate an event.
     */

    msleep(3000);

    condition = true;

    wake_up_interruptible(&myWaitQueue);

    return 0;
}

//-------------------------------------------

static void __exit wait_queue_exit(void)
{
    if (workerThread)
    {
        condition = true;

        wake_up_interruptible(&myWaitQueue);

        kthread_stop(workerThread);

        workerThread = NULL;
    }

    printk(KERN_INFO
        "ldd_20261005_waitQueue: exit\n");
}

//-------------------------------------------

module_init(wait_queue_init);
module_exit(wait_queue_exit);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Lr");
MODULE_DESCRIPTION("Wait queue demonstration");

//-------------------------------------------


