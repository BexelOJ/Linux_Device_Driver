#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/pci.h>

/*
 * Demonstrates struct pci_dev.
 */

 //-------------------------------------------

#define PCI_VENDOR_ID_LDD    0x1234
#define PCI_DEVICE_ID_LDD    0x5678

//-------------------------------------------

static int pci_device_probe(struct pci_dev* pdev,
    const struct pci_device_id* id)
{
    pr_info("pciDevice: probe()\n");

    pr_info("pciDevice: vendor = 0x%04X\n",
        pdev->vendor);

    pr_info("pciDevice: device = 0x%04X\n",
        pdev->device);

    pr_info("pciDevice: revision = 0x%02X\n",
        pdev->revision);

    pr_info("pciDevice: IRQ = %d\n",
        pdev->irq);

    pr_info("pciDevice: bus = %u\n",
        pdev->bus->number);

    pr_info("pciDevice: device/function = 0x%02X\n",
        pdev->devfn);

    return 0;
}

//-------------------------------------------

static void pci_device_remove(struct pci_dev* pdev)
{
    pr_info("pciDevice: remove()\n");
}

//-------------------------------------------

static const struct pci_device_id pci_device_ids[] =
{
    {
        PCI_DEVICE(PCI_VENDOR_ID_LDD,
                   PCI_DEVICE_ID_LDD)
    },

    { }
};

MODULE_DEVICE_TABLE(pci, pci_device_ids);

//-------------------------------------------

static struct pci_driver pci_device_driver =
{
    .name = "ldd_pci_device",

    .id_table = pci_device_ids,

    .probe = pci_device_probe,
    .remove = pci_device_remove,
};

//-------------------------------------------

module_pci_driver(pci_device_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("PCI device structure demonstration");



