//-------------------------------------------
// ldd_20261005_semaphore.c
//-------------------------------------------

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/semaphore.h>
#include <linux/kthread.h>
#include <linux/delay.h>

static struct semaphore mySemaphore;

static struct task_struct* thread1;
static struct task_struct* thread2;

//-------------------------------------------

static int worker_function(void* data)
{
    int id = *(int*)data;

    printk(KERN_INFO
        "Thread %d waiting for semaphore\n",
        id);

    if (down_interruptible(&mySemaphore))
        return -ERESTARTSYS;

    printk(KERN_INFO
        "Thread %d entered critical section\n",
        id);

    msleep(2000);

    printk(KERN_INFO
        "Thread %d leaving critical section\n",
        id);

    up(&mySemaphore);

    return 0;
}

//-------------------------------------------

static int threadId1 = 1;
static int threadId2 = 2;

//-------------------------------------------

static int __init semaphore_init_module(void)
{
    printk(KERN_INFO
        "ldd_20261005_semaphore: init\n");

    sema_init(&mySemaphore, 1);

    thread1 = kthread_run(
        worker_function,
        &threadId1,
        "sem_thread1"
    );

    thread2 = kthread_run(
        worker_function,
        &threadId2,
        "sem_thread2"
    );

    if (IS_ERR(thread1) ||
        IS_ERR(thread2))
    {
        return -ENOMEM;
    }

    return 0;
}

//-------------------------------------------

static void __exit semaphore_exit_module(void)
{
    if (!IS_ERR_OR_NULL(thread1))
        kthread_stop(thread1);

    if (!IS_ERR_OR_NULL(thread2))
        kthread_stop(thread2);

    printk(KERN_INFO
        "ldd_20261005_semaphore: exit\n");
}

//-------------------------------------------

module_init(semaphore_init_module);
module_exit(semaphore_exit_module);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Lr");
MODULE_DESCRIPTION("Semaphore demonstration");

//-------------------------------------------


