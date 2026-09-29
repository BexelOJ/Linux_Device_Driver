#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/of_irq.h>
#include <linux/interrupt.h>

//-------------------------------------------

static irqreturn_t dt_irq_handler(int irq, void* data)
{
    pr_info("Device Tree IRQ received: %d\n", irq);

    return IRQ_HANDLED;
}

//-------------------------------------------

static int dt_irq_probe(struct platform_device* pdev)
{
    int irq;
    int ret;

    irq = platform_get_irq(pdev, 0);

    if (irq < 0) {
        dev_err(&pdev->dev,
            "Failed to get IRQ\n");

        return irq;
    }

    dev_info(&pdev->dev,
        "IRQ from Device Tree: %d\n",
        irq);

    ret = devm_request_irq(&pdev->dev,
        irq,
        dt_irq_handler,
        0,
        "ldd_dt_irq",
        pdev);

    if (ret) {
        dev_err(&pdev->dev,
            "Failed to request IRQ\n");

        return ret;
    }

    return 0;
}

//-------------------------------------------

static const struct of_device_id dt_irq_match[] = {
    {
        .compatible = "ldd,irq-demo",
    },
    { }
};

MODULE_DEVICE_TABLE(of, dt_irq_match);

//-------------------------------------------

static struct platform_driver dt_irq_driver = {
    .probe = dt_irq_probe,

    .driver = {
        .name = "ldd_dt_interrupt",
        .of_match_table = dt_irq_match,
    },
};

//-------------------------------------------

module_platform_driver(dt_irq_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Device Tree interrupt example");


/*
//-------------------------------------------



//-------------------------------------------
*/


