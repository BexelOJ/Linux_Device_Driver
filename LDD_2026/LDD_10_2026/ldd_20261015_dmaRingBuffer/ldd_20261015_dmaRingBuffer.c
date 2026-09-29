#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/dma-mapping.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("DMA ring buffer example");

//-------------------------------------------

#define LDD_DMA_RING_SIZE 8
#define LDD_DMA_BUFFER_SIZE 4096

//-------------------------------------------

struct ldd_dma_ring_entry {
    void* cpu_addr;
    dma_addr_t dma_addr;
};

//-------------------------------------------

struct ldd_dmaRingBuffer_data {
    struct ldd_dma_ring_entry ring[LDD_DMA_RING_SIZE];

    unsigned int head;
    unsigned int tail;
};

//-------------------------------------------

static int ldd_dmaRingBuffer_probe(struct platform_device* pdev)
{
    struct device* dev = &pdev->dev;
    struct ldd_dmaRingBuffer_data* data;
    int i;

    //-------------------------------------------

    if (dma_set_mask_and_coherent(dev, DMA_BIT_MASK(32)))
        return -EIO;

    //-------------------------------------------

    data = devm_kzalloc(dev, sizeof(*data), GFP_KERNEL);

    if (!data)
        return -ENOMEM;

    //-------------------------------------------
    // Allocate ring entries
    //-------------------------------------------

    for (i = 0; i < LDD_DMA_RING_SIZE; i++) {

        data->ring[i].cpu_addr =
            dma_alloc_coherent(dev,
                LDD_DMA_BUFFER_SIZE,
                &data->ring[i].dma_addr,
                GFP_KERNEL);

        if (!data->ring[i].cpu_addr) {
            dev_err(dev,
                "Failed to allocate ring entry %d\n",
                i);

            goto cleanup;
        }

        //---------------------------------------

        memset(data->ring[i].cpu_addr,
            0,
            LDD_DMA_BUFFER_SIZE);

        //---------------------------------------

        dev_info(dev,
            "Ring[%d] CPU=%p DMA=%pad\n",
            i,
            data->ring[i].cpu_addr,
            &data->ring[i].dma_addr);
    }

    //-------------------------------------------

    data->head = 0;
    data->tail = 0;

    //-------------------------------------------

    platform_set_drvdata(pdev, data);

    dev_info(dev,
        "DMA ring buffer created\n");

    return 0;

    //-------------------------------------------

cleanup:

    while (--i >= 0) {

        dma_free_coherent(
            dev,
            LDD_DMA_BUFFER_SIZE,
            data->ring[i].cpu_addr,
            data->ring[i].dma_addr
        );
    }

    return -ENOMEM;
}

//-------------------------------------------

static void ldd_dmaRingBuffer_remove(struct platform_device* pdev)
{
    struct device* dev = &pdev->dev;
    struct ldd_dmaRingBuffer_data* data;
    int i;

    data = platform_get_drvdata(pdev);

    if (!data)
        return;

    //-------------------------------------------
    // Stop hardware DMA before freeing buffers
    //-------------------------------------------

    for (i = 0; i < LDD_DMA_RING_SIZE; i++) {

        if (data->ring[i].cpu_addr) {

            dma_free_coherent(
                dev,
                LDD_DMA_BUFFER_SIZE,
                data->ring[i].cpu_addr,
                data->ring[i].dma_addr
            );
        }
    }

    //-------------------------------------------

    dev_info(dev,
        "DMA ring buffer destroyed\n");
}

//-------------------------------------------

static const struct of_device_id ldd_dmaRingBuffer_of_match[] = {
    {
        .compatible = "ldd,dma-ring-buffer"
    },
    { }
};

MODULE_DEVICE_TABLE(of, ldd_dmaRingBuffer_of_match);

//-------------------------------------------

static struct platform_driver ldd_dmaRingBuffer_driver = {
    .probe = ldd_dmaRingBuffer_probe,
    .remove = ldd_dmaRingBuffer_remove,

    .driver = {
        .name = "ldd_dmaRingBuffer",
        .of_match_table = ldd_dmaRingBuffer_of_match,
    },
};

//-------------------------------------------

module_platform_driver(ldd_dmaRingBuffer_driver);

//-------------------------------------------


/*
//-------------------------------------------



//-------------------------------------------
*/


