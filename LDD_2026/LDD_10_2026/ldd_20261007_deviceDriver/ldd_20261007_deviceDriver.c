// Basic struct device + struct device_driver

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/device.h>

//-------------------------------------------

static int my_driver_probe(struct device* dev)
{
    dev_info(dev, "Driver probe called\n");

    return 0;
}

//-------------------------------------------

static int my_driver_remove(struct device* dev)
{
    dev_info(dev, "Driver remove called\n");

    return 0;
}

//-------------------------------------------

static struct device_driver my_driver = {
    .name = "ldd_device_driver",

    .probe = my_driver_probe,
    .remove = my_driver_remove,
};

//-------------------------------------------

static int __init my_driver_init(void)
{
    int ret;

    ret = driver_register(&my_driver);

    if (ret) {
        pr_err("driver_register() failed\n");
        return ret;
    }

    pr_info("Device driver registered\n");

    return 0;
}

//-------------------------------------------

static void __exit my_driver_exit(void)
{
    driver_unregister(&my_driver);

    pr_info("Device driver unregistered\n");
}

//-------------------------------------------

module_init(my_driver_init);
module_exit(my_driver_exit);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Basic device_driver example");



