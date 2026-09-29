#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/input.h>

//-------------------------------------------

static struct input_dev* ldd_mouse;

//-------------------------------------------

static int __init ldd_inputMouse_init(void)
{
    int ret;

    pr_info("ldd_inputMouse: init\n");

    //-------------------------------------------
    // Allocate
    //-------------------------------------------

    ldd_mouse = input_allocate_device();

    if (!ldd_mouse)
        return -ENOMEM;

    //-------------------------------------------
    // Configure
    //-------------------------------------------

    ldd_mouse->name = "ldd-virtual-mouse";
    ldd_mouse->phys = "ldd/mouse0";
    ldd_mouse->id.bustype = BUS_VIRTUAL;

    //-------------------------------------------
    // Mouse buttons
    //-------------------------------------------

    input_set_capability(ldd_mouse,
        EV_KEY,
        BTN_LEFT);

    input_set_capability(ldd_mouse,
        EV_KEY,
        BTN_RIGHT);

    //-------------------------------------------
    // Mouse movement
    //-------------------------------------------

    input_set_capability(ldd_mouse,
        EV_REL,
        REL_X);

    input_set_capability(ldd_mouse,
        EV_REL,
        REL_Y);

    //-------------------------------------------
    // Register
    //-------------------------------------------

    ret = input_register_device(ldd_mouse);

    if (ret) {
        input_free_device(ldd_mouse);
        return ret;
    }

    //-------------------------------------------
    // Simulate movement
    //-------------------------------------------

    input_report_rel(ldd_mouse, REL_X, 100);
    input_report_rel(ldd_mouse, REL_Y, 50);

    input_sync(ldd_mouse);

    //-------------------------------------------
    // Simulate left click
    //-------------------------------------------

    input_report_key(ldd_mouse, BTN_LEFT, 1);
    input_sync(ldd_mouse);

    input_report_key(ldd_mouse, BTN_LEFT, 0);
    input_sync(ldd_mouse);

    pr_info("ldd_inputMouse: events generated\n");

    return 0;
}

//-------------------------------------------

static void __exit ldd_inputMouse_exit(void)
{
    input_unregister_device(ldd_mouse);

    pr_info("ldd_inputMouse: exit\n");
}

//-------------------------------------------

module_init(ldd_inputMouse_init);
module_exit(ldd_inputMouse_exit);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Virtual mouse input device");


/*



*/


