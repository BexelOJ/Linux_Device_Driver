#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/pm_runtime.h>

//-------------------------------------------

static int ldd_pm_runtime_suspend(struct device* dev)
{
    dev_info(dev,
        "pmRuntime: runtime_suspend()\n");

    return 0;
}

//-------------------------------------------

static int ldd_pm_runtime_resume(struct device* dev)
{
    dev_info(dev,
        "pmRuntime: runtime_resume()\n");

    return 0;
}

//-------------------------------------------

static const struct dev_pm_ops ldd_pm_runtime_ops =
{
    .runtime_suspend = ldd_pm_runtime_suspend,
    .runtime_resume = ldd_pm_runtime_resume,
};

//-------------------------------------------

static int ldd_pm_runtime_probe(struct platform_device* pdev)
{
    int ret;

    dev_info(&pdev->dev,
        "pmRuntime: probe()\n");

    pm_runtime_set_active(&pdev->dev);
    pm_runtime_enable(&pdev->dev);

    /*
     * Increase runtime PM usage count.
     * Device must stay active while we use it.
     */

    ret = pm_runtime_resume_and_get(&pdev->dev);

    if (ret < 0)
    {
        pm_runtime_disable(&pdev->dev);
        return ret;
    }

    dev_info(&pdev->dev,
        "pmRuntime: device acquired\n");

    /*
     * Hardware access would happen here.
     */

    pm_runtime_mark_last_busy(&pdev->dev);

    /*
     * Release runtime PM reference.
     */

    pm_runtime_put_autosuspend(&pdev->dev);

    pm_runtime_set_autosuspend_delay(&pdev->dev,
        3000);

    pm_runtime_use_autosuspend(&pdev->dev);

    return 0;
}

//-------------------------------------------

static void ldd_pm_runtime_remove(struct platform_device* pdev)
{
    dev_info(&pdev->dev,
        "pmRuntime: remove()\n");

    pm_runtime_disable(&pdev->dev);
}

//-------------------------------------------

static const struct of_device_id ldd_pm_runtime_of_match[] =
{
    {
        .compatible = "ldd,pm-runtime",
    },
    { }
};

MODULE_DEVICE_TABLE(of, ldd_pm_runtime_of_match);

//-------------------------------------------

static struct platform_driver ldd_pm_runtime_driver =
{
    .probe = ldd_pm_runtime_probe,
    .remove = ldd_pm_runtime_remove,

    .driver =
    {
        .name = "ldd-pm-runtime",
        .of_match_table = ldd_pm_runtime_of_match,
        .pm = &ldd_pm_runtime_ops,
    },
};

//-------------------------------------------

module_platform_driver(ldd_pm_runtime_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Linux Runtime PM reference example");


/*
//-------------------------------------------



//-------------------------------------------
*/


