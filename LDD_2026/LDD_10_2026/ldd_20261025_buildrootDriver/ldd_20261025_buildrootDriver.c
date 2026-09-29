#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

#define DRIVER_NAME "ldd_20261025_buildrootDriver"

static int __init buildrootDriver_init(void)
{
    pr_info("%s: Buildroot driver loaded\n",
        DRIVER_NAME);

    pr_info("%s: running inside embedded Linux\n",
        DRIVER_NAME);

    return 0;
}

static void __exit buildrootDriver_exit(void)
{
    pr_info("%s: Buildroot driver unloaded\n",
        DRIVER_NAME);
}

module_init(buildrootDriver_init);
module_exit(buildrootDriver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Buildroot external Linux driver example");


/*
//-------------------------------------------



//-------------------------------------------
*/


