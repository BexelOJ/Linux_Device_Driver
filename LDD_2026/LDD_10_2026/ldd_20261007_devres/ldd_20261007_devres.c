// Demonstrates Device Resource Management(devres) using devm_kzalloc()

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/slab.h>

//-------------------------------------------

static int devres_probe(struct platform_device* pdev)
{
    struct device* dev = &pdev->dev;

    void* buffer;

    buffer = devm_kzalloc(dev, 1024, GFP_KERNEL);

    if (!buffer)
        return -ENOMEM;

    dev_info(dev, "devm_kzalloc() allocated 1024 bytes\n");

    /*
     * No kfree() is required.
     *
     * The kernel automatically releases this memory
     * when the device is detached.
     */

    return 0;
}

//-------------------------------------------

static int devres_remove(struct platform_device* pdev)
{
    dev_info(&pdev->dev, "remove() called\n");

    /*
     * devm_kzalloc() memory is automatically released
     * after remove() completes.
     */

    return 0;
}

//-------------------------------------------

static struct platform_driver devres_driver = {
    .probe = devres_probe,
    .remove = devres_remove,

    .driver = {
        .name = "ldd_devres",
    },
};

//-------------------------------------------

module_platform_driver(devres_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Device Resource Management example");



/*
//-------------------------------------------

devm_kzalloc()
      ↓
device-managed resource
      ↓
device removed
      ↓
kernel automatically frees resource

//-------------------------------------------
*/


