#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/pci.h>

/*
 * PCI MMIO demonstration.
 *
 * BAR -> physical MMIO region
 * pci_iomap() -> kernel virtual I/O mapping
 * ioread32()/iowrite32() -> access registers
 */

 //-------------------------------------------

#define PCI_VENDOR_ID_LDD    0x1234
#define PCI_DEVICE_ID_LDD    0x5678

//-------------------------------------------

static int pci_mmio_probe(struct pci_dev* pdev,
    const struct pci_device_id* id)
{
    void __iomem* base;

    resource_size_t start;
    resource_size_t length;

    u32 value;

    int ret;

    pr_info("pciMMIO: probe()\n");

    ret = pci_enable_device(pdev);

    if (ret)
    {
        pr_err("pciMMIO: pci_enable_device() failed: %d\n",
            ret);

        return ret;
    }

    start = pci_resource_start(pdev, 0);
    length = pci_resource_len(pdev, 0);

    pr_info("pciMMIO: BAR0 start = %pa\n", &start);
    pr_info("pciMMIO: BAR0 length = %pa\n", &length);

    base = pci_iomap(pdev, 0, 0);

    if (!base)
    {
        pr_err("pciMMIO: pci_iomap() failed\n");

        pci_disable_device(pdev);

        return -ENOMEM;
    }

    /*
     * Demonstration register access.
     *
     * Offset 0x00 is assumed to be a readable
     * device register.
     *
     * Real hardware defines its own register map.
     */

    value = ioread32(base + 0x00);

    pr_info("pciMMIO: register[0x00] = 0x%08X\n",
        value);

    pci_iounmap(pdev, base);

    pci_disable_device(pdev);

    return 0;
}

//-------------------------------------------

static void pci_mmio_remove(struct pci_dev* pdev)
{
    pr_info("pciMMIO: remove()\n");
}

//-------------------------------------------

static const struct pci_device_id pci_mmio_ids[] =
{
    {
        PCI_DEVICE(PCI_VENDOR_ID_LDD,
                   PCI_DEVICE_ID_LDD)
    },

    { }
};

MODULE_DEVICE_TABLE(pci, pci_mmio_ids);

//-------------------------------------------

static struct pci_driver pci_mmio_driver =
{
    .name = "ldd_pci_mmio",

    .id_table = pci_mmio_ids,

    .probe = pci_mmio_probe,
    .remove = pci_mmio_remove,
};

//-------------------------------------------

module_pci_driver(pci_mmio_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("PCI MMIO demonstration");



