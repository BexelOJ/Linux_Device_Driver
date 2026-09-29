#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/dma-mapping.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Basic DMA mapping example");

//-------------------------------------------

struct ldd_dmaMap_data {
    void* cpu_buffer;
    dma_addr_t dma_address;
};

//-------------------------------------------

static int ldd_dmaMap_probe(struct platform_device* pdev)
{
    struct device* dev = &pdev->dev;
    struct ldd_dmaMap_data* data;
    size_t size = 1024;

    //-------------------------------------------

    if (dma_set_mask_and_coherent(dev, DMA_BIT_MASK(32))) {
        dev_err(dev, "DMA configuration failed\n");
        return -EIO;
    }

    //-------------------------------------------
    // Allocate normal kernel memory
    //-------------------------------------------

    data = devm_kzalloc(dev, sizeof(*data), GFP_KERNEL);

    if (!data)
        return -ENOMEM;

    data->cpu_buffer = kmalloc(size, GFP_KERNEL);

    if (!data->cpu_buffer)
        return -ENOMEM;

    memset(data->cpu_buffer, 0xAA, size);

    //-------------------------------------------
    // Map CPU buffer for DMA
    //-------------------------------------------

    data->dma_address =
        dma_map_single(dev,
            data->cpu_buffer,
            size,
            DMA_TO_DEVICE);

    if (dma_mapping_error(dev, data->dma_address)) {
        dev_err(dev, "DMA mapping failed\n");
        kfree(data->cpu_buffer);
        return -EIO;
    }

    //-------------------------------------------

    dev_info(dev, "CPU buffer : %p\n", data->cpu_buffer);
    dev_info(dev, "DMA address: %pad\n", &data->dma_address);

    platform_set_drvdata(pdev, data);

    return 0;
}

//-------------------------------------------

static void ldd_dmaMap_remove(struct platform_device* pdev)
{
    struct ldd_dmaMap_data* data;
    struct device* dev = &pdev->dev;

    data = platform_get_drvdata(pdev);

    if (!data)
        return;

    //-------------------------------------------
    // Unmap before freeing CPU memory
    //-------------------------------------------

    dma_unmap_single(dev,
        data->dma_address,
        1024,
        DMA_TO_DEVICE);

    kfree(data->cpu_buffer);

    dev_info(dev, "DMA mapping removed\n");
}

//-------------------------------------------

static const struct of_device_id ldd_dmaMap_of_match[] = {
    {
        .compatible = "ldd,dma-map"
    },
    { }
};

MODULE_DEVICE_TABLE(of, ldd_dmaMap_of_match);

//-------------------------------------------

static struct platform_driver ldd_dmaMap_driver = {
    .probe = ldd_dmaMap_probe,
    .remove = ldd_dmaMap_remove,

    .driver = {
        .name = "ldd_dmaMap",
        .of_match_table = ldd_dmaMap_of_match,
    },
};

//-------------------------------------------

module_platform_driver(ldd_dmaMap_driver);

//-------------------------------------------



