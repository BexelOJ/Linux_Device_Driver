#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/gpio/consumer.h>

//-------------------------------------------

static int gpio_output_probe(struct platform_device* pdev)
{
    struct gpio_desc* gpio;

    gpio = devm_gpiod_get(&pdev->dev,
        "output",
        GPIOD_OUT_LOW);

    if (IS_ERR(gpio)) {
        dev_err(&pdev->dev,
            "Failed to get output GPIO\n");

        return PTR_ERR(gpio);
    }

    dev_info(&pdev->dev,
        "Setting GPIO HIGH\n");

    gpiod_set_value_cansleep(gpio, 1);

    return 0;
}

//-------------------------------------------

static const struct of_device_id gpio_output_match[] = {
    {
        .compatible = "ldd,gpio-output",
    },
    { }
};

MODULE_DEVICE_TABLE(of, gpio_output_match);

//-------------------------------------------

static struct platform_driver gpio_output_driver = {
    .probe = gpio_output_probe,

    .driver = {
        .name = "ldd_gpio_output",
        .of_match_table = gpio_output_match,
    },
};

//-------------------------------------------

module_platform_driver(gpio_output_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("GPIO output example");


/*
//-------------------------------------------



//-------------------------------------------
*/


