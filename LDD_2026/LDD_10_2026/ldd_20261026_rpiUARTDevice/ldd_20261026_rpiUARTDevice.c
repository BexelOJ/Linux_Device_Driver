#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/tty.h>

#define DRIVER_NAME "ldd_20261026_rpiUARTDevice"

static int __init rpiUARTDevice_init(void)
{
    pr_info("%s: UART device example loaded\n",
        DRIVER_NAME);

    pr_info("%s: UART devices are exposed through TTY\n",
        DRIVER_NAME);

    return 0;
}

static void __exit rpiUARTDevice_exit(void)
{
    pr_info("%s: UART device example unloaded\n",
        DRIVER_NAME);
}

module_init(rpiUARTDevice_init);
module_exit(rpiUARTDevice_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Raspberry Pi UART device example");


/*
//-------------------------------------------



//-------------------------------------------
*/


