#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/configfs.h>

#define DRIVER_NAME "ldd_20261023_configfs"

struct ldd_config {
    struct config_item item;
    int value;
};

static struct ldd_config ldd_item;

static ssize_t value_show(struct config_item* item,
    char* buf)
{
    struct ldd_config* config;

    config = container_of(item,
        struct ldd_config,
        item);

    return sprintf(buf, "%d\n", config->value);
}

static ssize_t value_store(struct config_item* item,
    const char* buf,
    size_t count)
{
    struct ldd_config* config;

    config = container_of(item,
        struct ldd_config,
        item);

    if (kstrtoint(buf, 10, &config->value))
        return -EINVAL;

    return count;
}

CONFIGFS_ATTR(ldd_config_, value);

static struct configfs_attribute* ldd_attrs[] = {
    &ldd_config_attr_value,
    NULL,
};

static const struct config_item_type ldd_item_type = {
    .ct_attrs = ldd_attrs,
    .ct_owner = THIS_MODULE,
};

static struct configfs_subsystem ldd_subsystem = {
    .su_group = {
        .cg_item = {
            .ci_namebuf = DRIVER_NAME,
            .ci_type = &ldd_item_type,
        },
    },
};

static int __init configfs_init(void)
{
    int ret;

    config_group_init(&ldd_subsystem.su_group);

    mutex_init(&ldd_subsystem.su_mutex);

    ret = configfs_register_subsystem(&ldd_subsystem);

    if (ret) {
        pr_err("%s: registration failed\n", DRIVER_NAME);
        return ret;
    }

    pr_info("%s: ConfigFS subsystem registered\n",
        DRIVER_NAME);

    return 0;
}

static void __exit configfs_exit(void)
{
    configfs_unregister_subsystem(&ldd_subsystem);

    pr_info("%s: ConfigFS subsystem unregistered\n",
        DRIVER_NAME);
}

module_init(configfs_init);
module_exit(configfs_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("ConfigFS example");


/*
//-------------------------------------------



//-------------------------------------------
*/


