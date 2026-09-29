#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/gpio/consumer.h>

//-------------------------------------------

static int gpio_led_probe(struct platform_device* pdev)
{
    struct gpio_desc* led;

    led = devm_gpiod_get(&pdev->dev,
        "led",
        GPIOD_OUT_LOW);

    if (IS_ERR(led)) {
        dev_err(&pdev->dev,
            "Failed to get LED GPIO\n");

        return PTR_ERR(led);
    }

    dev_info(&pdev->dev,
        "Turning LED ON\n");

    gpiod_set_value_cansleep(led, 1);

    return 0;
}

//-------------------------------------------

static int gpio_led_remove(struct platform_device* pdev)
{
    dev_info(&pdev->dev,
        "Turning LED OFF\n");

    /*
     * Normally the GPIO descriptor should be stored
     * in driver data if it is needed during remove().
     */

    return 0;
}

//-------------------------------------------

static const struct of_device_id gpio_led_match[] = {
    {
        .compatible = "ldd,gpio-led",
    },
    { }
};

MODULE_DEVICE_TABLE(of, gpio_led_match);

//-------------------------------------------

static struct platform_driver gpio_led_driver = {
    .probe = gpio_led_probe,
    .remove = gpio_led_remove,

    .driver = {
        .name = "ldd_gpio_led",
        .of_match_table = gpio_led_match,
    },
};

//-------------------------------------------

module_platform_driver(gpio_led_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("GPIO LED example");


/*
//-------------------------------------------



//-------------------------------------------
*/


