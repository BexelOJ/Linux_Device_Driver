#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

#define DRIVER_NAME "ldd_20261025_kernelSourceBuild"

static int __init kernelSourceBuild_init(void)
{
    pr_info("%s: built against kernel source/build tree\n",
        DRIVER_NAME);

    pr_info("%s: kernel version: %s\n",
        DRIVER_NAME,
        UTS_RELEASE);

    return 0;
}

static void __exit kernelSourceBuild_exit(void)
{
    pr_info("%s: unloaded\n", DRIVER_NAME);
}

module_init(kernelSourceBuild_init);
module_exit(kernelSourceBuild_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Kernel source build example");


/*
//-------------------------------------------



//-------------------------------------------
*/


