#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

#define DRIVER_NAME "ldd_20261023_kernelInterface"

int ldd_kernel_interface(int value)
{
    pr_info("%s: interface called with value=%d\n",
        DRIVER_NAME, value);

    return value * 2;
}

EXPORT_SYMBOL(ldd_kernel_interface);

static int __init kernelInterface_init(void)
{
    pr_info("%s: interface provider loaded\n",
        DRIVER_NAME);

    return 0;
}

static void __exit kernelInterface_exit(void)
{
    pr_info("%s: interface provider unloaded\n",
        DRIVER_NAME);
}

module_init(kernelInterface_init);
module_exit(kernelInterface_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Kernel interface provider example");


/*
//-------------------------------------------



//-------------------------------------------
*/


