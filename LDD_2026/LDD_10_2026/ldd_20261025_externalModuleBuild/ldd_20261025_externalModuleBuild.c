#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

#define DRIVER_NAME "ldd_20261025_externalModuleBuild"

static int __init externalModuleBuild_init(void)
{
    pr_info("%s: external module loaded\n",
        DRIVER_NAME);

    return 0;
}

static void __exit externalModuleBuild_exit(void)
{
    pr_info("%s: external module unloaded\n",
        DRIVER_NAME);
}

module_init(externalModuleBuild_init);
module_exit(externalModuleBuild_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("External kernel module build example");


/*
//-------------------------------------------



//-------------------------------------------
*/


