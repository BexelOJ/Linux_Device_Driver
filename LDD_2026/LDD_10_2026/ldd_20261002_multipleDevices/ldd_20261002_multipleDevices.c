#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>

//---------------------------------------------------

#define DEVICE_NAME "ldd_multi"
#define CLASS_NAME  "ldd_multi"
#define DEVICE_COUNT 3

//---------------------------------------------------

static dev_t deviceNumber;
static struct cdev ldd_cdev;
static struct class *ldd_class;

//---------------------------------------------------

static int ldd_deviceOpen(
    struct inode *inode,
    struct file *file
)
{
    pr_info(
        "ldd_20261002_multipleDevices: "
        "Opened minor = %u\n",
        iminor(inode)
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
        "ldd_20261002_multipleDevices: "
        "Released minor = %u\n",
        iminor(inode)
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
    int index;

    //---------------------------------------------------

    result = alloc_chrdev_region(
        &deviceNumber,
        0,
        DEVICE_COUNT,
        DEVICE_NAME
    );

    if (result < 0)
        return result;

    //---------------------------------------------------

    cdev_init(
        &ldd_cdev,
        &ldd_fops
    );

    result = cdev_add(
        &ldd_cdev,
        deviceNumber,
        DEVICE_COUNT
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

    for (index = 0; index < DEVICE_COUNT; index++)
    {
        struct device *device;

        device = device_create(
            ldd_class,
            NULL,
            MKDEV(
                MAJOR(deviceNumber),
                MINOR(deviceNumber) + index
            ),
            NULL,
            "%s%d",
            DEVICE_NAME,
            index
        );

        if (IS_ERR(device))
        {
            result = PTR_ERR(device);

            while (index > 0)
            {
                index--;

                device_destroy(
                    ldd_class,
                    MKDEV(
                        MAJOR(deviceNumber),
                        MINOR(deviceNumber) + index
                    )
                );
            }

            goto error_device;
        }
    }

    //---------------------------------------------------

    pr_info(
        "ldd_20261002_multipleDevices: "
        "Major = %u\n",
        MAJOR(deviceNumber)
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
        DEVICE_COUNT
    );

    return result;
}

//---------------------------------------------------

static void __exit ldd_moduleExit(void)
{
    int index;

    //---------------------------------------------------

    for (index = 0; index < DEVICE_COUNT; index++)
    {
        device_destroy(
            ldd_class,
            MKDEV(
                MAJOR(deviceNumber),
                MINOR(deviceNumber) + index
            )
        );
    }

    //---------------------------------------------------

    class_destroy(ldd_class);

    cdev_del(&ldd_cdev);

    unregister_chrdev_region(
        deviceNumber,
        DEVICE_COUNT
    );

    //---------------------------------------------------

    pr_info(
        "ldd_20261002_multipleDevices: "
        "All devices removed\n"
    );
}

//---------------------------------------------------

module_init(ldd_moduleInit);
module_exit(ldd_moduleExit);

//---------------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Lr");
MODULE_DESCRIPTION(
    "Linux multiple character devices"
);

//---------------------------------------------------



