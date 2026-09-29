//-------------------------------------------
// ldd_20261005_workqueue.c
//-------------------------------------------

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/workqueue.h>

static struct work_struct myWork;

//-------------------------------------------

static void work_function(struct work_struct* work)
{
    printk(KERN_INFO
        "workqueue: work function executed\n");
}

//-------------------------------------------

static int __init workqueue_init(void)
{
    printk(KERN_INFO
        "ldd_20261005_workqueue: init\n");

    INIT_WORK(
        &myWork,
        work_function
    );

    schedule_work(&myWork);

    printk(KERN_INFO
        "Work scheduled\n");

    return 0;
}

//-------------------------------------------

static void __exit workqueue_exit(void)
{
    flush_work(&myWork);

    printk(KERN_INFO
        "ldd_20261005_workqueue: exit\n");
}

//-------------------------------------------

module_init(workqueue_init);
module_exit(workqueue_exit);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Workqueue demonstration");

//-------------------------------------------


/*
//-------------------------------------------



//-------------------------------------------
*/


