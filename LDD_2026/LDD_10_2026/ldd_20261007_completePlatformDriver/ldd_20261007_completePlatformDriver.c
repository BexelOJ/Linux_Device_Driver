#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/slab.h>

//-------------------------------------------

struct my_device_data {
    int value;
};

//-------------------------------------------

static int complete_probe(struct platform_device* pdev)
{
    struct my_device_data* data;

    dev_info(&pdev->dev, "probe() called\n");

    data = devm_kzalloc(&pdev->dev,
        sizeof(*data),
        GFP_KERNEL);

    if (!data)
        return -ENOMEM;

    data->value = 1234;

    platform_set_drvdata(pdev, data);

    dev_info(&pdev->dev,
        "private data value = %d\n",
        data->value);

    return 0;
}

//-------------------------------------------

static int complete_remove(struct platform_device* pdev)
{
    struct my_device_data* data;

    data = platform_get_drvdata(pdev);

    if (data)
        dev_info(&pdev->dev,
            "remove: value = %d\n",
            data->value);

    return 0;
}

//-------------------------------------------

static struct platform_driver complete_driver = {
    .probe = complete_probe,
    .remove = complete_remove,

    .driver = {
        .name = "ldd_complete_platform",
    },
};

//-------------------------------------------

static struct platform_device* complete_device;

//-------------------------------------------

static int __init complete_init(void)
{
    int ret;

    ret = platform_driver_register(&complete_driver);

    if (ret)
        return ret;

    complete_device =
        platform_device_register_simple(
            "ldd_complete_platform",
            -1,
            NULL,
            0);

    if (IS_ERR(complete_device)) {
        ret = PTR_ERR(complete_device);
        platform_driver_unregister(&complete_driver);
        return ret;
    }

    pr_info("Complete platform driver example loaded\n");

    return 0;
}

//-------------------------------------------

static void __exit complete_exit(void)
{
    platform_device_unregister(complete_device);

    platform_driver_unregister(&complete_driver);

    pr_info("Complete platform driver example unloaded\n");
}

//-------------------------------------------

module_init(complete_init);
module_exit(complete_exit);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Lr");
MODULE_DESCRIPTION("Complete platform driver example");


