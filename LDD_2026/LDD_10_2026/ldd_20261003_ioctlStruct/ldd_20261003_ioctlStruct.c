#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "ldd_20261003_ioctlStruct"
#define CLASS_NAME  "ldd_ioctlStruct"

//-------------------------------------------
// IOCTL definitions
//-------------------------------------------

#define LDD_IOCTL_MAGIC 'L'

#define LDD_IOCTL_SET_DATA \
    _IOW(LDD_IOCTL_MAGIC, 1, struct ldd_data)

#define LDD_IOCTL_GET_DATA \
    _IOR(LDD_IOCTL_MAGIC, 2, struct ldd_data)

#define LDD_IOCTL_RESET_DATA \
    _IO(LDD_IOCTL_MAGIC, 3)

//-------------------------------------------
// Data structure
//-------------------------------------------

struct ldd_data
{
    int id;

    int temperature;

    int status;

    char name[32];
};

//-------------------------------------------
// Global data
//-------------------------------------------

static struct ldd_data ldd_kernelData;

static dev_t ldd_devNumber;

static struct cdev ldd_cdev;
static struct class* ldd_class;
static struct device* ldd_device;

//-------------------------------------------
// Open
//-------------------------------------------

static int ldd_open(
    struct inode* inode,
    struct file* file)
{
    pr_info(
        "ldd_20261003_ioctlStruct: device opened\n"
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
        "ldd_20261003_ioctlStruct: device closed\n"
    );

    return 0;
}

//-------------------------------------------
// IOCTL
//-------------------------------------------

static long ldd_ioctl(
    struct file* file,
    unsigned int command,
    unsigned long argument)
{
    struct ldd_data userData;

    switch (command)
    {
        //-------------------------------------------
        // User -> Kernel
        //-------------------------------------------

    case LDD_IOCTL_SET_DATA:

        if (copy_from_user(
            &userData,
            (struct ldd_data __user*)argument,
            sizeof(struct ldd_data)))
        {
            pr_err(
                "ldd_20261003_ioctlStruct: "
                "copy_from_user failed\n"
            );

            return -EFAULT;
        }

        ldd_kernelData = userData;

        pr_info(
            "ldd_20261003_ioctlStruct: "
            "SET_DATA\n"
        );

        pr_info(
            "id          = %d\n",
            ldd_kernelData.id
        );

        pr_info(
            "temperature = %d\n",
            ldd_kernelData.temperature
        );

        pr_info(
            "status      = %d\n",
            ldd_kernelData.status
        );

        pr_info(
            "name        = %s\n",
            ldd_kernelData.name
        );

        break;

        //-------------------------------------------
        // Kernel -> User
        //-------------------------------------------

    case LDD_IOCTL_GET_DATA:

        userData = ldd_kernelData;

        if (copy_to_user(
            (struct ldd_data __user*)argument,
            &userData,
            sizeof(struct ldd_data)))
        {
            pr_err(
                "ldd_20261003_ioctlStruct: "
                "copy_to_user failed\n"
            );

            return -EFAULT;
        }

        pr_info(
            "ldd_20261003_ioctlStruct: "
            "GET_DATA\n"
        );

        break;

        //-------------------------------------------
        // Reset
        //-------------------------------------------

    case LDD_IOCTL_RESET_DATA:

        memset(
            &ldd_kernelData,
            0,
            sizeof(ldd_kernelData)
        );

        pr_info(
            "ldd_20261003_ioctlStruct: "
            "data reset\n"
        );

        break;

        //-------------------------------------------
        // Unknown command
        //-------------------------------------------

    default:

        pr_err(
            "ldd_20261003_ioctlStruct: "
            "unknown ioctl command\n"
        );

        return -ENOTTY;
    }

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

    .unlocked_ioctl = ldd_ioctl,
};

//-------------------------------------------
// Module initialization
//-------------------------------------------

static int __init ldd_init(void)
{
    int result;

    pr_info(
        "ldd_20261003_ioctlStruct: "
        "module loading\n"
    );

    //-------------------------------------------
    // Initialize structure
    //-------------------------------------------

    memset(
        &ldd_kernelData,
        0,
        sizeof(ldd_kernelData)
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
            "ldd_20261003_ioctlStruct: "
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
            "ldd_20261003_ioctlStruct: "
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
            "ldd_20261003_ioctlStruct: "
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
            "ldd_20261003_ioctlStruct: "
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
        "ldd_20261003_ioctlStruct: "
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
        "ldd_20261003_ioctlStruct: "
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
    "Linux ioctl structure data transfer example"
);



