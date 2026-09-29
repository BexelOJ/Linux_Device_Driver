#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/moduleparam.h>

//-------------------------------------------

static bool fail_probe;

module_param(fail_probe, bool, 0644);

MODULE_PARM_DESC(fail_probe,
    "Force probe() failure");

//-------------------------------------------

static int ldd_failure_probe(struct platform_device* pdev)
{
    void* memory;

    dev_info(&pdev->dev,
        "failureInjection: probe()\n");

    if (fail_probe)
    {
        dev_err(&pdev->dev,
            "Intentional probe failure\n");

        return -EIO;
    }

    memory = devm_kzalloc(&pdev->dev,
        1024,
        GFP_KERNEL);

    if (!memory)
    {
        dev_err(&pdev->dev,
            "Memory allocation failed\n");

        return -ENOMEM;
    }

    dev_info(&pdev->dev,
        "failureInjection: probe successful\n");

    return 0;
}

//-------------------------------------------

static void ldd_failure_remove(struct platform_device* pdev)
{
    dev_info(&pdev->dev,
        "failureInjection: remove()\n");
}

//-------------------------------------------

static const struct of_device_id ldd_failure_of_match[] =
{
    {
        .compatible = "ldd,failure-injection",
    },
    { }
};

MODULE_DEVICE_TABLE(of, ldd_failure_of_match);

//-------------------------------------------

static struct platform_driver ldd_failure_driver =
{
    .probe = ldd_failure_probe,
    .remove = ldd_failure_remove,

    .driver =
    {
        .name = "ldd-failure-injection",
        .of_match_table = ldd_failure_of_match,
    },
};

//-------------------------------------------

module_platform_driver(ldd_failure_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Driver failure injection example");


/*
//-------------------------------------------



//-------------------------------------------
*/


