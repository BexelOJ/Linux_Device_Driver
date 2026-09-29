#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

//---------------------------------------------------

extern int ldd_dependencyFunction(void);
extern int ldd_addNumbers(int a, int b);
//---------------------------------------------------

static int __init ldd_consumerInit(void)
{
    int result;
    int result_2;

    pr_info(
        "ldd_20261001_dependencyConsumer: "
        "Consumer initialized\n"
    );

    result = ldd_dependencyFunction();
    result_2 = ldd_addNumbers(50,6);

    pr_info(
        "ldd_20261001_dependencyConsumer: "
        "Function returned %d\n",
        result
    );

    pr_info(
        "ldd_20261001_dependencyConsumer: "
        "Function returned %d\n",
        result_2
    );

    return 0;
}

//---------------------------------------------------

static void __exit ldd_consumerExit(void)
{
    pr_info(
        "ldd_20261001_dependencyConsumer: "
        "Consumer exited\n"
    );
}

//---------------------------------------------------

module_init(ldd_consumerInit);
module_exit(ldd_consumerExit);

//---------------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION(
    "Kernel module dependency consumer"
);

//---------------------------------------------------


/*
//---------------------------------------------------



//---------------------------------------------------
*/


