#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/pci.h>

/*
 * struct pci_driver represents the
 * software PCI driver.
 */

 //-------------------------------------------

#define PCI_VENDOR_ID_LDD    0x1234
#define PCI_DEVICE_ID_LDD    0x5678

//-------------------------------------------

static int pci_driver_probe(struct pci_dev* pdev,
    const struct pci_device_id* id)
{
    pr_info("pciDriver: probe()\n");

    pr_info("pciDriver: matched vendor = 0x%04X\n",
        id->vendor);

    pr_info("pciDriver: matched device = 0x%04X\n",
        id->device);

    return 0;
}

//-------------------------------------------

static void pci_driver_remove(struct pci_dev* pdev)
{
    pr_info("pciDriver: remove()\n");
}

//-------------------------------------------

static const struct pci_device_id pci_driver_ids[] =
{
    {
        PCI_DEVICE(PCI_VENDOR_ID_LDD,
                   PCI_DEVICE_ID_LDD)
    },

    { }
};

MODULE_DEVICE_TABLE(pci, pci_driver_ids);

//-------------------------------------------

static struct pci_driver pci_driver =
{
    .name = "ldd_pci_driver",

    .id_table = pci_driver_ids,

    .probe = pci_driver_probe,
    .remove = pci_driver_remove,
};

//-------------------------------------------

module_pci_driver(pci_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Basic PCI driver structure");


/*
//-------------------------------------------



//-------------------------------------------
*/


