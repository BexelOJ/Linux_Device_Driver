#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

//---------------------------------------------------

int ldd_addNumbers(int a, int b)
{
    return a + b;
}

//---------------------------------------------------

EXPORT_SYMBOL(ldd_addNumbers);

//---------------------------------------------------

static int __init ldd_providerInit(void)
{
    pr_info(
        "ldd_20261001_symbolProvider: "
        "Provider module initialized\n"
    );

    return 0;
}

//---------------------------------------------------

static void __exit ldd_providerExit(void)
{
    pr_info(
        "ldd_20261001_symbolProvider: "
        "Provider module exited\n"
    );
}

//---------------------------------------------------

module_init(ldd_providerInit);
module_exit(ldd_providerExit);

//---------------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION(
    "Kernel symbol provider module"
);

//---------------------------------------------------


/*
//---------------------------------------------------



//---------------------------------------------------
*/


