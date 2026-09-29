#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/tty.h>

//-------------------------------------------

static int __init ldd_uartBasic_init(void)
{
    pr_info("ldd_uartBasic: init\n");

    pr_info("TTY core available\n");

    return 0;
}

//-------------------------------------------

static void __exit ldd_uartBasic_exit(void)
{
    pr_info("ldd_uartBasic: exit\n");
}

//-------------------------------------------

module_init(ldd_uartBasic_init);
module_exit(ldd_uartBasic_exit);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Basic UART and TTY demonstration");


/*
//-------------------------------------------



//-------------------------------------------
*/


