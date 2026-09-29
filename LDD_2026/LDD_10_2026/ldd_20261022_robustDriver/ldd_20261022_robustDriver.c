#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/mutex.h>

//-------------------------------------------

struct ldd_robust_data
{
    struct mutex lock;
    int value;
};

//-------------------------------------------

static int ldd_robust_probe(struct platform_device* pdev)
{
    struct ldd_robust_data* data;

    dev_info(&pdev->dev,
        "robustDriver: probe()\n");

    data = devm_kzalloc(&pdev->dev,
        sizeof(*data),
        GFP_KERNEL);

    if (!data)
    {
        dev_err(&pdev->dev,
            "Failed to allocate private data\n");

        return -ENOMEM;
    }

    mutex_init(&data->lock);

    data->value = 0;

    platform_set_drvdata(pdev, data);

    dev_info(&pdev->dev,
        "robustDriver: initialization successful\n");

    return 0;
}

//-------------------------------------------

static void ldd_robust_remove(struct platform_device* pdev)
{
    struct ldd_robust_data* data;

    data = platform_get_drvdata(pdev);

    dev_info(&pdev->dev,
        "robustDriver: remove()\n");

    if (!data)
        return;

    mutex_lock(&data->lock);

    data->value = 0;

    mutex_unlock(&data->lock);
}

//-------------------------------------------

static const struct of_device_id ldd_robust_of_match[] =
{
    {
        .compatible = "ldd,robust-driver",
    },
    { }
};

MODULE_DEVICE_TABLE(of, ldd_robust_of_match);

//-------------------------------------------

static struct platform_driver ldd_robust_driver =
{
    .probe = ldd_robust_probe,
    .remove = ldd_robust_remove,

    .driver =
    {
        .name = "ldd-robust-driver",
        .of_match_table = ldd_robust_of_match,
    },
};

//-------------------------------------------

module_platform_driver(ldd_robust_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Robust Linux driver example");


/*
//-------------------------------------------



//-------------------------------------------
*/


