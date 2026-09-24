#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>

//---------------------------------------------------

#define DEVICE_NAME "ldd_fileOperations"

static dev_t deviceNumber;
static struct cdev ldd_cdev;

//---------------------------------------------------

static int ldd_deviceOpen(
    struct inode *inode,
    struct file *file
)
{
    pr_info(
        "ldd_20261002_fileOperations: "
        "Device opened\n"
    );

    return 0;
}

//---------------------------------------------------

static int ldd_deviceRelease(
    struct inode *inode,
    struct file *file
)
{
    pr_info(
        "ldd_20261002_fileOperations: "
        "Device released\n"
    );

    return 0;
}

//---------------------------------------------------

static const struct file_operations ldd_fops =
{
    .owner = THIS_MODULE,
    .open = ldd_deviceOpen,
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
            "ldd_20261002_fileOperations: "
            "Failed to allocate device number\n"
        );

        return result;
    }

    //---------------------------------------------------

    pr_info(
        "ldd_20261002_fileOperations: "
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
            "ldd_20261002_fileOperations: "
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
        "ldd_20261002_fileOperations: "
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
        "ldd_20261002_fileOperations: "
        "Character device unregistered\n"
    );
}

//---------------------------------------------------

module_init(ldd_moduleInit);
module_exit(ldd_moduleExit);

//---------------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Lr");
MODULE_DESCRIPTION(
    "Linux character device file_operations demonstration"
);

//---------------------------------------------------



