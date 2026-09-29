#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/pm_wakeup.h>

//-------------------------------------------

struct ldd_wakeup_data
{
    struct wakeup_source* ws;
};

//-------------------------------------------

static int ldd_wakeup_probe(struct platform_device* pdev)
{
    struct ldd_wakeup_data* data;

    dev_info(&pdev->dev,
        "wakeupSource: probe()\n");

    data = devm_kzalloc(&pdev->dev,
        sizeof(*data),
        GFP_KERNEL);

    if (!data)
        return -ENOMEM;

    data->ws =
        wakeup_source_register(&pdev->dev,
            "ldd_wakeup_source");

    if (!data->ws)
        return -ENOMEM;

    platform_set_drvdata(pdev, data);

    device_set_wakeup_capable(&pdev->dev, true);
    device_set_wakeup_enable(&pdev->dev, true);

    dev_info(&pdev->dev,
        "wakeup source registered\n");

    /*
     * Demonstration:
     *
     * Notify the PM core that a wakeup event
     * occurred and keep the system awake for
     * 1000 ms.
     *
     * In a real driver this would normally be
     * called from the actual event path.
     */

    __pm_wakeup_event(data->ws, 1000);

    return 0;
}

//-------------------------------------------

static void ldd_wakeup_remove(struct platform_device* pdev)
{
    struct ldd_wakeup_data* data;

    data = platform_get_drvdata(pdev);

    dev_info(&pdev->dev,
        "wakeupSource: remove()\n");

    device_set_wakeup_enable(&pdev->dev, false);
    device_set_wakeup_capable(&pdev->dev, false);

    if (data->ws)
        wakeup_source_unregister(data->ws);
}

//-------------------------------------------

static const struct of_device_id ldd_wakeup_of_match[] =
{
    {
        .compatible = "ldd,wakeup-source",
    },
    { }
};

MODULE_DEVICE_TABLE(of, ldd_wakeup_of_match);

//-------------------------------------------

static struct platform_driver ldd_wakeup_driver =
{
    .probe = ldd_wakeup_probe,
    .remove = ldd_wakeup_remove,

    .driver =
    {
        .name = "ldd-wakeup-source",
        .of_match_table = ldd_wakeup_of_match,
    },
};

//-------------------------------------------

module_platform_driver(ldd_wakeup_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Linux wakeup source example");



