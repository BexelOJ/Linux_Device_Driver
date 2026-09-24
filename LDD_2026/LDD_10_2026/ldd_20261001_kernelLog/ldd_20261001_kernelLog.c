#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

//---------------------------------------------------

static int __init ldd_moduleInit(void)
{
    printk(KERN_INFO
           "ldd_20261001_kernelLog: printk info message\n");

    pr_info(
        "ldd_20261001_kernelLog: pr_info message\n"
    );

    pr_warn(
        "ldd_20261001_kernelLog: pr_warn message\n"
    );

    pr_err(
        "ldd_20261001_kernelLog: pr_err message\n"
    );

    return 0;
}

//---------------------------------------------------

static void __exit ldd_moduleExit(void)
{
    pr_info(
        "ldd_20261001_kernelLog: Module exited\n"
    );
}

//---------------------------------------------------

module_init(ldd_moduleInit);
module_exit(ldd_moduleExit);

//---------------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Lr");
MODULE_DESCRIPTION("Linux kernel logging demonstration");

//---------------------------------------------------



