#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/device.h>

//---------------------------------------------------

#define CLASS_NAME "ldd_class"

//---------------------------------------------------

static struct class *ldd_class;

//---------------------------------------------------

static int __init ldd_moduleInit(void)
{
    ldd_class = class_create(CLASS_NAME);

    if (IS_ERR(ldd_class))
    {
        pr_err(
            "ldd_20261002_classCreate: "
            "class_create() failed\n"
        );

        return PTR_ERR(ldd_class);
    }

    //---------------------------------------------------

    pr_info(
        "ldd_20261002_classCreate: "
        "Class created\n"
    );

    return 0;
}

//---------------------------------------------------

static void __exit ldd_moduleExit(void)
{
    class_destroy(ldd_class);

    pr_info(
        "ldd_20261002_classCreate: "
        "Class destroyed\n"
    );
}

//---------------------------------------------------

module_init(ldd_moduleInit);
module_exit(ldd_moduleExit);

//---------------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Lr");
MODULE_DESCRIPTION(
    "Linux device class creation demonstration"
);

//---------------------------------------------------



