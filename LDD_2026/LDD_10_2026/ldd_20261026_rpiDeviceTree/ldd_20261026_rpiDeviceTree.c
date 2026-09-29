#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/of.h>

#define DRIVER_NAME "ldd_20261026_rpiDeviceTree"

static int rpiDeviceTree_probe(struct platform_device* pdev)
{
    const char* label;
    u32 frequency;

    dev_info(&pdev->dev,
        "%s: Device Tree node matched\n",
        DRIVER_NAME);

    if (!of_property_read_string(pdev->dev.of_node,
        "label",
        &label)) {

        dev_info(&pdev->dev,
            "label = %s\n",
            label);
    }

    if (!of_property_read_u32(pdev->dev.of_node,
        "clock-frequency",
        &frequency)) {

        dev_info(&pdev->dev,
            "clock-frequency = %u\n",
            frequency);
    }

    return 0;
}

static void rpiDeviceTree_remove(struct platform_device* pdev)
{
    dev_info(&pdev->dev,
        "%s: Device Tree driver removed\n",
        DRIVER_NAME);
}

static const struct of_device_id rpiDeviceTree_of_match[] = {
    {
        .compatible = "bexel,rpi-device-tree",
    },
    { }
};

MODULE_DEVICE_TABLE(of, rpiDeviceTree_of_match);

static struct platform_driver rpiDeviceTree_driver = {
    .probe = rpiDeviceTree_probe,
    .remove = rpiDeviceTree_remove,

    .driver = {
        .name = DRIVER_NAME,
        .of_match_table = rpiDeviceTree_of_match,
    },
};

module_platform_driver(rpiDeviceTree_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Raspberry Pi Device Tree driver");


/*
//-------------------------------------------



//-------------------------------------------
*/


