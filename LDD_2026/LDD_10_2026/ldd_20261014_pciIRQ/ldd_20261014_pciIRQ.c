#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/pci.h>

/*
 * PCI IRQ demonstration.
 */

 //-------------------------------------------

#define PCI_VENDOR_ID_LDD    0x1234
#define PCI_DEVICE_ID_LDD    0x5678

//-------------------------------------------

struct pci_irq_data
{
    int irq_count;
};

//-------------------------------------------

static irqreturn_t pci_irq_handler(
    int irq,
    void* dev_id)
{
    struct pci_irq_data* data;

    data = dev_id;

    data->irq_count++;

    pr_info("pciIRQ: interrupt received, count = %d\n",
        data->irq_count);

    return IRQ_HANDLED;
}

//-------------------------------------------

static int pci_irq_probe(struct pci_dev* pdev,
    const struct pci_device_id* id)
{
    struct pci_irq_data* data;

    int ret;

    pr_info("pciIRQ: probe()\n");

    //---------------------------------------
    // Enable device
    //---------------------------------------

    ret = pci_enable_device(pdev);

    if (ret)
        return ret;

    //---------------------------------------
    // Request managed IRQ
    //---------------------------------------

    data = devm_kzalloc(&pdev->dev,
        sizeof(*data),
        GFP_KERNEL);

    if (!data)
    {
        pci_disable_device(pdev);

        return -ENOMEM;
    }

    ret = devm_request_irq(
        &pdev->dev,
        pdev->irq,
        pci_irq_handler,
        IRQF_SHARED,
        "ldd_pci_irq",
        data);

    if (ret)
    {
        pr_err("pciIRQ: request_irq() failed: %d\n",
            ret);

        pci_disable_device(pdev);

        return ret;
    }

    pci_set_drvdata(pdev, data);

    pr_info("pciIRQ: IRQ = %d\n",
        pdev->irq);

    pr_info("pciIRQ: handler registered\n");

    return 0;
}

//-------------------------------------------

static void pci_irq_remove(struct pci_dev* pdev)
{
    pr_info("pciIRQ: remove()\n");

    pci_disable_device(pdev);
}

//-------------------------------------------

static const struct pci_device_id pci_irq_ids[] =
{
    {
        PCI_DEVICE(PCI_VENDOR_ID_LDD,
                   PCI_DEVICE_ID_LDD)
    },

    { }
};

MODULE_DEVICE_TABLE(pci, pci_irq_ids);

//-------------------------------------------

static struct pci_driver pci_irq_driver =
{
    .name = "ldd_pci_irq",

    .id_table = pci_irq_ids,

    .probe = pci_irq_probe,
    .remove = pci_irq_remove,
};

//-------------------------------------------

module_pci_driver(pci_irq_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("PCI IRQ demonstration");



