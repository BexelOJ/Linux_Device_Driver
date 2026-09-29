#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/gpio/consumer.h>

#define DRIVER_NAME "ldd_20261024_embeddedGPIO"

struct ldd_gpio_data {
    struct gpio_desc* gpio;
};

static int embeddedGPIO_probe(struct platform_device* pdev)
{
    struct ldd_gpio_data* data;

    data = devm_kzalloc(&pdev->dev,
        sizeof(*data),
        GFP_KERNEL);

    if (!data)
        return -ENOMEM;

    data->gpio =
        devm_gpiod_get(&pdev->dev,
            "led",
            GPIOD_OUT_LOW);

    if (IS_ERR(data->gpio))
        return PTR_ERR(data->gpio);

    gpiod_set_value_cansleep(data->gpio, 1);

    platform_set_drvdata(pdev, data);

    pr_info("%s: GPIO configured\n", DRIVER_NAME);

    return 0;
}

static void embeddedGPIO_remove(struct platform_device* pdev)
{
    struct ldd_gpio_data* data;

    data = platform_get_drvdata(pdev);

    gpiod_set_value_cansleep(data->gpio, 0);

    pr_info("%s: GPIO released\n", DRIVER_NAME);
}

static const struct of_device_id embeddedGPIO_of_match[] = {
    {
        .compatible = "bexel,embedded-gpio",
    },
    { }
};

MODULE_DEVICE_TABLE(of, embeddedGPIO_of_match);

static struct platform_driver embeddedGPIO_driver = {
    .probe = embeddedGPIO_probe,
    .remove = embeddedGPIO_remove,

    .driver = {
        .name = DRIVER_NAME,
        .of_match_table = embeddedGPIO_of_match,
    },
};

module_platform_driver(embeddedGPIO_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Embedded Linux GPIO example");


/*
//-------------------------------------------



//-------------------------------------------
*/


