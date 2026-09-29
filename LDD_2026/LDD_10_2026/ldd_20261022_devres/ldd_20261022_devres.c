#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/slab.h>

//-------------------------------------------

struct ldd_devres_data
{
    int value;
    char name[32];
};

//-------------------------------------------

static int ldd_devres_probe(struct platform_device* pdev)
{
    struct ldd_devres_data* data;

    dev_info(&pdev->dev,
        "devres: probe()\n");

    data = devm_kzalloc(&pdev->dev,
        sizeof(*data),
        GFP_KERNEL);

    if (!data)
        return -ENOMEM;

    data->value = 100;

    snprintf(data->name,
        sizeof(data->name),
        "ldd-devres");

    platform_set_drvdata(pdev, data);

    dev_info(&pdev->dev,
        "devres: resource allocated\n");

    return 0;
}

//-------------------------------------------

static void ldd_devres_remove(struct platform_device* pdev)
{
    struct ldd_devres_data* data;

    data = platform_get_drvdata(pdev);

    dev_info(&pdev->dev,
        "devres: remove()\n");

    if (data)
        dev_info(&pdev->dev,
            "value = %d\n",
            data->value);

    /*
     * No kfree().
     *
     * devm_kzalloc() automatically frees the
     * memory when the device is detached.
     */
}

//-------------------------------------------

static const struct of_device_id ldd_devres_of_match[] =
{
    {
        .compatible = "ldd,devres",
    },
    { }
};

MODULE_DEVICE_TABLE(of, ldd_devres_of_match);

//-------------------------------------------

static struct platform_driver ldd_devres_driver =
{
    .probe = ldd_devres_probe,
    .remove = ldd_devres_remove,

    .driver =
    {
        .name = "ldd-devres",
        .of_match_table = ldd_devres_of_match,
    },
};

//-------------------------------------------

module_platform_driver(ldd_devres_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Linux Device Resource Management example");


/*
//-------------------------------------------



//-------------------------------------------
*/


