#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/pci.h>

/*
 * PCI configuration space demonstration.
 */

 //-------------------------------------------

#define PCI_VENDOR_ID_LDD    0x1234
#define PCI_DEVICE_ID_LDD    0x5678

//-------------------------------------------

static int pci_config_probe(struct pci_dev* pdev,
    const struct pci_device_id* id)
{
    u16 vendor;
    u16 device;

    u16 command;
    u16 status;

    u8 revision;

    int ret;

    pr_info("pciConfigSpace: probe()\n");

    ret = pci_read_config_word(pdev,
        PCI_VENDOR_ID,
        &vendor);

    if (ret)
        return ret;

    ret = pci_read_config_word(pdev,
        PCI_DEVICE_ID,
        &device);

    if (ret)
        return ret;

    ret = pci_read_config_word(pdev,
        PCI_COMMAND,
        &command);

    if (ret)
        return ret;

    ret = pci_read_config_word(pdev,
        PCI_STATUS,
        &status);

    if (ret)
        return ret;

    ret = pci_read_config_byte(pdev,
        PCI_REVISION_ID,
        &revision);

    if (ret)
        return ret;

    pr_info("pciConfigSpace: vendor = 0x%04X\n",
        vendor);

    pr_info("pciConfigSpace: device = 0x%04X\n",
        device);

    pr_info("pciConfigSpace: command = 0x%04X\n",
        command);

    pr_info("pciConfigSpace: status = 0x%04X\n",
        status);

    pr_info("pciConfigSpace: revision = 0x%02X\n",
        revision);

    return 0;
}

//-------------------------------------------

static void pci_config_remove(struct pci_dev* pdev)
{
    pr_info("pciConfigSpace: remove()\n");
}

//-------------------------------------------

static const struct pci_device_id pci_config_ids[] =
{
    {
        PCI_DEVICE(PCI_VENDOR_ID_LDD,
                   PCI_DEVICE_ID_LDD)
    },

    { }
};

MODULE_DEVICE_TABLE(pci, pci_config_ids);

//-------------------------------------------

static struct pci_driver pci_config_driver =
{
    .name = "ldd_pci_config_space",

    .id_table = pci_config_ids,

    .probe = pci_config_probe,
    .remove = pci_config_remove,
};

//-------------------------------------------

module_pci_driver(pci_config_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("PCI configuration space demonstration");



