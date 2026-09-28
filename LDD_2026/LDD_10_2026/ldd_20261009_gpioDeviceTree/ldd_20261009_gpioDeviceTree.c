#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/gpio/consumer.h>

//-------------------------------------------

static int gpio_dt_probe(struct platform_device* pdev)
{
    struct gpio_desc* gpio;

    gpio = devm_gpiod_get(&pdev->dev,
        "enable",
        GPIOD_OUT_LOW);

    if (IS_ERR(gpio)) {
        dev_err(&pdev->dev,
            "Failed to get GPIO\n");

        return PTR_ERR(gpio);
    }

    dev_info(&pdev->dev,
        "GPIO obtained from Device Tree\n");

    gpiod_set_value_cansleep(gpio, 1);

    dev_info(&pdev->dev,
        "GPIO set HIGH\n");

    return 0;
}

//-------------------------------------------

static int gpio_dt_remove(struct platform_device* pdev)
{
    dev_info(&pdev->dev,
        "GPIO Device Tree driver removed\n");

    return 0;
}

//-------------------------------------------

static const struct of_device_id gpio_dt_match[] = {
    {
        .compatible = "ldd,gpio-device",
    },
    { }
};

MODULE_DEVICE_TABLE(of, gpio_dt_match);

//-------------------------------------------

static struct platform_driver gpio_dt_driver = {
    .probe = gpio_dt_probe,
    .remove = gpio_dt_remove,

    .driver = {
        .name = "ldd_gpio_device_tree",
        .of_match_table = gpio_dt_match,
    },
};

//-------------------------------------------

module_platform_driver(gpio_dt_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("GPIO Device Tree example");


/*
//-------------------------------------------
Device Tree
    │
    │ enable-gpios
    ↓
devm_gpiod_get()
    │
    ↓
struct gpio_desc *

//-------------------------------------------
*/


