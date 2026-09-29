#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/pm.h>

#define DRIVER_NAME "ldd_20261024_embeddedPM"

static int embeddedPM_suspend(struct device* dev)
{
    dev_info(dev,
        "%s: suspend\n",
        DRIVER_NAME);

    return 0;
}

static int embeddedPM_resume(struct device* dev)
{
    dev_info(dev,
        "%s: resume\n",
        DRIVER_NAME);

    return 0;
}

static const struct dev_pm_ops embeddedPM_ops = {
    .suspend = embeddedPM_suspend,
    .resume = embeddedPM_resume,
};

static int embeddedPM_probe(struct platform_device* pdev)
{
    dev_info(&pdev->dev,
        "%s: probe\n",
        DRIVER_NAME);

    return 0;
}

static void embeddedPM_remove(struct platform_device* pdev)
{
    dev_info(&pdev->dev,
        "%s: remove\n",
        DRIVER_NAME);
}

static const struct of_device_id embeddedPM_of_match[] = {
    {
        .compatible = "bexel,embedded-pm",
    },
    { }
};

MODULE_DEVICE_TABLE(of, embeddedPM_of_match);

static struct platform_driver embeddedPM_driver = {
    .probe = embeddedPM_probe,
    .remove = embeddedPM_remove,

    .driver = {
        .name = DRIVER_NAME,
        .of_match_table = embeddedPM_of_match,
        .pm = &embeddedPM_ops,
    },
};

module_platform_driver(embeddedPM_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Embedded Linux power management example");


/*
//-------------------------------------------



//-------------------------------------------
*/


