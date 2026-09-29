#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>

//---------------------------------------------------

static dev_t deviceNumber;

//---------------------------------------------------

static int __init ldd_moduleInit(void)
{
    int result;

    result = alloc_chrdev_region(
        &deviceNumber,
        0,
        1,
        "ldd_charDevice"
    );

    if (result < 0)
    {
        pr_err(
            "ldd_20261002_charDevice: "
            "Failed to allocate device number\n"
        );

        return result;
    }

    pr_info(
        "ldd_20261002_charDevice: "
        "Major = %d, Minor = %d\n",
        MAJOR(deviceNumber),
        MINOR(deviceNumber)
    );

    pr_info(
        "ldd_20261002_charDevice: "
        "Character device registered\n"
    );

    return 0;
}

//---------------------------------------------------

static void __exit ldd_moduleExit(void)
{
    unregister_chrdev_region(
        deviceNumber,
        1
    );

    pr_info(
        "ldd_20261002_charDevice: "
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
    "Basic Linux character device registration"
);

//---------------------------------------------------


/*
//---------------------------------------------------



//---------------------------------------------------
*/


