#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>

//---------------------------------------------------

#define DEVICE_NAME "ldd_read"

static dev_t deviceNumber;
static struct cdev ldd_cdev;

static const char message[] =
    "Hello from Linux kernel!\n";

//---------------------------------------------------

static int ldd_deviceOpen(
    struct inode *inode,
    struct file *file
)
{
    pr_info(
        "ldd_20261002_read: "
        "Device opened\n"
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
    size_t messageLength;
    size_t bytesToCopy;
    size_t bytesNotCopied;

    //---------------------------------------------------

    messageLength = strlen(message);

    //---------------------------------------------------

    if (*offset >= messageLength)
    {
        return 0;
    }

    //---------------------------------------------------

    bytesToCopy = min(
        count,
        messageLength - (size_t)*offset
    );

    //---------------------------------------------------

    bytesNotCopied = copy_to_user(
        buffer,
        message + *offset,
        bytesToCopy
    );

    if (bytesNotCopied != 0)
    {
        pr_err(
            "ldd_20261002_read: "
            "copy_to_user() failed\n"
        );

        return -EFAULT;
    }

    //---------------------------------------------------

    *offset += bytesToCopy;

    //---------------------------------------------------

    pr_info(
        "ldd_20261002_read: "
        "Sent %zu bytes to userspace\n",
        bytesToCopy
    );

    return bytesToCopy;
}

//---------------------------------------------------

static int ldd_deviceRelease(
    struct inode *inode,
    struct file *file
)
{
    pr_info(
        "ldd_20261002_read: "
        "Device released\n"
    );

    return 0;
}

//---------------------------------------------------

static const struct file_operations ldd_fops =
{
    .owner = THIS_MODULE,
    .open = ldd_deviceOpen,
    .read = ldd_deviceRead,
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
    {
        pr_err(
            "ldd_20261002_read: "
            "Failed to allocate device number\n"
        );

        return result;
    }

    //---------------------------------------------------

    pr_info(
        "ldd_20261002_read: "
        "Major = %u, Minor = %u\n",
        MAJOR(deviceNumber),
        MINOR(deviceNumber)
    );

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
        pr_err(
            "ldd_20261002_read: "
            "Failed to add cdev\n"
        );

        unregister_chrdev_region(
            deviceNumber,
            1
        );

        return result;
    }

    //---------------------------------------------------

    pr_info(
        "ldd_20261002_read: "
        "Character device registered\n"
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

    pr_info(
        "ldd_20261002_read: "
        "Character device unregistered\n"
    );
}

//---------------------------------------------------

module_init(ldd_moduleInit);
module_exit(ldd_moduleExit);

//---------------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION(
    "Linux character device read operation"
);

//---------------------------------------------------


/*
//---------------------------------------------------



//---------------------------------------------------
*/


