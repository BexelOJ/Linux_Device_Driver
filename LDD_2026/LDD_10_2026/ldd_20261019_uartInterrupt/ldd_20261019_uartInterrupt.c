#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/interrupt.h>

//-------------------------------------------

struct ldd_uart_irq_data {
    int irq;
};

//-------------------------------------------

static irqreturn_t ldd_uart_irq_handler(int irq,
    void* dev_id)
{
    struct ldd_uart_irq_data* data = dev_id;

    pr_info("ldd_uartInterrupt: UART IRQ received\n");

    /*
     * A real UART driver would:
     *
     * 1. Read UART interrupt status
     * 2. Determine RX/TX/error reason
     * 3. Read/write UART FIFO
     * 4. Notify TTY core
     */

    return IRQ_HANDLED;
}

//-------------------------------------------

static int ldd_uart_probe(struct platform_device* pdev)
{
    struct ldd_uart_irq_data* data;
    int ret;

    dev_info(&pdev->dev,
        "ldd_uartInterrupt: probe\n");

    //-------------------------------------------
    // Allocate private data
    //-------------------------------------------

    data = devm_kzalloc(&pdev->dev,
        sizeof(*data),
        GFP_KERNEL);

    if (!data)
        return -ENOMEM;

    //-------------------------------------------
    // Get IRQ from platform firmware
    //-------------------------------------------

    data->irq = platform_get_irq(pdev, 0);

    if (data->irq < 0)
        return data->irq;

    //-------------------------------------------
    // Request IRQ
    //-------------------------------------------

    ret = devm_request_irq(&pdev->dev,
        data->irq,
        ldd_uart_irq_handler,
        0,
        "ldd_uartInterrupt",
        data);

    if (ret) {
        dev_err(&pdev->dev,
            "request_irq failed\n");

        return ret;
    }

    platform_set_drvdata(pdev, data);

    dev_info(&pdev->dev,
        "UART IRQ registered: %d\n",
        data->irq);

    return 0;
}

//-------------------------------------------

static void ldd_uart_remove(struct platform_device* pdev)
{
    dev_info(&pdev->dev,
        "ldd_uartInterrupt: remove\n");
}

//-------------------------------------------

static const struct of_device_id
ldd_uart_of_match[] = {

    {
        .compatible = "ldd,uart-irq",
    },

    { }
};

MODULE_DEVICE_TABLE(of, ldd_uart_of_match);

//-------------------------------------------

static struct platform_driver ldd_uart_driver = {

    .probe = ldd_uart_probe,
    .remove = ldd_uart_remove,

    .driver = {
        .name = "ldd-uart-irq",
        .of_match_table = ldd_uart_of_match,
    },
};

//-------------------------------------------

module_platform_driver(ldd_uart_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("UART interrupt demonstration");


/*
//-------------------------------------------

UART interrupt framework:


//-------------------------------------------
*/


