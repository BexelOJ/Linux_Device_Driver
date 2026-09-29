#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/tty.h>
#include <linux/tty_driver.h>

//-------------------------------------------

static struct tty_driver* ldd_tty;

//-------------------------------------------

static int ldd_tty_open(struct tty_struct* tty,
    struct file* file)
{
    pr_info("ldd_ttyBasi: open\n");

    return 0;
}

//-------------------------------------------

static void ldd_tty_close(struct tty_struct* tty,
    struct file* file)
{
    pr_info("ldd_ttyBasi: close\n");
}

//-------------------------------------------

static ssize_t ldd_tty_write(struct tty_struct* tty,
    const unsigned char* buf,
    size_t count)
{
    pr_info("ldd_ttyBasi: write %zu bytes\n",
        count);

    return count;
}

//-------------------------------------------

static const struct tty_operations ldd_tty_ops = {

    .open = ldd_tty_open,
    .close = ldd_tty_close,
    .write = ldd_tty_write,
};

//-------------------------------------------

static int __init ldd_ttyBasi_init(void)
{
    int ret;

    pr_info("ldd_ttyBasi: init\n");

    //-------------------------------------------
    // Allocate TTY driver
    //-------------------------------------------

    ldd_tty =
        tty_alloc_driver(1,
            TTY_DRIVER_REAL_RAW |
            TTY_DRIVER_DYNAMIC_DEV);

    if (IS_ERR(ldd_tty))
        return PTR_ERR(ldd_tty);

    //-------------------------------------------
    // Configure
    //-------------------------------------------

    ldd_tty->driver_name = "ldd_tty_basic";
    ldd_tty->name = "lddttybasic";

    ldd_tty->type = TTY_DRIVER_TYPE_SERIAL;
    ldd_tty->subtype = SERIAL_TYPE_NORMAL;

    //-------------------------------------------
    // Operations
    //-------------------------------------------

    tty_set_operations(ldd_tty,
        &ldd_tty_ops);

    //-------------------------------------------
    // Register
    //-------------------------------------------

    ret = tty_register_driver(ldd_tty);

    if (ret) {
        put_tty_driver(ldd_tty);
        return ret;
    }

    pr_info("ldd_ttyBasi: registered\n");

    return 0;
}

//-------------------------------------------

static void __exit ldd_ttyBasi_exit(void)
{
    pr_info("ldd_ttyBasi: exit\n");

    tty_unregister_driver(ldd_tty);

    put_tty_driver(ldd_tty);
}

//-------------------------------------------

module_init(ldd_ttyBasi_init);
module_exit(ldd_ttyBasi_exit);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Basic TTY driver");


/*



*/


