#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/dma-mapping.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Streaming DMA mapping example");

//-------------------------------------------

struct ldd_dmaStreaming_data {
    void* buffer;
    dma_addr_t dma_addr;
    size_t size;
};

//-------------------------------------------

static int ldd_dmaStreaming_probe(struct platform_device* pdev)
{
    struct device* dev = &pdev->dev;
    struct ldd_dmaStreaming_data* data;

    //-------------------------------------------

    if (dma_set_mask_and_coherent(dev, DMA_BIT_MASK(32)))
        return -EIO;

    //-------------------------------------------

    data = devm_kzalloc(dev, sizeof(*data), GFP_KERNEL);

    if (!data)
        return -ENOMEM;

    data->size = 4096;

    data->buffer = kmalloc(data->size, GFP_KERNEL);

    if (!data->buffer)
        return -ENOMEM;

    //-------------------------------------------
    // CPU writes data
    //-------------------------------------------

    memset(data->buffer, 0x55, data->size);

    //-------------------------------------------
    // Map for device -> device reads memory
    //-------------------------------------------

    data->dma_addr =
        dma_map_single(dev,
            data->buffer,
            data->size,
            DMA_TO_DEVICE);

    if (dma_mapping_error(dev, data->dma_addr)) {
        kfree(data->buffer);
        return -EIO;
    }

    dev_info(dev, "Streaming DMA mapped\n");
    dev_info(dev, "DMA address: %pad\n", &data->dma_addr);

    //-------------------------------------------
    // Device would now perform DMA
    //-------------------------------------------

    platform_set_drvdata(pdev, data);

    return 0;
}

//-------------------------------------------

static void ldd_dmaStreaming_remove(struct platform_device* pdev)
{
    struct ldd_dmaStreaming_data* data;
    struct device* dev = &pdev->dev;

    data = platform_get_drvdata(pdev);

    if (!data)
        return;

    //-------------------------------------------
    // Device must have stopped using buffer
    //-------------------------------------------

    dma_unmap_single(dev,
        data->dma_addr,
        data->size,
        DMA_TO_DEVICE);

    kfree(data->buffer);

    dev_info(dev, "Streaming DMA unmapped\n");
}

//-------------------------------------------

static const struct of_device_id ldd_dmaStreaming_of_match[] = {
    {
        .compatible = "ldd,dma-streaming"
    },
    { }
};

MODULE_DEVICE_TABLE(of, ldd_dmaStreaming_of_match);

//-------------------------------------------

static struct platform_driver ldd_dmaStreaming_driver = {
    .probe = ldd_dmaStreaming_probe,
    .remove = ldd_dmaStreaming_remove,

    .driver = {
        .name = "ldd_dmaStreaming",
        .of_match_table = ldd_dmaStreaming_of_match,
    },
};

//-------------------------------------------

module_platform_driver(ldd_dmaStreaming_driver);

//-------------------------------------------


/*
//-------------------------------------------



//-------------------------------------------
*/


