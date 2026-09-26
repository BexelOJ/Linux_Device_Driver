#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/poll.h>
#include <linux/wait.h>

#define DEVICE_NAME "ldd_20261003_select"
#define CLASS_NAME  "ldd_select"

static dev_t ldd_devNumber;
static struct cdev ldd_cdev;
static struct class* ldd_class;
static struct device* ldd_device;

static DECLARE_WAIT_QUEUE_HEAD(ldd_waitQueue);

static bool ldd_dataReady = false;

static char ldd_data[128] = "No data\n";

//-------------------------------------------
// Open
//-------------------------------------------

static int ldd_open(
    struct inode* inode,
    struct file* file)
{
    pr_info("ldd_20261003_select: device opened\n");

    return 0;
}

//-------------------------------------------
// Release
//-------------------------------------------

static int ldd_release(
    struct inode* inode,
    struct file* file)
{
    pr_info("ldd_20261003_select: device closed\n");

    return 0;
}

//-------------------------------------------
// Read
//-------------------------------------------

static ssize_t ldd_read(
    struct file* file,
    char __user* buffer,
    size_t count,
    loff_t* offset)
{
    size_t dataLength;

    //-------------------------------------------
    // Wait until data is available
    //-------------------------------------------

    if (!ldd_dataReady)
    {
        if (file->f_flags & O_NONBLOCK)
            return -EAGAIN;

        if (wait_event_interruptible(
            ldd_waitQueue,
            ldd_dataReady))
        {
            return -ERESTARTSYS;
        }
    }

    //-------------------------------------------
    // Get data length
    //-------------------------------------------

    dataLength = strlen(ldd_data);

    if (*offset >= dataLength)
    {
        ldd_dataReady = false;
        return 0;
    }

    if (count > dataLength - *offset)
        count = dataLength - *offset;

    //-------------------------------------------
    // Copy data to user
    //-------------------------------------------

    if (copy_to_user(
        buffer,
        ldd_data + *offset,
        count))
    {
        return -EFAULT;
    }

    *offset += count;

    //-------------------------------------------
    // Data consumed
    //-------------------------------------------

    if (*offset >= dataLength)
        ldd_dataReady = false;

    return count;
}

//-------------------------------------------
// Write
//-------------------------------------------

static ssize_t ldd_write(
    struct file* file,
    const char __user* buffer,
    size_t count,
    loff_t* offset)
{
    size_t copySize;

    //-------------------------------------------
    // Limit input size
    //-------------------------------------------

    copySize = min(count, sizeof(ldd_data) - 1);

    //-------------------------------------------
    // Copy data from user
    //-------------------------------------------

    if (copy_from_user(
        ldd_data,
        buffer,
        copySize))
    {
        return -EFAULT;
    }

    ldd_data[copySize] = '\0';

    //-------------------------------------------
    // New data available
    //-------------------------------------------

    ldd_dataReady = true;

    //-------------------------------------------
    // Wake processes waiting on device
    //-------------------------------------------

    wake_up_interruptible(&ldd_waitQueue);

    pr_info(
        "ldd_20261003_select: data received: %s\n",
        ldd_data
    );

    return copySize;
}

//-------------------------------------------
// Poll callback
//-------------------------------------------

static __poll_t ldd_poll(
    struct file* file,
    poll_table* wait)
{
    __poll_t mask = 0;

    pr_info(
        "ldd_20261003_select: poll callback\n"
    );

    //-------------------------------------------
    // Add process to wait queue
    //-------------------------------------------

    poll_wait(
        file,
        &ldd_waitQueue,
        wait
    );

    //-------------------------------------------
    // Tell select() that data is available
    //-------------------------------------------

    if (ldd_dataReady)
    {
        mask |= EPOLLIN;
        mask |= EPOLLRDNORM;
    }

    return mask;
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
    .poll = ldd_poll,
};

//-------------------------------------------
// Module initialization
//-------------------------------------------

static int __init ldd_init(void)
{
    int result;

    pr_info(
        "ldd_20261003_select: module loading\n"
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
            "ldd_20261003_select: alloc_chrdev_region failed\n"
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
            "ldd_20261003_select: cdev_add failed\n"
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
        pr_err(
            "ldd_20261003_select: class_create failed\n"
        );

        cdev_del(&ldd_cdev);

        unregister_chrdev_region(
            ldd_devNumber,
            1
        );

        return PTR_ERR(ldd_class);
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
        pr_err(
            "ldd_20261003_select: device_create failed\n"
        );

        class_destroy(ldd_class);
        cdev_del(&ldd_cdev);

        unregister_chrdev_region(
            ldd_devNumber,
            1
        );

        return PTR_ERR(ldd_device);
    }

    //-------------------------------------------
    // Initialize state
    //-------------------------------------------

    ldd_dataReady = false;

    pr_info(
        "ldd_20261003_select: module loaded\n"
    );

    pr_info(
        "ldd_20261003_select: device = /dev/%s\n",
        DEVICE_NAME
    );

    return 0;
}

//-------------------------------------------
// Module cleanup
//-------------------------------------------

static void __exit ldd_exit(void)
{
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

    pr_info(
        "ldd_20261003_select: module unloaded\n"
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
    "Linux driver demonstrating select() support"
);