#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/gpio/consumer.h>

//-------------------------------------------

static int gpiod_api_probe(struct platform_device* pdev)
{
    struct gpio_desc* gpio;
    int value;

    //-------------------------------------------
    // Get GPIO
    //-------------------------------------------

    gpio = devm_gpiod_get(&pdev->dev,
        "control",
        GPIOD_OUT_LOW);

    if (IS_ERR(gpio))
        return PTR_ERR(gpio);

    //-------------------------------------------
    // Set GPIO
    //-------------------------------------------

    gpiod_set_value_cansleep(gpio, 1);

    //-------------------------------------------
    // Read GPIO
    //-------------------------------------------

    value = gpiod_get_value_cansleep(gpio);

    dev_info(&pdev->dev,
        "GPIO value = %d\n",
        value);

    //-------------------------------------------
    // Set LOW
    //-------------------------------------------

    gpiod_set_value_cansleep(gpio, 0);

    //-------------------------------------------
    // Read again
    //-------------------------------------------

    value = gpiod_get_value_cansleep(gpio);

    dev_info(&pdev->dev,
        "GPIO value = %d\n",
        value);

    return 0;
}

//-------------------------------------------

static const struct of_device_id gpiod_api_match[] = {
    {
        .compatible = "ldd,gpiod-api",
    },
    { }
};

MODULE_DEVICE_TABLE(of, gpiod_api_match);

//-------------------------------------------

static struct platform_driver gpiod_api_driver = {
    .probe = gpiod_api_probe,

    .driver = {
        .name = "ldd_gpiod_api",
        .of_match_table = gpiod_api_match,
    },
};

//-------------------------------------------

module_platform_driver(gpiod_api_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("GPIO descriptor API example");


/*
//-------------------------------------------



//-------------------------------------------
*/


