#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>

//-------------------------------------------

static int match_probe(struct platform_device* pdev)
{
    dev_info(&pdev->dev,
        "MATCH FOUND -> probe() called\n");

    return 0;
}

//-------------------------------------------

static int match_remove(struct platform_device* pdev)
{
    dev_info(&pdev->dev,
        "remove() called\n");

    return 0;
}

//-------------------------------------------

static struct platform_driver match_driver = {
    .probe = match_probe,
    .remove = match_remove,

    .driver = {
        .name = "ldd_match_device",
    },
};

//-------------------------------------------

static struct platform_device* match_device;

//-------------------------------------------

static int __init match_init(void)
{
    int ret;

    ret = platform_driver_register(&match_driver);

    if (ret)
        return ret;

    match_device =
        platform_device_register_simple(
            "ldd_match_device",
            -1,
            NULL,
            0);

    if (IS_ERR(match_device)) {
        ret = PTR_ERR(match_device);
        platform_driver_unregister(&match_driver);
        return ret;
    }

    pr_info("Device and driver registered\n");

    return 0;
}

//-------------------------------------------

static void __exit match_exit(void)
{
    platform_device_unregister(match_device);

    platform_driver_unregister(&match_driver);

    pr_info("Device and driver unregistered\n");
}

//-------------------------------------------

module_init(match_init);
module_exit(match_exit);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Platform device-driver matching example");



/*
//-------------------------------------------

platform_device
      name
       │
       │ match
       ↓
platform_driver
      name
       │
       ↓
     probe()

//-------------------------------------------
*/


