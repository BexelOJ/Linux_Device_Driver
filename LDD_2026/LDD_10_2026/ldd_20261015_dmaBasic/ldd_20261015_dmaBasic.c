#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/dma-mapping.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Basic Linux DMA API example");

//-------------------------------------------

static int ldd_dmaBasic_probe(struct platform_device* pdev)
{
    struct device* dev = &pdev->dev;

    dev_info(dev, "DMA basic probe\n");

    //-------------------------------------------
    // Check whether the device supports DMA
    //-------------------------------------------

    if (!dma_supported(dev, DMA_BIT_MASK(32))) {
        dev_err(dev, "32-bit DMA is not supported\n");
        return -EIO;
    }

    //-------------------------------------------
    // Set DMA mask
    //-------------------------------------------

    if (dma_set_mask_and_coherent(dev, DMA_BIT_MASK(32))) {
        dev_err(dev, "Failed to set DMA mask\n");
        return -EIO;
    }

    dev_info(dev, "32-bit DMA configured\n");

    return 0;
}

//-------------------------------------------

static void ldd_dmaBasic_remove(struct platform_device* pdev)
{
    dev_info(&pdev->dev, "DMA basic remove\n");
}

//-------------------------------------------

static const struct of_device_id ldd_dmaBasic_of_match[] = {
    {
        .compatible = "ldd,dma-basic"
    },
    { }
};

MODULE_DEVICE_TABLE(of, ldd_dmaBasic_of_match);

//-------------------------------------------

static struct platform_driver ldd_dmaBasic_driver = {
    .probe = ldd_dmaBasic_probe,
    .remove = ldd_dmaBasic_remove,

    .driver = {
        .name = "ldd_dmaBasic",
        .of_match_table = ldd_dmaBasic_of_match,
    },
};

//-------------------------------------------

module_platform_driver(ldd_dmaBasic_driver);

//-------------------------------------------


/*
//-------------------------------------------



//-------------------------------------------
*/


