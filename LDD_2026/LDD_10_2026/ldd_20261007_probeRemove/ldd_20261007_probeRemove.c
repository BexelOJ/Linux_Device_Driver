// Basic probe() / remove()

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>

//-------------------------------------------

static int my_probe(struct platform_device* pdev)
{
    dev_info(&pdev->dev,
        "probe() called for %s\n",
        pdev->name);

    return 0;
}

//-------------------------------------------

static int my_remove(struct platform_device* pdev)
{
    dev_info(&pdev->dev,
        "remove() called for %s\n",
        pdev->name);

    return 0;
}

//-------------------------------------------

static struct platform_driver my_driver = {
    .probe = my_probe,
    .remove = my_remove,

    .driver = {
        .name = "ldd_probe_remove",
    },
};

//-------------------------------------------

module_platform_driver(my_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Platform driver probe/remove example");


/*
//-------------------------------------------

Device appears
     ↓
Device Model finds matching driver
     ↓
probe()
     ↓
Driver initializes device

//-------------------------------------------
When removed:

Device removed
     ↓
remove()
     ↓
Driver releases resources

//-------------------------------------------
*/


