#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>

//---------------------------------------------------

#define DEVICE_NAME "ldd_cdev"

static dev_t deviceNumber;
static struct cdev ldd_cdev;

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
            "ldd_20261002_cdev: "
            "Failed to allocate device number\n"
        );

        return result;
    }

    //---------------------------------------------------

    pr_info(
        "ldd_20261002_cdev: "
        "Major = %u, Minor = %u\n",
        MAJOR(deviceNumber),
        MINOR(deviceNumber)
    );

    //---------------------------------------------------

    cdev_init(
        &ldd_cdev,
        NULL
    );

    //---------------------------------------------------

    ldd_cdev.owner = THIS_MODULE;

    //---------------------------------------------------

    result = cdev_add(
        &ldd_cdev,
        deviceNumber,
        1
    );

    if (result < 0)
    {
        pr_err(
            "ldd_20261002_cdev: "
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
        "ldd_20261002_cdev: "
        "Character device added\n"
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
        "ldd_20261002_cdev: "
        "Character device removed\n"
    );
}

//---------------------------------------------------

module_init(ldd_moduleInit);
module_exit(ldd_moduleExit);

//---------------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION(
    "Linux character device cdev demonstration"
);

//---------------------------------------------------


/*
//---------------------------------------------------



//---------------------------------------------------
*/


