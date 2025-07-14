#include <linux/init.h>
#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/delay.h>

#define LED_GPIO 17  // Change this to your LED GPIO number

MODULE_LICENSE("GPL");
MODULE_AUTHOR("BexelOJ");
MODULE_DESCRIPTION("Simple LED blink kernel module");
MODULE_VERSION("1.0");

// if (!gpio_is_valid(LED_GPIO)) {
//     printk(KERN_ALERT "LED: Invalid GPIO %d\n", LED_GPIO);
//     return -ENODEV;
// }

static int __init led_init(void)
{
    printk(KERN_INFO "LED: Init\n");

    if (gpio_request(LED_GPIO, "LED_GPIO")) {
        printk(KERN_ALERT "LED: Failed to request GPIO\n");
        return -1;
    }

    gpio_direction_output(LED_GPIO, 1);
    gpio_set_value(LED_GPIO, 1);
    msleep(500);
    gpio_set_value(LED_GPIO, 0);
    msleep(500);
    gpio_set_value(LED_GPIO, 1);
    msleep(500);
    gpio_set_value(LED_GPIO, 0);
    msleep(500);

    printk(KERN_INFO "LED: Blink Done\n");
    return 0;
}

static void __exit led_exit(void)
{
    gpio_set_value(LED_GPIO, 0);
    gpio_free(LED_GPIO);
    printk(KERN_INFO "LED: Exit and cleanup\n");
}

module_init(led_init);
module_exit(led_exit);

