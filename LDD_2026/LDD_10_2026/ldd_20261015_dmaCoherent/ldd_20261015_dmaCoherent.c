#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/dma-mapping.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("DMA coherent memory example");

//-------------------------------------------

struct ldd_dmaCoherent_data {
    void* cpu_addr;
    dma_addr_t dma_addr;
};

//-------------------------------------------

static int ldd_dmaCoherent_probe(struct platform_device* pdev)
{
    struct device* dev = &pdev->dev;
    struct ldd_dmaCoherent_data* data;
    size_t size = 4096;

    //-------------------------------------------

    if (dma_set_mask_and_coherent(dev, DMA_BIT_MASK(32)))
        return -EIO;

    //-------------------------------------------

    data = devm_kzalloc(dev, sizeof(*data), GFP_KERNEL);

    if (!data)
        return -ENOMEM;

    //-------------------------------------------
    // Allocate coherent DMA memory
    //-------------------------------------------

    data->cpu_addr =
        dma_alloc_coherent(dev,
            size,
            &data->dma_addr,
            GFP_KERNEL);

    if (!data->cpu_addr)
        return -ENOMEM;

    //-------------------------------------------

    memset(data->cpu_addr, 0, size);

    dev_info(dev, "Coherent buffer allocated\n");
    dev_info(dev, "CPU address: %p\n", data->cpu_addr);
    dev_info(dev, "DMA address: %pad\n", &data->dma_addr);

    //-------------------------------------------

    platform_set_drvdata(pdev, data);

    return 0;
}

//-------------------------------------------

static void ldd_dmaCoherent_remove(struct platform_device* pdev)
{
    struct device* dev = &pdev->dev;
    struct ldd_dmaCoherent_data* data;

    data = platform_get_drvdata(pdev);

    if (!data)
        return;

    dma_free_coherent(dev,
        4096,
        data->cpu_addr,
        data->dma_addr);

    dev_info(dev, "Coherent buffer released\n");
}

//-------------------------------------------

static const struct of_device_id ldd_dmaCoherent_of_match[] = {
    {
        .compatible = "ldd,dma-coherent"
    },
    { }
};

MODULE_DEVICE_TABLE(of, ldd_dmaCoherent_of_match);

//-------------------------------------------

static struct platform_driver ldd_dmaCoherent_driver = {
    .probe = ldd_dmaCoherent_probe,
    .remove = ldd_dmaCoherent_remove,

    .driver = {
        .name = "ldd_dmaCoherent",
        .of_match_table = ldd_dmaCoherent_of_match,
    },
};

//-------------------------------------------

module_platform_driver(ldd_dmaCoherent_driver);

//-------------------------------------------



