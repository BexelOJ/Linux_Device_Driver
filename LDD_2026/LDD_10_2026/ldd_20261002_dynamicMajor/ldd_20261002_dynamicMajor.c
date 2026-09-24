#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>

//---------------------------------------------------

#define DEVICE_NAME "ldd_dynamicMajor"

static dev_t deviceNumber;

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
            "ldd_20261002_dynamicMajor: "
            "alloc_chrdev_region() failed\n"
        );

        return result;
    }

    //---------------------------------------------------

    pr_info(
        "ldd_20261002_dynamicMajor: "
        "Dynamic device number allocated\n"
    );

    pr_info(
        "ldd_20261002_dynamicMajor: "
        "Major = %u\n",
        MAJOR(deviceNumber)
    );

    pr_info(
        "ldd_20261002_dynamicMajor: "
        "Minor = %u\n",
        MINOR(deviceNumber)
    );

    pr_info(
        "ldd_20261002_dynamicMajor: "
        "dev_t = %u\n",
        deviceNumber
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
        "ldd_20261002_dynamicMajor: "
        "Device number released\n"
    );
}

//---------------------------------------------------

module_init(ldd_moduleInit);
module_exit(ldd_moduleExit);

//---------------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Lr");
MODULE_DESCRIPTION(
    "Linux dynamic major number allocation"
);

//---------------------------------------------------



