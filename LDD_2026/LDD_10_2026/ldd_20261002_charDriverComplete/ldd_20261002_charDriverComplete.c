#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>

//---------------------------------------------------

#define DEVICE_NAME "ldd_charDriver"
#define CLASS_NAME  "ldd_charDriver"
#define BUFFER_SIZE 256

//---------------------------------------------------

static dev_t deviceNumber;
static struct cdev ldd_cdev;
static struct class *ldd_class;
static struct device *ldd_device;

static char kernelBuffer[BUFFER_SIZE];
static size_t dataLength;

//---------------------------------------------------

static int ldd_deviceOpen(
    struct inode *inode,
    struct file *file
)
{
    pr_info(
        "ldd_20261002_charDriverComplete: "
        "open()\n"
    );

    return 0;
}

//---------------------------------------------------

static ssize_t ldd_deviceRead(
    struct file *file,
    char __user *buffer,
    size_t count,
    loff_t *offset
)
{
    size_t bytesToCopy;

    //---------------------------------------------------

    if (*offset >= dataLength)
    {
        return 0;
    }

    //---------------------------------------------------

    bytesToCopy = min(
        count,
        dataLength - (size_t)*offset
    );

    //---------------------------------------------------

    if (copy_to_user(
            buffer,
            kernelBuffer + *offset,
            bytesToCopy))
    {
        return -EFAULT;
    }

    //---------------------------------------------------

    *offset += bytesToCopy;

    //---------------------------------------------------

    return bytesToCopy;
}

//---------------------------------------------------

static ssize_t ldd_deviceWrite(
    struct file *file,
    const char __user *buffer,
    size_t count,
    loff_t *offset
)
{
    size_t bytesToCopy;

    //---------------------------------------------------

    bytesToCopy = min(
        count,
        (size_t)(BUFFER_SIZE - 1)
    );

    //---------------------------------------------------

    if (copy_from_user(
            kernelBuffer,
            buffer,
            bytesToCopy))
    {
        return -EFAULT;
    }

    //---------------------------------------------------

    kernelBuffer[bytesToCopy] = '\0';
    dataLength = bytesToCopy;

    //---------------------------------------------------

    pr_info(
        "ldd_20261002_charDriverComplete: "
        "Received %zu bytes\n",
        bytesToCopy
    );

    //---------------------------------------------------

    return bytesToCopy;
}

//---------------------------------------------------

static int ldd_deviceRelease(
    struct inode *inode,
    struct file *file
)
{
    pr_info(
        "ldd_20261002_charDriverComplete: "
        "release()\n"
    );

    return 0;
}

//---------------------------------------------------

static const struct file_operations ldd_fops =
{
    .owner = THIS_MODULE,
    .open = ldd_deviceOpen,
    .read = ldd_deviceRead,
    .write = ldd_deviceWrite,
    .release = ldd_deviceRelease,
};

//---------------------------------------------------

static int __init ldd_moduleInit(void)
{
    int result;

    //---------------------------------------------------

    result = alloc_chrdev_region(
        &deviceNumber,
        0,
        1,
        DEVICE_NAME
    );

    if (result < 0)
        return result;

    //---------------------------------------------------

    cdev_init(
        &ldd_cdev,
        &ldd_fops
    );

    //---------------------------------------------------

    result = cdev_add(
        &ldd_cdev,
        deviceNumber,
        1
    );

    if (result < 0)
        goto error_cdev;

    //---------------------------------------------------

    ldd_class = class_create(CLASS_NAME);

    if (IS_ERR(ldd_class))
    {
        result = PTR_ERR(ldd_class);
        goto error_class;
    }

    //---------------------------------------------------

    ldd_device = device_create(
        ldd_class,
        NULL,
        deviceNumber,
        NULL,
        DEVICE_NAME
    );

    if (IS_ERR(ldd_device))
    {
        result = PTR_ERR(ldd_device);
        goto error_device;
    }

    //---------------------------------------------------

    pr_info(
        "ldd_20261002_charDriverComplete: "
        "Driver initialized\n"
    );

    pr_info(
        "ldd_20261002_charDriverComplete: "
        "Major = %u, Minor = %u\n",
        MAJOR(deviceNumber),
        MINOR(deviceNumber)
    );

    return 0;

//---------------------------------------------------

error_device:

    class_destroy(ldd_class);

//---------------------------------------------------

error_class:

    cdev_del(&ldd_cdev);

//---------------------------------------------------

error_cdev:

    unregister_chrdev_region(
        deviceNumber,
        1
    );

    return result;
}

//---------------------------------------------------

static void __exit ldd_moduleExit(void)
{
    device_destroy(
        ldd_class,
        deviceNumber
    );

    class_destroy(ldd_class);

    cdev_del(&ldd_cdev);

    unregister_chrdev_region(
        deviceNumber,
        1
    );

    pr_info(
        "ldd_20261002_charDriverComplete: "
        "Driver unloaded\n"
    );
}

//---------------------------------------------------

module_init(ldd_moduleInit);
module_exit(ldd_moduleExit);

//---------------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Lr");
MODULE_DESCRIPTION(
    "Complete basic Linux character device driver"
);

//---------------------------------------------------



