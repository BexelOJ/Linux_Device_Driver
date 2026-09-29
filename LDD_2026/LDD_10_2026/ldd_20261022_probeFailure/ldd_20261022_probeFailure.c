#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/moduleparam.h>

//-------------------------------------------

static int fail_stage;

module_param(fail_stage, int, 0644);

MODULE_PARM_DESC(fail_stage,
    "Probe failure stage");

//-------------------------------------------

static int ldd_probe_failure_probe(struct platform_device* pdev)
{
    void* buffer;

    dev_info(&pdev->dev,
        "probeFailure: probe()\n");

    if (fail_stage == 1)
    {
        dev_err(&pdev->dev,
            "Intentional failure at stage 1\n");

        return -EINVAL;
    }

    buffer = devm_kzalloc(&pdev->dev,
        1024,
        GFP_KERNEL);

    if (!buffer)
    {
        dev_err(&pdev->dev,
            "Allocation failed\n");

        return -ENOMEM;
    }

    dev_info(&pdev->dev,
        "Resource allocation successful\n");

    if (fail_stage == 2)
    {
        dev_err(&pdev->dev,
            "Intentional failure at stage 2\n");

        return -EIO;
    }

    dev_info(&pdev->dev,
        "probeFailure: probe successful\n");

    return 0;
}

//-------------------------------------------

static void ldd_probe_failure_remove(struct platform_device* pdev)
{
    dev_info(&pdev->dev,
        "probeFailure: remove()\n");
}

//-------------------------------------------

static const struct of_device_id ldd_probe_failure_of_match[] =
{
    {
        .compatible = "ldd,probe-failure",
    },
    { }
};

MODULE_DEVICE_TABLE(of, ldd_probe_failure_of_match);

//-------------------------------------------

static struct platform_driver ldd_probe_failure_driver =
{
    .probe = ldd_probe_failure_probe,
    .remove = ldd_probe_failure_remove,

    .driver =
    {
        .name = "ldd-probe-failure",
        .of_match_table = ldd_probe_failure_of_match,
    },
};

//-------------------------------------------

module_platform_driver(ldd_probe_failure_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Linux probe failure handling example");


/*
//-------------------------------------------



//-------------------------------------------
*/


