#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/io.h>
#include <linux/interrupt.h>
#include <linux/tty.h>
#include <linux/tty_driver.h>

//-------------------------------------------

#define LDD_UART_MINORS 1

//-------------------------------------------

struct ldd_uart {
    void __iomem* base;

    int irq;

    struct tty_driver* tty_driver;
};

//-------------------------------------------

static irqreturn_t ldd_uart_irq(int irq,
    void* dev_id)
{
    struct ldd_uart* uart = dev_id;

    dev_info(uart->tty_driver->dev,
        "UART interrupt received\n");

    /*
     * A real UART driver would:
     *
     * read UART interrupt status
     * read RX FIFO
     * write TX FIFO
     * handle errors
     * pass received characters to TTY core
     */

    return IRQ_HANDLED;
}

//-------------------------------------------

static int ldd_uart_open(struct tty_struct* tty,
    struct file* file)
{
    struct ldd_uart* uart = tty->driver_data;

    if (!uart)
        return -ENODEV;

    pr_info("ldd_uartComplete: open\n");

    return 0;
}

//-------------------------------------------

static void ldd_uart_close(struct tty_struct* tty,
    struct file* file)
{
    pr_info("ldd_uartComplete: close\n");
}

//-------------------------------------------

static ssize_t ldd_uart_write(
    struct tty_struct* tty,
    const unsigned char* buf,
    size_t count)
{
    size_t i;

    pr_info("ldd_uartComplete: TX %zu bytes\n",
        count);

    /*
     * A real driver would write each byte
     * to the UART TX FIFO/register.
     */

    for (i = 0; i < count; i++)
        pr_debug("TX byte = 0x%02x\n",
            buf[i]);

    return count;
}

//-------------------------------------------

static const struct tty_operations
ldd_uart_tty_ops = {

    .open = ldd_uart_open,
    .close = ldd_uart_close,
    .write = ldd_uart_write,
};

//-------------------------------------------

static int ldd_uart_probe(struct platform_device* pdev)
{
    struct ldd_uart* uart;
    struct resource* res;
    int ret;

    dev_info(&pdev->dev,
        "ldd_uartComplete: probe\n");

    //-------------------------------------------
    // Allocate private data
    //-------------------------------------------

    uart = devm_kzalloc(&pdev->dev,
        sizeof(*uart),
        GFP_KERNEL);

    if (!uart)
        return -ENOMEM;

    //-------------------------------------------
    // Get UART memory resource
    //-------------------------------------------

    res = platform_get_resource(pdev,
        IORESOURCE_MEM,
        0);

    if (!res)
        return -ENODEV;

    //-------------------------------------------
    // Map registers
    //-------------------------------------------

    uart->base =
        devm_ioremap_resource(&pdev->dev,
            res);

    if (IS_ERR(uart->base))
        return PTR_ERR(uart->base);

    //-------------------------------------------
    // Get IRQ
    //-------------------------------------------

    uart->irq = platform_get_irq(pdev, 0);

    if (uart->irq < 0)
        return uart->irq;

    //-------------------------------------------
    // Request IRQ
    //-------------------------------------------

    ret = devm_request_irq(&pdev->dev,
        uart->irq,
        ldd_uart_irq,
        0,
        "ldd_uartComplete",
        uart);

    if (ret)
        return ret;

    //-------------------------------------------
    // Allocate TTY driver
    //-------------------------------------------

    uart->tty_driver =
        tty_alloc_driver(LDD_UART_MINORS,
            TTY_DRIVER_REAL_RAW |
            TTY_DRIVER_DYNAMIC_DEV);

    if (IS_ERR(uart->tty_driver))
        return PTR_ERR(uart->tty_driver);

    //-------------------------------------------
    // Configure TTY
    //-------------------------------------------

    uart->tty_driver->driver_name =
        "ldd_uart_complete";

    uart->tty_driver->name =
        "ldduartcomplete";

    uart->tty_driver->type =
        TTY_DRIVER_TYPE_SERIAL;

    uart->tty_driver->subtype =
        SERIAL_TYPE_NORMAL;

    tty_set_operations(uart->tty_driver,
        &ldd_uart_tty_ops);

    //-------------------------------------------
    // Register TTY
    //-------------------------------------------

    ret = tty_register_driver(uart->tty_driver);

    if (ret) {
        put_tty_driver(uart->tty_driver);
        return ret;
    }

    //-------------------------------------------

    platform_set_drvdata(pdev, uart);

    dev_info(&pdev->dev,
        "UART complete driver registered\n");

    dev_info(&pdev->dev,
        "IRQ = %d\n",
        uart->irq);

    return 0;
}

//-------------------------------------------

static void ldd_uart_remove(struct platform_device* pdev)
{
    struct ldd_uart* uart;

    uart = platform_get_drvdata(pdev);

    //-------------------------------------------
    // Unregister TTY
    //-------------------------------------------

    tty_unregister_driver(uart->tty_driver);

    put_tty_driver(uart->tty_driver);

    //-------------------------------------------

    dev_info(&pdev->dev,
        "ldd_uartComplete: remove\n");
}

//-------------------------------------------

static const struct of_device_id
ldd_uart_of_match[] = {

    {
        .compatible = "ldd,uart-complete",
    },

    { }
};

MODULE_DEVICE_TABLE(of, ldd_uart_of_match);

//-------------------------------------------

static struct platform_driver
ldd_uart_driver = {

    .probe = ldd_uart_probe,
    .remove = ldd_uart_remove,

    .driver = {
        .name = "ldd-uart-complete",

        .of_match_table =
            ldd_uart_of_match,
    },
};

//-------------------------------------------

module_platform_driver(ldd_uart_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Complete UART TTY driver");


/*
//-------------------------------------------



//-------------------------------------------
*/


