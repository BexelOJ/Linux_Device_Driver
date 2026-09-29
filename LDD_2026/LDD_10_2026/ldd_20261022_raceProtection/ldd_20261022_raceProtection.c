#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/mutex.h>
#include <linux/kthread.h>
#include <linux/delay.h>

//-------------------------------------------

static DEFINE_MUTEX(counter_lock);

static int shared_counter;

static struct task_struct* thread1;
static struct task_struct* thread2;

//-------------------------------------------

static int race_thread(void* data)
{
    int i;

    for (i = 0; i < 100; i++)
    {
        if (kthread_should_stop())
            break;

        mutex_lock(&counter_lock);

        shared_counter++;

        mutex_unlock(&counter_lock);

        msleep(10);
    }

    return 0;
}

//-------------------------------------------

static int __init race_protection_init(void)
{
    pr_info("raceProtection: module loaded\n");

    shared_counter = 0;

    thread1 = kthread_run(race_thread,
        NULL,
        "ldd_race_1");

    if (IS_ERR(thread1))
        return PTR_ERR(thread1);

    thread2 = kthread_run(race_thread,
        NULL,
        "ldd_race_2");

    if (IS_ERR(thread2))
    {
        kthread_stop(thread1);
        return PTR_ERR(thread2);
    }

    return 0;
}

//-------------------------------------------

static void __exit race_protection_exit(void)
{
    if (thread1)
        kthread_stop(thread1);

    if (thread2)
        kthread_stop(thread2);

    pr_info("raceProtection: final counter = %d\n",
        shared_counter);

    pr_info("raceProtection: module unloaded\n");
}

//-------------------------------------------

module_init(race_protection_init);
module_exit(race_protection_exit);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Race protection using mutex");


