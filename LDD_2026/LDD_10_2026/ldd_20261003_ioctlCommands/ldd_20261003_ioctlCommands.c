#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "ldd_20261003_ioctlCommands"
#define CLASS_NAME  "ldd_ioctlCommands"

//-------------------------------------------
// IOCTL magic number
//-------------------------------------------

#define LDD_IOCTL_MAGIC 'L'

//-------------------------------------------
// IOCTL commands
//-------------------------------------------

// No data transfer
#define LDD_IOCTL_RESET \
    _IO(LDD_IOCTL_MAGIC, 1)

// User -> Kernel
#define LDD_IOCTL_SET_VALUE \
    _IOW(LDD_IOCTL_MAGIC, 2, int)

// Kernel -> User
#define LDD_IOCTL_GET_VALUE \
    _IOR(LDD_IOCTL_MAGIC, 3, int)

// User <-> Kernel
#define LDD_IOCTL_EXCHANGE_VALUE \
    _IOWR(LDD_IOCTL_MAGIC, 4, int)

//-------------------------------------------
// Kernel data
//-------------------------------------------

static int ldd_value = 100;

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
        "ldd_20261003_ioctlCommands: device opened\n"
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
        "ldd_20261003_ioctlCommands: device closed\n"
    );

    return 0;
}

//-------------------------------------------
// IOCTL handler
//-------------------------------------------

static long ldd_ioctl(
    struct file* file,
    unsigned int command,
    unsigned long argument)
{
    int userValue;

    //-------------------------------------------
    // Check magic number
    //-------------------------------------------

    if (_IOC_TYPE(command) != LDD_IOCTL_MAGIC)
    {
        pr_err(
            "ldd_20261003_ioctlCommands: "
            "invalid magic number\n"
        );

        return -ENOTTY;
    }

    //-------------------------------------------
    // Check command number
    //-------------------------------------------

    if (_IOC_NR(command) > 4)
    {
        pr_err(
            "ldd_20261003_ioctlCommands: "
            "invalid command number\n"
        );

        return -ENOTTY;
    }

    //-------------------------------------------
    // Handle command
    //-------------------------------------------

    switch (command)
    {
        //-------------------------------------------
        // _IO
        // No data
        //-------------------------------------------

    case LDD_IOCTL_RESET:

        ldd_value = 0;

        pr_info(
            "ldd_20261003_ioctlCommands: "
            "RESET -> value = %d\n",
            ldd_value
        );

        break;

        //-------------------------------------------
        // _IOW
        // User -> Kernel
        //-------------------------------------------

    case LDD_IOCTL_SET_VALUE:

        if (copy_from_user(
            &userValue,
            (int __user*)argument,
            sizeof(userValue)))
        {
            pr_err(
                "ldd_20261003_ioctlCommands: "
                "copy_from_user failed\n"
            );

            return -EFAULT;
        }

        ldd_value = userValue;

        pr_info(
            "ldd_20261003_ioctlCommands: "
            "SET_VALUE -> %d\n",
            ldd_value
        );

        break;

        //-------------------------------------------
        // _IOR
        // Kernel -> User
        //-------------------------------------------

    case LDD_IOCTL_GET_VALUE:

        userValue = ldd_value;

        if (copy_to_user(
            (int __user*)argument,
            &userValue,
            sizeof(userValue)))
        {
            pr_err(
                "ldd_20261003_ioctlCommands: "
                "copy_to_user failed\n"
            );

            return -EFAULT;
        }

        pr_info(
            "ldd_20261003_ioctlCommands: "
            "GET_VALUE -> %d\n",
            userValue
        );

        break;

        //-------------------------------------------
        // _IOWR
        // User <-> Kernel
        //-------------------------------------------

    case LDD_IOCTL_EXCHANGE_VALUE:

        //-------------------------------------------
        // User -> Kernel
        //-------------------------------------------

        if (copy_from_user(
            &userValue,
            (int __user*)argument,
            sizeof(userValue)))
        {
            pr_err(
                "ldd_20261003_ioctlCommands: "
                "copy_from_user failed\n"
            );

            return -EFAULT;
        }

        pr_info(
            "ldd_20261003_ioctlCommands: "
            "EXCHANGE received = %d\n",
            userValue
        );

        //-------------------------------------------
        // Exchange values
        //-------------------------------------------

        {
            int oldValue = ldd_value;

            ldd_value = userValue;

            userValue = oldValue;
        }

        //-------------------------------------------
        // Kernel -> User
        //-------------------------------------------

        if (copy_to_user(
            (int __user*)argument,
            &userValue,
            sizeof(userValue)))
        {
            pr_err(
                "ldd_20261003_ioctlCommands: "
                "copy_to_user failed\n"
            );

            return -EFAULT;
        }

        pr_info(
            "ldd_20261003_ioctlCommands: "
            "EXCHANGE returned old value = %d\n",
            userValue
        );

        break;

        //-------------------------------------------
        // Unknown command
        //-------------------------------------------

    default:

        pr_err(
            "ldd_20261003_ioctlCommands: "
            "unknown ioctl command = 0x%x\n",
            command
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
        "ldd_20261003_ioctlCommands: "
        "module loading\n"
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
            "ldd_20261003_ioctlCommands: "
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
            "ldd_20261003_ioctlCommands: "
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
            "ldd_20261003_ioctlCommands: "
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
            "ldd_20261003_ioctlCommands: "
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

    //-------------------------------------------
    // Initial value
    //-------------------------------------------

    ldd_value = 100;

    pr_info(
        "ldd_20261003_ioctlCommands: "
        "device = /dev/%s\n",
        DEVICE_NAME
    );

    pr_info(
        "ldd_20261003_ioctlCommands: "
        "initial value = %d\n",
        ldd_value
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
        "ldd_20261003_ioctlCommands: "
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
    "Linux ioctl command types example"
);


/*
//---------------------------------------------------



//---------------------------------------------------
*/





