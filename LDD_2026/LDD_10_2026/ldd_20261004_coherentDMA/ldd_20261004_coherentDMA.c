//-------------------------------------------
// ldd_20261004_dmaMapping.c
//-------------------------------------------

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/dma-mapping.h>
#include <linux/slab.h>

#define DMA_BUFFER_SIZE 4096

static char* dma_buffer;
static dma_addr_t dma_address;

//-------------------------------------------
// Module Init
//-------------------------------------------

static int __init dma_mapping_init(void)
{
    printk(KERN_INFO
        "ldd_20261004_dmaMapping: init\n");

    //-------------------------------------------
    // Allocate normal kernel memory
    //-------------------------------------------

    dma_buffer = kmalloc(
        DMA_BUFFER_SIZE,
        GFP_KERNEL
    );

    if (!dma_buffer)
    {
        printk(KERN_ERR
            "ldd_20261004_dmaMapping: kmalloc failed\n");

        return -ENOMEM;
    }

    printk(KERN_INFO
        "Kernel virtual address: %px\n",
        dma_buffer);

    //-------------------------------------------
    // Put some data into the buffer
    //-------------------------------------------

    memset(
        dma_buffer,
        0xAA,
        DMA_BUFFER_SIZE
    );

    //-------------------------------------------
    // Map kernel memory for DMA
    //-------------------------------------------

    dma_address = dma_map_single(
        NULL,
        dma_buffer,
        DMA_BUFFER_SIZE,
        DMA_TO_DEVICE
    );

    //-------------------------------------------
    // Check whether mapping succeeded
    //-------------------------------------------

    if (dma_mapping_error(NULL, dma_address))
    {
        printk(KERN_ERR
            "ldd_20261004_dmaMapping: DMA mapping failed\n");

        kfree(dma_buffer);
        dma_buffer = NULL;

        return -EIO;
    }

    printk(KERN_INFO
        "DMA mapping successful\n");

    printk(KERN_INFO
        "DMA address: %pad\n",
        &dma_address);

    //-------------------------------------------
    // At this point a real device could use
    // dma_address for the DMA operation.
    //-------------------------------------------

    printk(KERN_INFO
        "Buffer mapped for DMA_TO_DEVICE\n");

    return 0;
}

//-------------------------------------------
// Module Exit
//-------------------------------------------

static void __exit dma_mapping_exit(void)
{
    printk(KERN_INFO
        "ldd_20261004_dmaMapping: exit\n");

    //-------------------------------------------
    // Unmap DMA buffer
    //-------------------------------------------

    if (dma_buffer)
    {
        dma_unmap_single(
            NULL,
            dma_address,
            DMA_BUFFER_SIZE,
            DMA_TO_DEVICE
        );

        printk(KERN_INFO
            "DMA mapping removed\n");

        //-------------------------------------------
        // Free normal kernel memory
        //-------------------------------------------

        kfree(dma_buffer);
        dma_buffer = NULL;

        printk(KERN_INFO
            "Kernel buffer freed\n");
    }
}

//-------------------------------------------

module_init(dma_mapping_init);
module_exit(dma_mapping_exit);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Basic DMA mapping example");

//-------------------------------------------



