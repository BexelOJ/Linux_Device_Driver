#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/device.h>

//-------------------------------------------

static struct bus_type ldd_bus;

//-------------------------------------------

static ssize_t version_show(const struct bus_type* bus,
    char* buf)
{
    return sysfs_emit(buf, "LDD Bus Version 1.0\n");
}

//-------------------------------------------

static BUS_ATTR_RO(version);

//-------------------------------------------

static struct attribute* ldd_bus_attrs[] = {
    &bus_attr_version.attr,
    NULL,
};

//-------------------------------------------

static const struct attribute_group ldd_bus_group = {
    .attrs = ldd_bus_attrs,
};

//-------------------------------------------

static const struct attribute_group* ldd_bus_groups[] = {
    &ldd_bus_group,
    NULL,
};

//-------------------------------------------

static struct bus_type ldd_bus = {
    .name = "ldd_bus_sysfs",
    .bus_groups = ldd_bus_groups,
};

//-------------------------------------------

static int __init bus_sysfs_init(void)
{
    int ret;

    ret = bus_register(&ldd_bus);

    if (ret) {
        pr_err("bus_register() failed\n");
        return ret;
    }

    pr_info("LDD bus registered with sysfs attribute\n");

    return 0;
}

//-------------------------------------------

static void __exit bus_sysfs_exit(void)
{
    bus_unregister(&ldd_bus);

    pr_info("LDD bus unregistered\n");
}

//-------------------------------------------

module_init(bus_sysfs_init);
module_exit(bus_sysfs_exit);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Bus sysfs attribute example");



/*
//-------------------------------------------
/sys/bus/
       │
       └── ldd_bus_sysfs/
              │
              └── version

//-------------------------------------------
*/


