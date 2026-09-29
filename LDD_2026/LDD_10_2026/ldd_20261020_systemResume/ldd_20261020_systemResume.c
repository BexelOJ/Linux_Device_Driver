#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/pm.h>

//-------------------------------------------

static int ldd_resume_prepare(struct device* dev)
{
    dev_info(dev,
        "systemResume: prepare()\n");

    return 0;
}

//-------------------------------------------

static int ldd_resume_suspend(struct device* dev)
{
    dev_info(dev,
        "systemResume: suspend()\n");

    return 0;
}

//-------------------------------------------

static int ldd_resume_resume(struct device* dev)
{
    dev_info(dev,
        "systemResume: resume()\n");

    /*
     * Typical resume sequence:
     *
     * 1. Restore power
     * 2. Restore clock
     * 3. Restore registers
     * 4. Restore IRQ configuration
     * 5. Restart device
     */

    return 0;
}

//-------------------------------------------

static void ldd_resume_complete(struct device* dev)
{
    dev_info(dev,
        "systemResume: complete()\n");
}

//-------------------------------------------

static const struct dev_pm_ops ldd_resume_pm_ops =
{
    .prepare = ldd_resume_prepare,
    .suspend = ldd_resume_suspend,
    .resume = ldd_resume_resume,
    .complete = ldd_resume_complete,
};

//-------------------------------------------

static int ldd_resume_probe(struct platform_device* pdev)
{
    dev_info(&pdev->dev,
        "systemResume: probe()\n");

    return 0;
}

//-------------------------------------------

static void ldd_resume_remove(struct platform_device* pdev)
{
    dev_info(&pdev->dev,
        "systemResume: remove()\n");
}

//-------------------------------------------

static const struct of_device_id ldd_resume_of_match[] =
{
    {
        .compatible = "ldd,system-resume",
    },
    { }
};

MODULE_DEVICE_TABLE(of, ldd_resume_of_match);

//-------------------------------------------

static struct platform_driver ldd_resume_driver =
{
    .probe = ldd_resume_probe,
    .remove = ldd_resume_remove,

    .driver =
    {
        .name = "ldd-system-resume",
        .of_match_table = ldd_resume_of_match,
        .pm = &ldd_resume_pm_ops,
    },
};

//-------------------------------------------

module_platform_driver(ldd_resume_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Linux system resume example");


/*
//-------------------------------------------

System suspend
      │
      ▼
 prepare()
      │
      ▼
 suspend()
      │
      ▼
      ...
      │
      ▼
 System wakes
      │
      ▼
 resume()
      │
      ▼
 complete()

//-------------------------------------------
*/


