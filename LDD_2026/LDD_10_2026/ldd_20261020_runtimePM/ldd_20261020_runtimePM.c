#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/pm_runtime.h>

//-------------------------------------------

static int ldd_runtime_suspend(struct device* dev)
{
    dev_info(dev,
        "runtimePM: runtime_suspend()\n");

    /*
     * Real hardware:
     *
     * - stop device
     * - disable clock
     * - disable power
     */

    return 0;
}

//-------------------------------------------

static int ldd_runtime_resume(struct device* dev)
{
    dev_info(dev,
        "runtimePM: runtime_resume()\n");

    /*
     * Real hardware:
     *
     * - enable power
     * - enable clock
     * - restore device
     */

    return 0;
}

//-------------------------------------------

static const struct dev_pm_ops ldd_runtime_pm_ops =
{
    .runtime_suspend = ldd_runtime_suspend,
    .runtime_resume = ldd_runtime_resume,
};

//-------------------------------------------

static int ldd_runtime_probe(struct platform_device* pdev)
{
    int ret;

    dev_info(&pdev->dev,
        "runtimePM: probe()\n");

    pm_runtime_set_active(&pdev->dev);

    pm_runtime_enable(&pdev->dev);

    ret = pm_runtime_resume_and_get(&pdev->dev);

    if (ret < 0)
    {
        pm_runtime_disable(&pdev->dev);
        return ret;
    }

    dev_info(&pdev->dev,
        "runtimePM: device is active\n");

    pm_runtime_mark_last_busy(&pdev->dev);

    pm_runtime_put_autosuspend(&pdev->dev);

    pm_runtime_set_autosuspend_delay(&pdev->dev,
        5000);

    pm_runtime_use_autosuspend(&pdev->dev);

    return 0;
}

//-------------------------------------------

static void ldd_runtime_remove(struct platform_device* pdev)
{
    dev_info(&pdev->dev,
        "runtimePM: remove()\n");

    pm_runtime_disable(&pdev->dev);
}

//-------------------------------------------

static const struct of_device_id ldd_runtime_of_match[] =
{
    {
        .compatible = "ldd,runtime-pm",
    },
    { }
};

MODULE_DEVICE_TABLE(of, ldd_runtime_of_match);

//-------------------------------------------

static struct platform_driver ldd_runtime_driver =
{
    .probe = ldd_runtime_probe,
    .remove = ldd_runtime_remove,

    .driver =
    {
        .name = "ldd-runtime-pm",
        .of_match_table = ldd_runtime_of_match,
        .pm = &ldd_runtime_pm_ops,
    },
};

//-------------------------------------------

module_platform_driver(ldd_runtime_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Linux Runtime PM example");


/*
//-------------------------------------------

Device active
     │
     │ no activity
     ▼
runtime PM
     │
     ▼
runtime_suspend()
     │
     ▼
Low power
     │
     │ activity
     ▼
runtime_resume()

//-------------------------------------------
*/


