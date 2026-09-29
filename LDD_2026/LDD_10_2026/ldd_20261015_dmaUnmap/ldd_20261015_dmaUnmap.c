#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/dma-mapping.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("DMA unmap example");

//-------------------------------------------

struct ldd_dmaUnmap_data {
    void* buffer;
    dma_addr_t dma_addr;
    size_t size;
};

//-------------------------------------------

static int ldd_dmaUnmap_probe(struct platform_device* pdev)
{
    struct device* dev = &pdev->dev;
    struct ldd_dmaUnmap_data* data;

    //-------------------------------------------

    if (dma_set_mask_and_coherent(dev, DMA_BIT_MASK(32)))
        return -EIO;

    //-------------------------------------------

    data = devm_kzalloc(dev, sizeof(*data), GFP_KERNEL);

    if (!data)
        return -ENOMEM;

    data->size = 2048;

    data->buffer = kmalloc(data->size, GFP_KERNEL);

    if (!data->buffer)
        return -ENOMEM;

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

    dev_info(dev, "Buffer mapped: %pad\n", &data->dma_addr);

    platform_set_drvdata(pdev, data);

    return 0;
}

//-------------------------------------------

static void ldd_dmaUnmap_remove(struct platform_device* pdev)
{
    struct device* dev = &pdev->dev;
    struct ldd_dmaUnmap_data* data;

    data = platform_get_drvdata(pdev);

    if (!data)
        return;

    //-------------------------------------------
    // Important:
    // Device must no longer access the buffer
    //-------------------------------------------

    dma_unmap_single(dev,
        data->dma_addr,
        data->size,
        DMA_TO_DEVICE);

    //-------------------------------------------

    kfree(data->buffer);

    dev_info(dev, "DMA buffer unmapped and freed\n");
}

//-------------------------------------------

static const struct of_device_id ldd_dmaUnmap_of_match[] = {
    {
        .compatible = "ldd,dma-unmap"
    },
    { }
};

MODULE_DEVICE_TABLE(of, ldd_dmaUnmap_of_match);

//-------------------------------------------

static struct platform_driver ldd_dmaUnmap_driver = {
    .probe = ldd_dmaUnmap_probe,
    .remove = ldd_dmaUnmap_remove,

    .driver = {
        .name = "ldd_dmaUnmap",
        .of_match_table = ldd_dmaUnmap_of_match,
    },
};

//-------------------------------------------

module_platform_driver(ldd_dmaUnmap_driver);

//-------------------------------------------



