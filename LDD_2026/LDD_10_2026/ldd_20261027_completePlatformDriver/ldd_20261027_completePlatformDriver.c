#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/io.h>

struct complete_platform_data {
    void __iomem* base;
    resource_size_t size;
};

//-------------------------------------------
// Probe
//-------------------------------------------

static int complete_platform_probe(struct platform_device* pdev)
{
    struct complete_platform_data* data;
    struct resource* res;

    data = devm_kzalloc(&pdev->dev,
        sizeof(*data),
        GFP_KERNEL);

    if (!data)
        return -ENOMEM;

    res = platform_get_resource(pdev,
        IORESOURCE_MEM,
        0);

    if (!res) {
        dev_err(&pdev->dev,
            "memory resource missing\n");
        return -ENODEV;
    }

    data->size = resource_size(res);

    data->base = devm_ioremap_resource(&pdev->dev, res);

    if (IS_ERR(data->base))
        return PTR_ERR(data->base);

    platform_set_drvdata(pdev, data);

    dev_info(&pdev->dev,
        "complete platform driver probed\n");

    dev_info(&pdev->dev,
        "resource start = %pa\n",
        &res->start);

    dev_info(&pdev->dev,
        "resource size = %pa\n",
        &data->size);

    return 0;
}

//-------------------------------------------
// Remove
//-------------------------------------------

static void complete_platform_remove(struct platform_device* pdev)
{
    dev_info(&pdev->dev,
        "complete platform driver removed\n");
}

//-------------------------------------------
// Device Tree
//-------------------------------------------

static const struct of_device_id complete_platform_of_match[] = {
    {.compatible = "bexel,complete-platform" },
    { }
};

MODULE_DEVICE_TABLE(of, complete_platform_of_match);

//-------------------------------------------
// Platform driver
//-------------------------------------------

static struct platform_driver complete_platform_driver = {
    .probe = complete_platform_probe,
    .remove = complete_platform_remove,

    .driver = {
        .name = "complete-platform-driver",
        .of_match_table = complete_platform_of_match,
    },
};

module_platform_driver(complete_platform_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Complete platform driver");


/*
//-------------------------------------------



//-------------------------------------------
*/


