#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/dma-mapping.h>
#include <linux/completion.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("DMA completion synchronization example");

//-------------------------------------------

struct ldd_dmaComplete_data {
    void* cpu_addr;
    dma_addr_t dma_addr;

    struct completion dma_done;
};

//-------------------------------------------

static int ldd_dmaComplete_probe(struct platform_device* pdev)
{
    struct device* dev = &pdev->dev;
    struct ldd_dmaComplete_data* data;

    //-------------------------------------------

    if (dma_set_mask_and_coherent(dev, DMA_BIT_MASK(32)))
        return -EIO;

    //-------------------------------------------

    data = devm_kzalloc(dev, sizeof(*data), GFP_KERNEL);

    if (!data)
        return -ENOMEM;

    //-------------------------------------------

    init_completion(&data->dma_done);

    //-------------------------------------------

    data->cpu_addr =
        dma_alloc_coherent(dev,
            PAGE_SIZE,
            &data->dma_addr,
            GFP_KERNEL);

    if (!data->cpu_addr)
        return -ENOMEM;

    //-------------------------------------------

    dev_info(dev, "DMA buffer allocated\n");
    dev_info(dev, "DMA address: %pad\n", &data->dma_addr);

    //-------------------------------------------
    // Normally:
    //
    // 1. Program device with dma_addr
    // 2. Start DMA
    // 3. Device generates IRQ
    // 4. IRQ handler calls complete()
    // 5. Worker/thread calls wait_for_completion()
    //
    //-------------------------------------------

    platform_set_drvdata(pdev, data);

    return 0;
}

//-------------------------------------------

static void ldd_dmaComplete_remove(struct platform_device* pdev)
{
    struct device* dev = &pdev->dev;
    struct ldd_dmaComplete_data* data;

    data = platform_get_drvdata(pdev);

    if (!data)
        return;

    //-------------------------------------------

    dma_free_coherent(dev,
        PAGE_SIZE,
        data->cpu_addr,
        data->dma_addr);

    dev_info(dev, "DMA completion example removed\n");
}

//-------------------------------------------

static const struct of_device_id ldd_dmaComplete_of_match[] = {
    {
        .compatible = "ldd,dma-complete"
    },
    { }
};

MODULE_DEVICE_TABLE(of, ldd_dmaComplete_of_match);

//-------------------------------------------

static struct platform_driver ldd_dmaComplete_driver = {
    .probe = ldd_dmaComplete_probe,
    .remove = ldd_dmaComplete_remove,

    .driver = {
        .name = "ldd_dmaComplete",
        .of_match_table = ldd_dmaComplete_of_match,
    },
};

//-------------------------------------------

module_platform_driver(ldd_dmaComplete_driver);

//-------------------------------------------


/*
//-------------------------------------------



//-------------------------------------------
*/


