#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/sysfs.h>

//-------------------------------------------

static int value = 100;

//-------------------------------------------

static ssize_t value_show(struct device* dev,
    struct device_attribute* attr,
    char* buf)
{
    return sysfs_emit(buf, "%d\n", value);
}

//-------------------------------------------

static ssize_t value_store(struct device* dev,
    struct device_attribute* attr,
    const char* buf,
    size_t count)
{
    int ret;

    ret = kstrtoint(buf, 10, &value);

    if (ret)
        return ret;

    return count;
}

//-------------------------------------------

static DEVICE_ATTR_RW(value);

//-------------------------------------------

static struct attribute* my_attrs[] = {
    &dev_attr_value.attr,
    NULL,
};

//-------------------------------------------

static const struct attribute_group my_attr_group = {
    .attrs = my_attrs,
};

//-------------------------------------------

static const struct attribute_group* my_groups[] = {
    &my_attr_group,
    NULL,
};

//-------------------------------------------

static struct platform_driver devgroups_driver = {
    .driver = {
        .name = "ldd_dev_groups",
        .dev_groups = my_groups,
    },
};

//-------------------------------------------

static int __init devgroups_init(void)
{
    return platform_driver_register(&devgroups_driver);
}

//-------------------------------------------

static void __exit devgroups_exit(void)
{
    platform_driver_unregister(&devgroups_driver);
}

//-------------------------------------------

module_init(devgroups_init);
module_exit(devgroups_exit);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Device attribute groups example");



/*
//-------------------------------------------

The important structure is:

device
  │
  └── sysfs
       │
       └── value


//-------------------------------------------
*/


