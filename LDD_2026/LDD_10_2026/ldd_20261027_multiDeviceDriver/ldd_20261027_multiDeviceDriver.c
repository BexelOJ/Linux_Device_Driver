#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/of.h>

struct multi_device_data {
    const char* name;
    int sensor_id;
};

//-------------------------------------------
// Probe
//-------------------------------------------

static int multi_device_probe(struct platform_device* pdev)
{
    struct multi_device_data* data;

    data = devm_kzalloc(&pdev->dev,
        sizeof(*data),
        GFP_KERNEL);

    if (!data)
        return -ENOMEM;

    data->name = "multi-sensor";
    data->sensor_id = pdev->id;

    platform_set_drvdata(pdev, data);

    dev_info(&pdev->dev,
        "multi-device driver probed\n");

    dev_info(&pdev->dev,
        "device name = %s\n",
        data->name);

    dev_info(&pdev->dev,
        "sensor id = %d\n",
        data->sensor_id);

    return 0;
}

//-------------------------------------------
// Remove
//-------------------------------------------

static void multi_device_remove(struct platform_device* pdev)
{
    dev_info(&pdev->dev,
        "multi-device driver removed\n");
}

//-------------------------------------------
// Device Tree match
//-------------------------------------------

static const struct of_device_id multi_device_of_match[] = {
    {.compatible = "bexel,multi-sensor" },
    { }
};

MODULE_DEVICE_TABLE(of, multi_device_of_match);

//-------------------------------------------
// Platform driver
//-------------------------------------------

static struct platform_driver multi_device_driver = {
    .probe = multi_device_probe,
    .remove = multi_device_remove,

    .driver = {
        .name = "multi-device-driver",
        .of_match_table = multi_device_of_match,
    },
};

module_platform_driver(multi_device_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Multi-device platform driver");


/*
//-------------------------------------------



//-------------------------------------------
*/


