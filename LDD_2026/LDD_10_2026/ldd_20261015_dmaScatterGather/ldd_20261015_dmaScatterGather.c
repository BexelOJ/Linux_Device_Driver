#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/dma-mapping.h>
#include <linux/scatterlist.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("DMA scatter gather example");

//-------------------------------------------

struct ldd_dmaScatterGather_data {
    struct page* pages[4];
    struct sg_table sg_table;
    int mapped_nents;
};

//-------------------------------------------

static int ldd_dmaScatterGather_probe(struct platform_device* pdev)
{
    struct device* dev = &pdev->dev;
    struct ldd_dmaScatterGather_data* data;
    int i;
    int ret;

    //-------------------------------------------

    if (dma_set_mask_and_coherent(dev, DMA_BIT_MASK(32)))
        return -EIO;

    //-------------------------------------------

    data = devm_kzalloc(dev, sizeof(*data), GFP_KERNEL);

    if (!data)
        return -ENOMEM;

    //-------------------------------------------
    // Allocate four separate pages
    //-------------------------------------------

    for (i = 0; i < 4; i++) {
        data->pages[i] = alloc_page(GFP_KERNEL);

        if (!data->pages[i]) {
            ret = -ENOMEM;
            goto free_pages;
        }
    }

    //-------------------------------------------
    // Create SG table
    //-------------------------------------------

    ret = sg_alloc_table(&data->sg_table, 4, GFP_KERNEL);

    if (ret)
        goto free_pages;

    //-------------------------------------------
    // Add pages to SG entries
    //-------------------------------------------

    for_each_sg(data->sg_table.sgl,
        data->sg_table.sgl,
        data->sg_table.nents,
        i) {

        struct page* page = data->pages[i];

        sg_set_page(data->sg_table.sgl + i,
            page,
            PAGE_SIZE,
            0);
    }

    //-------------------------------------------
    // Map SG list for DMA
    //-------------------------------------------

    data->mapped_nents =
        dma_map_sg(dev,
            data->sg_table.sgl,
            data->sg_table.nents,
            DMA_TO_DEVICE);

    if (data->mapped_nents == 0) {
        dev_err(dev, "dma_map_sg() failed\n");
        sg_free_table(&data->sg_table);
        goto free_pages;
    }

    //-------------------------------------------

    dev_info(dev, "Original SG entries: %u\n",
        data->sg_table.nents);

    dev_info(dev, "Mapped SG entries: %d\n",
        data->mapped_nents);

    platform_set_drvdata(pdev, data);

    return 0;

    //-------------------------------------------

free_pages:

    for (i = 0; i < 4; i++) {
        if (data->pages[i])
            __free_page(data->pages[i]);
    }

    return ret;
}

//-------------------------------------------

static void ldd_dmaScatterGather_remove(struct platform_device* pdev)
{
    struct device* dev = &pdev->dev;
    struct ldd_dmaScatterGather_data* data;
    int i;

    data = platform_get_drvdata(pdev);

    if (!data)
        return;

    //-------------------------------------------
    // Unmap SG list
    //-------------------------------------------

    dma_unmap_sg(dev,
        data->sg_table.sgl,
        data->sg_table.nents,
        DMA_TO_DEVICE);

    //-------------------------------------------

    sg_free_table(&data->sg_table);

    //-------------------------------------------

    for (i = 0; i < 4; i++) {
        if (data->pages[i])
            __free_page(data->pages[i]);
    }

    dev_info(dev, "Scatter-gather DMA released\n");
}

//-------------------------------------------

static const struct of_device_id ldd_dmaScatterGather_of_match[] = {
    {
        .compatible = "ldd,dma-scatter-gather"
    },
    { }
};

MODULE_DEVICE_TABLE(of, ldd_dmaScatterGather_of_match);

//-------------------------------------------

static struct platform_driver ldd_dmaScatterGather_driver = {
    .probe = ldd_dmaScatterGather_probe,
    .remove = ldd_dmaScatterGather_remove,

    .driver = {
        .name = "ldd_dmaScatterGather",
        .of_match_table = ldd_dmaScatterGather_of_match,
    },
};

//-------------------------------------------

module_platform_driver(ldd_dmaScatterGather_driver);

//-------------------------------------------



