#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/pci.h>

/*
 * PCI BAR discovery.
 */

 //-------------------------------------------

#define PCI_VENDOR_ID_LDD    0x1234
#define PCI_DEVICE_ID_LDD    0x5678

//-------------------------------------------

static int pci_bar_probe(struct pci_dev* pdev,
    const struct pci_device_id* id)
{
    int bar;

    pr_info("pciBAR: probe()\n");

    for (bar = 0; bar < PCI_STD_NUM_BARS; bar++)
    {
        resource_size_t start;
        resource_size_t length;
        unsigned long flags;

        start = pci_resource_start(pdev, bar);
        length = pci_resource_len(pdev, bar);
        flags = pci_resource_flags(pdev, bar);

        if (!length)
            continue;

        pr_info("pciBAR: BAR%d\n", bar);

        pr_info("pciBAR: start = %pa\n",
            &start);

        pr_info("pciBAR: length = %pa\n",
            &length);

        if (flags & IORESOURCE_MEM)
            pr_info("pciBAR: type = MMIO\n");

        if (flags & IORESOURCE_IO)
            pr_info("pciBAR: type = I/O port\n");
    }

    return 0;
}

//-------------------------------------------

static void pci_bar_remove(struct pci_dev* pdev)
{
    pr_info("pciBAR: remove()\n");
}

//-------------------------------------------

static const struct pci_device_id pci_bar_ids[] =
{
    {
        PCI_DEVICE(PCI_VENDOR_ID_LDD,
                   PCI_DEVICE_ID_LDD)
    },

    { }
};

MODULE_DEVICE_TABLE(pci, pci_bar_ids);

//-------------------------------------------

static struct pci_driver pci_bar_driver =
{
    .name = "ldd_pci_bar",

    .id_table = pci_bar_ids,

    .probe = pci_bar_probe,
    .remove = pci_bar_remove,
};

//-------------------------------------------

module_pci_driver(pci_bar_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("PCI BAR discovery demonstration");


/*
//-------------------------------------------



//-------------------------------------------
*/


