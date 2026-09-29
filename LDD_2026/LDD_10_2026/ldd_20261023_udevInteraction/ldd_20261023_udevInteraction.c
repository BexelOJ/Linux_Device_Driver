#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>

#define DRIVER_NAME "ldd_20261023_udevInteraction"

static dev_t device_number;
static struct cdev ldd_cdev;
static struct class* ldd_class;
static struct device* ldd_device;

static int ldd_open(struct inode* inode,
    struct file* file)
{
    pr_info("%s: device opened\n", DRIVER_NAME);

    return 0;
}

static int ldd_release(struct inode* inode,
    struct file* file)
{
    pr_info("%s: device closed\n", DRIVER_NAME);

    return 0;
}

static const struct file_operations ldd_fops = {
    .owner = THIS_MODULE,
    .open = ldd_open,
    .release = ldd_release,
};

static int __init udevInteraction_init(void)
{
    int ret;

    ret = alloc_chrdev_region(&device_number,
        0,
        1,
        DRIVER_NAME);

    if (ret)
        return ret;

    cdev_init(&ldd_cdev, &ldd_fops);

    ret = cdev_add(&ldd_cdev, device_number, 1);

    if (ret)
        goto unregister_region;

    ldd_class = class_create(DRIVER_NAME);

    if (IS_ERR(ldd_class)) {
        ret = PTR_ERR(ldd_class);
        goto delete_cdev;
    }

    ldd_device = device_create(ldd_class,
        NULL,
        device_number,
        NULL,
        DRIVER_NAME);

    if (IS_ERR(ldd_device)) {
        ret = PTR_ERR(ldd_device);
        goto destroy_class;
    }

    pr_info("%s: device created major=%d minor=%d\n",
        DRIVER_NAME,
        MAJOR(device_number),
        MINOR(device_number));

    return 0;

destroy_class:
    class_destroy(ldd_class);

delete_cdev:
    cdev_del(&ldd_cdev);

unregister_region:
    unregister_chrdev_region(device_number, 1);

    return ret;
}

static void __exit udevInteraction_exit(void)
{
    device_destroy(ldd_class, device_number);
    class_destroy(ldd_class);

    cdev_del(&ldd_cdev);

    unregister_chrdev_region(device_number, 1);

    pr_info("%s: device removed\n", DRIVER_NAME);
}

module_init(udevInteraction_init);
module_exit(udevInteraction_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("udev interaction example");


/*
//-------------------------------------------



//-------------------------------------------
*/


