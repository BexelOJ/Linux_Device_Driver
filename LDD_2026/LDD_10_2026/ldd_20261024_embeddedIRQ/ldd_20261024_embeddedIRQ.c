#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/interrupt.h>

#define DRIVER_NAME "ldd_20261024_embeddedIRQ"

static irqreturn_t embeddedIRQ_handler(
    int irq,
    void* data)
{
    pr_info("%s: IRQ received: %d\n",
        DRIVER_NAME,
        irq);

    return IRQ_HANDLED;
}

static int embeddedIRQ_probe(struct platform_device* pdev)
{
    int irq;
    int ret;

    irq = platform_get_irq(pdev, 0);

    if (irq < 0)
        return irq;

    ret = devm_request_irq(&pdev->dev,
        irq,
        embeddedIRQ_handler,
        IRQF_SHARED,
        DRIVER_NAME,
        pdev);

    if (ret) {
        dev_err(&pdev->dev,
            "failed to request IRQ %d\n",
            irq);

        return ret;
    }

    pr_info("%s: IRQ %d registered\n",
        DRIVER_NAME,
        irq);

    return 0;
}

static void embeddedIRQ_remove(struct platform_device* pdev)
{
    pr_info("%s: IRQ driver removed\n",
        DRIVER_NAME);
}

static const struct of_device_id embeddedIRQ_of_match[] = {
    {
        .compatible = "bexel,embedded-irq",
    },
    { }
};

MODULE_DEVICE_TABLE(of, embeddedIRQ_of_match);

static struct platform_driver embeddedIRQ_driver = {
    .probe = embeddedIRQ_probe,
    .remove = embeddedIRQ_remove,

    .driver = {
        .name = DRIVER_NAME,
        .of_match_table = embeddedIRQ_of_match,
    },
};

module_platform_driver(embeddedIRQ_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Embedded Linux IRQ example");


/*
//-------------------------------------------



//-------------------------------------------
*/


