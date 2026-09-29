#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>

#define DRIVER_NAME "ldd_20261025_yoctoDriver"

struct yocto_driver_data {
    int value;
};

static int yoctoDriver_probe(struct platform_device* pdev)
{
    struct yocto_driver_data* data;

    data = devm_kzalloc(&pdev->dev,
        sizeof(*data),
        GFP_KERNEL);

    if (!data)
        return -ENOMEM;

    data->value = 100;

    platform_set_drvdata(pdev, data);

    dev_info(&pdev->dev,
        "%s: driver probed\n",
        DRIVER_NAME);

    dev_info(&pdev->dev,
        "value=%d\n",
        data->value);

    return 0;
}

static void yoctoDriver_remove(struct platform_device* pdev)
{
    dev_info(&pdev->dev,
        "%s: driver removed\n",
        DRIVER_NAME);
}

static const struct of_device_id yoctoDriver_of_match[] = {
    {
        .compatible = "bexel,yocto-driver",
    },
    { }
};

MODULE_DEVICE_TABLE(of, yoctoDriver_of_match);

static struct platform_driver yoctoDriver_driver = {
    .probe = yoctoDriver_probe,
    .remove = yoctoDriver_remove,

    .driver = {
        .name = DRIVER_NAME,
        .of_match_table = yoctoDriver_of_match,
    },
};

module_platform_driver(yoctoDriver_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Yocto platform driver example");


/*
//-------------------------------------------



//-------------------------------------------
*/


