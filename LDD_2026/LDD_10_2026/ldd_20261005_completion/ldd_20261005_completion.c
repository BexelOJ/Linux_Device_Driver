//-------------------------------------------
// ldd_20261005_tasklet.c
//-------------------------------------------

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/interrupt.h>

static void tasklet_function(struct tasklet_struct* tasklet);

//-------------------------------------------

DECLARE_TASKLET(my_tasklet, tasklet_function);

//-------------------------------------------

static void tasklet_function(struct tasklet_struct* tasklet)
{
    printk(KERN_INFO
        "tasklet: function executed\n");
}

//-------------------------------------------

static int __init tasklet_init(void)
{
    printk(KERN_INFO
        "ldd_20261005_tasklet: init\n");

    tasklet_schedule(&my_tasklet);

    return 0;
}

//-------------------------------------------

static void __exit tasklet_exit(void)
{
    tasklet_kill(&my_tasklet);

    printk(KERN_INFO
        "ldd_20261005_tasklet: exit\n");
}

//-------------------------------------------

module_init(tasklet_init);
module_exit(tasklet_exit);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Tasklet demonstration");

//-------------------------------------------


