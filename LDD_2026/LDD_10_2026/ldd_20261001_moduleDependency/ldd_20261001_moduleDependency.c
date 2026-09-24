#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

//---------------------------------------------------

int ldd_dependencyFunction(void)
{
    pr_info(
        "ldd_20261001_dependencyProvider: "
        "Exported function called\n"
    );

    return 100;
}

//---------------------------------------------------

EXPORT_SYMBOL(ldd_dependencyFunction);

//---------------------------------------------------

static int __init ldd_providerInit(void)
{
    pr_info(
        "ldd_20261001_dependencyProvider: "
        "Provider initialized\n"
    );

    return 0;
}

//---------------------------------------------------

static void __exit ldd_providerExit(void)
{
    pr_info(
        "ldd_20261001_dependencyProvider: "
        "Provider exited\n"
    );
}

//---------------------------------------------------

module_init(ldd_providerInit);
module_exit(ldd_providerExit);

//---------------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Lr");
MODULE_DESCRIPTION(
    "Kernel module dependency provider"
);

//---------------------------------------------------



