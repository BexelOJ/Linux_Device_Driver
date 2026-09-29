#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/interrupt.h>
#include <linux/completion.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("DMA interrupt example");

//-------------------------------------------

struct ldd_dmaInterrupt_data {
    struct completion dma_done;
    unsigned int irq_count;
};

//-------------------------------------------

static irqreturn_t ldd_dmaInterrupt_handler(int irq, void* dev_id)
{
    struct ldd_dmaInterrupt_data* data = dev_id;

    //-------------------------------------------
    // Hardware-specific status register
    // should normally be checked here.
    //-------------------------------------------

    data->irq_count++;

    dev_info_once(
        NULL,
        "DMA interrupt received\n"
    );

    //-------------------------------------------
    // Notify waiting code that DMA completed
    //-------------------------------------------

    complete(&data->dma_done);

    return IRQ_HANDLED;
}

//-------------------------------------------

static int ldd_dmaInterrupt_probe(struct platform_device* pdev)
{
    struct device* dev = &pdev->dev;
    struct ldd_dmaInterrupt_data* data;
    int irq;
    int ret;

    //-------------------------------------------

    data = devm_kzalloc(dev, sizeof(*data), GFP_KERNEL);

    if (!data)
        return -ENOMEM;

    //-------------------------------------------

    init_completion(&data->dma_done);

    //-------------------------------------------
    // Get IRQ from Device Tree / firmware
    //-------------------------------------------

    irq = platform_get_irq(pdev, 0);

    if (irq < 0)
        return irq;

    //-------------------------------------------
    // Request IRQ
    //-------------------------------------------

    ret = devm_request_irq(dev,
        irq,
        ldd_dmaInterrupt_handler,
        0,
        "ldd_dmaInterrupt",
        data);

    if (ret) {
        dev_err(dev,
            "Failed to request IRQ %d\n",
            irq);

        return ret;
    }

    //-------------------------------------------

    dev_info(dev,
        "DMA IRQ registered: %d\n",
        irq);

    platform_set_drvdata(pdev, data);

    return 0;
}

//-------------------------------------------

static void ldd_dmaInterrupt_remove(struct platform_device* pdev)
{
    struct ldd_dmaInterrupt_data* data;

    data = platform_get_drvdata(pdev);

    if (data) {
        dev_info(&pdev->dev,
            "DMA IRQ count: %u\n",
            data->irq_count);
    }

    dev_info(&pdev->dev,
        "DMA interrupt removed\n");
}

//-------------------------------------------

static const struct of_device_id ldd_dmaInterrupt_of_match[] = {
    {
        .compatible = "ldd,dma-interrupt"
    },
    { }
};

MODULE_DEVICE_TABLE(of, ldd_dmaInterrupt_of_match);

//-------------------------------------------

static struct platform_driver ldd_dmaInterrupt_driver = {
    .probe = ldd_dmaInterrupt_probe,
    .remove = ldd_dmaInterrupt_remove,

    .driver = {
        .name = "ldd_dmaInterrupt",
        .of_match_table = ldd_dmaInterrupt_of_match,
    },
};

//-------------------------------------------

module_platform_driver(ldd_dmaInterrupt_driver);

//-------------------------------------------


/*
//-------------------------------------------



//-------------------------------------------
*/


