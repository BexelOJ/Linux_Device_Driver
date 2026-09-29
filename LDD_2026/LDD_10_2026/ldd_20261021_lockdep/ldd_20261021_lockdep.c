#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/mutex.h>

//-------------------------------------------

static DEFINE_MUTEX(lock_a);
static DEFINE_MUTEX(lock_b);

//-------------------------------------------

static void lockdep_demo(void)
{
    mutex_lock(&lock_a);

    pr_info("lockdep: acquired lock_a\n");

    mutex_lock(&lock_b);

    pr_info("lockdep: acquired lock_b\n");

    mutex_unlock(&lock_b);
    mutex_unlock(&lock_a);
}

//-------------------------------------------

static int __init lockdep_init(void)
{
    pr_info("lockdep: module loaded\n");

    lockdep_demo();

    return 0;
}

//-------------------------------------------

static void __exit lockdep_exit(void)
{
    pr_info("lockdep: module unloaded\n");
}

//-------------------------------------------

module_init(lockdep_init);
module_exit(lockdep_exit);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Linux Lockdep demonstration");


/*



*/


