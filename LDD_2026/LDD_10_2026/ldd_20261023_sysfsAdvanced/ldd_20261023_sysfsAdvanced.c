#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>

#define DRIVER_NAME "ldd_20261023_sysfsAdvanced"

static struct kobject* ldd_kobj;

static int temperature = 25;
static int enabled = 1;

static ssize_t temperature_show(struct kobject* kobj,
    struct kobj_attribute* attr,
    char* buf)
{
    return sprintf(buf, "%d\n", temperature);
}

static ssize_t temperature_store(struct kobject* kobj,
    struct kobj_attribute* attr,
    const char* buf,
    size_t count)
{
    if (kstrtoint(buf, 10, &temperature))
        return -EINVAL;

    return count;
}

static ssize_t enabled_show(struct kobject* kobj,
    struct kobj_attribute* attr,
    char* buf)
{
    return sprintf(buf, "%d\n", enabled);
}

static ssize_t enabled_store(struct kobject* kobj,
    struct kobj_attribute* attr,
    const char* buf,
    size_t count)
{
    if (kstrtoint(buf, 10, &enabled))
        return -EINVAL;

    return count;
}

static struct kobj_attribute temperature_attr =
__ATTR(temperature, 0644,
    temperature_show,
    temperature_store);

static struct kobj_attribute enabled_attr =
__ATTR(enabled, 0644,
    enabled_show,
    enabled_store);

static struct attribute* ldd_attrs[] = {
    &temperature_attr.attr,
    &enabled_attr.attr,
    NULL,
};

static const struct attribute_group ldd_attr_group = {
    .attrs = ldd_attrs,
};

static int __init sysfsAdvanced_init(void)
{
    int ret;

    ldd_kobj = kobject_create_and_add(DRIVER_NAME,
        kernel_kobj);

    if (!ldd_kobj)
        return -ENOMEM;

    ret = sysfs_create_group(ldd_kobj,
        &ldd_attr_group);

    if (ret) {
        kobject_put(ldd_kobj);
        return ret;
    }

    pr_info("%s: sysfs attributes created\n",
        DRIVER_NAME);

    return 0;
}

static void __exit sysfsAdvanced_exit(void)
{
    sysfs_remove_group(ldd_kobj,
        &ldd_attr_group);

    kobject_put(ldd_kobj);

    pr_info("%s: sysfs attributes removed\n",
        DRIVER_NAME);
}

module_init(sysfsAdvanced_init);
module_exit(sysfsAdvanced_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Advanced sysfs example");


/*
//-------------------------------------------



//-------------------------------------------
*/


