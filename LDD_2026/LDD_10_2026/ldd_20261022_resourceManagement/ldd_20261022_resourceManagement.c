#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/slab.h>

//-------------------------------------------

struct ldd_resource_data
{
    void* manual_buffer;
    void* managed_buffer;
};

//-------------------------------------------

static int ldd_resource_probe(struct platform_device* pdev)
{
    struct ldd_resource_data* data;

    dev_info(&pdev->dev,
        "resourceManagement: probe()\n");

    data = devm_kzalloc(&pdev->dev,
        sizeof(*data),
        GFP_KERNEL);

    if (!data)
        return -ENOMEM;

    /*
     * Managed allocation.
     */
    data->managed_buffer =
        devm_kzalloc(&pdev->dev,
            1024,
            GFP_KERNEL);

    if (!data->managed_buffer)
        return -ENOMEM;

    /*
     * Manual allocation.
     */
    data->manual_buffer =
        kmalloc(1024, GFP_KERNEL);

    if (!data->manual_buffer)
        return -ENOMEM;

    platform_set_drvdata(pdev, data);

    dev_info(&pdev->dev,
        "resources allocated\n");

    return 0;
}

//-------------------------------------------

static void ldd_resource_remove(struct platform_device* pdev)
{
    struct ldd_resource_data* data;

    data = platform_get_drvdata(pdev);

    dev_info(&pdev->dev,
        "resourceManagement: remove()\n");

    /*
     * Manual resource must be released manually.
     */

    if (data && data->manual_buffer)
        kfree(data->manual_buffer);

    /*
     * managed_buffer is automatically released
     * by devres.
     */
}

//-------------------------------------------

static const struct of_device_id ldd_resource_of_match[] =
{
    {
        .compatible = "ldd,resource-management",
    },
    { }
};

MODULE_DEVICE_TABLE(of, ldd_resource_of_match);

//-------------------------------------------

static struct platform_driver ldd_resource_driver =
{
    .probe = ldd_resource_probe,
    .remove = ldd_resource_remove,

    .driver =
    {
        .name = "ldd-resource-management",
        .of_match_table = ldd_resource_of_match,
    },
};

//-------------------------------------------

module_platform_driver(ldd_resource_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Linux resource management example");


/*
//-------------------------------------------
The distinction:

kmalloc()
   │
   ▼
kfree()


Versus:

devm_kzalloc()
   │
   ▼
device detached
   │
   ▼
automatic cleanup

//-------------------------------------------
*/


