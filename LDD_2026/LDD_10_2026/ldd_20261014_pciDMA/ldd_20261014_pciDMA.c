#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/pci.h>

/*
 * PCI DMA demonstration.
 */

 //-------------------------------------------

#define PCI_VENDOR_ID_LDD    0x1234
#define PCI_DEVICE_ID_LDD    0x5678

#define DMA_BUFFER_SIZE      4096

//-------------------------------------------

struct pci_dma_data
{
    void* cpu_buffer;

    dma_addr_t dma_address;
};

//-------------------------------------------

static int pci_dma_probe(struct pci_dev* pdev,
    const struct pci_device_id* id)
{
    struct pci_dma_data* data;

    int ret;

    pr_info("pciDMA: probe()\n");

    //---------------------------------------
    // Enable PCI device
    //---------------------------------------

    ret = pci_enable_device(pdev);

    if (ret)
        return ret;

    //---------------------------------------
    // Set DMA mask
    //---------------------------------------

    ret = dma_set_mask_and_coherent(
        &pdev->dev,
        DMA_BIT_MASK(64));

    if (ret)
    {
        ret = dma_set_mask_and_coherent(
            &pdev->dev,
            DMA_BIT_MASK(32));
    }

    if (ret)
    {
        pr_err("pciDMA: DMA mask not supported\n");

        pci_disable_device(pdev);

        return ret;
    }

    //---------------------------------------
    // Allocate DMA-coherent memory
    //---------------------------------------

    data = devm_kzalloc(&pdev->dev,
        sizeof(*data),
        GFP_KERNEL);

    if (!data)
    {
        pci_disable_device(pdev);

        return -ENOMEM;
    }

    data->cpu_buffer =
        dma_alloc_coherent(
            &pdev->dev,
            DMA_BUFFER_SIZE,
            &data->dma_address,
            GFP_KERNEL);

    if (!data->cpu_buffer)
    {
        pr_err("pciDMA: DMA allocation failed\n");

        pci_disable_device(pdev);

        return -ENOMEM;
    }

    pci_set_drvdata(pdev, data);

    pr_info("pciDMA: CPU address = %px\n",
        data->cpu_buffer);

    pr_info("pciDMA: DMA address = %pad\n",
        &data->dma_address);

    return 0;
}

//-------------------------------------------

static void pci_dma_remove(struct pci_dev* pdev)
{
    struct pci_dma_data* data;

    data = pci_get_drvdata(pdev);

    pr_info("pciDMA: remove()\n");

    if (data && data->cpu_buffer)
    {
        dma_free_coherent(
            &pdev->dev,
            DMA_BUFFER_SIZE,
            data->cpu_buffer,
            data->dma_address);
    }

    pci_disable_device(pdev);
}

//-------------------------------------------

static const struct pci_device_id pci_dma_ids[] =
{
    {
        PCI_DEVICE(PCI_VENDOR_ID_LDD,
                   PCI_DEVICE_ID_LDD)
    },

    { }
};

MODULE_DEVICE_TABLE(pci, pci_dma_ids);

//-------------------------------------------

static struct pci_driver pci_dma_driver =
{
    .name = "ldd_pci_dma",

    .id_table = pci_dma_ids,

    .probe = pci_dma_probe,
    .remove = pci_dma_remove,
};

//-------------------------------------------

module_pci_driver(pci_dma_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("PCI DMA demonstration");


/*
//-------------------------------------------



//-------------------------------------------
*/


