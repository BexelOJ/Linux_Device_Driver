#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/input.h>
#include <linux/ktime.h>  // For ktime_get_real()
#include <linux/time64.h> // For time64_to_tm()

static struct input_dev* mouse_dev;

// This function is called whenever a mouse event occurs
static int mouse_event(struct input_handle* handle, unsigned int type, unsigned int code, int value)
{
    if (type == EV_KEY && (code == BTN_LEFT || code == BTN_RIGHT)) {
        // Get current time
        ktime_t kt = ktime_get_real();  // Get real-time clock
        struct timespec64 ts = ktime_to_timespec64(kt);  // Convert to timespec64

        struct tm tm;
        time64_to_tm(ts.tv_sec, 0, &tm);  // Convert to broken-down time (tm)

        // Log mouse click event with timestamp to the kernel log
        pr_info("Mouse click detected at %04d-%02d-%02d %02d:%02d:%02d:%09ld: %s button\n",
            tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
            tm.tm_hour, tm.tm_min, tm.tm_sec, ts.tv_nsec,
            (code == BTN_LEFT) ? "Left" : "Right");
    }
    return 0;
}

static int __init mouse_click_logger_init(void)
{
    int error;

    pr_info("Mouse click logger module loaded\n");

    // Register input device
    mouse_dev = input_allocate_device();
    if (!mouse_dev) {
        pr_err("Failed to allocate input device\n");
        return -ENOMEM;
    }

    mouse_dev->name = "Mouse Click Logger";
    mouse_dev->evbit[0] = BIT(EV_KEY);  // EV_KEY events
    mouse_dev->keybit[BIT_WORD(BTN_LEFT)] = BIT(BTN_LEFT);  // Left button
    mouse_dev->keybit[BIT_WORD(BTN_RIGHT)] = BIT(BTN_RIGHT); // Right button

    // Register input device
    error = input_register_device(mouse_dev);
    if (error) {
        pr_err("Failed to register input device\n");
        input_free_device(mouse_dev);
        return error;
    }

    pr_info("Mouse click logger registered\n");

    return 0;
}

static void __exit mouse_click_logger_exit(void)
{
    pr_info("Mouse click logger module unloaded\n");

    // Cleanup
    input_unregister_device(mouse_dev);
    input_free_device(mouse_dev);
}

// Register the module's init and exit functions
module_init(mouse_click_logger_init);
module_exit(mouse_click_logger_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A simple module to log mouse clicks with timestamp.");
