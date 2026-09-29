#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/ioctl.h>

//---------------------------------------------------

#define DEVICE_NAME "ldd_20261003_ioctl"

//---------------------------------------------------
// IOCTL magic number
//---------------------------------------------------

#define LDD_IOCTL_MAGIC 'L'

//---------------------------------------------------
// IOCTL commands
//---------------------------------------------------

#define LDD_IOCTL_GET_VALUE \
    _IOR(LDD_IOCTL_MAGIC, 1, int)

#define LDD_IOCTL_SET_VALUE \
    _IOW(LDD_IOCTL_MAGIC, 2, int)

#define LDD_IOCTL_RESET_VALUE \
    _IO(LDD_IOCTL_MAGIC, 3)

//---------------------------------------------------

static dev_t ldd_devNumber;

static struct cdev ldd_cdev;
static struct class* ldd_class;
static struct device* ldd_device;

//---------------------------------------------------

static int ldd_value = 100;

//---------------------------------------------------

static int ldd_open(
    struct inode* inode,
    struct file* file
)
{
    pr_info(
        "ldd_20261003_ioctl: Device opened\n"
    );

    return 0;
}

//---------------------------------------------------

static int ldd_release(
    struct inode* inode,
    struct file* file
)
{
    pr_info(
        "ldd_20261003_ioctl: Device closed\n"
    );

    return 0;
}

//---------------------------------------------------

static long ldd_ioctl(
    struct file* file,
    unsigned int command,
    unsigned long argument
)
{
    int value;

    switch (command) {

        //------------------------------------------------
        // GET VALUE
        //------------------------------------------------

    case LDD_IOCTL_GET_VALUE:

        pr_info(
            "ldd_20261003_ioctl: GET_VALUE\n"
        );

        value = ldd_value;

        if (copy_to_user(
            (int __user*)argument,
            &value,
            sizeof(value))) {

            return -EFAULT;
        }

        break;

        //------------------------------------------------
        // SET VALUE
        //------------------------------------------------

    case LDD_IOCTL_SET_VALUE:

        pr_info(
            "ldd_20261003_ioctl: SET_VALUE\n"
        );

        if (copy_from_user(
            &value,
            (int __user*)argument,
            sizeof(value))) {

            return -EFAULT;
        }

        ldd_value = value;

        pr_info(
            "ldd_20261003_ioctl: "
            "Value set to %d\n",
            ldd_value
        );

        break;

        //------------------------------------------------
        // RESET VALUE
        //------------------------------------------------

    case LDD_IOCTL_RESET_VALUE:

        pr_info(
            "ldd_20261003_ioctl: RESET_VALUE\n"
        );

        ldd_value = 100;

        break;

        //------------------------------------------------
        // UNKNOWN COMMAND
        //------------------------------------------------

    default:

        pr_err(
            "ldd_20261003_ioctl: "
            "Unknown ioctl command: %u\n",
            command
        );

        return -ENOTTY;
    }

    return 0;
}

//---------------------------------------------------

static const struct file_operations ldd_fops = {

    .owner = THIS_MODULE,

    .open = ldd_open,

    .release = ldd_release,

    .unlocked_ioctl = ldd_ioctl,
};

//---------------------------------------------------

static int __init ldd_moduleInit(void)
{
    int ret;

    pr_info(
        "ldd_20261003_ioctl: Module initialized\n"
    );

    //------------------------------------------------
    // Allocate device number
    //------------------------------------------------

    ret = alloc_chrdev_region(
        &ldd_devNumber,
        0,
        1,
        DEVICE_NAME
    );

    if (ret < 0) {

        pr_err(
            "ldd_20261003_ioctl: "
            "alloc_chrdev_region failed\n"
        );

        return ret;
    }

    //------------------------------------------------
    // Initialize character device
    //------------------------------------------------

    cdev_init(
        &ldd_cdev,
        &ldd_fops
    );

    ldd_cdev.owner = THIS_MODULE;

    //------------------------------------------------
    // Add character device
    //------------------------------------------------

    ret = cdev_add(
        &ldd_cdev,
        ldd_devNumber,
        1
    );

    if (ret < 0) {

        pr_err(
            "ldd_20261003_ioctl: "
            "cdev_add failed\n"
        );

        unregister_chrdev_region(
            ldd_devNumber,
            1
        );

        return ret;
    }

    //------------------------------------------------
    // Create class
    //------------------------------------------------

    ldd_class = class_create(
        DEVICE_NAME
    );

    if (IS_ERR(ldd_class)) {

        cdev_del(
            &ldd_cdev
        );

        unregister_chrdev_region(
            ldd_devNumber,
            1
        );

        return PTR_ERR(
            ldd_class
        );
    }

    //------------------------------------------------
    // Create device
    //------------------------------------------------

    ldd_device = device_create(
        ldd_class,
        NULL,
        ldd_devNumber,
        NULL,
        DEVICE_NAME
    );

    if (IS_ERR(ldd_device)) {

        class_destroy(
            ldd_class
        );

        cdev_del(
            &ldd_cdev
        );

        unregister_chrdev_region(
            ldd_devNumber,
            1
        );

        return PTR_ERR(
            ldd_device
        );
    }

    //------------------------------------------------

    pr_info(
        "ldd_20261003_ioctl: "
        "Device created: /dev/%s\n",
        DEVICE_NAME
    );

    return 0;
}

//---------------------------------------------------

static void __exit ldd_moduleExit(void)
{
    //------------------------------------------------
    // Remove device
    //------------------------------------------------

    device_destroy(
        ldd_class,
        ldd_devNumber
    );

    //------------------------------------------------
    // Remove class
    //------------------------------------------------

    class_destroy(
        ldd_class
    );

    //------------------------------------------------
    // Remove cdev
    //------------------------------------------------

    cdev_del(
        &ldd_cdev
    );

    //------------------------------------------------
    // Release device number
    //------------------------------------------------

    unregister_chrdev_region(
        ldd_devNumber,
        1
    );

    pr_info(
        "ldd_20261003_ioctl: Module exited\n"
    );
}

//---------------------------------------------------

module_init(
    ldd_moduleInit
);

module_exit(
    ldd_moduleExit
);

//---------------------------------------------------

MODULE_LICENSE("GPL");

MODULE_AUTHOR("Er Bexel O J");

MODULE_DESCRIPTION(
    "Linux kernel ioctl character driver"
);

//---------------------------------------------------


/*
//---------------------------------------------------



//---------------------------------------------------
*/


