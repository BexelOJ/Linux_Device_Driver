#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/gpio/consumer.h>
#include <linux/slab.h>

//-------------------------------------------

struct gpio_driver_data {
    struct gpio_desc* gpio;
};

//-------------------------------------------

static int gpio_complete_probe(struct platform_device* pdev)
{
    struct gpio_driver_data* data;
    int value;

    data = devm_kzalloc(&pdev->dev,
        sizeof(*data),
        GFP_KERNEL);

    if (!data)
        return -ENOMEM;

    //-------------------------------------------
    // Get GPIO from Device Tree
    //-------------------------------------------

    data->gpio = devm_gpiod_get(&pdev->dev,
        "control",
        GPIOD_OUT_LOW);

    if (IS_ERR(data->gpio)) {
        dev_err(&pdev->dev,
            "Failed to get GPIO\n");

        return PTR_ERR(data->gpio);
    }

    //-------------------------------------------
    // Set GPIO HIGH
    //-------------------------------------------

    gpiod_set_value_cansleep(data->gpio, 1);

    //-------------------------------------------
    // Read GPIO
    //-------------------------------------------

    value = gpiod_get_value_cansleep(data->gpio);

    dev_info(&pdev->dev,
        "GPIO value = %d\n",
        value);

    platform_set_drvdata(pdev, data);

    return 0;
}

//-------------------------------------------

static int gpio_complete_remove(struct platform_device* pdev)
{
    struct gpio_driver_data* data;

    data = platform_get_drvdata(pdev);

    if (data)
        gpiod_set_value_cansleep(data->gpio, 0);

    dev_info(&pdev->dev,
        "GPIO set LOW during remove\n");

    return 0;
}

//-------------------------------------------

static const struct of_device_id gpio_complete_match[] = {
    {
        .compatible = "ldd,gpio-complete",
    },
    { }
};

MODULE_DEVICE_TABLE(of, gpio_complete_match);

//-------------------------------------------

static struct platform_driver gpio_complete_driver = {
    .probe = gpio_complete_probe,
    .remove = gpio_complete_remove,

    .driver = {
        .name = "ldd_gpio_complete",
        .of_match_table = gpio_complete_match,
    },
};

//-------------------------------------------

module_platform_driver(gpio_complete_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Complete GPIO driver example");


/*
//-------------------------------------------



//-------------------------------------------
*/


