#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/dmaengine.h>

#define DRIVER_NAME "ldd_20261024_embeddedDMA"

static struct dma_chan* ldd_dma_channel;

static int __init embeddedDMA_init(void)
{
    ldd_dma_channel = dma_request_chan(NULL, "rx");

    if (IS_ERR(ldd_dma_channel)) {
        pr_info("%s: DMA channel not available\n",
            DRIVER_NAME);

        ldd_dma_channel = NULL;

        return 0;
    }

    pr_info("%s: DMA channel acquired\n",
        DRIVER_NAME);

    return 0;
}

static void __exit embeddedDMA_exit(void)
{
    if (ldd_dma_channel) {
        dma_release_channel(ldd_dma_channel);

        pr_info("%s: DMA channel released\n",
            DRIVER_NAME);
    }

    pr_info("%s: module unloaded\n", DRIVER_NAME);
}

module_init(embeddedDMA_init);
module_exit(embeddedDMA_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Embedded Linux DMA example");



/*
//-------------------------------------------

Device Driver
     |
     v
DMA Engine API
     |
     v
DMA Controller
     |
     v
Memory <----> Peripheral

//-------------------------------------------
*/


