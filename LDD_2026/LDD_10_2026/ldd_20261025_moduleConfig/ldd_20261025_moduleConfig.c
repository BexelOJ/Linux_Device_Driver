#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

#define DRIVER_NAME "ldd_20261025_moduleConfig"

#ifdef CONFIG_LDD_20261025_FEATURE

static int __init moduleConfig_init(void)
{
    pr_info("%s: optional feature ENABLED\n",
        DRIVER_NAME);

    return 0;
}

static void __exit moduleConfig_exit(void)
{
    pr_info("%s: optional feature disabled on unload\n",
        DRIVER_NAME);
}

#else

static int __init moduleConfig_init(void)
{
    pr_info("%s: optional feature DISABLED\n",
        DRIVER_NAME);

    return 0;
}

static void __exit moduleConfig_exit(void)
{
    pr_info("%s: unloaded\n",
        DRIVER_NAME);
}

#endif

module_init(moduleConfig_init);
module_exit(moduleConfig_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Kernel module configuration example");


/*
//-------------------------------------------



//-------------------------------------------
*/


