#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/gpio/consumer.h>

//-------------------------------------------

struct button_data {
    struct gpio_desc* gpio;
};

//-------------------------------------------

static int gpio_button_probe(struct platform_device* pdev)
{
    struct button_data* data;
    int value;

    data = devm_kzalloc(&pdev->dev,
        sizeof(*data),
        GFP_KERNEL);

    if (!data)
        return -ENOMEM;

    data->gpio = devm_gpiod_get(&pdev->dev,
        "button",
        GPIOD_IN);

    if (IS_ERR(data->gpio))
        return PTR_ERR(data->gpio);

    value = gpiod_get_value_cansleep(data->gpio);

    if (value)
        dev_info(&pdev->dev,
            "Button state: PRESSED\n");
    else
        dev_info(&pdev->dev,
            "Button state: RELEASED\n");

    platform_set_drvdata(pdev, data);

    return 0;
}

//-------------------------------------------

static const struct of_device_id gpio_button_match[] = {
    {
        .compatible = "ldd,gpio-button",
    },
    { }
};

MODULE_DEVICE_TABLE(of, gpio_button_match);

//-------------------------------------------

static struct platform_driver gpio_button_driver = {
    .probe = gpio_button_probe,

    .driver = {
        .name = "ldd_gpio_button",
        .of_match_table = gpio_button_match,
    },
};

//-------------------------------------------

module_platform_driver(gpio_button_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("GPIO button input example");


/*
//-------------------------------------------



//-------------------------------------------
*/


