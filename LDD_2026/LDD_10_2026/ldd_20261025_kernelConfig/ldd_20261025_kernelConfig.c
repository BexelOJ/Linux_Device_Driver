#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

#define DRIVER_NAME "ldd_20261025_kernelConfig"

static int __init kernelConfig_init(void)
{
#ifdef CONFIG_PREEMPT
    pr_info("%s: PREEMPT enabled\n",
        DRIVER_NAME);
#else
    pr_info("%s: PREEMPT disabled\n",
        DRIVER_NAME);
#endif

#ifdef CONFIG_MODULES
    pr_info("%s: module support enabled\n",
        DRIVER_NAME);
#else
    pr_info("%s: module support disabled\n",
        DRIVER_NAME);
#endif

#ifdef CONFIG_OF
    pr_info("%s: Device Tree support enabled\n",
        DRIVER_NAME);
#else
    pr_info("%s: Device Tree support disabled\n",
        DRIVER_NAME);
#endif

    return 0;
}

static void __exit kernelConfig_exit(void)
{
    pr_info("%s: unloaded\n", DRIVER_NAME);
}

module_init(kernelConfig_init);
module_exit(kernelConfig_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Linux kernel configuration example");


/*
//-------------------------------------------



//-------------------------------------------
*/


