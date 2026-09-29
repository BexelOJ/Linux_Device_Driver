#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/pci.h>

/*
 * Complete PCI driver skeleton.
 */

 //-------------------------------------------

#define PCI_VENDOR_ID_LDD    0x1234
#define PCI_DEVICE_ID_LDD    0x5678

//-------------------------------------------

struct pci_complete_data
{
    void __iomem* bar0;

    resource_size_t bar0_start;
    resource_size_t bar0_length;
};

//-------------------------------------------

static int pci_complete_probe(
    struct pci_dev* pdev,
    const struct pci_device_id* id)
{
    struct pci_complete_data* data;

    int ret;

    pr_info("pciComplete: probe()\n");

    //---------------------------------------
    // Enable PCI device
    //---------------------------------------

    ret = pci_enable_device(pdev);

    if (ret)
    {
        pr_err("pciComplete: enable failed: %d\n",
            ret);

        return ret;
    }

    //---------------------------------------
    // Request BAR0
    //---------------------------------------

    ret = pci_request_region(pdev,
        0,
        "ldd_pci_complete");

    if (ret)
    {
        pr_err("pciComplete: BAR0 request failed: %d\n",
            ret);

        pci_disable_device(pdev);

        return ret;
    }

    //---------------------------------------
    // DMA capability
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
        pr_err("pciComplete: DMA mask failed\n");

        pci_release_region(pdev, 0);
        pci_disable_device(pdev);

        return ret;
    }

    //---------------------------------------
    // Map BAR0
    //---------------------------------------

    data = devm_kzalloc(&pdev->dev,
        sizeof(*data),
        GFP_KERNEL);

    if (!data)
    {
        pci_release_region(pdev, 0);
        pci_disable_device(pdev);

        return -ENOMEM;
    }

    data->bar0_start =
        pci_resource_start(pdev, 0);

    data->bar0_length =
        pci_resource_len(pdev, 0);

    data->bar0 =
        pci_iomap(pdev, 0, 0);

    if (!data->bar0)
    {
        pr_err("pciComplete: BAR0 mapping failed\n");

        pci_release_region(pdev, 0);
        pci_disable_device(pdev);

        return -ENOMEM;
    }

    pci_set_drvdata(pdev, data);

    pr_info("pciComplete: driver initialized\n");

    pr_info("pciComplete: BAR0 start = %pa\n",
        &data->bar0_start);

    pr_info("pciComplete: BAR0 size = %pa\n",
        &data->bar0_length);

    return 0;
}

//-------------------------------------------

static void pci_complete_remove(struct pci_dev* pdev)
{
    struct pci_complete_data* data;

    data = pci_get_drvdata(pdev);

    pr_info("pciComplete: remove()\n");

    if (data && data->bar0)
        pci_iounmap(pdev, data->bar0);

    pci_release_region(pdev, 0);

    pci_disable_device(pdev);
}

//-------------------------------------------

static const struct pci_device_id pci_complete_ids[] =
{
    {
        PCI_DEVICE(PCI_VENDOR_ID_LDD,
                   PCI_DEVICE_ID_LDD)
    },

    { }
};

MODULE_DEVICE_TABLE(pci, pci_complete_ids);

//-------------------------------------------

static struct pci_driver pci_complete_driver =
{
    .name = "ldd_pci_complete",

    .id_table = pci_complete_ids,

    .probe = pci_complete_probe,
    .remove = pci_complete_remove,
};

//-------------------------------------------

module_pci_driver(pci_complete_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Complete PCI driver skeleton");


/*
//-------------------------------------------



//-------------------------------------------
*/


