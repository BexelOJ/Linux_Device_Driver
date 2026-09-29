#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/sysfs.h>
#include <linux/kstrtox.h>
#include <linux/mutex.h>

#define DEVICE_NAME "ldd_20261003_sysfs"
#define CLASS_NAME  "ldd_sysfs"

static dev_t ldd_devNumber;

static struct cdev ldd_cdev;
static struct class* ldd_class;
static struct device* ldd_device;

static int ldd_value = 100;

static char ldd_message[128] = "Hello from sysfs\n";

static DEFINE_MUTEX(ldd_mutex);

//-------------------------------------------
// Device open
//-------------------------------------------

static int ldd_open(
    struct inode* inode,
    struct file* file)
{
    pr_info(
        "ldd_20261003_sysfs: device opened\n"
    );

    return 0;
}

//-------------------------------------------
// Device release
//-------------------------------------------

static int ldd_release(
    struct inode* inode,
    struct file* file)
{
    pr_info(
        "ldd_20261003_sysfs: device closed\n"
    );

    return 0;
}

//-------------------------------------------
// File operations
//-------------------------------------------

static const struct file_operations ldd_fops =
{
    .owner = THIS_MODULE,
    .open = ldd_open,
    .release = ldd_release,
};

//-------------------------------------------
// value - show
//-------------------------------------------

static ssize_t value_show(
    struct device* device,
    struct device_attribute* attr,
    char* buffer)
{
    int value;

    mutex_lock(&ldd_mutex);

    value = ldd_value;

    mutex_unlock(&ldd_mutex);

    return sysfs_emit(
        buffer,
        "%d\n",
        value
    );
}

//-------------------------------------------
// value - store
//-------------------------------------------

static ssize_t value_store(
    struct device* device,
    struct device_attribute* attr,
    const char* buffer,
    size_t count)
{
    int value;
    int result;

    result = kstrtoint(
        buffer,
        10,
        &value
    );

    if (result)
    {
        pr_err(
            "ldd_20261003_sysfs: invalid value\n"
        );

        return result;
    }

    mutex_lock(&ldd_mutex);

    ldd_value = value;

    mutex_unlock(&ldd_mutex);

    pr_info(
        "ldd_20261003_sysfs: value = %d\n",
        value
    );

    return count;
}

//-------------------------------------------
// Create value attribute
//-------------------------------------------

static DEVICE_ATTR_RW(value);

//-------------------------------------------
// message - show
//-------------------------------------------

static ssize_t message_show(
    struct device* device,
    struct device_attribute* attr,
    char* buffer)
{
    ssize_t result;

    mutex_lock(&ldd_mutex);

    result = sysfs_emit(
        buffer,
        "%s",
        ldd_message
    );

    mutex_unlock(&ldd_mutex);

    return result;
}

//-------------------------------------------
// message - store
//-------------------------------------------

static ssize_t message_store(
    struct device* device,
    struct device_attribute* attr,
    const char* buffer,
    size_t count)
{
    size_t copy_size;

    copy_size = min(
        count,
        sizeof(ldd_message) - 1
    );

    mutex_lock(&ldd_mutex);

    memcpy(
        ldd_message,
        buffer,
        copy_size
    );

    ldd_message[copy_size] = '\0';

    mutex_unlock(&ldd_mutex);

    pr_info(
        "ldd_20261003_sysfs: message updated\n"
    );

    return count;
}

//-------------------------------------------
// Create message attribute
//-------------------------------------------

static DEVICE_ATTR_RW(message);

//-------------------------------------------
// Module initialization
//-------------------------------------------

static int __init ldd_init(void)
{
    int result;

    pr_info(
        "ldd_20261003_sysfs: module loading\n"
    );

    //-------------------------------------------
    // Allocate device number
    //-------------------------------------------

    result = alloc_chrdev_region(
        &ldd_devNumber,
        0,
        1,
        DEVICE_NAME
    );

    if (result < 0)
    {
        pr_err(
            "ldd_20261003_sysfs: alloc_chrdev_region failed\n"
        );

        return result;
    }

    //-------------------------------------------
    // Initialize cdev
    //-------------------------------------------

    cdev_init(
        &ldd_cdev,
        &ldd_fops
    );

    ldd_cdev.owner = THIS_MODULE;

    //-------------------------------------------
    // Add cdev
    //-------------------------------------------

    result = cdev_add(
        &ldd_cdev,
        ldd_devNumber,
        1
    );

    if (result < 0)
    {
        pr_err(
            "ldd_20261003_sysfs: cdev_add failed\n"
        );

        unregister_chrdev_region(
            ldd_devNumber,
            1
        );

        return result;
    }

    //-------------------------------------------
    // Create class
    //-------------------------------------------

    ldd_class = class_create(CLASS_NAME);

    if (IS_ERR(ldd_class))
    {
        result = PTR_ERR(ldd_class);

        pr_err(
            "ldd_20261003_sysfs: class_create failed\n"
        );

        cdev_del(&ldd_cdev);

        unregister_chrdev_region(
            ldd_devNumber,
            1
        );

        return result;
    }

    //-------------------------------------------
    // Create device
    //-------------------------------------------

    ldd_device = device_create(
        ldd_class,
        NULL,
        ldd_devNumber,
        NULL,
        DEVICE_NAME
    );

    if (IS_ERR(ldd_device))
    {
        result = PTR_ERR(ldd_device);

        pr_err(
            "ldd_20261003_sysfs: device_create failed\n"
        );

        class_destroy(ldd_class);
        cdev_del(&ldd_cdev);

        unregister_chrdev_region(
            ldd_devNumber,
            1
        );

        return result;
    }

    //-------------------------------------------
    // Create value sysfs attribute
    //-------------------------------------------

    result = device_create_file(
        ldd_device,
        &dev_attr_value
    );

    if (result)
    {
        pr_err(
            "ldd_20261003_sysfs: value attribute failed\n"
        );

        device_destroy(
            ldd_class,
            ldd_devNumber
        );

        class_destroy(ldd_class);
        cdev_del(&ldd_cdev);

        unregister_chrdev_region(
            ldd_devNumber,
            1
        );

        return result;
    }

    //-------------------------------------------
    // Create message sysfs attribute
    //-------------------------------------------

    result = device_create_file(
        ldd_device,
        &dev_attr_message
    );

    if (result)
    {
        pr_err(
            "ldd_20261003_sysfs: message attribute failed\n"
        );

        device_remove_file(
            ldd_device,
            &dev_attr_value
        );

        device_destroy(
            ldd_class,
            ldd_devNumber
        );

        class_destroy(ldd_class);
        cdev_del(&ldd_cdev);

        unregister_chrdev_region(
            ldd_devNumber,
            1
        );

        return result;
    }

    //-------------------------------------------
    // Initialize values
    //-------------------------------------------

    ldd_value = 100;

    strscpy(
        ldd_message,
        "Hello from sysfs\n",
        sizeof(ldd_message)
    );

    pr_info(
        "ldd_20261003_sysfs: module loaded\n"
    );

    pr_info(
        "ldd_20261003_sysfs: device = /dev/%s\n",
        DEVICE_NAME
    );

    return 0;
}

//-------------------------------------------
// Module cleanup
//-------------------------------------------

static void __exit ldd_exit(void)
{
    //-------------------------------------------
    // Remove sysfs attributes
    //-------------------------------------------

    device_remove_file(
        ldd_device,
        &dev_attr_message
    );

    device_remove_file(
        ldd_device,
        &dev_attr_value
    );

    //-------------------------------------------
    // Remove device
    //-------------------------------------------

    device_destroy(
        ldd_class,
        ldd_devNumber
    );

    //-------------------------------------------
    // Remove class
    //-------------------------------------------

    class_destroy(ldd_class);

    //-------------------------------------------
    // Remove cdev
    //-------------------------------------------

    cdev_del(&ldd_cdev);

    //-------------------------------------------
    // Release device number
    //-------------------------------------------

    unregister_chrdev_region(
        ldd_devNumber,
        1
    );

    pr_info(
        "ldd_20261003_sysfs: module unloaded\n"
    );
}

//-------------------------------------------
// Module registration
//-------------------------------------------

module_init(ldd_init);
module_exit(ldd_exit);

//-------------------------------------------
// Module information
//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION(
    "Linux sysfs driver example"
);


/*
//-------------------------------------------



//-------------------------------------------
*/


