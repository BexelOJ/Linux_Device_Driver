#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/pci.h>

/*
 * PCI probe/remove lifecycle.
 *
 * Device found
 *      ↓
 * ID matching
 *      ↓
 * probe()
 *
 * Device removed / driver unloaded
 *      ↓
 * remove()
 */

 //-------------------------------------------

#define PCI_VENDOR_ID_LDD    0x1234
#define PCI_DEVICE_ID_LDD    0x5678

//-------------------------------------------

static int pci_probe_remove_probe(
    struct pci_dev* pdev,
    const struct pci_device_id* id)
{
    pr_info("pciProbeRemove: probe()\n");

    pr_info("pciProbeRemove: vendor = 0x%04X\n",
        pdev->vendor);

    pr_info("pciProbeRemove: device = 0x%04X\n",
        pdev->device);

    pr_info("pciProbeRemove: bus = %u\n",
        pdev->bus->number);

    pr_info("pciProbeRemove: slot = %u\n",
        PCI_SLOT(pdev->devfn));

    pr_info("pciProbeRemove: function = %u\n",
        PCI_FUNC(pdev->devfn));

    return 0;
}

//-------------------------------------------

static void pci_probe_remove_remove(
    struct pci_dev* pdev)
{
    pr_info("pciProbeRemove: remove()\n");
}

//-------------------------------------------

static const struct pci_device_id pci_probe_remove_ids[] =
{
    {
        PCI_DEVICE(PCI_VENDOR_ID_LDD,
                   PCI_DEVICE_ID_LDD)
    },

    { }
};

MODULE_DEVICE_TABLE(pci,
    pci_probe_remove_ids);

//-------------------------------------------

static struct pci_driver pci_probe_remove_driver =
{
    .name = "ldd_pci_probe_remove",

    .id_table = pci_probe_remove_ids,

    .probe = pci_probe_remove_probe,
    .remove = pci_probe_remove_remove,
};

//-------------------------------------------

module_pci_driver(pci_probe_remove_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("PCI probe and remove demonstration");



