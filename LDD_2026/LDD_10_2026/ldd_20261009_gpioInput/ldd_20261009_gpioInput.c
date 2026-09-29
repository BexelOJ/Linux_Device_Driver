#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/gpio/consumer.h>

//-------------------------------------------

static int gpio_input_probe(struct platform_device* pdev)
{
    struct gpio_desc* gpio;
    int value;

    gpio = devm_gpiod_get(&pdev->dev,
        "input",
        GPIOD_IN);

    if (IS_ERR(gpio)) {
        dev_err(&pdev->dev,
            "Failed to get input GPIO\n");

        return PTR_ERR(gpio);
    }

    value = gpiod_get_value_cansleep(gpio);

    dev_info(&pdev->dev,
        "GPIO input value = %d\n",
        value);

    return 0;
}

//-------------------------------------------

static const struct of_device_id gpio_input_match[] = {
    {
        .compatible = "ldd,gpio-input",
    },
    { }
};

MODULE_DEVICE_TABLE(of, gpio_input_match);

//-------------------------------------------

static struct platform_driver gpio_input_driver = {
    .probe = gpio_input_probe,

    .driver = {
        .name = "ldd_gpio_input",
        .of_match_table = gpio_input_match,
    },
};

//-------------------------------------------

module_platform_driver(gpio_input_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("GPIO input example");


/*
//-------------------------------------------



//-------------------------------------------
*/


