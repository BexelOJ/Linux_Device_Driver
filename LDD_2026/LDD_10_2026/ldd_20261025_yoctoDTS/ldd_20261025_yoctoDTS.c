#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/of.h>

#define DRIVER_NAME "ldd_20261025_yoctoDTS"

static int yoctoDTS_probe(struct platform_device* pdev)
{
    const char* label;
    u32 frequency;

    dev_info(&pdev->dev,
        "%s: probe called\n",
        DRIVER_NAME);

    if (!of_property_read_string(
        pdev->dev.of_node,
        "label",
        &label)) {

        dev_info(&pdev->dev,
            "label=%s\n",
            label);
    }

    if (!of_property_read_u32(
        pdev->dev.of_node,
        "clock-frequency",
        &frequency)) {

        dev_info(&pdev->dev,
            "clock-frequency=%u\n",
            frequency);
    }

    return 0;
}

static void yoctoDTS_remove(struct platform_device* pdev)
{
    dev_info(&pdev->dev,
        "%s: remove called\n",
        DRIVER_NAME);
}

static const struct of_device_id yoctoDTS_of_match[] = {
    {
        .compatible = "bexel,yocto-dts",
    },
    { }
};

MODULE_DEVICE_TABLE(of, yoctoDTS_of_match);

static struct platform_driver yoctoDTS_driver = {
    .probe = yoctoDTS_probe,
    .remove = yoctoDTS_remove,

    .driver = {
        .name = DRIVER_NAME,
        .of_match_table = yoctoDTS_of_match,
    },
};

module_platform_driver(yoctoDTS_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Yocto Device Tree driver example");


/*
//-------------------------------------------



//-------------------------------------------
*/


