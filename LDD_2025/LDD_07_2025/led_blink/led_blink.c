#include <linux/init.h>
#include <linux/module.h>
#include <linux/gpio/consumer.h>
#include <linux/platform_device.h>
#include <linux/delay.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Bexel OJ");
MODULE_DESCRIPTION("Blink LED using GPIO descriptor API");
MODULE_VERSION("1.0");

static struct gpio_desc *led_gpio;

static int __init led_init(void)
{
    pr_info("LED: Init using gpiod\n");

    led_gpio = gpiod_get(NULL, "led", GPIOD_OUT_LOW);
    if (IS_ERR(led_gpio)) {
        pr_err("LED: gpiod_get failed\n");
        return PTR_ERR(led_gpio);
    }

    gpiod_set_value(led_gpio, 1);
    msleep(500);
    gpiod_set_value(led_gpio, 0);
    msleep(500);
    gpiod_set_value(led_gpio, 1);
    msleep(500);
    gpiod_set_value(led_gpio, 0);

    return 0;
}

static void __exit led_exit(void)
{
    gpiod_set_value(led_gpio, 0);
    gpiod_put(led_gpio);
    pr_info("LED: Exit and cleanup\n");
}

module_init(led_init);
module_exit(led_exit);
