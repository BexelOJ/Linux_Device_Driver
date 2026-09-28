#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/device.h>

//-------------------------------------------

static struct bus_type my_bus = {
    .name = "ldd_bus",
};

//-------------------------------------------

static int __init bus_driver_init(void)
{
    int ret;

    ret = bus_register(&my_bus);

    if (ret) {
        pr_err("bus_register() failed\n");
        return ret;
    }

    pr_info("LDD bus registered\n");

    return 0;
}

//-------------------------------------------

static void __exit bus_driver_exit(void)
{
    bus_unregister(&my_bus);

    pr_info("LDD bus unregistered\n");
}

//-------------------------------------------

module_init(bus_driver_init);
module_exit(bus_driver_exit);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Linux bus registration example");



/*
//-------------------------------------------
Device Model
     │
     ├── Bus
     │    ├── Devices
     │    └── Drivers
//-------------------------------------------

*/


