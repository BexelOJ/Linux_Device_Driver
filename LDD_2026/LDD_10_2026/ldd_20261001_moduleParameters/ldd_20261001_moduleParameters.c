#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/moduleparam.h>

//---------------------------------------------------

static int value = 10;
static int debug = 0;
static int buffer_size = 1024;

module_param(value, int, 0644);
module_param(debug, int, 0644);
module_param(buffer_size, int, 0644);
// module_param(variable, type, permissions);

MODULE_PARM_DESC(
    value,
    "Integer value passed to the kernel module"
);

MODULE_PARM_DESC(
    debug,
    "Enable or disable debug messages"
);

MODULE_PARM_DESC(
    buffer_size,
    "Buffer size used by the kernel module"
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

    pr_info(
        "ldd_20261001_moduleParameters: debug = %d\n",
        debug
    );

    pr_info(
        "ldd_20261001_moduleParameters: buffer_size = %d\n",
        buffer_size
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



