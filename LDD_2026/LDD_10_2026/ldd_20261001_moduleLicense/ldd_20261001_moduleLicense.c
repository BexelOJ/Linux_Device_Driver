#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

//---------------------------------------------------

static int __init ldd_moduleInit(void)
{
    pr_info(
        "ldd_20261001_moduleLicense: Module initialized\n"
    );

    return 0;
}

//---------------------------------------------------

static void __exit ldd_moduleExit(void)
{
    pr_info(
        "ldd_20261001_moduleLicense: Module exited\n"
    );
}

//---------------------------------------------------

module_init(ldd_moduleInit);
module_exit(ldd_moduleExit);

//---------------------------------------------------

MODULE_LICENSE("GPL");

//---------------------------------------------------

MODULE_AUTHOR("Er Bexel O J");

MODULE_DESCRIPTION(
    "Linux kernel module license demonstration"
);

//---------------------------------------------------


/*
//---------------------------------------------------



//---------------------------------------------------
*/


