#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/dma-mapping.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("DMA coherent memory allocation example");

//-------------------------------------------

struct ldd_dmaAlloc_data {
    void* cpu_addr;
    dma_addr_t dma_addr;
};

//-------------------------------------------

static int ldd_dmaAlloc_probe(struct platform_device* pdev)
{
    struct device* dev = &pdev->dev;
    struct ldd_dmaAlloc_data* data;
    size_t size = PAGE_SIZE;

    //-------------------------------------------
    // Configure DMA
    //-------------------------------------------

    if (dma_set_mask_and_coherent(dev, DMA_BIT_MASK(32))) {
        dev_err(dev, "Unable to configure DMA\n");
        return -EIO;
    }

    //-------------------------------------------
    // Allocate DMA coherent memory
    //-------------------------------------------

    data = devm_kzalloc(dev, sizeof(*data), GFP_KERNEL);

    if (!data)
        return -ENOMEM;

    data->cpu_addr =
        dma_alloc_coherent(dev,
            size,
            &data->dma_addr,
            GFP_KERNEL);

    if (!data->cpu_addr) {
        dev_err(dev, "dma_alloc_coherent() failed\n");
        return -ENOMEM;
    }

    //-------------------------------------------

    dev_info(dev, "CPU address : %p\n", data->cpu_addr);
    dev_info(dev, "DMA address : %pad\n", &data->dma_addr);
    dev_info(dev, "Size        : %zu bytes\n", size);

    platform_set_drvdata(pdev, data);

    return 0;
}

//-------------------------------------------

static void ldd_dmaAlloc_remove(struct platform_device* pdev)
{
    struct ldd_dmaAlloc_data* data;
    struct device* dev = &pdev->dev;

    data = platform_get_drvdata(pdev);

    if (!data)
        return;

    //-------------------------------------------
    // Free coherent DMA memory
    //-------------------------------------------

    dma_free_coherent(dev,
        PAGE_SIZE,
        data->cpu_addr,
        data->dma_addr);

    dev_info(dev, "DMA memory freed\n");
}

//-------------------------------------------

static const struct of_device_id ldd_dmaAlloc_of_match[] = {
    {
        .compatible = "ldd,dma-alloc"
    },
    { }
};

MODULE_DEVICE_TABLE(of, ldd_dmaAlloc_of_match);

//-------------------------------------------

static struct platform_driver ldd_dmaAlloc_driver = {
    .probe = ldd_dmaAlloc_probe,
    .remove = ldd_dmaAlloc_remove,

    .driver = {
        .name = "ldd_dmaAlloc",
        .of_match_table = ldd_dmaAlloc_of_match,
    },
};

//-------------------------------------------

module_platform_driver(ldd_dmaAlloc_driver);

//-------------------------------------------


/*
//-------------------------------------------



//-------------------------------------------
*/


