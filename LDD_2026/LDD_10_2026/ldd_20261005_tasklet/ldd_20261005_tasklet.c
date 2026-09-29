//-------------------------------------------
// ldd_20261005_kernelThread.c
//-------------------------------------------

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/kthread.h>
#include <linux/delay.h>

static struct task_struct* workerThread;

//-------------------------------------------

static int worker_function(void* data)
{
    int counter = 0;

    while (!kthread_should_stop())
    {
        printk(KERN_INFO
            "kernelThread: counter = %d\n",
            counter++);

        msleep(1000);
    }

    printk(KERN_INFO
        "kernelThread: stopping\n");

    return 0;
}

//-------------------------------------------

static int __init kernel_thread_init(void)
{
    printk(KERN_INFO
        "ldd_20261005_kernelThread: init\n");

    workerThread = kthread_run(
        worker_function,
        NULL,
        "ldd_kernel_thread"
    );

    if (IS_ERR(workerThread))
    {
        printk(KERN_ERR
            "Failed to create kernel thread\n");

        return PTR_ERR(workerThread);
    }

    return 0;
}

//-------------------------------------------

static void __exit kernel_thread_exit(void)
{
    printk(KERN_INFO
        "Stopping kernel thread\n");

    if (workerThread)
    {
        kthread_stop(workerThread);

        workerThread = NULL;
    }

    printk(KERN_INFO
        "ldd_20261005_kernelThread: exit\n");
}

//-------------------------------------------

module_init(kernel_thread_init);
module_exit(kernel_thread_exit);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Basic kernel thread demonstration");

//-------------------------------------------


/*
//-------------------------------------------



//-------------------------------------------
*/


