#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

//---------------------------------------------------

static int __init ldd_moduleInit(void)
{
    pr_info(
        "ldd_20261001_kernelBuild: Module initialized\n"
    );

    return 0;
}

//---------------------------------------------------

static void __exit ldd_moduleExit(void)
{
    pr_info(
        "ldd_20261001_kernelBuild: Module exited\n"
    );
}

//---------------------------------------------------

module_init(ldd_moduleInit);
module_exit(ldd_moduleExit);

//---------------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Lr");
MODULE_DESCRIPTION("Linux kernel out-of-tree module build");

//---------------------------------------------------



