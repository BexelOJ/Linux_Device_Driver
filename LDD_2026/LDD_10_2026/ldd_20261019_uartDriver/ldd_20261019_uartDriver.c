#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/tty.h>
#include <linux/tty_driver.h>

//-------------------------------------------

#define LDD_TTY_NAME "ldduart"
#define LDD_TTY_MINORS 1

//-------------------------------------------

static struct tty_driver* ldd_tty_driver;

//-------------------------------------------

static int ldd_tty_open(struct tty_struct* tty,
    struct file* file)
{
    pr_info("ldd_uartDriver: open\n");

    return 0;
}

//-------------------------------------------

static void ldd_tty_close(struct tty_struct* tty,
    struct file* file)
{
    pr_info("ldd_uartDriver: close\n");
}

//-------------------------------------------

static ssize_t ldd_tty_write(struct tty_struct* tty,
    const unsigned char* buf,
    size_t count)
{
    size_t i;

    pr_info("ldd_uartDriver: write count=%zu\n",
        count);

    for (i = 0; i < count; i++)
        pr_info("TX: 0x%02x\n", buf[i]);

    return count;
}

//-------------------------------------------

static const struct tty_operations ldd_tty_ops = {

    .open = ldd_tty_open,
    .close = ldd_tty_close,
    .write = ldd_tty_write,
};

//-------------------------------------------

static int __init ldd_uartDriver_init(void)
{
    int ret;

    pr_info("ldd_uartDriver: init\n");

    //-------------------------------------------
    // Allocate TTY driver
    //-------------------------------------------

    ldd_tty_driver =
        tty_alloc_driver(LDD_TTY_MINORS,
            TTY_DRIVER_REAL_RAW |
            TTY_DRIVER_DYNAMIC_DEV);

    if (IS_ERR(ldd_tty_driver))
        return PTR_ERR(ldd_tty_driver);

    //-------------------------------------------
    // Configure
    //-------------------------------------------

    ldd_tty_driver->driver_name = LDD_TTY_NAME;
    ldd_tty_driver->name = LDD_TTY_NAME;
    ldd_tty_driver->major = 0;
    ldd_tty_driver->minor_start = 0;
    ldd_tty_driver->type = TTY_DRIVER_TYPE_SERIAL;
    ldd_tty_driver->subtype = SERIAL_TYPE_NORMAL;

    tty_set_operations(ldd_tty_driver,
        &ldd_tty_ops);

    //-------------------------------------------
    // Register
    //-------------------------------------------

    ret = tty_register_driver(ldd_tty_driver);

    if (ret) {
        pr_err("ldd_uartDriver: registration failed\n");

        put_tty_driver(ldd_tty_driver);

        return ret;
    }

    pr_info("ldd_uartDriver: registered\n");

    return 0;
}

//-------------------------------------------

static void __exit ldd_uartDriver_exit(void)
{
    pr_info("ldd_uartDriver: exit\n");

    tty_unregister_driver(ldd_tty_driver);

    put_tty_driver(ldd_tty_driver);
}

//-------------------------------------------

module_init(ldd_uartDriver_init);
module_exit(ldd_uartDriver_exit);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Basic TTY UART driver");


/*
//-------------------------------------------



//-------------------------------------------
*/


