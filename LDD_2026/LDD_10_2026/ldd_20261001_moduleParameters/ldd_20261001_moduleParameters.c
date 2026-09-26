#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/moduleparam.h>

//---------------------------------------------------

static int value = 10;

module_param(value, int, 0644);

MODULE_PARM_DESC(
    value,
    "Integer value passed to the kernel module"
);

//---------------------------------------------------

static int __init ldd_moduleInit(void)
{
    pr_info(
        "ldd_20261001_moduleParameters: Module initialized\n"
    );

    pr_info(
        "ldd_20261001_moduleParameters: value = %d\n",
        value
    );

    return 0;
}

//---------------------------------------------------

static void __exit ldd_moduleExit(void)
{
    pr_info(
        "ldd_20261001_moduleParameters: Module exited\n"
    );
}

//---------------------------------------------------

module_init(ldd_moduleInit);
module_exit(ldd_moduleExit);

//---------------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Linux kernel module parameters");

//---------------------------------------------------



