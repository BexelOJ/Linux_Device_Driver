#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/input.h>

//-------------------------------------------

static struct input_dev* ldd_input_dev;

//-------------------------------------------

static int __init ldd_inputEvent_init(void)
{
    int ret;

    pr_info("ldd_inputEvent: init\n");

    //-------------------------------------------
    // Allocate
    //-------------------------------------------

    ldd_input_dev = input_allocate_device();

    if (!ldd_input_dev)
        return -ENOMEM;

    //-------------------------------------------
    // Configure
    //-------------------------------------------

    ldd_input_dev->name = "ldd-input-event";
    ldd_input_dev->phys = "ldd/event0";
    ldd_input_dev->id.bustype = BUS_VIRTUAL;

    input_set_capability(ldd_input_dev,
        EV_KEY,
        KEY_A);

    //-------------------------------------------
    // Register
    //-------------------------------------------

    ret = input_register_device(ldd_input_dev);

    if (ret) {
        input_free_device(ldd_input_dev);
        return ret;
    }

    //-------------------------------------------
    // KEY_A press
    //-------------------------------------------

    input_report_key(ldd_input_dev, KEY_A, 1);
    input_sync(ldd_input_dev);

    //-------------------------------------------
    // KEY_A release
    //-------------------------------------------

    input_report_key(ldd_input_dev, KEY_A, 0);
    input_sync(ldd_input_dev);

    pr_info("ldd_inputEvent: KEY_A generated\n");

    return 0;
}

//-------------------------------------------

static void __exit ldd_inputEvent_exit(void)
{
    input_unregister_device(ldd_input_dev);

    pr_info("ldd_inputEvent: exit\n");
}

//-------------------------------------------

module_init(ldd_inputEvent_init);
module_exit(ldd_inputEvent_exit);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Linux Input Event example");


/*


*/


