#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/mutex.h>

#define DEVICE_NAME "ldd_20261003_userKernelMemory"
#define CLASS_NAME  "ldd_userKernelMemory"

#define BUFFER_SIZE 256

static dev_t ldd_devNumber;

static struct cdev ldd_cdev;
static struct class* ldd_class;
static struct device* ldd_device;

static char ldd_kernelBuffer[BUFFER_SIZE];

static DEFINE_MUTEX(ldd_mutex);

//-------------------------------------------
// Open
//-------------------------------------------

static int ldd_open(
    struct inode* inode,
    struct file* file)
{
    pr_info(
        "ldd_20261003_userKernelMemory: device opened\n"
    );

    return 0;
}

//-------------------------------------------
// Release
//-------------------------------------------

static int ldd_release(
    struct inode* inode,
    struct file* file)
{
    pr_info(
        "ldd_20261003_userKernelMemory: device closed\n"
    );

    return 0;
}

//-------------------------------------------
// Read
//
// Kernel memory -> User memory
//-------------------------------------------

static ssize_t ldd_read(
    struct file* file,
    char __user* buffer,
    size_t count,
    loff_t* offset)
{
    size_t dataLength;

    mutex_lock(&ldd_mutex);

    dataLength = strlen(ldd_kernelBuffer);

    //-------------------------------------------
    // End of data
    //-------------------------------------------

    if (*offset >= dataLength)
    {
        mutex_unlock(&ldd_mutex);

        return 0;
    }

    //-------------------------------------------
    // Limit read size
    //-------------------------------------------

    if (count > dataLength - *offset)
        count = dataLength - *offset;

    //-------------------------------------------
    // Kernel memory -> User memory
    //-------------------------------------------

    if (copy_to_user(
        buffer,
        ldd_kernelBuffer + *offset,
        count))
    {
        mutex_unlock(&ldd_mutex);

        return -EFAULT;
    }

    //-------------------------------------------
    // Update file offset
    //-------------------------------------------

    *offset += count;

    mutex_unlock(&ldd_mutex);

    pr_info(
        "ldd_20261003_userKernelMemory: "
        "copy_to_user() transferred %zu bytes\n",
        count
    );

    return count;
}

//-------------------------------------------
// Write
//
// User memory -> Kernel memory
//-------------------------------------------

static ssize_t ldd_write(
    struct file* file,
    const char __user* buffer,
    size_t count,
    loff_t* offset)
{
    size_t copySize;

    //-------------------------------------------
    // Limit size
    //-------------------------------------------

    copySize = min(
        count,
        sizeof(ldd_kernelBuffer) - 1
    );

    mutex_lock(&ldd_mutex);

    //-------------------------------------------
    // User memory -> Kernel memory
    //-------------------------------------------

    if (copy_from_user(
        ldd_kernelBuffer,
        buffer,
        copySize))
    {
        mutex_unlock(&ldd_mutex);

        return -EFAULT;
    }

    //-------------------------------------------
    // Null terminate
    //-------------------------------------------

    ldd_kernelBuffer[copySize] = '\0';

    mutex_unlock(&ldd_mutex);

    pr_info(
        "ldd_20261003_userKernelMemory: "
        "copy_from_user() transferred %zu bytes\n",
        copySize
    );

    return copySize;
}

//-------------------------------------------
// File operations
//-------------------------------------------

static const struct file_operations ldd_fops =
{
    .owner = THIS_MODULE,

    .open = ldd_open,
    .release = ldd_release,

    .read = ldd_read,
    .write = ldd_write,
};

//-------------------------------------------
// Module initialization
//-------------------------------------------

static int __init ldd_init(void)
{
    int result;

    pr_info(
        "ldd_20261003_userKernelMemory: "
        "module loading\n"
    );

    //-------------------------------------------
    // Initialize kernel buffer
    //-------------------------------------------

    strscpy(
        ldd_kernelBuffer,
        "Hello from kernel memory\n",
        sizeof(ldd_kernelBuffer)
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
            "ldd_20261003_userKernelMemory: "
            "alloc_chrdev_region failed\n"
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
            "ldd_20261003_userKernelMemory: "
            "cdev_add failed\n"
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
            "ldd_20261003_userKernelMemory: "
            "class_create failed\n"
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
            "ldd_20261003_userKernelMemory: "
            "device_create failed\n"
        );

        class_destroy(ldd_class);

        cdev_del(&ldd_cdev);

        unregister_chrdev_region(
            ldd_devNumber,
            1
        );

        return result;
    }

    pr_info(
        "ldd_20261003_userKernelMemory: "
        "device created: /dev/%s\n",
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
        "ldd_20261003_userKernelMemory: "
        "module unloaded\n"
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
    "User space and kernel memory transfer example"
);



