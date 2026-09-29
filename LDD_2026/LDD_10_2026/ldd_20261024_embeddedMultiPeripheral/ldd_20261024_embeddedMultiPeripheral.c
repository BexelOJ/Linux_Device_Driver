#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>

#define DRIVER_NAME "ldd_20261024_embeddedMultiPeripheral"

static int embeddedMultiPeripheral_probe(
    struct platform_device* pdev)
{
    struct resource* res;

    pr_info("%s: probe\n", DRIVER_NAME);

    res = platform_get_resource(pdev,
        IORESOURCE_MEM,
        0);

    if (res) {
        pr_info("%s: memory resource start=0x%lx\n",
            DRIVER_NAME,
            (unsigned long)res->start);
    }

    res = platform_get_resource(pdev,
        IORESOURCE_IRQ,
        0);

    if (res) {
        pr_info("%s: IRQ=%lu\n",
            DRIVER_NAME,
            (unsigned long)res->start);
    }

    return 0;
}

static void embeddedMultiPeripheral_remove(
    struct platform_device* pdev)
{
    pr_info("%s: remove\n", DRIVER_NAME);
}

static const struct of_device_id embeddedMultiPeripheral_of_match[] = {
    {
        .compatible = "bexel,multi-peripheral",
    },
    { }
};

MODULE_DEVICE_TABLE(of, embeddedMultiPeripheral_of_match);

static struct platform_driver embeddedMultiPeripheral_driver = {
    .probe = embeddedMultiPeripheral_probe,
    .remove = embeddedMultiPeripheral_remove,

    .driver = {
        .name = DRIVER_NAME,
        .of_match_table =
            embeddedMultiPeripheral_of_match,
    },
};

module_platform_driver(embeddedMultiPeripheral_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Embedded Linux multi-peripheral example");


/*
//-------------------------------------------



//-------------------------------------------
*/


