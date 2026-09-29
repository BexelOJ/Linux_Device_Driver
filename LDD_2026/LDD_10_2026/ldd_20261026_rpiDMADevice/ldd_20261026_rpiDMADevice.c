#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/dmaengine.h>

#define DRIVER_NAME "ldd_20261026_rpiDMADevice"

struct rpi_dma_data {
    struct dma_chan* channel;
};

static int rpiDMADevice_probe(struct platform_device* pdev)
{
    struct rpi_dma_data* data;

    data = devm_kzalloc(&pdev->dev,
        sizeof(*data),
        GFP_KERNEL);

    if (!data)
        return -ENOMEM;

    data->channel = dma_request_chan(&pdev->dev, "rx");

    if (IS_ERR(data->channel)) {
        dev_info(&pdev->dev,
            "%s: RX DMA channel not available\n",
            DRIVER_NAME);

        data->channel = NULL;
    }
    else {
        dev_info(&pdev->dev,
            "%s: RX DMA channel acquired\n",
            DRIVER_NAME);
    }

    platform_set_drvdata(pdev, data);

    return 0;
}

static void rpiDMADevice_remove(struct platform_device* pdev)
{
    struct rpi_dma_data* data;

    data = platform_get_drvdata(pdev);

    if (data->channel)
        dma_release_channel(data->channel);

    dev_info(&pdev->dev,
        "%s: DMA device removed\n",
        DRIVER_NAME);
}

static const struct of_device_id rpiDMADevice_of_match[] = {
    {
        .compatible = "bexel,rpi-dma-device",
    },
    { }
};

MODULE_DEVICE_TABLE(of, rpiDMADevice_of_match);

static struct platform_driver rpiDMADevice_driver = {
    .probe = rpiDMADevice_probe,
    .remove = rpiDMADevice_remove,

    .driver = {
        .name = DRIVER_NAME,
        .of_match_table = rpiDMADevice_of_match,
    },
};

module_platform_driver(rpiDMADevice_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Raspberry Pi DMA device example");


/*
//-------------------------------------------



//-------------------------------------------
*/


