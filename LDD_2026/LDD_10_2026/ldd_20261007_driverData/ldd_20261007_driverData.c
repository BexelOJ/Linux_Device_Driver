#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/slab.h>

//-------------------------------------------

struct my_driver_data {
    int value;
    char name[32];
};

//-------------------------------------------

static int driver_data_probe(struct platform_device* pdev)
{
    struct my_driver_data* data;

    data = devm_kzalloc(&pdev->dev,
        sizeof(*data),
        GFP_KERNEL);

    if (!data)
        return -ENOMEM;

    data->value = 100;

    snprintf(data->name,
        sizeof(data->name),
        "LDD_Device");

    platform_set_drvdata(pdev, data);

    dev_info(&pdev->dev,
        "driver data stored: value=%d name=%s\n",
        data->value,
        data->name);

    return 0;
}

//-------------------------------------------

static int driver_data_remove(struct platform_device* pdev)
{
    struct my_driver_data* data;

    data = platform_get_drvdata(pdev);

    if (data)
        dev_info(&pdev->dev,
            "driver data retrieved: value=%d\n",
            data->value);

    return 0;
}

//-------------------------------------------

static struct platform_driver driver_data_driver = {
    .probe = driver_data_probe,
    .remove = driver_data_remove,

    .driver = {
        .name = "ldd_driver_data",
    },
};

//-------------------------------------------

module_platform_driver(driver_data_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Platform driver private data example");



