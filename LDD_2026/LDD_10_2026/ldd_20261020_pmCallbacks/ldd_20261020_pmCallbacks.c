#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/pm.h>

//-------------------------------------------

static int ldd_pm_prepare(struct device* dev)
{
    dev_info(dev,
        "pmCallbacks: prepare()\n");

    return 0;
}

//-------------------------------------------

static void ldd_pm_complete(struct device* dev)
{
    dev_info(dev,
        "pmCallbacks: complete()\n");
}

//-------------------------------------------

static int ldd_pm_suspend(struct device* dev)
{
    dev_info(dev,
        "pmCallbacks: suspend()\n");

    return 0;
}

//-------------------------------------------

static int ldd_pm_resume(struct device* dev)
{
    dev_info(dev,
        "pmCallbacks: resume()\n");

    return 0;
}

//-------------------------------------------

static int ldd_pm_suspend_late(struct device* dev)
{
    dev_info(dev,
        "pmCallbacks: suspend_late()\n");

    return 0;
}

//-------------------------------------------

static int ldd_pm_resume_early(struct device* dev)
{
    dev_info(dev,
        "pmCallbacks: resume_early()\n");

    return 0;
}

//-------------------------------------------

static const struct dev_pm_ops ldd_pm_ops =
{
    .prepare = ldd_pm_prepare,
    .complete = ldd_pm_complete,

    .suspend = ldd_pm_suspend,
    .resume = ldd_pm_resume,

    .suspend_late = ldd_pm_suspend_late,
    .resume_early = ldd_pm_resume_early,
};

//-------------------------------------------

static int ldd_pm_callbacks_probe(struct platform_device* pdev)
{
    dev_info(&pdev->dev,
        "pmCallbacks: probe()\n");

    return 0;
}

//-------------------------------------------

static void ldd_pm_callbacks_remove(struct platform_device* pdev)
{
    dev_info(&pdev->dev,
        "pmCallbacks: remove()\n");
}

//-------------------------------------------

static const struct of_device_id ldd_pm_callbacks_of_match[] =
{
    {
        .compatible = "ldd,pm-callbacks",
    },
    { }
};

MODULE_DEVICE_TABLE(of, ldd_pm_callbacks_of_match);

//-------------------------------------------

static struct platform_driver ldd_pm_callbacks_driver =
{
    .probe = ldd_pm_callbacks_probe,
    .remove = ldd_pm_callbacks_remove,

    .driver =
    {
        .name = "ldd-pm-callbacks",
        .of_match_table = ldd_pm_callbacks_of_match,
        .pm = &ldd_pm_ops,
    },
};

//-------------------------------------------

module_platform_driver(ldd_pm_callbacks_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Linux PM callbacks example");


/*
//-------------------------------------------



//-------------------------------------------
*/


