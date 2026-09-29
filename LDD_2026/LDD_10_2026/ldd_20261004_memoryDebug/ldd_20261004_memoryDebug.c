//-------------------------------------------
// ldd_20261004_coherentDMA.c
//-------------------------------------------

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/dma-mapping.h>

static int __init coherent_dma_init(void)
{
    printk(KERN_INFO
        "ldd_20261004_coherentDMA: init\n");

    printk(KERN_INFO
        "Coherent DMA memory is normally allocated with:\n");

    printk(KERN_INFO
        "dma_alloc_coherent(dev, size,\n"
        "                   &dma_handle,\n"
        "                   GFP_KERNEL)\n");

    printk(KERN_INFO
        "It returns a CPU-accessible address and a DMA address.\n");

    return 0;
}

//-------------------------------------------

static void __exit coherent_dma_exit(void)
{
    printk(KERN_INFO
        "ldd_20261004_coherentDMA: exit\n");
}

//-------------------------------------------

module_init(coherent_dma_init);
module_exit(coherent_dma_exit);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Coherent DMA API demonstration");

//-------------------------------------------


/*
//-------------------------------------------



//-------------------------------------------
*/


