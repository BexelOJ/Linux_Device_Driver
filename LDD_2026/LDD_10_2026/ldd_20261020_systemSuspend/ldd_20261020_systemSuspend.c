#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/pm.h>

//-------------------------------------------

static int ldd_system_suspend(struct device* dev)
{
    dev_info(dev, "systemSuspend: suspend()\n");

    /*
     * Real driver:
     *
     * - stop hardware
     * - stop DMA
     * - disable interrupts
     * - save registers
     * - disable clocks
     * - enter low-power state
     */

    return 0;
}

//-------------------------------------------

static int ldd_system_resume(struct device* dev)
{
    dev_info(dev, "systemSuspend: resume()\n");

    /*
     * Real driver:
     *
     * - enable power
     * - enable clocks
     * - restore registers
     * - restart hardware
     */

    return 0;
}

//-------------------------------------------

static const struct dev_pm_ops ldd_system_pm_ops =
{
    .suspend = ldd_system_suspend,
    .resume = ldd_system_resume,
};

//-------------------------------------------

static int ldd_system_suspend_probe(struct platform_device* pdev)
{
    dev_info(&pdev->dev,
        "systemSuspend: probe()\n");

    return 0;
}

//-------------------------------------------

static void ldd_system_suspend_remove(struct platform_device* pdev)
{
    dev_info(&pdev->dev,
        "systemSuspend: remove()\n");
}

//-------------------------------------------

static const struct of_device_id ldd_system_suspend_of_match[] =
{
    {
        .compatible = "ldd,system-suspend",
    },
    { }
};

MODULE_DEVICE_TABLE(of, ldd_system_suspend_of_match);

//-------------------------------------------

static struct platform_driver ldd_system_suspend_driver =
{
    .probe = ldd_system_suspend_probe,
    .remove = ldd_system_suspend_remove,

    .driver =
    {
        .name = "ldd-system-suspend",
        .of_match_table = ldd_system_suspend_of_match,
        .pm = &ldd_system_pm_ops,
    },
};

//-------------------------------------------

module_platform_driver(ldd_system_suspend_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Linux system suspend and resume example");


/*



*/


