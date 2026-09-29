#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>

//-------------------------------------------

static int platform_probe(struct platform_device* pdev)
{
    dev_info(&pdev->dev,
        "Platform driver probe()\n");

    return 0;
}

//-------------------------------------------

static int platform_remove(struct platform_device* pdev)
{
    dev_info(&pdev->dev,
        "Platform driver remove()\n");

    return 0;
}

//-------------------------------------------

static struct platform_driver my_platform_driver = {
    .probe = platform_probe,
    .remove = platform_remove,

    .driver = {
        .name = "ldd_platform_driver",
    },
};

//-------------------------------------------

module_platform_driver(my_platform_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Basic platform driver");


/*
//-------------------------------------------



//-------------------------------------------
*/


