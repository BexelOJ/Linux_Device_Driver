#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/of.h>

#define DRIVER_NAME "ldd_20261024_embeddedDT"

static int embeddedDT_probe(struct platform_device* pdev)
{
    u32 value;
    const char* label;

    if (!of_property_read_u32(pdev->dev.of_node,
        "clock-frequency",
        &value)) {

        pr_info("%s: clock-frequency=%u\n",
            DRIVER_NAME,
            value);
    }

    if (!of_property_read_string(pdev->dev.of_node,
        "label",
        &label)) {

        pr_info("%s: label=%s\n",
            DRIVER_NAME,
            label);
    }

    pr_info("%s: Device Tree node matched\n",
        DRIVER_NAME);

    return 0;
}

static void embeddedDT_remove(struct platform_device* pdev)
{
    pr_info("%s: Device Tree driver removed\n",
        DRIVER_NAME);
}

static const struct of_device_id embeddedDT_of_match[] = {
    {
        .compatible = "bexel,embedded-dt",
    },
    { }
};

MODULE_DEVICE_TABLE(of, embeddedDT_of_match);

static struct platform_driver embeddedDT_driver = {
    .probe = embeddedDT_probe,
    .remove = embeddedDT_remove,

    .driver = {
        .name = DRIVER_NAME,
        .of_match_table = embeddedDT_of_match,
    },
};

module_platform_driver(embeddedDT_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Embedded Linux Device Tree example");


/*
//-------------------------------------------



//-------------------------------------------
*/


