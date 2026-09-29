#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/input.h>

//-------------------------------------------

static struct input_dev* ldd_keyboard;

//-------------------------------------------

static int __init ldd_inputKeyboard_init(void)
{
    int ret;

    pr_info("ldd_inputKeyboard: init\n");

    //-------------------------------------------
    // Allocate
    //-------------------------------------------

    ldd_keyboard = input_allocate_device();

    if (!ldd_keyboard)
        return -ENOMEM;

    //-------------------------------------------
    // Configure
    //-------------------------------------------

    ldd_keyboard->name = "ldd-virtual-keyboard";
    ldd_keyboard->phys = "ldd/keyboard0";
    ldd_keyboard->id.bustype = BUS_VIRTUAL;

    //-------------------------------------------
    // Keyboard capabilities
    //-------------------------------------------

    input_set_capability(ldd_keyboard, EV_KEY, KEY_A);
    input_set_capability(ldd_keyboard, EV_KEY, KEY_B);
    input_set_capability(ldd_keyboard, EV_KEY, KEY_ENTER);
    input_set_capability(ldd_keyboard, EV_KEY, KEY_ESC);

    //-------------------------------------------
    // Register
    //-------------------------------------------

    ret = input_register_device(ldd_keyboard);

    if (ret) {
        input_free_device(ldd_keyboard);
        return ret;
    }

    pr_info("ldd_inputKeyboard: registered\n");

    return 0;
}

//-------------------------------------------

static void __exit ldd_inputKeyboard_exit(void)
{
    input_unregister_device(ldd_keyboard);

    pr_info("ldd_inputKeyboard: exit\n");
}

//-------------------------------------------

module_init(ldd_inputKeyboard_init);
module_exit(ldd_inputKeyboard_exit);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Virtual keyboard input device");


/*
//-------------------------------------------



//-------------------------------------------
*/


