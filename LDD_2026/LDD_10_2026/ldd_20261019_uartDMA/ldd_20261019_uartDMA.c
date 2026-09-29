#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/dmaengine.h>
#include <linux/dma-mapping.h>

//-------------------------------------------

struct ldd_uart_dma {
    struct dma_chan* tx_chan;
    struct dma_chan* rx_chan;
};

//-------------------------------------------

static int ldd_uart_dma_probe(struct platform_device* pdev)
{
    struct ldd_uart_dma* data;
    int ret;

    dev_info(&pdev->dev,
        "ldd_uartDMA: probe\n");

    //-------------------------------------------
    // Allocate private data
    //-------------------------------------------

    data = devm_kzalloc(&pdev->dev,
        sizeof(*data),
        GFP_KERNEL);

    if (!data)
        return -ENOMEM;

    //-------------------------------------------
    // Request TX DMA channel
    //-------------------------------------------

    data->tx_chan =
        dma_request_chan(&pdev->dev, "tx");

    if (IS_ERR(data->tx_chan)) {

        ret = PTR_ERR(data->tx_chan);

        if (ret == -ENODEV ||
            ret == -ENXIO ||
            ret == -EPROBE_DEFER) {

            dev_info(&pdev->dev,
                "TX DMA channel unavailable\n");
        }

        return ret;
    }

    //-------------------------------------------
    // Request RX DMA channel
    //-------------------------------------------

    data->rx_chan =
        dma_request_chan(&pdev->dev, "rx");

    if (IS_ERR(data->rx_chan)) {

        ret = PTR_ERR(data->rx_chan);

        dma_release_channel(data->tx_chan);

        return ret;
    }

    //-------------------------------------------

    platform_set_drvdata(pdev, data);

    dev_info(&pdev->dev,
        "UART DMA channels acquired\n");

    return 0;
}

//-------------------------------------------

static void ldd_uart_dma_remove(struct platform_device* pdev)
{
    struct ldd_uart_dma* data;

    data = platform_get_drvdata(pdev);

    dma_release_channel(data->rx_chan);
    dma_release_channel(data->tx_chan);

    dev_info(&pdev->dev,
        "UART DMA channels released\n");
}

//-------------------------------------------

static const struct of_device_id
ldd_uart_dma_of_match[] = {

    {
        .compatible = "ldd,uart-dma",
    },

    { }
};

MODULE_DEVICE_TABLE(of, ldd_uart_dma_of_match);

//-------------------------------------------

static struct platform_driver
ldd_uart_dma_driver = {

    .probe = ldd_uart_dma_probe,
    .remove = ldd_uart_dma_remove,

    .driver = {
        .name = "ldd-uart-dma",
        .of_match_table = ldd_uart_dma_of_match,
    },
};

//-------------------------------------------

module_platform_driver(ldd_uart_dma_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("UART DMA demonstration");


/*
//-------------------------------------------

CPU memory
    │
    │ DMA
    ▼
UART DMA controller
    │
    ▼
UART FIFO

//-------------------------------------------
*/


