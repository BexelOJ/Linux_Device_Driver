#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

#define DRIVER_NAME "ldd_20261023_syscallInterface"

static long ldd_kernel_operation(unsigned long value)
{
    pr_info("%s: operation received value=%lu\n",
        DRIVER_NAME, value);

    return value + 1;
}

static int __init syscallInterface_init(void)
{
    long result;

    result = ldd_kernel_operation(100);

    pr_info("%s: result=%ld\n", DRIVER_NAME, result);

    return 0;
}

static void __exit syscallInterface_exit(void)
{
    pr_info("%s: module unloaded\n", DRIVER_NAME);
}

module_init(syscallInterface_init);
module_exit(syscallInterface_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Kernel syscall-interface concept example");


/*
//-------------------------------------------



//-------------------------------------------
*/


