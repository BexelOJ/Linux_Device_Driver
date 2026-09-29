#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/pci.h>

/*
 * Basic PCI driver.
 */

 //-------------------------------------------

#define PCI_VENDOR_ID_LDD    0x1234
#define PCI_DEVICE_ID_LDD    0x5678

//-------------------------------------------

static int pci_basic_probe(struct pci_dev* pdev,
    const struct pci_device_id* id)
{
    pr_info("pciBasic: PCI device detected\n");

    pr_info("pciBasic: vendor = 0x%04X\n",
        pdev->vendor);

    pr_info("pciBasic: device = 0x%04X\n",
        pdev->device);

    pr_info("pciBasic: subsystem vendor = 0x%04X\n",
        pdev->subsystem_vendor);

    pr_info("pciBasic: subsystem device = 0x%04X\n",
        pdev->subsystem_device);

    pr_info("pciBasic: bus = %u\n",
        pdev->bus->number);

    pr_info("pciBasic: devfn = 0x%02X\n",
        pdev->devfn);

    return 0;
}

//-------------------------------------------

static void pci_basic_remove(struct pci_dev* pdev)
{
    pr_info("pciBasic: PCI device removed\n");
}

//-------------------------------------------

static const struct pci_device_id pci_basic_ids[] =
{
    {
        PCI_DEVICE(PCI_VENDOR_ID_LDD,
                   PCI_DEVICE_ID_LDD)
    },

    { }
};

MODULE_DEVICE_TABLE(pci, pci_basic_ids);

//-------------------------------------------

static struct pci_driver pci_basic_driver =
{
    .name = "ldd_pci_basic",

    .id_table = pci_basic_ids,

    .probe = pci_basic_probe,
    .remove = pci_basic_remove,
};

//-------------------------------------------

module_pci_driver(pci_basic_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Basic PCI driver");


/*
//-------------------------------------------



//-------------------------------------------
*/


