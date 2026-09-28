#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/gpio/consumer.h>

//-------------------------------------------

static int dt_gpio_probe(struct platform_device* pdev)
{
    struct gpio_desc* gpio;

    gpio = devm_gpiod_get(&pdev->dev, "enable", GPIOD_OUT_LOW);

    if (IS_ERR(gpio)) {
        dev_err(&pdev->dev,
            "Failed to get enable GPIO\n");

        return PTR_ERR(gpio);
    }

    dev_info(&pdev->dev,
        "GPIO successfully obtained from Device Tree\n");

    gpiod_set_value_cansleep(gpio, 1);

    dev_info(&pdev->dev,
        "GPIO set HIGH\n");

    return 0;
}

//-------------------------------------------

static int dt_gpio_remove(struct platform_device* pdev)
{
    dev_info(&pdev->dev,
        "GPIO driver remove()\n");

    return 0;
}

//-------------------------------------------

static const struct of_device_id dt_gpio_of_match[] = {
    {
        .compatible = "ldd,gpio-demo",
    },
    { }
};

MODULE_DEVICE_TABLE(of, dt_gpio_of_match);

//-------------------------------------------

static struct platform_driver dt_gpio_driver = {
    .probe = dt_gpio_probe,
    .remove = dt_gpio_remove,

    .driver = {
        .name = "ldd_dt_gpio",
        .of_match_table = dt_gpio_of_match,
    },
};

//-------------------------------------------

module_platform_driver(dt_gpio_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Device Tree GPIO example");


