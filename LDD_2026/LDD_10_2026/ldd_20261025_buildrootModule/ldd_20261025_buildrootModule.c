#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/moduleparam.h>

#define DRIVER_NAME "ldd_20261025_buildrootModule"

static int debug_level = 1;

module_param(debug_level, int, 0644);
MODULE_PARM_DESC(debug_level,
    "Driver debug level");

static int __init buildrootModule_init(void)
{
    pr_info("%s: loaded\n", DRIVER_NAME);

    pr_info("%s: debug_level=%d\n",
        DRIVER_NAME,
        debug_level);

    return 0;
}

static void __exit buildrootModule_exit(void)
{
    pr_info("%s: unloaded\n", DRIVER_NAME);
}

module_init(buildrootModule_init);
module_exit(buildrootModule_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Buildroot kernel module example");


/*
//-------------------------------------------



//-------------------------------------------
*/


