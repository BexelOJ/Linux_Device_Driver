#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/input.h>

//-------------------------------------------

static struct input_dev* ldd_input_dev;

//-------------------------------------------

static int __init ldd_inputBasic_init(void)
{
    int ret;

    pr_info("ldd_inputBasic: init\n");

    //-------------------------------------------
    // Allocate input device
    //-------------------------------------------

    ldd_input_dev = input_allocate_device();

    if (!ldd_input_dev)
        return -ENOMEM;

    //-------------------------------------------
    // Configure input device
    //-------------------------------------------

    ldd_input_dev->name = "ldd-input-basic";
    ldd_input_dev->phys = "ldd/input0";
    ldd_input_dev->id.bustype = BUS_HOST;

    //-------------------------------------------
    // Declare KEY_A capability
    //-------------------------------------------

    input_set_capability(ldd_input_dev,
        EV_KEY,
        KEY_A);

    //-------------------------------------------
    // Register input device
    //-------------------------------------------

    ret = input_register_device(ldd_input_dev);

    if (ret) {
        pr_err("ldd_inputBasic: registration failed\n");

        input_free_device(ldd_input_dev);

        return ret;
    }

    pr_info("ldd_inputBasic: registered\n");

    return 0;
}

//-------------------------------------------

static void __exit ldd_inputBasic_exit(void)
{
    pr_info("ldd_inputBasic: exit\n");

    input_unregister_device(ldd_input_dev);
}

//-------------------------------------------

module_init(ldd_inputBasic_init);
module_exit(ldd_inputBasic_exit);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Basic Linux Input device");


/*
//-------------------------------------------



//-------------------------------------------
*/


