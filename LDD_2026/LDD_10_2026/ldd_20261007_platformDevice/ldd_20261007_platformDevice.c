#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>

//-------------------------------------------

static struct platform_device* my_device;

//-------------------------------------------

static int __init platform_device_init(void)
{
    my_device =
        platform_device_register_simple(
            "ldd_platform_device",
            -1,
            NULL,
            0);

    if (IS_ERR(my_device))
        return PTR_ERR(my_device);

    pr_info("Platform device registered\n");

    return 0;
}

//-------------------------------------------

static void __exit platform_device_exit(void)
{
    platform_device_unregister(my_device);

    pr_info("Platform device unregistered\n");
}

//-------------------------------------------

module_init(platform_device_init);
module_exit(platform_device_exit);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Platform device example");


/*
//-------------------------------------------



//-------------------------------------------
*/


