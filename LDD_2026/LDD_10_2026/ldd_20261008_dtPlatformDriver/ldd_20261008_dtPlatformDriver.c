#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/of.h>

//-------------------------------------------

static int dt_platform_probe(struct platform_device* pdev)
{
    dev_info(&pdev->dev,
        "Device Tree platform driver probe()\n");

    dev_info(&pdev->dev,
        "Device name: %s\n",
        dev_name(&pdev->dev));

    return 0;
}

//-------------------------------------------

static int dt_platform_remove(struct platform_device* pdev)
{
    dev_info(&pdev->dev,
        "Device Tree platform driver remove()\n");

    return 0;
}

//-------------------------------------------

static const struct of_device_id dt_platform_match[] = {
    {
        .compatible = "ldd,dt-platform-device",
    },
    { }
};

MODULE_DEVICE_TABLE(of, dt_platform_match);

//-------------------------------------------

static struct platform_driver dt_platform_driver = {
    .probe = dt_platform_probe,
    .remove = dt_platform_remove,

    .driver = {
        .name = "ldd_dt_platform",
        .of_match_table = dt_platform_match,
    },
};

//-------------------------------------------

module_platform_driver(dt_platform_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Device Tree platform driver");


/*
//-------------------------------------------



//-------------------------------------------
*/


