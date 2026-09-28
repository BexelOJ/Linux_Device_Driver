//-------------------------------------------
// ldd_20261005_mutex.c
//-------------------------------------------

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/mutex.h>
#include <linux/kthread.h>
#include <linux/delay.h>

static DEFINE_MUTEX(myMutex);

static int sharedCounter;

static struct task_struct* thread1;
static struct task_struct* thread2;

//-------------------------------------------

static int worker_function(void* data)
{
    int i;

    for (i = 0; i < 10000; i++)
    {
        mutex_lock(&myMutex);

        sharedCounter++;

        mutex_unlock(&myMutex);

        if (kthread_should_stop())
            break;
    }

    return 0;
}

//-------------------------------------------

static int __init mutex_init_module(void)
{
    printk(KERN_INFO
        "ldd_20261005_mutex: init\n");

    sharedCounter = 0;

    thread1 = kthread_run(
        worker_function,
        NULL,
        "mutex_thread1"
    );

    thread2 = kthread_run(
        worker_function,
        NULL,
        "mutex_thread2"
    );

    if (IS_ERR(thread1) || IS_ERR(thread2))
        return -ENOMEM;

    msleep(1000);

    printk(KERN_INFO
        "Protected counter = %d\n",
        sharedCounter);

    return 0;
}

//-------------------------------------------

static void __exit mutex_exit_module(void)
{
    if (!IS_ERR_OR_NULL(thread1))
        kthread_stop(thread1);

    if (!IS_ERR_OR_NULL(thread2))
        kthread_stop(thread2);

    printk(KERN_INFO
        "ldd_20261005_mutex: exit\n");
}

//-------------------------------------------

module_init(mutex_init_module);
module_exit(mutex_exit_module);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Lr");
MODULE_DESCRIPTION("Mutex demonstration");

//-------------------------------------------


