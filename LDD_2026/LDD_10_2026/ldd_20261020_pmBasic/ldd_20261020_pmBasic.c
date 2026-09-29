#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/pm.h>

//-------------------------------------------

static int __init pm_basic_init(void)
{
    pr_info("pmBasic: Power Management module loaded\n");

    pr_info("pmBasic: PM framework is available\n");

    return 0;
}

//-------------------------------------------

static void __exit pm_basic_exit(void)
{
    pr_info("pmBasic: Power Management module unloaded\n");
}

//-------------------------------------------

module_init(pm_basic_init);
module_exit(pm_basic_exit);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Basic Linux Power Management example");


/*



*/


