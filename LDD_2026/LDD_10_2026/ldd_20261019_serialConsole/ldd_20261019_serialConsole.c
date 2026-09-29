#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/console.h>
#include <linux/tty.h>

//-------------------------------------------

static void ldd_console_write(struct console* co,
    const char* buf,
    unsigned int count)
{
    unsigned int i;

    /*
     * Educational demonstration only.
     *
     * A real serial console writes directly
     * to UART hardware registers/FIFO.
     */

    for (i = 0; i < count; i++)
        pr_debug("UART console char: %c\n", buf[i]);
}

//-------------------------------------------

static struct console ldd_console = {

    .name = "ldduart",
    .write = ldd_console_write,
    .flags = CON_PRINTBUFFER,
    .index = -1,
};

//-------------------------------------------

static int __init ldd_serialConsole_init(void)
{
    pr_info("ldd_serialConsole: init\n");

    register_console(&ldd_console);

    return 0;
}

//-------------------------------------------

static void __exit ldd_serialConsole_exit(void)
{
    unregister_console(&ldd_console);

    pr_info("ldd_serialConsole: exit\n");
}

//-------------------------------------------

module_init(ldd_serialConsole_init);
module_exit(ldd_serialConsole_exit);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Serial console demonstration");


/*
//-------------------------------------------

printk()
   │
   ▼
Console subsystem
   │
   ▼
serial console
   │
   ▼
UART TX register/FIFO
   │
   ▼
UART hardware

//-------------------------------------------
*/


