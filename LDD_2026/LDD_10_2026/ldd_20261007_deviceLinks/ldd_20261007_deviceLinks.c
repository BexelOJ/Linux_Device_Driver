// Demonstrates a device link

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>

//-------------------------------------------

static struct platform_device* consumer_device;
static struct platform_device* supplier_device;

//-------------------------------------------

static int __init device_links_init(void)
{
    struct device_link* link;

    supplier_device =
        platform_device_register_simple("ldd_supplier", -1, NULL, 0);

    if (IS_ERR(supplier_device))
        return PTR_ERR(supplier_device);

    consumer_device =
        platform_device_register_simple("ldd_consumer", -1, NULL, 0);

    if (IS_ERR(consumer_device)) {
        platform_device_unregister(supplier_device);
        return PTR_ERR(consumer_device);
    }

    link = device_link_add(&consumer_device->dev,
        &supplier_device->dev,
        DL_FLAG_AUTOREMOVE_CONSUMER);

    if (!link) {
        platform_device_unregister(consumer_device);
        platform_device_unregister(supplier_device);
        return -ENOMEM;
    }

    pr_info("Device link created\n");

    return 0;
}

//-------------------------------------------

static void __exit device_links_exit(void)
{
    platform_device_unregister(consumer_device);
    platform_device_unregister(supplier_device);

    pr_info("Devices unregistered\n");
}

//-------------------------------------------

module_init(device_links_init);
module_exit(device_links_exit);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Device link example");


/*
//-------------------------------------------



//-------------------------------------------
*/


