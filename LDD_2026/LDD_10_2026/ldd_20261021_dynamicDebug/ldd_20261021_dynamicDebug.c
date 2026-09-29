#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

//-------------------------------------------

static int __init dynamic_debug_init(void)
{
    pr_debug("dynamicDebug: debug message 1\n");

    pr_debug("dynamicDebug: debug message 2\n");

    pr_info("dynamicDebug: module loaded\n");

    return 0;
}

//-------------------------------------------

static void __exit dynamic_debug_exit(void)
{
    pr_debug("dynamicDebug: module unloading\n");

    pr_info("dynamicDebug: module unloaded\n");
}

//-------------------------------------------

module_init(dynamic_debug_init);
module_exit(dynamic_debug_exit);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Linux Dynamic Debug example");


/*



*/


