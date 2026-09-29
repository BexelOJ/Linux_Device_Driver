#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>

#define DRIVER_NAME "ldd_20261025_embeddedImage"

static int embeddedImage_probe(struct platform_device* pdev)
{
    dev_info(&pdev->dev,
        "%s: embedded image driver initialized\n",
        DRIVER_NAME);

    dev_info(&pdev->dev,
        "Kernel + DT + RootFS integration successful\n");

    return 0;
}

static void embeddedImage_remove(
    struct platform_device* pdev)
{
    dev_info(&pdev->dev,
        "%s: driver removed\n",
        DRIVER_NAME);
}

static const struct of_device_id embeddedImage_of_match[] = {
    {
        .compatible = "bexel,embedded-image",
    },
    { }
};

MODULE_DEVICE_TABLE(of, embeddedImage_of_match);

static struct platform_driver embeddedImage_driver = {
    .probe = embeddedImage_probe,
    .remove = embeddedImage_remove,

    .driver = {
        .name = DRIVER_NAME,
        .of_match_table =
            embeddedImage_of_match,
    },
};

module_platform_driver(embeddedImage_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Embedded Linux image integration driver");


/*
//-------------------------------------------



//-------------------------------------------
*/


