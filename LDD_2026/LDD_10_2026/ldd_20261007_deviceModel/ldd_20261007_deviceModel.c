#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/device.h>

//-------------------------------------------

static struct class* my_class;

//-------------------------------------------

static int __init device_model_init(void)
{
    my_class = class_create("ldd_device_model");

    if (IS_ERR(my_class))
        return PTR_ERR(my_class);

    pr_info("Device Model class created\n");

    return 0;
}

//-------------------------------------------

static void __exit device_model_exit(void)
{
    class_destroy(my_class);

    pr_info("Device Model class destroyed\n");
}

//-------------------------------------------

module_init(device_model_init);
module_exit(device_model_exit);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Linux Device Model basic example");



/*
//-------------------------------------------
This introduces:

struct device
struct device_driver
struct bus_type
struct class

//-------------------------------------------
*/


