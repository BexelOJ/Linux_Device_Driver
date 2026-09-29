#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/tty.h>
#include <linux/tty_driver.h>

//-------------------------------------------

static struct tty_driver* ldd_driver;

//-------------------------------------------

static int ldd_open(struct tty_struct* tty,
    struct file* file)
{
    pr_info("ldd_uartFlowControl: open\n");

    return 0;
}

//-------------------------------------------

static void ldd_close(struct tty_struct* tty,
    struct file* file)
{
    pr_info("ldd_uartFlowControl: close\n");
}

//-------------------------------------------

static void ldd_set_termios(struct tty_struct* tty,
    const struct ktermios* old)
{
    struct ktermios* termios = &tty->termios;

    pr_info("ldd_uartFlowControl: set_termios\n");

    //-------------------------------------------
    // Hardware flow control
    //-------------------------------------------

    if (termios->c_cflag & CRTSCTS)
        pr_info("RTS/CTS flow control enabled\n");
    else
        pr_info("RTS/CTS flow control disabled\n");

    //-------------------------------------------
    // Software flow control
    //-------------------------------------------

    if (termios->c_iflag & IXON)
        pr_info("XON/XOFF TX flow control enabled\n");

    if (termios->c_iflag & IXOFF)
        pr_info("XON/XOFF RX flow control enabled\n");
}

//-------------------------------------------

static const struct tty_operations ldd_ops = {

    .open = ldd_open,
    .close = ldd_close,
    .set_termios = ldd_set_termios,
};

//-------------------------------------------

static int __init ldd_uartFlowControl_init(void)
{
    int ret;

    pr_info("ldd_uartFlowControl: init\n");

    //-------------------------------------------
    // Allocate TTY driver
    //-------------------------------------------

    ldd_driver =
        tty_alloc_driver(1,
            TTY_DRIVER_REAL_RAW |
            TTY_DRIVER_DYNAMIC_DEV);

    if (IS_ERR(ldd_driver))
        return PTR_ERR(ldd_driver);

    //-------------------------------------------

    ldd_driver->driver_name = "ldd_uart_flow";
    ldd_driver->name = "ldduartflow";

    ldd_driver->type = TTY_DRIVER_TYPE_SERIAL;
    ldd_driver->subtype = SERIAL_TYPE_NORMAL;

    tty_set_operations(ldd_driver,
        &ldd_ops);

    //-------------------------------------------

    ret = tty_register_driver(ldd_driver);

    if (ret) {
        put_tty_driver(ldd_driver);
        return ret;
    }

    return 0;
}

//-------------------------------------------

static void __exit ldd_uartFlowControl_exit(void)
{
    tty_unregister_driver(ldd_driver);

    put_tty_driver(ldd_driver);

    pr_info("ldd_uartFlowControl: exit\n");
}

//-------------------------------------------

module_init(ldd_uartFlowControl_init);
module_exit(ldd_uartFlowControl_exit);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("UART flow control demonstration");


/*
//-------------------------------------------



//-------------------------------------------
*/


