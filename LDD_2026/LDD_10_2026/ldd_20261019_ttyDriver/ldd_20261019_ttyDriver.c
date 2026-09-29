#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/tty.h>
#include <linux/tty_driver.h>

//-------------------------------------------

#define LDD_TTY_MINORS 1

//-------------------------------------------

static struct tty_driver* ldd_driver;

//-------------------------------------------

static int ldd_tty_open(struct tty_struct* tty,
    struct file* file)
{
    pr_info("ldd_ttyDriver: open\n");

    return 0;
}

//-------------------------------------------

static void ldd_tty_close(struct tty_struct* tty,
    struct file* file)
{
    pr_info("ldd_ttyDriver: close\n");
}

//-------------------------------------------

static ssize_t ldd_tty_write(struct tty_struct* tty,
    const unsigned char* buf,
    size_t count)
{
    pr_info("ldd_ttyDriver: write %zu bytes\n",
        count);

    /*
     * Real UART driver:
     *
     * buf
     *  ↓
     * UART TX FIFO
     */

    return count;
}

//-------------------------------------------

static unsigned int ldd_tty_write_room(
    struct tty_struct* tty)
{
    return 256;
}

//-------------------------------------------

static const struct tty_operations ldd_tty_ops = {

    .open = ldd_tty_open,
    .close = ldd_tty_close,
    .write = ldd_tty_write,
    .write_room = ldd_tty_write_room,
};

//-------------------------------------------

static int __init ldd_ttyDriver_init(void)
{
    int ret;

    pr_info("ldd_ttyDriver: init\n");

    //-------------------------------------------
    // Allocate
    //-------------------------------------------

    ldd_driver =
        tty_alloc_driver(LDD_TTY_MINORS,
            TTY_DRIVER_REAL_RAW |
            TTY_DRIVER_DYNAMIC_DEV);

    if (IS_ERR(ldd_driver))
        return PTR_ERR(ldd_driver);

    //-------------------------------------------
    // Configure
    //-------------------------------------------

    ldd_driver->driver_name = "ldd_tty";
    ldd_driver->name = "lddtty";
    ldd_driver->type = TTY_DRIVER_TYPE_SERIAL;
    ldd_driver->subtype = SERIAL_TYPE_NORMAL;

    //-------------------------------------------
    // Operations
    //-------------------------------------------

    tty_set_operations(ldd_driver,
        &ldd_tty_ops);

    //-------------------------------------------
    // Register
    //-------------------------------------------

    ret = tty_register_driver(ldd_driver);

    if (ret) {
        put_tty_driver(ldd_driver);
        return ret;
    }

    pr_info("ldd_ttyDriver: registered\n");

    return 0;
}

//-------------------------------------------

static void __exit ldd_ttyDriver_exit(void)
{
    pr_info("ldd_ttyDriver: exit\n");

    tty_unregister_driver(ldd_driver);

    put_tty_driver(ldd_driver);
}

//-------------------------------------------

module_init(ldd_ttyDriver_init);
module_exit(ldd_ttyDriver_exit);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("TTY driver example");


/* 



*/


