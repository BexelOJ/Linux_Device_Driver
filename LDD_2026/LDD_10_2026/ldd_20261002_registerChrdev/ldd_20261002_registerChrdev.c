#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>

//---------------------------------------------------

#define DEVICE_NAME "ldd_registerChrdev"

static int majorNumber;

//---------------------------------------------------

static int __init ldd_moduleInit(void)
{
    majorNumber = register_chrdev(
        0,
        DEVICE_NAME,
        NULL
    );

    if (majorNumber < 0)
    {
        pr_err(
            "ldd_20261002_registerChrdev: "
            "Failed to register character device\n"
        );

        return majorNumber;
    }

    pr_info(
        "ldd_20261002_registerChrdev: "
        "Character device registered\n"
    );

    pr_info(
        "ldd_20261002_registerChrdev: "
        "Major number = %d\n",
        majorNumber
    );

    return 0;
}

//---------------------------------------------------

static void __exit ldd_moduleExit(void)
{
    unregister_chrdev(
        majorNumber,
        DEVICE_NAME
    );

    pr_info(
        "ldd_20261002_registerChrdev: "
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
    "Linux register_chrdev character device demonstration"
);

//---------------------------------------------------


/*
//---------------------------------------------------



//---------------------------------------------------
*/


