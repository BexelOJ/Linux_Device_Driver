#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/err.h>

//-------------------------------------------

static int ldd_deferred_probe(struct platform_device* pdev)
{
    dev_info(&pdev->dev,
        "deferredProbe: probe()\n");

    /*
     * Demonstration only.
     *
     * Pretend that a required dependency is
     * not ready yet.
     */

    dev_info(&pdev->dev,
        "Required dependency is not ready\n");

    return -EPROBE_DEFER;
}

//-------------------------------------------

static void ldd_deferred_remove(struct platform_device* pdev)
{
    dev_info(&pdev->dev,
        "deferredProbe: remove()\n");
}

//-------------------------------------------

static const struct of_device_id ldd_deferred_of_match[] =
{
    {
        .compatible = "ldd,deferred-probe",
    },
    { }
};

MODULE_DEVICE_TABLE(of, ldd_deferred_of_match);

//-------------------------------------------

static struct platform_driver ldd_deferred_driver =
{
    .probe = ldd_deferred_probe,
    .remove = ldd_deferred_remove,

    .driver =
    {
        .name = "ldd-deferred-probe",
        .of_match_table = ldd_deferred_of_match,
    },
};

//-------------------------------------------

module_platform_driver(ldd_deferred_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Linux deferred probe example");


/*
//-------------------------------------------

Driver probe
    │
    ▼
devm_clk_get()
    │
    ├── clock available
    │       │
    │       ▼
    │     continue
    │
    └── clock provider not ready
            │
            ▼
       -EPROBE_DEFER
            │
            ▼
       probe postponed
            │
            ▼
 provider becomes ready
            │
            ▼
       probe again

//-------------------------------------------
*/


