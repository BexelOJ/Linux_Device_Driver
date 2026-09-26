#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>

//---------------------------------------------------

#define DEVICE_NAME "ldd_copyFromUser"
#define BUFFER_SIZE 128

static dev_t deviceNumber;
static struct cdev ldd_cdev;

static char kernelBuffer[BUFFER_SIZE];

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

    //---------------------------------------------------

    pr_info(
        "ldd_20261002_copyFromUser: "
        "Received %zu bytes\n",
        bytesToCopy
    );

    pr_info(
        "ldd_20261002_copyFromUser: "
        "Data = %s\n",
        kernelBuffer
    );

    //---------------------------------------------------

    return bytesToCopy;
}

//---------------------------------------------------

static const struct file_operations ldd_fops =
{
    .owner = THIS_MODULE,
    .write = ldd_deviceWrite,
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
    {
        unregister_chrdev_region(
            deviceNumber,
            1
        );

        return result;
    }

    //---------------------------------------------------

    pr_info(
        "ldd_20261002_copyFromUser: "
        "Major=%u Minor=%u\n",
        MAJOR(deviceNumber),
        MINOR(deviceNumber)
    );

    return 0;
}

//---------------------------------------------------

static void __exit ldd_moduleExit(void)
{
    cdev_del(&ldd_cdev);

    unregister_chrdev_region(
        deviceNumber,
        1
    );
}

//---------------------------------------------------

module_init(ldd_moduleInit);
module_exit(ldd_moduleExit);

//---------------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION(
    "Linux copy_from_user demonstration"
);

//---------------------------------------------------



