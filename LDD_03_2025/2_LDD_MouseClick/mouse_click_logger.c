#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/input.h>
#include <linux/platform_device.h>
#include <linux/fs.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A Simple Mouse Click Logging Driver");

// Input device structure
static struct input_dev *mouse_dev;

// Input handler function to log mouse clicks
static void mouse_event_handler(struct input_handle *handle, unsigned int type, unsigned int code, int value)
{
    if (type == EV_KEY) {
        if (code == BTN_LEFT && value == 1) {
            printk(KERN_INFO "Mouse Left Click detected.\n");
        }
        if (code == BTN_RIGHT && value == 1) {
            printk(KERN_INFO "Mouse Right Click detected.\n");
        }
    }
}

// Input handler structure
static struct input_handler mouse_input_handler = {
    .event = mouse_event_handler,  // Function pointer for event handling
};

// Register the input handler
static int __init mouse_click_logger_init(void)
{
    int error;

    // Allocate input device
    mouse_dev = input_allocate_device(); 
    if (!mouse_dev) {
        printk(KERN_ERR "Failed to allocate input device\n");
        return -ENOMEM;
    }

    mouse_dev->name = "Mouse Click Logger";
    mouse_dev->evbit[0] = BIT_MASK(EV_KEY);
    mouse_dev->keybit[BIT_WORD(BTN_LEFT)] = BIT_MASK(BTN_LEFT) | BIT_MASK(BTN_RIGHT);

    // Register the input device
    error = input_register_device(mouse_dev);
    if (error) {
        printk(KERN_ERR "Failed to register input device\n");
        input_free_device(mouse_dev);
        return error;
    }

    // Register the input handler
    error = input_register_handler(&mouse_input_handler);
    if (error) {
        printk(KERN_ERR "Failed to register input handler\n");
        input_unregister_device(mouse_dev);
        return error;
    }

    printk(KERN_INFO "Mouse Click Logger initialized.\n");
    return 0;
}

// Cleanup function to unregister the handler and input device
static void __exit mouse_click_logger_exit(void)
{
    input_unregister_handler(&mouse_input_handler);
    input_unregister_device(mouse_dev);
    printk(KERN_INFO "Mouse Click Logger exited.\n");
}

module_init(mouse_click_logger_init);
module_exit(mouse_click_logger_exit);



