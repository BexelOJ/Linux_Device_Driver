//-------------------------------------------
// ldd_20261005_rwlock.c
//-------------------------------------------

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/rwlock.h>
#include <linux/kthread.h>
#include <linux/delay.h>

static rwlock_t myLock;

static int sharedData;

static struct task_struct* readerThread;
static struct task_struct* writerThread;

//-------------------------------------------

static int reader_function(void* data)
{
    int value;

    while (!kthread_should_stop())
    {
        read_lock(&myLock);

        value = sharedData;

        printk(KERN_INFO
            "Reader: value = %d\n",
            value);

        read_unlock(&myLock);

        msleep(1000);
    }

    return 0;
}

//-------------------------------------------

static int writer_function(void* data)
{
    while (!kthread_should_stop())
    {
        write_lock(&myLock);

        sharedData++;

        printk(KERN_INFO
            "Writer: value = %d\n",
            sharedData);

        write_unlock(&myLock);

        msleep(2000);
    }

    return 0;
}

//-------------------------------------------

static int __init rwlock_init_module(void)
{
    printk(KERN_INFO
        "ldd_20261005_rwlock: init\n");

    rwlock_init(&myLock);

    sharedData = 0;

    readerThread = kthread_run(
        reader_function,
        NULL,
        "rw_reader"
    );

    writerThread = kthread_run(
        writer_function,
        NULL,
        "rw_writer"
    );

    if (IS_ERR(readerThread) ||
        IS_ERR(writerThread))
    {
        return -ENOMEM;
    }

    return 0;
}

//-------------------------------------------

static void __exit rwlock_exit_module(void)
{
    if (!IS_ERR_OR_NULL(readerThread))
        kthread_stop(readerThread);

    if (!IS_ERR_OR_NULL(writerThread))
        kthread_stop(writerThread);

    printk(KERN_INFO
        "ldd_20261005_rwlock: exit\n");
}

//-------------------------------------------

module_init(rwlock_init_module);
module_exit(rwlock_exit_module);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Read/write lock demonstration");

//-------------------------------------------


