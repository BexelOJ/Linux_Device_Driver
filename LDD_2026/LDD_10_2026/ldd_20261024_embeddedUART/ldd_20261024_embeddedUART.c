#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/tty.h>
#include <linux/tty_driver.h>

#define DRIVER_NAME "ldd_20261024_embeddedUART"

static int __init embeddedUART_init(void)
{
    pr_info("%s: module loaded\n", DRIVER_NAME);

    pr_info("%s: Linux UART/TTY subsystem example\n",
        DRIVER_NAME);

    return 0;
}

static void __exit embeddedUART_exit(void)
{
    pr_info("%s: module unloaded\n", DRIVER_NAME);
}

module_init(embeddedUART_init);
module_exit(embeddedUART_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Embedded Linux UART/TTY example");


/*
//-------------------------------------------

Userspace
   ↓
/dev/ttyS*
/dev/ttyAMA*
/dev/ttyUSB*
   ↓
TTY subsystem
   ↓
UART driver
   ↓
UART hardware

//-------------------------------------------
*/


